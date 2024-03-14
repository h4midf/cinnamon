//===------------------------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// Also available under a BSD-style license. See LICENSE.
//
//===----------------------------------------------------------------------===//

#ifndef TORCHMLIR_DIALECT_UPMEM_TRANSFORMS_PASSES_H
#define TORCHMLIR_DIALECT_UPMEM_TRANSFORMS_PASSES_H

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Pass/Pass.h"

#include <memory>

namespace mlir {
class ModuleOp;

namespace upmem {

#include "mlir/Dialect/UPMEM/Transforms/Passes.h.inc"

// std::unique_ptr<OperationPass<ModuleOp>> createUPMEMBufferizePass();

// ////// Fusion
// std::unique_ptr<OperationPass<func::FuncOp>> createFuseUPMEMOpsPass();

// struct UPMEMFusionPipelineOptions
//     : public PassPipelineOptions<UPMEMFusionPipelineOptions> {
// };

// void createUPMEMOpsFusionPipeline(
//     OpPassManager &pm, const UPMEMFusionPipelineOptions &options);


// ////// Partitioning

// struct UPMEMPartitioningPipelineOptions
//     : public PassPipelineOptions<UPMEMPartitioningPipelineOptions> {

//     Option<int> row_count{
//       *this, "row_count",
//       llvm::cl::desc(
//           "Number of rows in the cam device. Default: 128"),
//       llvm::cl::init(128)};

//     Option<int> col_count{
//       *this, "col_count",
//       llvm::cl::desc(
//           "Number of columns in the cam device. Default: 128"),
//       llvm::cl::init(128)};
// };

// std::unique_ptr<OperationPass<func::FuncOp>> createPartitionUPMEMOpsPass();
// void createUPMEMOpsPartitioningPipeline(
//     OpPassManager &pm, const UPMEMPartitioningPipelineOptions &options);

// ////// Rewriting 
// std::unique_ptr<OperationPass<func::FuncOp>> createRewriteUPMEMOpsPass();

// struct UPMEMRewritePipelineOptions
//     : public PassPipelineOptions<UPMEMRewritePipelineOptions> {
// };

// void createUPMEMOpsRewritePipeline(
//     OpPassManager &pm, const UPMEMRewritePipelineOptions &options);



// ////// AffineLoopDistribution

// std::unique_ptr<OperationPass<func::FuncOp>> createUPMEMAffineLoopDistributionPass();

// struct UPMEMAffineLoopDistributionPipelineOptions
//     : public PassPipelineOptions<UPMEMAffineLoopDistributionPipelineOptions> {
// };

// void createUPMEMAffineLoopDistributionPipeline(
//     OpPassManager &pm, const UPMEMAffineLoopDistributionPipelineOptions &options);


// /// Registers all Torch transformation passes.
// void registerUPMEMPasses();




//===----------------------------------------------------------------------===//
// Pass registration
//===----------------------------------------------------------------------===//

#define GEN_PASS_REGISTRATION
#include "mlir/Dialect/UPMEM/Transforms/Passes.h.inc"

} // namespace upmem
} // namespace mlir

#endif // TORCHMLIR_DIALECT_TORCH_TRANSFORMS_PASSES_H
