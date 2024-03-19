add_mlir_conversion_library(MLIRDPUToLLVM
  DPUToLLVM.cpp

  ADDITIONAL_HEADER_DIRS
  ${MLIR_MAIN_INCLUDE_DIR}/mlir/Conversion/DPUToLLVM

  DEPENDS
  MLIRConversionPassIncGen

  LINK_COMPONENTS
  Core

  LINK_LIBS PUBLIC
  MLIRDPUDialect
  MLIRFuncDialect
  MLIRLLVMCommonConversion
  MLIRLLVMDialect
  )
