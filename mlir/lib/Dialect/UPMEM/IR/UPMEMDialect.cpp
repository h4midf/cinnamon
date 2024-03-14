//===- UPMEMDialect.cpp - MLIR Dialect for UPMEM Kernels implementation -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the UPMEM kernel-related dialect and its operations.
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/UPMEM/IR/UPMEMDialect.h"

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/IR/Attributes.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/DialectImplementation.h"
#include "mlir/IR/Matchers.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/IR/TypeUtilities.h"
#include "mlir/Interfaces/FunctionImplementation.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Transforms/InliningUtils.h"
#include "llvm/ADT/TypeSwitch.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/StringSaver.h"

using namespace mlir;
using namespace mlir::upmem;

#include "mlir/Dialect/UPMEM/IR/UPMEMOpsDialect.cpp.inc"

// //===----------------------------------------------------------------------===//
// // UPMEM Device Mapping Attributes
// //===----------------------------------------------------------------------===//

// int64_t UPMEMBlockMappingAttr::getMappingId() const {
//   return static_cast<int64_t>(getBlock());
// }

// bool UPMEMBlockMappingAttr::isLinearMapping() const {
//   return getMappingId() >= static_cast<int64_t>(MappingId::LinearDim0);
// }

// int64_t UPMEMBlockMappingAttr::getRelativeIndex() const {
//   return isLinearMapping()
//              ? getMappingId() - static_cast<int64_t>(MappingId::LinearDim0)
//              : getMappingId();
// }

// int64_t UPMEMWarpgroupMappingAttr::getMappingId() const {
//   return static_cast<int64_t>(getWarpgroup());
// }

// bool UPMEMWarpgroupMappingAttr::isLinearMapping() const {
//   return getMappingId() >= static_cast<int64_t>(MappingId::LinearDim0);
// }

// int64_t UPMEMWarpgroupMappingAttr::getRelativeIndex() const {
//   return isLinearMapping()
//              ? getMappingId() - static_cast<int64_t>(MappingId::LinearDim0)
//              : getMappingId();
// }

// int64_t UPMEMWarpMappingAttr::getMappingId() const {
//   return static_cast<int64_t>(getWarp());
// }

// bool UPMEMWarpMappingAttr::isLinearMapping() const {
//   return getMappingId() >= static_cast<int64_t>(MappingId::LinearDim0);
// }

// int64_t UPMEMWarpMappingAttr::getRelativeIndex() const {
//   return isLinearMapping()
//              ? getMappingId() - static_cast<int64_t>(MappingId::LinearDim0)
//              : getMappingId();
// }

// int64_t UPMEMThreadMappingAttr::getMappingId() const {
//   return static_cast<int64_t>(getThread());
// }

// bool UPMEMThreadMappingAttr::isLinearMapping() const {
//   return getMappingId() >= static_cast<int64_t>(MappingId::LinearDim0);
// }

// int64_t UPMEMThreadMappingAttr::getRelativeIndex() const {
//   return isLinearMapping()
//              ? getMappingId() - static_cast<int64_t>(MappingId::LinearDim0)
//              : getMappingId();
// }

// int64_t UPMEMMemorySpaceMappingAttr::getMappingId() const {
//   return static_cast<int64_t>(getAddressSpace());
// }

// bool UPMEMMemorySpaceMappingAttr::isLinearMapping() const {
//   llvm_unreachable("UPMEMMemorySpaceMappingAttr does not support linear mapping");
// }

// int64_t UPMEMMemorySpaceMappingAttr::getRelativeIndex() const {
//   llvm_unreachable("UPMEMMemorySpaceMappingAttr does not support relative index");
// }


//===----------------------------------------------------------------------===//
// UPMEMDialect
//===----------------------------------------------------------------------===//

//   UnitAttr isKernelAttr = op->getAttrOfType<UnitAttr>(getKernelFuncAttrName());
//   return static_cast<bool>(isKernelAttr);
// }

