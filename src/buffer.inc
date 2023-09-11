.scope Buffer

  .proc update
    jsr famistudio_update                         ; Call audio engine update (to progress audio frame per frame)
    jsr playSounds                                ; Play all sounds in memory
    jsr ApplyBackgroundScroll                     ; Scrolling position needs to be resetted
    rts
  .endproc

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Sub-routine that play all sound channels in buffer and resets the
  ;; play sound flag
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  .proc playSounds
  .if SFX_ENABLED = 1
    ldx #0
  @LoopPlaySound:
    cpx #02
    beq @EndPlaySound
      lda PlaySound,x
      beq :+
        lda SfxBuffer
        jsr famistudio_sfx_play
      :
      inx
      jmp @LoopPlaySound
  @EndPlaySound:
    lda #0
    sta PlaySound
  .endif
    rts
  .endproc
.endscope
