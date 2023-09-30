;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Prepare global parameters for eventual CALL_PROC call
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PREPARE_PARAMS param1,param2,param3,param4,param5
  .ifnblank param1
    lda param1
    sta Param1
  .endif
  .ifnblank param2
    lda param2
    sta Param2
  .endif
  .ifnblank param3
    lda param3
    sta Param3
  .endif
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Call procedure with parameters
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro CALL_PROC pointer,param1,param2,param3
    PREPARE_PARAMS param1,param2,param3
    jsr pointer                                   ; Jump to function's pointer sub-routine
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to set a 16-bit address to the PPU_ADDR register (at $2006)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PPU_SETADDR addr
    bit PPU_STATUS                                ; Read from PPU_STATUS to reset the address latch
    lda #>addr                                    ; Fetch the hi-byte from 16-bit address
    sta PPU_ADDR                                  ; Store the hi-byte into PPU_ADDR $2006
    lda #<addr                                    ; Fetch the lo-byte from 16-bit address
    sta PPU_ADDR                                  ; Store the lo-byte into PPU_ADDR $2006
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to push PPU_ADDR to the stack
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PUSH_PPU_ADDR
    lda #<PPU_ADDR
    pha
    lda #>PPU_ADDR
    pha
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Restore the PPU_ADDR from the stack
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PULL_PPU_ADDR
    bit PPU_STATUS                                ; Read from PPU_STATUS to reset the address latch
    pla                                           ; Fetch the hi-byte from 16-bit address
    sta PPU_ADDR                                  ; Store the hi-byte into PPU_ADDR $2006
    pla                                           ; Fetch the lo-byte from 16-bit address
    sta PPU_ADDR                                  ; Store the lo-byte into PPU_ADDR $2006
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to send a value to the PPU_DATA (at $2007)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PPU_SETDATA val
    lda val
    sta PPU_DATA                                  ; Send value to PPU register at $2007
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to push and preserv registers A, X, Y, and status flags on the Stack.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PUSH_REGS
    pha                                           ; Push A to the stack
    txa
    pha                                           ; Push X to the stack
    tya
    pha                                           ; Push Y to the stack
    php                                           ; Push Processor Status flags to the stack
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to pull and restore registers A,X,Y, and status flags from the Stack.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PULL_REGS
    plp                                           ; Restore the the status flags from the stack
    pla                                           ; Restore the old value of X from the stack
    tay
    pla                                           ; Restore the old value of X from the stack
    tax
    pla                                           ; Pull A from the stack
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Set the index of the bit in the accumulator to the value passed in parameter
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro SET_BIT_INDEX index,value
    lda value
    ldx #0                                        ; Start at index 1
@Loop:
    cpx index
    beq :+                                        ; If index is at zero, jump to flipping the bits
      asl                                         ; Shift bit to the left since not at index
      inx
      jmp @Loop
    :

    ldx value                                     ; Compare the value to determine if we set 0 or 1
    bne :+                                        ; If 0, we flip and invert bits
      ora value                                   ; Flip the bit at given index
      eor #%11111111                              ; Invert all bits
      jmp @EndLoop
    :
    ora value                                     ; Value was positive, just flip bit

@EndLoop:
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to select song and initiate playing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro PLAY_MUSIC addr
  .if MUSIC_ENABLED = 1
    ldx #<addr
    ldy #>addr

    lda #1                                        ; NTSC
    jsr famistudio_init

    lda #0
    jsr famistudio_music_play
  .endif
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Buffer sound effect to play later on after game logic is executed
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro BUFFER_SOUND index,channel
  .if SFX_ENABLED = 1
    SET_BIT_INDEX index,#1
    sta PlaySound+channel
    lda index
    sta SfxBuffer+channel
  .endif
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Rotate a set of tiles to the left
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro ROTATE_TILE_LEFT addr,tile,length

;    lda #0
;    sta PPU_CTRL                                  ; Disable NMI

    PUSH_PPU_ADDR

    PPU_SETADDR addr
    ldx #0                                        ; Tile offet
@LoopRotateTileLeft:
    cpx length
    bne @LoopRotateTileLeftContinue
        ldx #$FF                                  ; Tile rotation overflowed, starting over from #$FF
@LoopRotateTileLeftContinue:
    inx


    lda tile
    adc #11
    sta PPU_DATA

    cpx #0
    bne @LoopRotateTileLeft

    PULL_PPU_ADDR

;    lda #NMI_BIT_SEQUENCE                         ; Enable NMI and set background to use the 2nd pattern table (at $1000)
;    sta PPU_CTRL
.endmacro