namespace {
/// This class defines the interface for handling inlining with upmem
/// operations.
struct UPMEMInlinerInterface : public DialectInlinerInterface {
  using DialectInlinerInterface::DialectInlinerInterface;

  /// All upmem dialect ops can be inlined.
  bool isLegalToInline(Operation *, Region *, bool, IRMapping &) const final {
    return true;
  }
};
} // namespace

void UPMEMDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "mlir/Dialect/UPMEM/IR/UPMEMOps.cpp.inc"
      >();
  addAttributes<
#define GET_ATTRDEF_LIST
#include "mlir/Dialect/UPMEM/IR/UPMEMOpsAttributes.cpp.inc"
      >();
  addInterfaces<UPMEMInlinerInterface>();
}


Type UPMEMDialect::parseType(DialectAsmParser &parser) const {
  // Parse the main keyword for the type.
  StringRef keyword;
  if (parser.parseKeyword(&keyword))
    return Type();
  // MLIRContext *context = getContext();

  parser.emitError(parser.getNameLoc(), "unknown upmem type: " + keyword);
  return Type();
}
// TODO: print refined type here. Notice that should be corresponding to the
// parser
void UPMEMDialect::printType(Type type, DialectAsmPrinter &os) const {
}

LogicalResult UPMEMDialect::verifyOperationAttribute(Operation *op,
                                                   NamedAttribute attr) {
  // if (!llvm::isa<UnitAttr>(attr.getValue()) ||
  //     attr.getName() != getContainerModuleAttrName())
  //   return success();

  // auto module = dyn_cast<ModuleOp>(op);
  // if (!module)
  //   return op->emitError("expected '")
  //          << getContainerModuleAttrName() << "' attribute to be attached to '"
  //          << ModuleOp::getOperationName() << '\'';

  // auto walkResult = module.walk([&module](LaunchFuncOp launchOp) -> WalkResult {
  //   // Ignore launches that are nested more or less deep than functions in the
  //   // module we are currently checking.
  //   if (!launchOp->getParentOp() ||
  //       launchOp->getParentOp()->getParentOp() != module)
  //     return success();

  //   // Ignore launch ops with missing attributes here. The errors will be
  //   // reported by the verifiers of those ops.
  //   if (!launchOp->getAttrOfType<SymbolRefAttr>(
  //           LaunchFuncOp::getKernelAttrName(launchOp->getName())))
  //     return success();

  //   // Check that `launch_func` refers to a well-formed UPMEM kernel container.
  //   StringAttr kernelContainerName = launchOp.getKernelModuleName();
  //   Operation *kernelContainer = module.lookupSymbol(kernelContainerName);
  //   if (!kernelContainer)
  //     return launchOp.emitOpError()
  //            << "kernel container '" << kernelContainerName.getValue()
  //            << "' is undefined";

  //   // If the container is a UPMEM binary op return success.
  //   if (isa<BinaryOp>(kernelContainer))
  //     return success();

  //   auto kernelModule = dyn_cast<UPMEMModuleOp>(kernelContainer);
  //   if (!kernelModule)
  //     return launchOp.emitOpError()
  //            << "kernel module '" << kernelContainerName.getValue()
  //            << "' is undefined";

  //   // Check that `launch_func` refers to a well-formed kernel function.
  //   Operation *kernelFunc = module.lookupSymbol(launchOp.getKernelAttr());
  //   if (!kernelFunc)
  //     return launchOp.emitOpError("kernel function '")
  //            << launchOp.getKernel() << "' is undefined";
  //   auto kernelConvertedFunction = dyn_cast<FunctionOpInterface>(kernelFunc);
  //   if (!kernelConvertedFunction) {
  //     InFlightDiagnostic diag = launchOp.emitOpError()
  //                               << "referenced kernel '" << launchOp.getKernel()
  //                               << "' is not a function";
  //     diag.attachNote(kernelFunc->getLoc()) << "see the kernel definition here";
  //     return diag;
  //   }

  //   if (!kernelFunc->getAttrOfType<mlir::UnitAttr>(
  //           UPMEMDialect::getKernelFuncAttrName()))
  //     return launchOp.emitOpError("kernel function is missing the '")
  //            << UPMEMDialect::getKernelFuncAttrName() << "' attribute";

  //   // TODO: If the kernel isn't a UPMEM function (which happens during separate
  //   // compilation), do not check type correspondence as it would require the
  //   // verifier to be aware of the type conversion.
  //   auto kernelUPMEMFunction = dyn_cast<upmem::UPMEMFuncOp>(kernelFunc);
  //   if (!kernelUPMEMFunction)
  //     return success();

  //   unsigned actualNumArguments = launchOp.getNumKernelOperands();
  //   unsigned expectedNumArguments = kernelUPMEMFunction.getNumArguments();
  //   if (expectedNumArguments != actualNumArguments)
  //     return launchOp.emitOpError("got ")
  //            << actualNumArguments << " kernel operands but expected "
  //            << expectedNumArguments;

  //   auto functionType = kernelUPMEMFunction.getFunctionType();
  //   for (unsigned i = 0; i < expectedNumArguments; ++i) {
  //     if (launchOp.getKernelOperand(i).getType() != functionType.getInput(i)) {
  //       return launchOp.emitOpError("type of function argument ")
  //              << i << " does not match";
  //     }
  //   }

  //   return success();
  // });

  // return walkResult.wasInterrupted() ? failure() : success();
  return success();
}


