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

#ifndef _ST_HPC_PPL_NN_ENGINES_RISCV_OPTIMIZER_OPS_ONNX_SLICE_OP_H_
#define _ST_HPC_PPL_NN_ENGINES_RISCV_OPTIMIZER_OPS_ONNX_SLICE_OP_H_

#include "ppl/nn/engines/riscv/optimizer/opt_kernel.h"
#include "ppl/nn/engines/riscv/params/slice_param.h"
#include "ppl/nn/params/onnx/slice_param.h"

namespace ppl { namespace nn { namespace riscv {

class SliceOp final : public RiscvOptKernel {
public:
    SliceOp(const ir::Node* node) : RiscvOptKernel(node) {}
    ppl::common::RetCode Init(const OptKernelOptions& options) override;
    KernelImpl* CreateKernelImpl() const override;
    ppl::common::RetCode SelectFormat(const InputOutputInfo& info,
                                      std::vector<ppl::common::dataformat_t>* selected_input_formats,
                                      std::vector<ppl::common::dataformat_t>* selected_output_formats) override;

private:
    ppl::nn::riscv::SliceParam aux_param_;
    std::shared_ptr<ppl::nn::onnx::SliceParam> param_;
};

}}} // namespace ppl::nn::riscv

#endif
