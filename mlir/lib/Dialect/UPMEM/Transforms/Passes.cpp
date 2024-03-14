//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// Also available under a BSD-style license. See LICENSE.
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/UPMEM/Transforms/Passes.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Transforms/Passes.h"

// void mlir::upmem::registerUPMEMPasses() {
//   mlir::upmem::registerPasses();
  // mlir::PassPipelineRegistration<upmem::UPMEMFusionPipelineOptions>(
  //     "upmem-fuse-pipeline",
  //     "upmem ops fusion.",
  //     mlir::upmem::createUPMEMOpsFusionPipeline);

  // mlir::PassPipelineRegistration<upmem::UPMEMPartitioningPipelineOptions>(
  //     "upmem-partition-pipeline",
  //     "upmem ops partitioning.",
  //     mlir::upmem::createUPMEMOpsPartitioningPipeline);

  // mlir::PassPipelineRegistration<upmem::UPMEMRewritePipelineOptions>(
  //     "upmem-rewrite-op-pipeline",
  //     "upmem ops rewriting.",
  //     mlir::upmem::createUPMEMOpsRewritePipeline);

  // mlir::PassPipelineRegistration<upmem::UPMEMAffineLoopDistributionPipelineOptions>(
  //     "upmem-affine-loop-distribution-pipeline",
  //     "upmem affine loop distribution.",
  //     mlir::upmem::createUPMEMAffineLoopDistributionPipeline);
// }

// void mlir::upmem::createUPMEMOpsFusionPipeline(
//     OpPassManager &pm, const UPMEMFusionPipelineOptions &options) {
//   pm.addPass(createFuseUPMEMOpsPass());
// }


// void mlir::upmem::createUPMEMOpsPartitioningPipeline(
//     OpPassManager &pm, const UPMEMPartitioningPipelineOptions &options) {
//   pm.addPass(createPartitionUPMEMOpsPass());

// }

// void mlir::upmem::createUPMEMOpsRewritePipeline(
//     OpPassManager &pm, const UPMEMRewritePipelineOptions &options) {
//   pm.addPass(createRewriteUPMEMOpsPass());
// }

// void mlir::upmem::createUPMEMAffineLoopDistributionPipeline(
//     OpPassManager &pm, const UPMEMAffineLoopDistributionPipelineOptions &options) {
//   pm.addPass(createUPMEMAffineLoopDistributionPass());
// }
