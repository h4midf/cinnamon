add_mlir_dialect(DPU dpu)
add_mlir_doc(DPU DPU Dialects/ -gen-dialect-doc -dialect=dpu)

set(LLVM_TARGET_DEFINITIONS DPU.td)
mlir_tablegen(DPUConversions.inc -gen-llvmir-conversions)
add_public_tablegen_target(MLIRDPUConversionsIncGen)
