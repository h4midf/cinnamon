source ~/upmem_toolkit/upmem-2023.2.0-Linux-x86_64/upmem_env.sh
clang --target=dpu-upmem-dpurte -S -emit-llvm task.c
# clang --target=dpu-upmem-dpurte test.ll