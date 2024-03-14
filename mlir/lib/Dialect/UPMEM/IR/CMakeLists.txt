add_mlir_dialect_library(MLIRUPMEMOps
  UPMEMDialect.cpp
  InferIntRangeInterfaceImpls.cpp

  ADDITIONAL_HEADER_DIRS
  ${MLIR_MAIN_INCLUDE_DIR}/mlir/Dialect/UPMEM

  DEPENDS
  MLIRUPMEMOpsIncGen
  MLIRUPMEMOpsAttributesIncGen
  MLIRUPMEMOpsEnumsGen
  MLIRUPMEMOpInterfacesIncGen

  LINK_LIBS PUBLIC
  MLIRArithDialect
  MLIRDLTIDialect
  MLIRInferIntRangeInterface
  MLIRIR
  MLIRMemRefDialect
  MLIRSideEffectInterfaces
  MLIRSupport
  )
