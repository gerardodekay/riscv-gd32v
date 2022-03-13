.global ctx_switch
ctx_switch:
    addi sp, sp, -48 // sp must be kept 16-byte aligned
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw s9, 36(sp)
    sw s10, 40(sp)
    sw ra, 44(sp) // skipping s11 and saving ra instead for now

    sw sp, 0(a0) // store sp into address in a0
    // push {r4-r11,lr}
    // str sp, [r0]

    mv sp, a1
    // mov sp, r1

    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw s9, 36(sp)
    lw s10, 40(sp)
    lw ra, 44(sp)
    addi sp, sp, 48
    // pop {r4-r11,lr}

    ret
    //bx lr

.global init_trampoline
init_trampoline:
    mv a0, s1 // Move argument to right location
    jalr x1, 0(s0) // Jump to code address

    jal thread_exit
