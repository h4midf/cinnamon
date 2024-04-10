// //===- DPUDialect.cpp - MLIR DPU ops implementation -----------------------===//
// //
// // Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// // See https://llvm.org/LICENSE.txt for license information.
// // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// //
// //===----------------------------------------------------------------------===//
// //
// // This file implements the DPU dialect and its operations.
// //
// //===----------------------------------------------------------------------===//

// #include "mlir/Dialect/DPU/IR/DPUDialect.h"
// #include "mlir/Dialect/LLVMIR/LLVMTypes.h"
// #include "mlir/IR/Builders.h"
// #include "mlir/IR/OpImplementation.h"
// #include "mlir/IR/TypeUtilities.h"

// using namespace mlir;
// using namespace mlir::dpu;

// #include "mlir/Dialect/DPU/IR/DPUDialect.cpp.inc"

// void dpu::DPUDialect::initialize() {
//   addTypes<
// #define GET_TYPEDEF_LIST
// #include "mlir/Dialect/DPU/IR/DPUTypes.cpp.inc"
//       >();
//   addAttributes<
// #define GET_ATTRDEF_LIST
// #include "mlir/Dialect/DPU/IR/DPUAttrDefs.cpp.inc"
//       >();
//   addOperations<
// #define GET_OP_LIST
// #include "mlir/Dialect/DPU/IR/DPU.cpp.inc"
//       >();
// }


// // static LogicalResult verifyTemplate(Operation *op, ...) {
// //   if(...){
// //     return op->emitOpError("bad mult shape: ")
// //            << cm << " x " << cn << " x " << ak;
// //   }
// //   return success();
// // }

// // LogicalResult dpu::XOp::verify() {
// //   return verifyTemplate(*this, ...);
// // }



// //===----------------------------------------------------------------------===//
// // TableGen'd dialect, type, and op definitions
// //===----------------------------------------------------------------------===//

// #define GET_ATTRDEF_CLASSES
// #include "mlir/Dialect/DPU/IR/DPUAttrDefs.cpp.inc"

// // #include "mlir/Dialect/DPU/IR/DPUEnums.cpp.inc"

// #define GET_OP_CLASSES
// #include "mlir/Dialect/DPU/IR/DPU.cpp.inc"

// #define GET_TYPEDEF_CLASSES
// #include "mlir/Dialect/DPU/IR/DPUTypes.cpp.inc"





//===- DPUDialect.cpp - MLIR DPU ops implementation -------------------===//
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

#include "mlir/Dialect/DPU/IR/DPUDialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Diagnostics.h"
#include "mlir/IR/DialectImplementation.h"
#include "mlir/IR/Matchers.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/IR/TypeUtilities.h"
#include "mlir/IR/Verifier.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/TypeSwitch.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Transforms/InliningUtils.h"
#include "mlir/Dialect/Utils/StaticValueUtils.h"
#include "mlir/IR/AffineMap.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Matchers.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/IR/TypeUtilities.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Interfaces/ViewLikeInterface.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallBitVector.h"

using namespace mlir;
using namespace mlir::dpu;

#include "mlir/Dialect/DPU/IR/DPUDialect.cpp.inc"

void dpu::DPUDialect::initialize() {
  addTypes<
#define GET_TYPEDEF_LIST
#include "mlir/Dialect/DPU/IR/DPUTypes.cpp.inc"
      >();
  addAttributes<
#define GET_ATTRDEF_LIST
#include "mlir/Dialect/DPU/IR/DPUAttrDefs.cpp.inc"
      >();
  addOperations<
#define GET_OP_LIST
#include "mlir/Dialect/DPU/IR/DPU.cpp.inc"
      >();
}


//===----------------------------------------------------------------------===//
// GetGlobalOp
//===----------------------------------------------------------------------===//

LogicalResult
BarrierWaitOp::verifySymbolUses(SymbolTableCollection &symbolTable) {
  // Verify that the result type is same as the type of the referenced
  // dpu.barrier_wait op.
  // auto global =
  //     symbolTable.lookupNearestSymbolFrom<BarrierWaitOp>(*this, getNameAttr());
  // if (!global)
  //   return emitOpError("'")
  //          << getName() << "' does not reference a valid global memref";

  // Type resultType = getResult().getType();
  // if (global.getType() != resultType)
  //   return emitOpError("result type ")
  //          << resultType << " does not match type " << global.getType()
  //          << " of the global memref @" << getName();
  return success();
}



//===----------------------------------------------------------------------===//
// TableGen'd dialect, type, and op definitions
//===----------------------------------------------------------------------===//

#define GET_ATTRDEF_CLASSES
#include "mlir/Dialect/DPU/IR/DPUAttrDefs.cpp.inc"

#include "mlir/Dialect/DPU/IR/DPUEnums.cpp.inc"

#define GET_OP_CLASSES
#include "mlir/Dialect/DPU/IR/DPU.cpp.inc"

#define GET_TYPEDEF_CLASSES
#include "mlir/Dialect/DPU/IR/DPUTypes.cpp.inc"