// UPMEM Memory attributions functions shared by LaunchOp and UPMEMFuncOp.
/// Parses a UPMEM function memory attribution.
///
/// memory-attribution ::= (`workgroup` `(` ssa-id-and-type-list `)`)?
///                        (`private` `(` ssa-id-and-type-list `)`)?
///
/// Note that this function parses only one of the two similar parts, with the
/// keyword provided as argument.
// static ParseResult
// parseAttributions(OpAsmParser &parser, StringRef keyword,
//                   SmallVectorImpl<OpAsmParser::Argument> &args) {
//   // If we could not parse the keyword, just assume empty list and succeed.
//   if (failed(parser.parseOptionalKeyword(keyword)))
//     return success();

//   return parser.parseArgumentList(args, OpAsmParser::Delimiter::Paren,
//                                   /*allowType=*/true);
// }

// /// Prints a UPMEM function memory attribution.
// static void printAttributions(OpAsmPrinter &p, StringRef keyword,
//                               ArrayRef<BlockArgument> values) {
//   // if (values.empty())
//   //   return;

//   // p << ' ' << keyword << '(';
//   // llvm::interleaveComma(
//   //     values, p, [&p](BlockArgument v) { p << v << " : " << v.getType(); });
//   // p << ')';
// }

/// Verifies a UPMEM function memory attribution.
// static LogicalResult verifyAttributions(Operation *op,
//                                         ArrayRef<BlockArgument> attributions,
//                                         upmem::AddressSpace memorySpace) {
  // for (Value v : attributions) {
  //   auto type = llvm::dyn_cast<MemRefType>(v.getType());
  //   if (!type)
  //     return op->emitOpError() << "expected memref type in attribution";

  //   // We can only verify the address space if it hasn't already been lowered
  //   // from the AddressSpaceAttr to a target-specific numeric value.
  //   auto addressSpace =
  //       llvm::dyn_cast_or_null<upmem::AddressSpaceAttr>(type.getMemorySpace());
  //   if (!addressSpace)
  //     continue;
  //   if (addressSpace.getValue() != memorySpace)
  //     return op->emitOpError()
  //            << "expected memory space " << stringifyAddressSpace(memorySpace)
  //            << " in attribution";
  // }
//   return success();
// }


//===----------------------------------------------------------------------===//
// LaunchOp
//===----------------------------------------------------------------------===//

