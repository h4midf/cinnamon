// //===-- DPUOps.td - DPU dialect operation definitions *- tablegen -*----------===//
// //
// // Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// // See https://llvm.org/LICENSE.txt for license information.
// // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// //
// //===----------------------------------------------------------------------===//
// //
// // This file defines the basic operations for the DPU dialect.
// //===----------------------------------------------------------------------===//

// #ifndef DPU_OPS
// #define DPU_OPS

// include "mlir/Dialect/LLVMIR/LLVMOpBase.td"
// include "mlir/Interfaces/SideEffectInterfaces.td"
// include "mlir/IR/BuiltinAttributeInterfaces.td"
// include "mlir/Interfaces/SideEffectInterfaces.td"
// include "mlir/IR/AttrTypeBase.td"
// include "mlir/IR/OpBase.td"
// include "mlir/IR/EnumAttr.td"




// //===----------------------------------------------------------------------===//
// // GlobalOp
// //===----------------------------------------------------------------------===//

// class DPU_Type<string name, string typeMnemonic,
//         list<Trait> traits = []> : TypeDef<DPU_Dialect, name, traits> {
//   let mnemonic = typeMnemonic;
// }


// def DPU_DeviceAsyncToken : DPU_Type<"DeviceAsyncToken",
//                                         "device.async.token", []> {
//   let summary = "device async token type";
//   let description = [{
//     `dpu.device.async.token` is a type returned by an asynchronous operation
//     that runs on the GPU (device). It is used to establish an SSA-based link
//     between the async operation (e.g. DeviceAsyncCopy) and operations that
//     group or synchronize the async operations (e.g. DeviceAsyncCreateGroupOp,
//     DeviceAsyncWaitOp).
//   }];
// }



// #endif // DPU_OPS



//===-- DPU.td - DPU dialect operation definitions *- tablegen -*------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the basic operations for the DPU dialect.
//
// This DPU provides a bridge between the target agnostic GPU and Vector
// dialects and lower level NVVM dialect. This allow representing PTX specific
// operations while using MLIR high level concepts like memref and 2-D vector.
//
// Ops semantic are going to be based on vendor specific PTX defintion:
// https://docs.nvidia.com/cuda/parallel-thread-execution/index.html
//
//===----------------------------------------------------------------------===//

#ifndef DPU
#define DPU

include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/IR/AttrTypeBase.td"
include "mlir/IR/OpBase.td"
include "mlir/IR/EnumAttr.td"
include "mlir/IR/BuiltinAttributeInterfaces.td"
include "mlir/Interfaces/CastInterfaces.td"
include "mlir/Interfaces/ControlFlowInterfaces.td"
include "mlir/Interfaces/CopyOpInterface.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/MemorySlotInterfaces.td"
include "mlir/Interfaces/ShapedOpInterfaces.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Interfaces/ViewLikeInterface.td"
include "mlir/IR/OpAsmInterface.td"
include "mlir/IR/SymbolInterfaces.td"

def DPU_Dialect : Dialect {
  let name = "dpu";
  let cppNamespace = "::mlir::dpu";
  let description = [{
    The `DPU` dialect provides a bridge between higher-level target-agnostic
    dialects (GPU and Vector) and the lower-level target-specific dialect
    (LLVM IR based NVVM dialect) for NVIDIA GPUs. This allow representing PTX
    specific operations while using MLIR high level dialects such as Memref
    and Vector for memory and target-specific register operands, respectively.
  }];

  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
  
  let extraClassDeclaration = [{
  }];
}

//===----------------------------------------------------------------------===//
// DPU Attribute Definitions
//===----------------------------------------------------------------------===//

def TensorMapSwizzleNone : I32EnumAttrCase<"SWIZZLE_NONE", 0, "none">;
def TensorMapSwizzle32B  : I32EnumAttrCase<"SWIZZLE_32B", 1, "swizzle_32b">;
def TensorMapSwizzle64B  : I32EnumAttrCase<"SWIZZLE_64B", 2, "swizzle_64b">;
def TensorMapSwizzle128B : I32EnumAttrCase<"SWIZZLE_128B", 3, "swizzle_128b">;
def TensorMapSwizzleKind : I32EnumAttr<"TensorMapSwizzleKind", 
                                "Tensor map swizzling mode of shared memory banks",
  [ TensorMapSwizzleNone, TensorMapSwizzle32B, TensorMapSwizzle64B, 
    TensorMapSwizzle128B]> {
  let genSpecializedAttr = 0;
  let cppNamespace = "::mlir::dpu";
}

def TensorMapSwizzleAttr : EnumAttr<DPU_Dialect, TensorMapSwizzleKind, "swizzle">;
//===----------------------------------------------------------------------===//
// DPU Type Definitions
//===----------------------------------------------------------------------===//

class DPU_Type<string name, string typeMnemonic,
        list<Trait> traits = []> : TypeDef<DPU_Dialect, name, traits> {
  let mnemonic = typeMnemonic;
}

def DPU_BarrierToken : DPU_Type<"BarrierToken", "barrier_token", []> {
  let summary = "barrier token type";
  let description = [{
  }];
}




//===----------------------------------------------------------------------===//
// DPU Op definitions.
//===----------------------------------------------------------------------===//

class DPU_Op<string mnemonic, list<Trait> traits = []> :
  Op<DPU_Dialect, mnemonic, traits> {}

// // The "internal" intrinsics are meant for compiler usage.
// class DPU_IntrOp<string mnemonic, int numResults, list<Trait> traits = []> :
//   LLVM_IntrOpBase<DPU_Dialect, mnemonic,
//                   "dpu_" # !subst(".", "_", mnemonic) # "_internal",
//                   [], [], traits, numResults>;


//===----------------------------------------------------------------------===//
// DPU Op definitions (user facing).
//===----------------------------------------------------------------------===//

//
// me operation (returns the tasklet id)
//

def MeOp : DPU_Op<"me", [Pure]> {
  let summary = "returns the id of a tasklet";
  let description = [{
    returns a number range(0, 23), determining the 
    id of a tasklet. 

    Example:

    ```mlir
      %0 = dpu.me : i32
    ```
  }];
  let results = (outs AnyI32:$me);
  let assemblyFormat = "attr-dict `:` type($me)";
}


def MemResetOp : DPU_Op<"mem_reset", [Pure]> {
  let summary = "Resets pointer allocators for managing the heap (WRAM)";
  let description = [{
    It's a void function with no arguments, and the role of it 
    is to reset the address pointers for allocating variables in the heap 
    which is the WRAM. The allocator is shared between the tasklets, so usually,
    only a single tasklet calls this operation, meaning, it is most of the time, 
    surrounded by a if block.

    Example:

    ```mlir
      dpu.mem_reset 
    ```
  }];
  let assemblyFormat = "attr-dict";
}


def BarrierInitOp : DPU_Op<"barrier_init", [Symbol]> {
  let summary = "";
  let description = [{
  }];
  // let arguments = (ins TypedAttrInterface:$barrier_id);

  let arguments = (ins SymbolNameAttr:$sym_name);

  let assemblyFormat = [{
    $sym_name attr-dict 
  }];
    // `:` type($barrierToken)
}

def BarrierWaitOp : DPU_Op<"barrier_wait", [Pure, DeclareOpInterfaceMethods<SymbolUserOpInterface>]> {
  let summary = "";
  let description = [{
  }];
  // let arguments = (ins DPU_BarrierToken:$barrier_token);
  let arguments = (ins FlatSymbolRefAttr:$name);
  let assemblyFormat = [{
    $name attr-dict
  }];
}

#endif // DPU
