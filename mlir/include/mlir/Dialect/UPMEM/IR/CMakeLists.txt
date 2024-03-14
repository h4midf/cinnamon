add_mlir_dialect(UPMEMOps upmem)
add_mlir_doc(UPMEMOps UPMEMOps Dialects/ -gen-op-doc)

set(LLVM_TARGET_DEFINITIONS UPMEMBase.td)
mlir_tablegen(UPMEMOpInterfaces.h.inc -gen-op-interface-decls)
mlir_tablegen(UPMEMOpInterfaces.cpp.inc -gen-op-interface-defs)
add_public_tablegen_target(MLIRUPMEMOpInterfacesIncGen)

set(LLVM_TARGET_DEFINITIONS UPMEMOps.td)
mlir_tablegen(UPMEMOpsEnums.h.inc -gen-enum-decls)
mlir_tablegen(UPMEMOpsEnums.cpp.inc -gen-enum-defs)
add_public_tablegen_target(MLIRUPMEMOpsEnumsGen)

set(LLVM_TARGET_DEFINITIONS UPMEMOps.td)
mlir_tablegen(UPMEMOpsAttributes.h.inc -gen-attrdef-decls -attrdefs-dialect=upmem)
mlir_tablegen(UPMEMOpsAttributes.cpp.inc -gen-attrdef-defs -attrdefs-dialect=upmem)
add_public_tablegen_target(MLIRUPMEMOpsAttributesIncGen)




