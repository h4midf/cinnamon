add_mlir_dialect_library(MLIRUPMEMTransforms
  Passes.cpp

  ADDITIONAL_HEADER_DIRS
  ${MLIR_MAIN_INCLUDE_DIR}/mlir/Dialect/UPMEM

  DEPENDS
  MLIRUPMEMPassIncGen

  LINK_LIBS PUBLIC
  MLIRArithDialect
  MLIRBufferizationDialect
  MLIRBufferizationTransforms
  MLIRDestinationStyleOpInterface
  MLIRDialectUtils
  MLIRIR
  MLIRMemRefDialect
  MLIRPass
  MLIRUPMEMOps
  MLIRSupport
  MLIRTensorDialect
  MLIRTensorTransforms
  MLIRTransforms
  MLIRTransformUtils
)

