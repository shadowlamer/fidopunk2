; Z80 Snake Game Prototype
; By Dr. Mikhailov, 1991

START:
    LD A, 0          ; Init score
    CALL DRAW_SNAKE  ; Draw snake
LOOP:
    CALL CHECK_INPUT ; Get user input
    CALL MOVE_SNAKE  ; Move snake
    CALL CHECK_COLL  ; Check collision
    JP LOOP          ; Loop forever

; End of code
