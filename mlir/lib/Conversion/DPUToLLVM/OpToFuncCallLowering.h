//===- OpToFuncCallLowering.h - DPU ops lowering to custom calls *- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef MLIR_CONVERSION_DPUOPTOFUNCCALLLOWERING_H_
#define MLIR_CONVERSION_DPUOPTOFUNCCALLLOWERING_H_

#include "mlir/Conversion/LLVMCommon/Pattern.h"
#include "mlir/Dialect/DPU/IR/DPUDialect.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/Builders.h"

namespace mlir {

// Rewriting that replace SourceOp with a CallOp 
// depending on the element type that Op operates upon. The function
// declaration is added in case it was not added before.
// 
template <typename SourceOp>
struct OpToFuncCallLowering : public ConvertOpToLLVMPattern<SourceOp> {
public:
  explicit OpToFuncCallLowering(LLVMTypeConverter &lowering, StringRef llvmFuncName)
      : ConvertOpToLLVMPattern<SourceOp>(lowering), llvmFuncName(llvmFuncName) {}
  
  LogicalResult
  matchAndRewrite(SourceOp op, typename SourceOp::Adaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    using LLVM::LLVMFuncOp;

    SmallVector<Value, 1> castedOperands;
    Type resultType;
    for (Value operand : adaptor.getOperands())
        castedOperands.push_back(maybeCast(operand, rewriter));
    if (isa<dpu::MeOp>(op)){
      resultType = IntegerType::get(rewriter.getContext(), 32);
    } 
    else if (isa<dpu::MemResetOp>(op)){
      resultType = LLVM::LLVMPointerType::get(IntegerType::get((rewriter.getContext()), 8));
    } 
    Type funcType = getFunctionType(resultType, castedOperands);

    StringRef funcName = llvmFuncName;
    if (funcName.empty())
      return failure();

    LLVMFuncOp funcOp = appendOrGetFuncOp(funcName, funcType, op);
    auto callOp =
        rewriter.create<LLVM::CallOp>(op->getLoc(), funcOp, castedOperands);
    if (op->getNumResults() == 0){
      rewriter.eraseOp(op);
    } else {
      rewriter.replaceOp(op, {callOp.getResult()});
    }
    return success();
  }

private:
  Value maybeCast(Value operand, PatternRewriter &rewriter) const {
    Type type = operand.getType();
    if (!isa<Float16Type>(type))
      return operand;

    return rewriter.create<LLVM::FPExtOp>(
        operand.getLoc(), Float32Type::get(rewriter.getContext()), operand);
  }

  Type getFunctionType(Type resultType, ValueRange operands) const {
    SmallVector<Type> operandTypes(operands.getTypes());
    return LLVM::LLVMFunctionType::get(resultType, operandTypes);
  }

  LLVM::LLVMFuncOp appendOrGetFuncOp(StringRef funcName, Type funcType,
                                     Operation *op) const {
    using LLVM::LLVMFuncOp;

    auto funcAttr = StringAttr::get(op->getContext(), funcName);
    Operation *funcOp = SymbolTable::lookupNearestSymbolFrom(op, funcAttr);
    if (funcOp)
      return cast<LLVMFuncOp>(*funcOp);

    mlir::OpBuilder b(op->getParentOfType<FunctionOpInterface>());
    return b.create<LLVMFuncOp>(op->getLoc(), funcName, funcType);
  }

  const std::string llvmFuncName;
};

} // namespace mlir

#endif // MLIR_CONVERSION_GPUCOMMON_OPTOFUNCCALLLOWERING_H_
