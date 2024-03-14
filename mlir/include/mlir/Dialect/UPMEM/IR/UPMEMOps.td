//===-- UPMEMOps.td - UPMEM dialect definitions ---------------*- tablegen -*-===//
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

#ifndef UPMEM_OPS
#define UPMEM_OPS

include "mlir/IR/AttrTypeBase.td"
include "mlir/IR/EnumAttr.td"
include "mlir/IR/OpBase.td"

include "mlir/Dialect/UPMEM/IR/UPMEMBase.td"
include "mlir/Interfaces/FunctionInterfaces.td"
include "mlir/IR/SymbolInterfaces.td"
include "mlir/Interfaces/DataLayoutInterfaces.td"
include "mlir/Interfaces/InferIntRangeInterface.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"



class UPMEM_Op<string mnemonic, list<Trait> traits = []> :
    Op<UPMEM_Dialect, mnemonic, traits>;



//===----------------------------------------------------------------------===//
// UPMEM Types.
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// UPMEM Interfaces.
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// UPMEM Attributes.
//===----------------------------------------------------------------------===//

class UPMEM_Attr<string attrName, string attrMnemonic, list<Trait> traits = []>
    : AttrDef<UPMEM_Dialect, attrName, traits> {
  let mnemonic = attrMnemonic;
}







//===----------------------------------------------------------------------===//
// UPMEM Ops.
//===----------------------------------------------------------------------===//