void LaunchOp::build(OpBuilder &builder, OperationState &result,
                     Value rankSize, Value dpuSize, Value taskletSize,
                     TypeRange privateWRAMAttributions,
                     TypeRange totalWRAMAttributions) {
  // Add a WorkGroup attribution attribute. This attribute is required to
  // identify private attributions in the list of block argguments.
  // result.addAttribute(getNumWorkgroupAttributionsAttrName(),
  //                     builder.getI64IntegerAttr(workgroupAttributions.size()));

  // Add grid and block sizes as op operands, followed by the data operands.
  result.addOperands({rankSize, dpuSize, taskletSize});

  // Create a kernel body region with kNumConfigRegionAttributes + N memory
  // attributions, where the first kNumConfigRegionAttributes arguments have
  // `index` type and the rest have the same types as the data operands.
  Region *kernelRegion = result.addRegion();
  Block *body = new Block();
  // TODO: Allow passing in proper locations here.
  for (unsigned i = 0; i < kNumConfigRegionAttributes; ++i)
    body->addArgument(builder.getIndexType(), result.location);
  // Add WorkGroup & Private attributions to the region arguments.
  // for (Type argTy : workgroupAttributions)
  //   body->addArgument(argTy, result.location);
  // for (Type argTy : privateAttributions)
  //   body->addArgument(argTy, result.location);
  kernelRegion->push_back(body);
  // Fill OperandSegmentSize Attribute.
  SmallVector<int32_t, 3> segmentSizes(3, 1);
  segmentSizes.front() = 0;
  segmentSizes.back() = 0;
  // segmentSizes.front() = asyncDependencies.size();
  // segmentSizes.back() = dynamicSharedMemorySize ? 1 : 0;
  // result.addAttribute(getOperandSegmentSizeAttr(),
  //                     builder.getDenseI32ArrayAttr(segmentSizes));
}

Value LaunchOp::getRankId() {
  assert(!getBody().empty() && "LaunchOp body must not be empty.");
  auto args = getBody().getArguments();
  return args[0];
}

Value LaunchOp::getDPUId() {
  assert(!getBody().empty() && "LaunchOp body must not be empty.");
  auto args = getBody().getArguments();
  return args[1];
}

Value LaunchOp::getTaskletId() {
  assert(!getBody().empty() && "LaunchOp body must not be empty.");
  auto args = getBody().getArguments();
  return args[2];
}

Value LaunchOp::getRanksSize() {
  assert(!getBody().empty() && "LaunchOp body must not be empty.");
  auto args = getBody().getArguments();
  return args[3];
}

Value LaunchOp::getDPUsSize() {
  assert(!getBody().empty() && "LaunchOp body must not be empty.");
  auto args = getBody().getArguments();
  return args[4];
}

Value LaunchOp::getTaskletsSize() {
  assert(!getBody().empty() && "LaunchOp body must not be empty.");
  auto args = getBody().getArguments();
  return args[5];
}

Value LaunchOp::getRanksSizeOperandValue() {
  auto operands = getOperands();
  return operands[0];
}

Value LaunchOp::getDPUsSizeOperandValue() {
  auto operands = getOperands();
  return operands[1];
}

Value LaunchOp::getTaskletsSizeOperandValue() {
  auto operands = getOperands();
  return operands[2];
}

LogicalResult LaunchOp::verifyRegions() {
  // Kernel launch takes kNumConfigOperands leading operands for rank/dpu/tasklet
  // sizes and transforms them into kNumConfigRegionAttributes region arguments
  // for rank/dpu/tasklet identifiers and grid/block sizes.
  if (!getBody().empty()) {
    if (getBody().getNumArguments() <
        kNumConfigRegionAttributes  )
        // kNumConfigRegionAttributes + getNumWorkgroupAttributions())
      return emitOpError("unexpected number of region arguments");
  }

  // // Verify Attributions Address Spaces.
  // if (failed(verifyAttributions(getOperation(), getWorkgroupAttributions(),
  //                               GPUDialect::getWorkgroupAddressSpace())) ||
  //     failed(verifyAttributions(getOperation(), getPrivateAttributions(),
  //                               GPUDialect::getPrivateAddressSpace())))
  //   return failure();

  // Block terminators without successors are expected to exit the kernel region
  // and must be `gpu.terminator`.
  for (Block &block : getBody()) {
    if (block.empty())
      continue;
    if (block.back().getNumSuccessors() != 0)
      continue;
    if (!isa<upmem::TerminatorOp>(&block.back())) {
      return block.back()
          .emitError()
          .append("expected '", upmem::TerminatorOp::getOperationName(),
                  "' or a terminator with successors")
          .attachNote(getLoc())
          .append("in '", LaunchOp::getOperationName(), "' body region");
    }
  }

  // if (getNumResults() == 0 && getAsyncToken())
  //   return emitOpError("needs to be named when async keyword is specified");

  return success();
}

