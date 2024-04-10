// // //===- DPUDialect.h - MLIR Dialect for DPU ----------------------*- C++ -*-===//
// // //
// // // Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// // // See https://llvm.org/LICENSE.txt for license information.
// // // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// // //
// // //===----------------------------------------------------------------------===//
// // //
// // // This file declares the Target dialect for DPU in MLIR.
// // //
// // //===----------------------------------------------------------------------===//

// // #ifndef MLIR_DIALECT_DPU_DPUDIALECT_H_
// // #define MLIR_DIALECT_DPU_DPUDIALECT_H_

// // #include "mlir/Bytecode/BytecodeOpInterface.h"
// // #include "mlir/IR/BuiltinTypes.h"
// // #include "mlir/IR/Dialect.h"
// // #include "mlir/IR/OpDefinition.h"
// // #include "mlir/Interfaces/SideEffectInterfaces.h"

// // // #define GET_ATTRDEF_CLASSES
// // // #include "mlir/Dialect/DPU/DPUAttrDefs.h.inc"

// // #define GET_TYPEDEF_CLASSES
// // #include "mlir/Dialect/DPU/DPUTypes.h.inc"

// // #include "mlir/Dialect/DPU/DPUDialect.h.inc"

// // #define GET_OP_CLASSES
// // #include "mlir/Dialect/DPU/DPU.h.inc"

// // #endif // MLIR_DIALECT_DPU_DPUDIALECT_H_


// //===- DPUDialect.h - MLIR Dialect for DPU ------------------*- C++ -*-===//
// //
// // Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// // See https://llvm.org/LICENSE.txt for license information.
// // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// //
// //===----------------------------------------------------------------------===//
// //
// // This file declares the Target dialect for DPU in MLIR.
// //
// //===----------------------------------------------------------------------===//

// #ifndef MLIR_DIALECT_DPU_DPUDIALECT_H_
// #define MLIR_DIALECT_DPU_DPUDIALECT_H_

// #include "mlir/Bytecode/BytecodeOpInterface.h"
// #include "mlir/IR/BuiltinTypes.h"
// #include "mlir/IR/Dialect.h"
// #include "mlir/IR/OpDefinition.h"
// #include "mlir/Interfaces/SideEffectInterfaces.h"

// // #include "mlir/Dialect/DPU/IR/DPUEnums.h.inc"

// // constexpr int kWarpSize = 32;

// #define GET_ATTRDEF_CLASSES
// #include "mlir/Dialect/DPU/IR/DPUAttrDefs.h.inc"

// #define GET_TYPEDEF_CLASSES
// #include "mlir/Dialect/DPU/IR/DPUTypes.h.inc"

// #include "mlir/Dialect/DPU/IR/DPUDialect.h.inc"

// #define GET_OP_CLASSES
// #include "mlir/Dialect/DPU/IR/DPU.h.inc"

// #endif // MLIR_DIALECT_DPU_DPUDIALECT_H_





//===- DPUDialect.h - MLIR Dialect for DPU ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the Target dialect for DPU in MLIR.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_DPU_DPUDIALECT_H_
#define MLIR_DIALECT_DPU_DPUDIALECT_H_

#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/CallInterfaces.h"
#include "mlir/Interfaces/CastInterfaces.h"
#include "mlir/Interfaces/ControlFlowInterfaces.h"
#include "mlir/Interfaces/CopyOpInterface.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/MemorySlotInterfaces.h"
#include "mlir/Interfaces/ShapedOpInterfaces.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Interfaces/ViewLikeInterface.h"

#include "mlir/Dialect/DPU/IR/DPUEnums.h.inc"

// constexpr int kWarpSize = 32;

#define GET_ATTRDEF_CLASSES
#include "mlir/Dialect/DPU/IR/DPUAttrDefs.h.inc"

#define GET_TYPEDEF_CLASSES
#include "mlir/Dialect/DPU/IR/DPUTypes.h.inc"

#include "mlir/Dialect/DPU/IR/DPUDialect.h.inc"

#define GET_OP_CLASSES
#include "mlir/Dialect/DPU/IR/DPU.h.inc"

#endif // MLIR_DIALECT_DPU_DPUDIALECT_H_