def UPMEM_LaunchOp : UPMEM_Op<"launch", [
      AutomaticAllocationScope, 
      DeclareOpInterfaceMethods<InferIntRangeInterface>]>,
    Arguments<(ins Index:$rankSize, Index:$dpuSize, Index:$taskletSize)> {
  let summary = "UPMEM kernel launch operation";

  let description = [{
    Launch a kernel on the specified ranks, dpus of a rank, and tasklets of dpus.  
    The body of the kernel is defined by the single region that this operation contains. 
    Ultimately, the plan is to add the capability of operation taking an optional list of 
    async dependencies.

    TODO start --:
    The `async` keyword indicates the kernel should be launched asynchronously;
    the operation returns a new !upmem.async.token when the keyword is specified.
    The kernel launched does not start executing until the ops producing its
    async dependencies (optional operands) have completed.
    -- TODO end. 

    The first operand is the rank size that are allocated from the hardware.
    The next operand determines the number of dpus that are enabled from the 
    allocated rank, same for the last operand, setting the number of tasklets 
    per DPU.

  
    -- TODO start:
    The body region should be grouped as follows:
    -   three arguments that contain rank, dpu, tasklet identifiers 
    -   a variadic number of Workgroup memory attributions.
    -   a variadic number of Private memory attributions.

    Syntax:

    ```
    operation ::= `upmem.launch` (`async` (`[` ssa-id-list `]`)? )?
                             `ranks` `(` ssa-id-list `)` `in` ssa-reassignment
                             `dpus` `(` ssa-id-list `)` `in` ssa-reassignment
                             `tasklets` `(` ssa-id-list `)` `in` ssa-reassignment
                             memory-attribution
                             region attr-dict?
    ssa-reassignment ::= `(` ssa-id `=` ssa-use (`,` ssa-id `=` ssa-use)* `)`
    memory-attribution ::= (`pwram` `(` ssa-id-and-type-list `)`)?
                           (`twram` `(` ssa-id-and-type-list `)`)?
    ```

    Example:

    ```mlir
    upmem.launch ranks(%r) in (%sz_r = %0) dpus(%d) in (%sz_d = %1)
               tasklets(%t) in (%sz_t = %2) {
      // Rank, dpu, and thread identifiers, as well as rank/dpu/thread sizes are
      // immediately usable inside body region.
      "some_op"(%bx, %tx) : (index, index) -> ()
      // Assuming %val1 is defined outside the upmem.launch region.
      %42 = load %val1[%bx] : memref<?xf32, 1>
    }

    // Generic syntax explains how the pretty syntax maps to the IR structure.
    "upmem.launch"(%cst, %cst, %c1) // rank, dpu, tasklet sizes
        {/*attributes*/}
        // All sizes and identifiers have "index" size.
        : (index, index, index) -> () {
    // The operation passes rank, dpu, and tasklet identifiers, followed by rank, dpu, tasklet and
    // sizes.
    ^bb0(%r : index, %d : index, %t : index,
         %num_r : index, %num_d : index, %num_t : index)
      "some_op"(%bx, %tx) : (index, index) -> ()
      %3 = "memref.load"(%val1, %bx) : (memref<?xf32, 1>, index) -> f32
    }

    // Launch with memory attributions.
      upmem.launch ranks(%r) in (%sz_r = %0) dpus(%d) in (%sz_d = %1)
              tasklets(%t) in (%sz_t = %2) {
              pwram(%private_wram: memref<1xf32, 3>)
              twram(%total_wram: memref<32xf32, 5>) {
      "some_op"(%bx, %tx) : (index, index) -> ()
      // Assuming %val1 is defined outside the upmem.launch region.
      %42 = load %priavate_wram[%bx] : memref<32xf32, 3>
    }
    ```
  }];

  let regions = (region AnyRegion:$body);

  let skipDefaultBuilders = 1;

  let builders = [
    OpBuilder<(ins "Value":$rankSize, "Value":$dpuSize, "Value":$taskletSize,
      // CArg<"Type", "nullptr">:$asyncTokenType,
      // CArg<"ValueRange", "{}">:$asyncDependencies,
      CArg<"TypeRange", "{}">:$privateWRAMAttributions,
      CArg<"TypeRange", "{}">:$totalWRAMAttributions)>
  ];

  let extraClassDeclaration = [{
    /// Get the SSA value corresponding to kernel rank identifier.
    Value getRankId();
    /// Get the SSA value corresponding to kernel dpu identifier.
    Value getDPUId();
    /// Get the SSA value corresponding to kernel tasklet identifier.
    Value getTaskletId();

    /// Get the SSA value corresponding to kernel rank size.
    Value getRanksSize();
    /// Get the SSA value corresponding to kernel dpu size.
    Value getDPUsSize();
    /// Get the SSA value corresponding to kernel tasklet size.
    Value getTaskletsSize();

    /// Get the SSA value passed as operands to specify the rank size.
    Value getRanksSizeOperandValue();
    /// Get the SSA value passed as operands to specify the dpu size.
    Value getDPUsSizeOperandValue();
    /// Get the SSA value passed as operands to specify the tasklet size.
    Value getTaskletsSizeOperandValue();

    static StringRef getRanksKeyword() { return "ranks"; }
    static StringRef getDPUsKeyword() { return "dpus"; }
    static StringRef getTaskletsKeyword() { return "tasklets"; }

    /// The number of launch configuration operands, placed at the leading
    /// positions of the operand list.
    static constexpr unsigned kNumConfigOperands = 3;

    /// The number of region attributes containing the launch configuration,
    /// placed in the leading positions of the argument list.
    static constexpr unsigned kNumConfigRegionAttributes = 6;

    /// Returns the keywords used in the custom syntax for this Op.
    static StringRef getPrivateWRAMKeyword() { return "pwram"; }
    static StringRef getTotalWRAMKeyword() { return "twram"; }

    // /// Returns the number of buffers located in the workgroup memory.
    // unsigned getNumWorkgroupAttributions() {
    //   auto attr = (*this)->getAttrOfType<IntegerAttr>(
    //       getNumWorkgroupAttributionsAttrName());
    //   return attr ? attr.getInt() : 0;
    // }

    // /// Returns a list of block arguments that correspond to buffers located in
    // /// the workgroup memory
    // ArrayRef<BlockArgument> getWorkgroupAttributions() {
    //   auto begin =
    //       std::next(getBody().args_begin(), kNumConfigRegionAttributes);
    //   auto end = std::next(begin, getNumWorkgroupAttributions());
    //   return {begin, end};
    // }

    // /// Adds a new block argument that corresponds to buffers located in
    // /// workgroup memory.
    // BlockArgument addWorkgroupAttribution(Type type, Location loc);

    // /// Returns the number of buffers located in the private memory.
    // unsigned getNumPrivateAttributions() {
    //   return getBody().getNumArguments() - kNumConfigRegionAttributes -
    //       getNumWorkgroupAttributions();
    // }

    // /// Returns a list of block arguments that correspond to buffers located in
    // /// the private memory.
    // ArrayRef<BlockArgument> getPrivateAttributions() {
    //   // Buffers on the private memory always come after buffers on the workgroup
    //   // memory.
    //   auto begin =
    //       std::next(getBody().args_begin(),
    //                 kNumConfigRegionAttributes + getNumWorkgroupAttributions());
    //   return {begin, getBody().args_end()};
    // }

    // /// Adds a new block argument that corresponds to buffers located in
    // /// private memory.
    // BlockArgument addPrivateAttribution(Type type, Location loc);

    // /// Returns the name of the attribute containing the number of buffers
    // /// located in the workgroup memory.
    // static StringRef getNumWorkgroupAttributionsAttrName() {
    //   return "workgroup_attributions";
    // }
  }];

  let hasCanonicalizer = 1;
  let hasCustomAssemblyFormat = 1;
  let hasRegionVerifier = 1;
}



