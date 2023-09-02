;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to load all 32 color palette values from ROM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro LOAD_PALETTE addr,data,total
    PPU_SETADDR addr
    ldy #0                                        ; Y = 0
:   lda data,y                                    ; Lookup byte in ROM
    sta PPU_DATA                                  ; Set value to send to PPU_DATA
    iny                                           ; Y++
    cpy total                                     ; Is Y equal to 32?
    bne :-                                        ; Not yet, keep looping
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Subroutine to a full 1024 bytes nametable
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro LOAD_NAMETABLE addr
    bit PPU_STATUS

    lda #>addr
    sta BgPtr+1
    lda #<addr
    sta BgPtr+0

    PPU_SETADDR $2000

    jsr LoadNametable
.endmacro
.proc LoadNametable
    ldy #0
    ldx #4
OuterLoop:
    cpx #0
    beq EndLoop
InnerLoop:
    lda (BgPtr),y
    sta PPU_DATA
    iny
    cpy #0
    bne InnerLoop
    dex
    inc BgPtr+1
    jmp OuterLoop
EndLoop:
    rts
.endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Loop through a metasprite dataset and send the sprites to the OAM-RAM buffer
;; pointer,yPos,xPos
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.proc SetMetasprite
    ldy Param1                                    ; Get total amount of bytes to read
    ldx #1
@LoopMetaSprite:
    tya
    pha

    ldy #0

    lda Param2
    adc Param1+1,x
    sta (SprPtr),y                                ; Y position
    inc SprPtr

    lda Param1+2,x
    sta (SprPtr),y                                ; Tile index

    lda Param1+3,x
    sta (SprPtr),y                                ; Attributes

    lda Param3
    adc Param1+0,x
    sta (SprPtr),y                                ; X position

    inx                                           ; Increment by fow bytes for next tile
    inx
    inx
    inx

    pla
    tay
    dey
    bne @LoopMetaSprite
.endproc