// Pretty-print the kernel grid/block size assignment as
//   (%iter-x, %iter-y, %iter-z) in
//   (%size-x = %ssa-use, %size-y = %ssa-use, %size-z = %ssa-use)
// where %size-* and %iter-* will correspond to the body region arguments.
static void printSizeAssignment(OpAsmPrinter &p, Value size,
                                Value operand, Value id) {
  p << '(' << id << ") in (";
  p << size << " = " << operand << ')';
}

void LaunchOp::print(OpAsmPrinter &p) {

  p << ' ' << getRanksKeyword();
  printSizeAssignment(p, getRanksSize(), getRanksSizeOperandValue(),
                      getRankId());

  p << ' ' << getDPUsKeyword();
  printSizeAssignment(p, getDPUsSize(), getDPUsSizeOperandValue(),
                      getDPUId());

  p << ' ' << getTaskletsKeyword();
  printSizeAssignment(p, getTaskletsSize(), getTaskletsSizeOperandValue(),
                      getTaskletId());

  // // printAttributions(p, getWorkgroupKeyword(), getWorkgroupAttributions());
  // // printAttributions(p, getPrivateKeyword(), getPrivateAttributions());

  p << ' ';

  p.printRegion(getBody(), /*printEntryBlockArgs=*/false);
}

// Parse the size assignment blocks for blocks and threads.  These have the form
//   (%region_arg, %region_arg, %region_arg) in
//   (%region_arg = %operand, %region_arg = %operand, %region_arg = %operand)
// where %region_arg are percent-identifiers for the region arguments to be
// introduced further (SSA defs), and %operand are percent-identifiers for the
// SSA value uses.
static ParseResult
parseSizeAssignment(OpAsmParser &parser,
                    MutableArrayRef<OpAsmParser::UnresolvedOperand> sizes,
                    MutableArrayRef<OpAsmParser::UnresolvedOperand> regionSizes,
                    MutableArrayRef<OpAsmParser::UnresolvedOperand> indices) {
  // assert(indices.size() == 3 && "space for three indices expected");
  assert(indices.size() == 1 && "space for one indices expected");
  
  SmallVector<OpAsmParser::UnresolvedOperand, 3> args;
  if (parser.parseOperandList(args, OpAsmParser::Delimiter::Paren,
                              /*allowResultNumber=*/false) ||
      parser.parseKeyword("in") || parser.parseLParen())
    return failure();
  std::move(args.begin(), args.end(), indices.begin());

  if (parser.parseOperand(regionSizes[0], /*allowResultNumber=*/false) ||
      parser.parseEqual() || parser.parseOperand(sizes[0]))
    return failure();

  return parser.parseRParen();
}