def UPMEM_TerminatorOp : UPMEM_Op<"terminator", [HasParent<"LaunchOp">,
                                             Pure, Terminator]>,
    Arguments<(ins)>, Results<(outs)> {
  let summary = "Terminator for UPMEM launch regions.";
  let description = [{
    A terminator operation for regions that appear in the body of `upmem.launch`
    operation.  These regions are not expected to return any value so the
    terminator takes no operands.
  }];

  let assemblyFormat = "attr-dict";
}


def UPMEM_PrivateWRAMAllocOp : UPMEM_Op<"pwram_alloc", [
    // AttrSizedOperandSegments
  ]> {

  let summary = "UPMEM WRAM (fast scratchpad) memory allocation operation for a single tasklet.";
  let description = [{
    The `upmem.pwram_alloc` operation allocates a wram region that is supposed to be only accessible 
    to a single tasklet. In reality, no constraints is placed on the memory itself, but this space 
    is allocated by an allocator that allocates WRAM for each tasklet seperately and the addresses 
    are assigned automatically. In the original C interface, it is possible to access the other 
    tasklet space, but the goal here is to make it more abstract, simpler, and to not reduce the power 
    as well. 
    Interface-wise, it is similar to the `memref.alloc` op.
    TODO: support async 

    Example:

    ```mlir
    %memref = upmem.pwram_alloc (%width) : memref<64x?xf32, 1>
    ```
  }];

  // let arguments = (ins Variadic<Index>:$dynamicSizes, Variadic<Index>:$symbolOperands);
  // let arguments = ();
  let results = (outs Res<AnyMemRef, "", [MemAlloc]>:$memref);

  let extraClassDeclaration = [{
    MemRefType getType() { return ::llvm::cast<MemRefType>(getMemref().getType()); }
  }];

    // `(` $dynamicSizes `)` (`` `[` $symbolOperands^ `]`)? attr-dict `:` type($memref)
  let assemblyFormat = [{
     attr-dict `:` type($memref)
  }];

  // let hasVerifier = 1;
  // let hasCanonicalizer = 1;
}



def UPMEM_MemcpyOpMRAMToWRAM: I32EnumAttrCase<"MRAMToWRAM", 0, "mram_to_wram">;
def UPMEM_MemcpyOpWRAMToMRAM: I32EnumAttrCase<"WRAMToMRAM", 1, "wram_to_mram">;

def MemcpyDir : I32EnumAttr<"MemcpyDirOp",
  "copy operation between wram and mram", [
    UPMEM_MemcpyOpMRAMToWRAM,
    UPMEM_MemcpyOpWRAMToMRAM
  ]> {
  let genSpecializedAttr = 0;
  let cppNamespace = "::mlir::upmem";
}
def MemcpyDirAttr : EnumAttr<UPMEM_Dialect, MemcpyDir,
                                  "memcpy_dir">;


def UPMEM_MemcpyOp : UPMEM_Op<"memcpy", []> {

  let summary = "UPMEM memcpy operation";

  let description = [{
    The `upmem.memcpy` operation copies the content of one memref to another.

    The op does not execute before all async dependencies have finished
    executing.
copy_to_mram
    If the `async` keyword is present, the op is executed asynchronously (i.e.
    it does not block until the execution has finished on the device). In
    that case, it returns a !upmem.async.token.

    Example:

    ```mlir
    %token = upmem.memcpy async [%dep] %dst, %src : memref<?xf32, 1>, memref<?xf32>
    ```
  }];

  let arguments = (ins MemcpyDirAttr:$direction,
                   Arg<AnyMemRef, "", [MemWrite]>:$buffer,
                   Index:$size, Index:$offset);

  let assemblyFormat = [{
    $direction $buffer`,` $size`,` $offset `:` type($buffer)`,` type($size)`,` type($offset) attr-dict
  }];

  // let hasFolder = 1;
  // let hasVerifier = 1;
  // let hasCanonicalizer = 1;
}



#endif // UPMEM_BASE
