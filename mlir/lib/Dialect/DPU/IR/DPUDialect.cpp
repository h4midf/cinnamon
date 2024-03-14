//===- DPUDialect.cpp - MLIR DPU ops implementation -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the DPU dialect and its operations.
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/DPU/DPUDialect.h"
#include "mlir/Dialect/LLVMIR/LLVMTypes.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/IR/TypeUtilities.h"

using namespace mlir;

#include "mlir/Dialect/DPU/DPUDialect.cpp.inc"

void dpu::DPUDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "mlir/Dialect/DPU/DPU.cpp.inc"
      >();
}


// static LogicalResult verifyTemplate(Operation *op, ...) {
//   if(...){
//     return op->emitOpError("bad mult shape: ")
//            << cm << " x " << cn << " x " << ak;
//   }
//   return success();
// }

// LogicalResult dpu::XOp::verify() {
//   return verifyTemplate(*this, ...);
// }



#define GET_OP_CLASSES
#include "mlir/Dialect/DPU/DPU.cpp.inc"
