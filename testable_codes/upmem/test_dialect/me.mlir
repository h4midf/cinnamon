module {
  dpu.barrier_init @barrier1 
  func.func @main() {
    %0 = dpu.me : i32
    %c0_i32 = arith.constant 0 : i32
    %1 = arith.cmpi eq, %0, %c0_i32 : i32
    scf.if %1 {
      dpu.mem_reset
    }
    dpu.barrier_wait @barrier1
    return
  }
}