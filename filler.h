  __asm
    .area _GSFINAL
    .db 0xde, 0xad, 0xbe, 0xef
    .area _CODE
    .rept 526
    nop
    .endm
    di
    call gsinit
  __endasm;  

