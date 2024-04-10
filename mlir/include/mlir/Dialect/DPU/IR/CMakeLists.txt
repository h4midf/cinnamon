add_mlir_dialect(DPU dpu)
add_mlir_doc(DPU DPU Dialects/ -gen-dialect-doc)

set(LLVM_TARGET_DEFINITIONS DPU.td)
mlir_tablegen(DPUEnums.h.inc -gen-enum-decls)
mlir_tablegen(DPUEnums.cpp.inc -gen-enum-defs)
add_public_tablegen_target(MLIRDPUEnumsIncGen)

set(LLVM_TARGET_DEFINITIONS DPU.td)
mlir_tablegen(DPUAttrDefs.h.inc -gen-attrdef-decls)
mlir_tablegen(DPUAttrDefs.cpp.inc -gen-attrdef-defs)
add_public_tablegen_target(MLIRDPUAttributesIncGen)

set(LLVM_TARGET_DEFINITIONS DPU.td)
mlir_tablegen(DPUAttrTypes.h.inc -gen-typedef-decls)
mlir_tablegen(DPUAttrTypes.cpp.inc -gen-typedef-decls)
add_public_tablegen_target(MLIRDPUTypesIncGen)
