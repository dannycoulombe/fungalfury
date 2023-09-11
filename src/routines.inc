;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to load all 32 color palette values from ROM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro LOAD_PALETTE data,total
    PPU_SETADDR $3F00
    ldy #0                                        ; Y = 0
:   lda data,y                                    ; Lookup byte in ROM
    sta PPU_DATA                                  ; Set value to send to PPU_DATA
    iny                                           ; Y++
    cpy total                                     ; Is Y equal to 32?
    bne :-                                        ; Not yet, keep looping
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to load all 32 color palette values from ROM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro CHANGE_PALETTE_COLOR_AT_RUNTIME addr,color
    lda #0
    sta PPU_CTRL                                  ; Disable NMI

    PPU_SETADDR addr
    lda color
    sta PPU_DATA                                  ; Set value to send to PPU_DATA

    jsr ApplyBackgroundScroll                     ; Scrolling position needs to be resetted

    lda #NMI_BIT_SEQUENCE                         ; Enable NMI and set background to use the 2nd pattern table (at $1000)
    sta PPU_CTRL
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
    ldx #4                                        ; We will loop 4 times 255 bytes
OuterLoop:
    cpx #0
    beq EndLoop                                   ; Finish the job if we've done 4 loop of 255
InnerLoop:
    lda (BgPtr),y
    sta PPU_DATA
    iny
    cpy #0
    bne InnerLoop                                 ; Continue looping current line if not done
    dex
    inc BgPtr+1
    jmp OuterLoop                                 ; Continue to the next block of 255
EndLoop:
    rts
.endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Prefill the current actor parameters and launch the SetMetasprite subroutine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro SET_CURRENT_ACTOR_METASPRITE addr
    lda #<addr
    sta ParamPtr1
    lda #>addr
    sta ParamPtr1+1
    lda ActorsArray+Actor::YPos,x
    sta Param1
    lda ActorsArray+Actor::XPos,x
    sta Param2
    jsr SetMetasprite
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Prefill the current actor parameters and launch the SetMetaObject subroutine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro SET_CURRENT_ACTOR_METAOBJECT addr
    lda #<addr
    sta ParamPtr1
    lda #>addr
    sta ParamPtr1+1
    lda ActorsArray+Actor::YPos,x
    sta Param1
    lda ActorsArray+Actor::XPos,x
    sta Param2
    jsr SetMetaObject
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Loop through a metasprite dataset and send the sprites to the OAM-RAM buffer
;; pointer,yPos,xPos
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.proc SetMetasprite
    ldy #0                                        ; Index of byte to read

@LoopMetaSprite:
    ldx #0

    lda (ParamPtr1),y                             ; Load first byte
    cmp $80                                       ; First check if current byte has $80 terminator
    beq @LoopMetaSpriteEnd                        ; If $80 terminator, end loop

    adc Param1                                    ; Add base Y position to it
    sta (SprPtr),y                                ; Push to sprite pointer
    iny

    lda (ParamPtr1),y                             ; Load sprite index
    sta (SprPtr),y                                ; Store it in sprite pointer
    iny

    lda (ParamPtr1),y                             ; Load sprite attributes
    sta (SprPtr),y                                ; Store it in sprite pointer
    iny

    lda (ParamPtr1),y                             ; Load sprite X position
    adc Param2                                    ; Add base X position to it
    sta (SprPtr),y                                ; Store it in sprite pointer
    iny

    pla                                           ; Retrieve sprite count
    tax
    dex                                           ; Decrement sprite count
    txa
    bne @LoopMetaSprite

@LoopMetaSpriteEnd:
    rts
.endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Loop through a background object dataset and send the sprites to the
;; background buffer to be printed out on render.
;; pointer,yPos,xPos
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.proc SetMetaObject
    ldy #0                                        ; Index of byte to read

@LoopMetaSprite:
    ldx #0

    lda (ParamPtr1),y                             ; Load first byte
    cmp $80                                       ; First check if current byte has $80 terminator
    beq @LoopMetaSpriteEnd                        ; If $80 terminator, end loop

    adc Param1                                    ; Add base Y position to it
    sta (BgPtr),y                                 ; Push to sprite pointer
    iny

    lda (ParamPtr1),y                             ; Load sprite index
    sta (BgPtr),y                                 ; Store it in sprite pointer
    iny

    lda (ParamPtr1),y                             ; Load sprite attributes
    sta (BgPtr),y                                 ; Store it in sprite pointer
    iny

    lda (ParamPtr1),y                             ; Load sprite X position
    adc Param2                                    ; Add base X position to it
    sta (BgPtr),y                                 ; Store it in sprite pointer
    iny

    pla                                           ; Retrieve sprite count
    tax
    dex                                           ; Decrement sprite count
    txa
    bne @LoopMetaSprite

@LoopMetaSpriteEnd:
    rts
.endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Apply the X and Y scrolling position from memory
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.proc ApplyBackgroundScroll
    lda xScroll
    sta PPU_SCROLL
    lda yScroll
    sta PPU_SCROLL
    rts
.endproc
