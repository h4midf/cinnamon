//===- UPMEMDialect.h - MLIR Dialect for UPMEM Kernels --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the UPMEM kernel-related operations and puts them in the
// corresponding dialect.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_UPMEM_IR_UPMEMDIALECT_H
#define MLIR_DIALECT_UPMEM_IR_UPMEMDIALECT_H

#include "mlir/Dialect/DLTI/Traits.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/Interfaces/FunctionInterfaces.h"

#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/IR/SymbolTable.h"
#include "mlir/Interfaces/InferIntRangeInterface.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "llvm/ADT/STLExtras.h"
#include "mlir/Bytecode/BytecodeOpInterface.h"



namespace mlir {
namespace upmem{

class AsyncTokenType
    : public Type::TypeBase<AsyncTokenType, Type, TypeStorage> {
public:
  // Used for generic hooks in TypeBase.
  using Base::Base;
};

struct UPMEM_IDF{
  Value val;
  Value zero;
};


}
}

#include "mlir/Dialect/UPMEM/IR/UPMEMOpsEnums.h.inc"

#include "mlir/Dialect/UPMEM/IR/UPMEMOpsDialect.h.inc"

#include "mlir/Dialect/UPMEM/IR/UPMEMOpInterfaces.h.inc"

#define GET_ATTRDEF_CLASSES
#include "mlir/Dialect/UPMEM/IR/UPMEMOpsAttributes.h.inc"

#define GET_OP_CLASSES
#include "mlir/Dialect/UPMEM/IR/UPMEMOps.h.inc"

#endif // MLIR_DIALECT_UPMEM_IR_UPMEMDIALECT_H