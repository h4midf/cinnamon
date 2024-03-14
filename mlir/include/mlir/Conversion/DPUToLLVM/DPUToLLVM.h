//===- ArithToLLVM.h - Arith to LLVM dialect conversion ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_CONVERSION_DPUTOLLVM_DPUTOLLVM_H
#define MLIR_CONVERSION_DPUTOLLVM_DPUTOLLVM_H

#include <memory>

namespace mlir {

class DialectRegistry;
class LLVMTypeConverter;
class RewritePatternSet;
class Pass;

#define GEN_PASS_DECL_DPUTOLLVMCONVERSIONPASS
#include "mlir/Conversion/Passes.h.inc"

namespace dpu {
void populateDPUToLLVMConversionPatterns(LLVMTypeConverter &converter,
                                           RewritePatternSet &patterns);

void registerConvertDPUToLLVMInterface(DialectRegistry &registry);
} // namespace dpu
} // namespace mlir

#endif // MLIR_CONVERSION_DPUTOLLVM_DPUTOLLVM_H
