//===-- UPMEMBase.td - UPMEM dialect definitions ---------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Defines the UPMEM dialect
//
//===----------------------------------------------------------------------===//

#ifndef UPMEM_BASE
#define UPMEM_BASE

include "mlir/IR/AttrTypeBase.td"
include "mlir/IR/EnumAttr.td"
include "mlir/IR/OpBase.td"

//===----------------------------------------------------------------------===//
// UPMEM Dialect.
//===----------------------------------------------------------------------===//


def UPMEM_Dialect : Dialect {
  let name = "upmem";
  let cppNamespace = "::mlir::upmem";
  let hasOperationAttrVerify = 1;

  let extraClassDeclaration = [{
    /// Get the name of the attribute used to annotate the modules that contain
    /// kernel modules.
    static StringRef getContainerModuleAttrName() {
      return "upmem.container_module";
    }
    /// Get the name of the attribute used to annotate external kernel
    /// functions.
    static StringRef getKernelFuncAttrName() { return "upmem.kernel"; }

    /// Returns whether the given function is a kernel function, i.e., has the
    /// 'upmem.kernel' attribute.
    static bool isKernel(Operation *op);

    /// Returns the number of workgroup (thread, block) dimensions supported in
    /// the UPMEM dialect.
    // TODO: consider generalizing this.
    static unsigned getNumWorkgroupDimensions() { return 3; }

    /// Returns the numeric value used to identify the workgroup memory address
    /// space.
    static AddressSpace getMRAMAddressSpace() { return AddressSpace::MRAM; }

    /// Returns the numeric value used to identify the Private WRAM memory address
    /// space. Using the common heap allocator, the WRAM is partitioned between the threads.
    /// This address space returnes the address space of the WRAM that is private to a thread.
    static AddressSpace getPrivateWRAMAddressSpace() { return AddressSpace::PWRAM; }

    /// Returns the numeric value used to identify the Total WRAM memory address
    /// space. The total WRAM is 64KB, which can be shared among the threads, but not usually used in this way.
    /// This is depends totally on the use case.
    static AddressSpace getTotalWRAMAddressSpace() { return AddressSpace::TWRAM; }

  }];

  let dependentDialects = ["arith::ArithDialect"];
  let useDefaultAttributePrinterParser = 1;
  let useDefaultTypePrinterParser = 1;
}


//===----------------------------------------------------------------------===//
// UPMEM Enums.
//===----------------------------------------------------------------------===//

class UPMEM_I32Enum<string name, string description, list<I32EnumAttrCase> cases>
    : I32EnumAttr<name, description, cases> {
  let genSpecializedAttr = 0;
  let cppNamespace = "::mlir::upmem";
}
class UPMEM_I32EnumAttr<string mnemonic, UPMEM_I32Enum enumInfo> :
    EnumAttr<UPMEM_Dialect, enumInfo, mnemonic> {
  let assemblyFormat = "`<` $value `>`";
}

def UPMEM_AddressSpaceMRAM : I32EnumAttrCase<"MRAM", 1, "mram">;
def UPMEM_AddressSpaceWRAM : I32EnumAttrCase<"WRAM", 2, "wram">;
def UPMEM_AddressSpacePrivateWRAM : I32EnumAttrCase<"PWRAM", 3, "pwram">;
def UPMEM_AddressSpaceTotalWRAM : I32EnumAttrCase<"TWRAM", 4, "twram">;
def UPMEM_AddressSpaceEnum : UPMEM_I32Enum<
  "AddressSpace", "UPMEM address space", [
    UPMEM_AddressSpaceMRAM,
    UPMEM_AddressSpaceWRAM,
    UPMEM_AddressSpacePrivateWRAM,
    UPMEM_AddressSpaceTotalWRAM
  ]>;

def UPMEM_AddressSpaceAttr :
  UPMEM_I32EnumAttr<"address_space", UPMEM_AddressSpaceEnum>;

#endif // UPMEM_BASE
