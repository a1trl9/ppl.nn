// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "ppl/nn/engines/cuda/kernels/onnx/pad_kernel.h"

#include <memory>

#include "cudakernel/memory/pad.h"
#include "ppl/nn/utils/destructor.h"

namespace ppl { namespace nn { namespace cuda {

ppl::common::RetCode PadKernel::DoExecute(KernelExecContext* ctx) {
    auto input = ctx->GetInput<TensorImpl>(0);
    auto output = ctx->GetOutput<TensorImpl>(0);
    
    void* pad_buffer; 
    BufferDesc tmp_buffer_desc;
    if (ctx->GetInputCount() > 1) {
        pad_buffer = ctx->GetInput<TensorImpl>(1)->GetBufferPtr();
    } else {
        auto size = input->GetShape()->GetDimCount();
        auto status = GetCudaDevice()->AllocTmpBuffer(size * sizeof(int64_t), &tmp_buffer_desc);
        if (status != ppl::common::RC_SUCCESS) {
            LOG(ERROR) << "alloc tmp buffer size[" << size << "] for kernel[" << GetName()
                    << "] failed: " << ppl::common::GetRetCodeStr(status);
            return status;
        }
        pad_buffer = tmp_buffer_desc.addr;
        std::vector<int64_t> pads;
        pads.resize(size);
        for (uint32_t i = 0; i < size; ++i) {
            pads[i] = param_->pads[i];
        }
        GetCudaDevice()->CopyFromHost(&tmp_buffer_desc, (void*)pads.data(), size * sizeof(int64_t));
    }
    utils::Destructor __tmp_buffer_guard([this, &tmp_buffer_desc]() -> void {
        GetCudaDevice()->FreeTmpBuffer(&tmp_buffer_desc);
    });

    PadKernelParam kernel_param;
    kernel_param.mode = param_->mode;
    if (ctx->GetInputCount() >= 3) {
        auto constant_data = ctx->GetInput<TensorImpl>(2);
        auto status = constant_data->CopyToHost(&(kernel_param.constant_value));
        if (status != ppl::common::RC_SUCCESS) {
            LOG(ERROR) << "Copy constant value failed: " << ppl::common::GetRetCodeStr(status);
            return status;
        }
    }

    ppl::common::RetCode status = ppl::common::RC_SUCCESS;
    if (input->GetEdge()->CalcConsumerCount() == 1 && input->GetType() == TENSORTYPE_NORMAL &&
        input->GetShape()->GetElementsIncludingPadding() == output->GetShape()->GetElementsIncludingPadding()) {
        output->TransferBufferFrom(input);
    } else {
        status = PPLCUDAPadForwardImp(GetStream(), kernel_param, input->GetShape(), input->GetBufferPtr(),
                                      (const int64_t*)pad_buffer, output->GetShape(),
                                      output->GetBufferPtr());
    }
    return status;
}

}}} // namespace ppl::nn::cuda
