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

#ifndef _ST_HPC_PPL_NN_ENGINES_RISCV_PARAMS_FC_PARAM_H_
#define _ST_HPC_PPL_NN_ENGINES_RISCV_PARAMS_FC_PARAM_H_

#include "ppl/kernel/riscv/common/fc.h"

namespace ppl { namespace nn { namespace riscv {

struct FCParam {
    ppl::kernel::riscv::fc_common_param param;
    ppl::kernel::riscv::fc_common_algo_info algo_info;
    ppl::kernel::riscv::fc_base_manager* mgr = nullptr;
};

}}}; // namespace ppl::nn::riscv

#endif