/// Parses a Launch operation.
/// operation ::= `gpu.launch` (`async` `[` ssa-id-list `]`)?
///       `blocks` `(` ssa-id-list `)` `in` ssa-reassignment
///       `threads` `(` ssa-id-list `)` `in` ssa-reassignment
///       memory-attribution
///       region attr-dict?
/// ssa-reassignment ::= `(` ssa-id `=` ssa-use (`,` ssa-id `=` ssa-use)* `)`
ParseResult LaunchOp::parse(OpAsmParser &parser, OperationState &result) {
  // Sizes of the grid and block.
  SmallVector<OpAsmParser::UnresolvedOperand, LaunchOp::kNumConfigOperands>
      sizes(LaunchOp::kNumConfigOperands);
  MutableArrayRef<OpAsmParser::UnresolvedOperand> sizesRef(sizes);

  // Actual (data) operands passed to the kernel.
  SmallVector<OpAsmParser::UnresolvedOperand, 4> dataOperands;

  // Region arguments to be created.
  SmallVector<OpAsmParser::UnresolvedOperand, 16> regionArgs(
      LaunchOp::kNumConfigRegionAttributes);
  MutableArrayRef<OpAsmParser::UnresolvedOperand> regionArgsRef(regionArgs);

  // Parse optional async dependencies.
  SmallVector<OpAsmParser::UnresolvedOperand, 4> asyncDependencies;
  

  // Parse the size assignment segments: the first segment assigns grid sizes
  // and defines values for block identifiers; the second segment assigns block
  // sizes and defines values for thread identifiers.  In the region argument
  // list, identifiers precede sizes, and block-related values precede
  // thread-related values.
  if (parser.parseKeyword(LaunchOp::getRanksKeyword().data()) ||

      parseSizeAssignment(parser, sizesRef.take_front(1),
                          regionArgsRef.slice(3, 1),
                          regionArgsRef.slice(0, 1)) ||
      parser.parseKeyword(LaunchOp::getDPUsKeyword().data()) ||
      parseSizeAssignment(parser, sizesRef.drop_front(1),
                          regionArgsRef.slice(4, 1),
                          regionArgsRef.slice(1, 1)) ||
      parser.parseKeyword(LaunchOp::getTaskletsKeyword().data()) ||
      parseSizeAssignment(parser, sizesRef.drop_front(2),
                          regionArgsRef.slice(5, 1),
                          regionArgsRef.slice(2, 1)) ||
      parser.resolveOperands(sizes, parser.getBuilder().getIndexType(),
                             result.operands))
    return failure();


  // Create the region arguments, it has kNumConfigRegionAttributes arguments
  // that correspond to block/thread identifiers and grid/block sizes, all
  // having `index` type, a variadic number of WorkGroup Attributions and
  // a variadic number of Private Attributions. The number of WorkGroup
  // Attributions is stored in the attr with name:
  // LaunchOp::getNumWorkgroupAttributionsAttrName().
  Type index = parser.getBuilder().getIndexType();
  SmallVector<Type, LaunchOp::kNumConfigRegionAttributes> dataTypes(
      LaunchOp::kNumConfigRegionAttributes, index);

  SmallVector<OpAsmParser::Argument> regionArguments;
  for (auto ssaValueAndType : llvm::zip(regionArgs, dataTypes)) {
    OpAsmParser::Argument arg;
    arg.ssaName = std::get<0>(ssaValueAndType);
    arg.type = std::get<1>(ssaValueAndType);
    regionArguments.push_back(arg);
  }

  // Builder &builder = parser.getBuilder();

  // Introduce the body region and parse it. The region has
  // kNumConfigRegionAttributes arguments that correspond to
  // block/thread identifiers and grid/block sizes, all having `index` type.
  Region *body = result.addRegion();
  if (parser.parseRegion(*body, regionArguments) ||
      parser.parseOptionalAttrDict(result.attributes))
    return failure();

  SmallVector<int32_t, 8> segmentSizes(8, 1);
  // segmentSizes.front() = asyncDependencies.size();
  // segmentSizes.back() = hasDynamicSharedMemorySize ? 1 : 0;

  segmentSizes.front() = 0;
  segmentSizes.back() = 0;

  return success();
}

