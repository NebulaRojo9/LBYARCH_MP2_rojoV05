; Must use functional scalar SIMD registers (XMM) and instructions (MOVSS, SUBSS, MULSS, ADDSS, SQRTSS)

section .text
global distance_kernel_asm


distance_kernel_asm:
    ; RCX: n
    ; RDX: X1 (pointer)
    ; R8:  X2 (pointer)
    ; R9:  Y1 (pointer)
    ; R10: loop index 'i' (Volatile, safe to use)
    
    ; Retrieve stack arguments (Y2 and Z)
    MOV R11, [RSP+40] ; R11 = Y2 (pointer)
    MOV R12, [RSP+48] ; R12 = Z (pointer)
    
    ; Setup loop index
    MOV R10, 0        ; i = 0

.loop_start:
    CMP R10, RCX 
    JGE .loop_end

    MOVSS XMM0, [RDX + R10*4]
    SUBSS XMM0, [R8 + R10*4]

    MOVSS XMM1, [R9 + R10*4]
    SUBSS XMM1, [R11 + R10*4]
    
    MOVSS XMM2, XMM0
    MULSS XMM2, XMM0

    MOVSS XMM3, XMM1
    MULSS XMM3, XMM1

    ADDSS XMM2, XMM3

    SQRTSS XMM4, XMM2

    MOVSS [R12 + R10*4], XMM4

    INC R10
    JMP .loop_start

.loop_end:
    RET


