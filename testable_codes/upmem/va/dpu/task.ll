; ModuleID = 'task.c'
source_filename = "task.c"
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-i64:64:64-n32"
target triple = "dpu-upmem-dpurte"

module asm ".section .data.my_barrier"
module asm ".type my_barrier,@object"
module asm ".globl my_barrier"
module asm ".p2align 2"
module asm "my_barrier:"
module asm ".byte 0xFF"
module asm ".byte 1"
module asm ".byte 1"
module asm ".byte __atomic_bit_barrier_my_barrier"
module asm ".size my_barrier, 4"
module asm ".text"
module asm ".section .data.my_barrier2"
module asm ".type my_barrier2,@object"
module asm ".globl my_barrier2"
module asm ".p2align 2"
module asm "my_barrier2:"
module asm ".byte 0xFF"
module asm ".byte 1"
module asm ".byte 1"
module asm ".byte __atomic_bit_barrier_my_barrier2"
module asm ".size my_barrier2, 4"
module asm ".text"

%struct.barrier_t = type { i8, i8, i8, i8 }

@my_barrier = external dso_local global %struct.barrier_t, align 1
@my_barrier2 = external dso_local global %struct.barrier_t, align 1
@__atomic_bit_barrier_my_barrier = dso_local global i8 0, section ".atomic", align 1, !dbg !0
@__atomic_bit_barrier_my_barrier2 = dso_local global i8 0, section ".atomic", align 1, !dbg !6

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 !dbg !15 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata i32* %2, metadata !19, metadata !DIExpression()), !dbg !21
  %3 = call i32 @me(), !dbg !22
  store i32 %3, i32* %2, align 4, !dbg !21
  %4 = load i32, i32* %2, align 4, !dbg !23
  %5 = icmp eq i32 %4, 0, !dbg !25
  br i1 %5, label %6, label %8, !dbg !26

6:                                                ; preds = %0
  %7 = call i8* @mem_reset(), !dbg !27
  br label %8, !dbg !29

8:                                                ; preds = %6, %0
  call void @barrier_wait(%struct.barrier_t* @my_barrier), !dbg !30
  call void @barrier_wait(%struct.barrier_t* @my_barrier2), !dbg !31
  ret i32 0, !dbg !32
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i8* @mem_reset() #2

declare dso_local void @barrier_wait(%struct.barrier_t*) #2

; Function Attrs: noinline nounwind optnone
define internal i32 @me() #0 !dbg !33 {
  %1 = call i32 @llvm.dpu.tid.i32(), !dbg !39
  ret i32 %1, !dbg !40
}

; Function Attrs: nounwind readnone
declare i32 @llvm.dpu.tid.i32() #3

attributes #0 = { noinline nounwind optnone "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!11, !12, !13}
!llvm.ident = !{!14}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "__atomic_bit_barrier_my_barrier", scope: !2, file: !3, line: 19, type: !8, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, globals: !5, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "task.c", directory: "/root/cinnamon/testable_codes/upmem/va/dpu")
!4 = !{}
!5 = !{!0, !6}
!6 = !DIGlobalVariableExpression(var: !7, expr: !DIExpression())
!7 = distinct !DIGlobalVariable(name: "__atomic_bit_barrier_my_barrier2", scope: !2, file: !3, line: 20, type: !8, isLocal: false, isDefinition: true)
!8 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !9, line: 40, baseType: !10)
!9 = !DIFile(filename: "upmem_toolkit/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/stdlib/stdint.h", directory: "/root")
!10 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!11 = !{i32 7, !"Dwarf Version", i32 4}
!12 = !{i32 2, !"Debug Info Version", i32 3}
!13 = !{i32 1, !"wchar_size", i32 1}
!14 = !{!"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)"}
!15 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 23, type: !16, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!16 = !DISubroutineType(types: !17)
!17 = !{!18}
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = !DILocalVariable(name: "tasklet_id", scope: !15, file: !3, line: 24, type: !20)
!20 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!21 = !DILocation(line: 24, column: 15, scope: !15)
!22 = !DILocation(line: 24, column: 28, scope: !15)
!23 = !DILocation(line: 25, column: 6, scope: !24)
!24 = distinct !DILexicalBlock(scope: !15, file: !3, line: 25, column: 6)
!25 = !DILocation(line: 25, column: 17, scope: !24)
!26 = !DILocation(line: 25, column: 6, scope: !15)
!27 = !DILocation(line: 26, column: 3, scope: !28)
!28 = distinct !DILexicalBlock(scope: !24, file: !3, line: 25, column: 22)
!29 = !DILocation(line: 27, column: 2, scope: !28)
!30 = !DILocation(line: 30, column: 2, scope: !15)
!31 = !DILocation(line: 31, column: 2, scope: !15)
!32 = !DILocation(line: 90, column: 2, scope: !15)
!33 = distinct !DISubprogram(name: "me", scope: !34, file: !34, line: 33, type: !35, scopeLine: 34, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!34 = !DIFile(filename: "upmem_toolkit/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h", directory: "/root")
!35 = !DISubroutineType(types: !36)
!36 = !{!37}
!37 = !DIDerivedType(tag: DW_TAG_typedef, name: "sysname_t", file: !38, line: 27, baseType: !20)
!38 = !DIFile(filename: "upmem_toolkit/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/sysdef.h", directory: "/root")
!39 = !DILocation(line: 35, column: 12, scope: !33)
!40 = !DILocation(line: 35, column: 5, scope: !33)