/// Simplify the gpu.launch when the range of a thread or block ID is
/// trivially known to be one.
struct FoldLaunchArguments : public OpRewritePattern<LaunchOp> {
  using OpRewritePattern<LaunchOp>::OpRewritePattern;
  LogicalResult matchAndRewrite(LaunchOp op,
                                PatternRewriter &rewriter) const override {
    // If the range implies a single value for `id`, replace `id`'s uses by
    // zero.
    Value zero;
    bool simplified = false;
    auto constPropIdUses = [&](Value id, Value size) {
      // Check if size is trivially one.
      if (!matchPattern(size, m_One()))
        return;
      if (id.getUses().empty())
        return;
      if (!simplified) {
        // Create a zero value the first time.
        OpBuilder::InsertionGuard guard(rewriter);
        rewriter.setInsertionPointToStart(&op.getBody().front());
        zero =
            rewriter.create<arith::ConstantIndexOp>(op.getLoc(), /*value=*/0);
      }
      rewriter.replaceAllUsesWith(id, zero);
      simplified = true;
    };
    constPropIdUses(op.getRankId(), op.getRanksSize());
    constPropIdUses(op.getDPUId(), op.getDPUsSize());
    constPropIdUses(op.getTaskletId(), op.getTaskletsSize());
    return success(simplified);
  }
};

void LaunchOp::getCanonicalizationPatterns(RewritePatternSet &rewrites,
                                           MLIRContext *context) {
  rewrites.add<FoldLaunchArguments>(context);
}



//===----------------------------------------------------------------------===//
// UPMEM_PrivateWRAMAllocOp
//===----------------------------------------------------------------------===//

// LogicalResult PrivateWRAMAllocOp::verify() {
//   auto memRefType = llvm::cast<MemRefType>(getMemref().getType());

//   // if (static_cast<int64_t>(getDynamicSizes().size()) !=
//   //     memRefType.getNumDynamicDims())
//   //   return emitOpError("dimension operand count does not equal memref "
//   //                      "dynamic dimension count");

//   // unsigned numSymbols = 0;
//   // if (!memRefType.getLayout().isIdentity())
//   //   numSymbols = memRefType.getLayout().getAffineMap().getNumSymbols();
//   // if (getSymbolOperands().size() != numSymbols) {
//   //   return emitOpError(
//   //       "symbol operand count does not equal memref symbol count");
//   // }

//   return success();
// }

// namespace {

// /// Folding of memref.dim(upmem.alloc(%size), %idx) -> %size similar to
// /// `memref::AllocOp`.
// struct SimplifyDimOfPrivateWRAMAllocOp : public OpRewritePattern<memref::DimOp> {
//   using OpRewritePattern<memref::DimOp>::OpRewritePattern;

//   LogicalResult matchAndRewrite(memref::DimOp dimOp,
//                                 PatternRewriter &rewriter) const override {
//     std::optional<int64_t> index = dimOp.getConstantIndex();
//     if (!index)
//       return failure();

//     auto memrefType = llvm::dyn_cast<MemRefType>(dimOp.getSource().getType());
//     if (!memrefType || !memrefType.isDynamicDim(index.value()))
//       return failure();

//     auto alloc = dimOp.getSource().getDefiningOp<>();
//     if (!alloc)
//       return failure();

//     Value substituteOp = *(alloc.getDynamicSizes().begin() +
//                            memrefType.getDynamicDimIndex(index.value()));
//     rewriter.replaceOp(dimOp, substituteOp);
//     return success();
//   }
// };

// } // namespace

// void PrivateWRAMAllocOp::getCanonicalizationPatterns(RewritePatternSet &results,
//                                           MLIRContext *context) {
//   // results.add<SimplifyDimOfAllocOp>(context);
// }





// MLIR_DEFINE_EXPLICIT_TYPE_ID(::mlir::upmem::TargetOptions)

#include "mlir/Dialect/UPMEM/IR/UPMEMOpInterfaces.cpp.inc"
#include "mlir/Dialect/UPMEM/IR/UPMEMOpsEnums.cpp.inc"

#define GET_ATTRDEF_CLASSES
#include "mlir/Dialect/UPMEM/IR/UPMEMOpsAttributes.cpp.inc"

#define GET_OP_CLASSES
#include "mlir/Dialect/UPMEM/IR/UPMEMOps.cpp.inc"

// #include "mlir/Dialect/UPMEM/IR/CompilationAttrInterfaces.cpp.inc"
