module attributes {llvm.data_layout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-i64:64:64-n32", llvm.target_triple = "dpu-upmem-dpurte"} {
  llvm.func @llvm.dpu.tid.i32() -> i32
  llvm.func @mem_reset() -> !llvm.ptr<i8>
  llvm.func @main() attributes {dso_local} {
    %0 = llvm.call @llvm.dpu.tid.i32() : () -> i32
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.icmp "eq" %0, %1 : i32
    llvm.cond_br %2, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %3 = llvm.call @mem_reset() : () -> !llvm.ptr<i8>
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    llvm.return
  }
}

