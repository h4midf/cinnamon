//===- PassDetail.h - Pass details ------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// Also available under a BSD-style license. See LICENSE.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_UPMEM_TRANSFORMS_PASSDETAIL_H
#define MLIR_DIALECT_UPMEM_TRANSFORMS_PASSDETAIL_H

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Pass/Pass.h"

namespace mlir {
class ModuleOp;
namespace upmem{
#define GEN_PASS_CLASSES

#include "torch-mlir/Dialect/UPMEM/Transforms/Passes.h.inc"

} // namespace torch
} // end namespace mlir

#endif // TORCHMLIR_DIALECT_UPMEM_TRANSFORMS_PASSDETAIL_H
