; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-i64:64:64-n32"
target triple = "dpu-upmem-dpurte"


; Function Attrs: nounwind memory(none)
declare i32 @llvm.dpu.tid.i32() #0

declare ptr addrspace(32) @mem_reset()

define dso_local void @main() {
  %1 = call i32 @llvm.dpu.tid.i32()
  %2 = icmp eq i32 %1, 0
  br i1 %2, label %3, label %5

3:                                                ; preds = %0
  %4 = call ptr addrspace(32) @mem_reset()
  br label %5

5:                                                ; preds = %3, %0
  ret void
}

attributes #0 = { nounwind memory(none) }

!llvm.module.flags = !{!0}

!0 = !{i32 2, !"Debug Info Version", i32 3}
