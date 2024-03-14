set(LLVM_TARGET_DEFINITIONS Passes.td)
mlir_tablegen(Passes.h.inc -gen-pass-decls)
mlir_tablegen(Transforms.capi.h.inc -gen-pass-capi-header)
mlir_tablegen(Transforms.capi.cpp.inc -gen-pass-capi-impl)
add_public_tablegen_target(MLIRUPMEMPassIncGen)

add_mlir_doc(Passes MLIRUPMEMTransforms ./ -gen-pass-doc)
