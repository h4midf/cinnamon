//===- DPUToLLVM.cpp - DPU ops to LLVM dialect conversion -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Conversion/DPUToLLVM/DPUToLLVM.h"
#include "OpToFuncCallLowering.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"


#include "mlir/Conversion/ConvertToLLVM/ToLLVMInterface.h"
#include "mlir/Conversion/LLVMCommon/ConversionTarget.h"
#include "mlir/Conversion/LLVMCommon/VectorPattern.h"
#include "mlir/Dialect/DPU/IR/DPUDialect.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/TypeUtilities.h"
#include "mlir/Pass/Pass.h"
#include <type_traits>

namespace mlir {
#define GEN_PASS_DEF_DPUTOLLVMCONVERSIONPASS
#include "mlir/Conversion/Passes.h.inc"
} // namespace mlir

using namespace mlir;

static constexpr StringRef dpu_triple = "dpu-upmem-dpurte";
static constexpr StringRef dpu_datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-i64:64:64-n32";
static constexpr StringRef dso_local = "dso_local";

//===----------------------------------------------------------------------===//
// Op Lowering Patterns
//===----------------------------------------------------------------------===//

/// Directly lower to LLVM op.
struct BarrierInitOpLowering : public ConvertOpToLLVMPattern<dpu::BarrierInitOp> {
  using ConvertOpToLLVMPattern<dpu::BarrierInitOp>::ConvertOpToLLVMPattern;


  LogicalResult
  matchAndRewrite(dpu::BarrierInitOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override;
};

LogicalResult
BarrierInitOpLowering::matchAndRewrite(dpu::BarrierInitOp op, OpAdaptor adaptor,
                                    ConversionPatternRewriter &rewriter) const {
  auto name = op.getSymName().data();
  // llvm::dbgs() << name <<"\n";
  // IntegerAttr barrierId = cast<IntegerAttr>(op.getBarrierId());
  // int barrier_id =  barrierId.getInt();
  std::string barrier_name = name;
  std::string atomic_name = "__atomic_bit_barrier_" + barrier_name;

  auto ctx = rewriter.getContext();

  Type i8Type = IntegerType::get(rewriter.getContext(), 8);
  Type barrierStructType =
      LLVM::LLVMStructType::getLiteral(ctx, {i8Type, i8Type, i8Type, i8Type});

  auto int32Type = IntegerType::get(rewriter.getContext(), 32);
  auto predTy = IntegerType::get(rewriter.getContext(), 1);
  auto resultTy = LLVM::LLVMStructType::getLiteral(rewriter.getContext(),
                                                    {predTy, predTy});


  SmallVector<NamedAttribute, 4> NamedAttributes;
  NamedAttributes.push_back(NamedAttribute(rewriter.getStringAttr("section"), rewriter.getI8IntegerAttr(0)));
  rewriter.create<LLVM::GlobalOp>(
            op.getLoc(), barrierStructType, false, LLVM::Linkage::External, barrier_name, rewriter.getI8IntegerAttr(0),1, 0, 
            true, false, SymbolRefAttr(), NamedAttributes);

  Type atomicType = IntegerType::get(ctx, 8);
  rewriter.create<LLVM::GlobalOp>(
            op.getLoc(), atomicType, false, LLVM::Linkage::External, atomic_name, rewriter.getI8IntegerAttr(0),1, 0, 
            true, false, SymbolRefAttr(), NamedAttributes);

  rewriter.eraseOp(op);
  return success();
}

template <typename OpTy>
static void populateOpPatterns(LLVMTypeConverter &converter,
                               RewritePatternSet &patterns, StringRef llvmFuncName) {

  patterns.add<OpToFuncCallLowering<OpTy>>(converter, llvmFuncName);
}

//===----------------------------------------------------------------------===//
// Pattern Population
//===----------------------------------------------------------------------===//

void mlir::dpu::populateDPUToLLVMConversionPatterns(
    LLVMTypeConverter &converter, RewritePatternSet &patterns) {
  populateOpPatterns<dpu::MeOp>(converter, patterns, /*llvm func name*/"llvm.dpu.tid.i32");
  populateOpPatterns<dpu::MemResetOp>(converter, patterns, /*llvm func name*/"mem_reset");
  patterns.add<BarrierInitOpLowering>(converter);
}

//===----------------------------------------------------------------------===//
// Pass Definition
//===----------------------------------------------------------------------===//

namespace {
struct DPUToLLVMConversionPass
    : public impl::DPUToLLVMConversionPassBase<DPUToLLVMConversionPass> {
  using Base::Base;

  void runOnOperation() override {
    ModuleOp module = getOperation();
    // MLIRContext *ctx = m.getContext();

    module->setAttr(LLVM::LLVMDialect::getTargetTripleAttrName(),
            StringAttr::get(module.getContext(), dpu_triple));
    module->setAttr(LLVM::LLVMDialect::getDataLayoutAttrName(),
            StringAttr::get(module.getContext(), dpu_datalayout));

    for (auto func : module.getOps<func::FuncOp>()) {
      func->setAttr(dso_local,
                    UnitAttr::get(&getContext()));
    }

    LLVMConversionTarget target(getContext());
    RewritePatternSet patterns(&getContext());

    LLVMTypeConverter converter(&getContext());
    mlir::dpu::populateDPUToLLVMConversionPatterns(converter, patterns);

    if (failed(applyPartialConversion(getOperation(), target,
                                      std::move(patterns))))
      signalPassFailure();
  }
};
} // namespace
