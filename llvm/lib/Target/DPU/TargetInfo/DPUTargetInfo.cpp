//===-- DPUTargetInfo.cpp - DPU Target Implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

// namespace llvm {
// Target TheDPUTarget;
// }

namespace llvm {
Target &getTheDPUTarget() {
  static Target TheDPUTarget;
  return TheDPUTarget;
}
} // namespace llvm


extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDPUTargetInfo() {
  RegisterTarget<Triple::dpu, /*HasJIT=*/false> X(getTheDPUTarget(), "dpu",
                                                  "UPMEM DPU", "DPU");
}
