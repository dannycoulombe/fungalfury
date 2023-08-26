.scope Titlescreen
    thunder:    .byte   0                         ; Flag to determine if thunder animation is playing or not
;    thunder = 0                            ; Flag to determine if thunder animation is playing or not

  .proc init
    LOAD_PALETTE $3F00,TitlescreenPaletteData,#32 ; Load the titlescreen palette to $3F00
    LOAD_NAMETABLE TitlescreenData                ; Load the titlescreen nametable
    PLAY_MUSIC music_data_titlescreen             ; Play the titlescreen music
    rts
  .endproc

  .proc update
    lda Clock60
    cmp #02
    bne :+                                        ; Start playing the thunder animation after 2 seconds
      lda thunder
      bne :++                                     ; If the thunder flag is not set yet, we can proceed with animation
        lda #01
        sta thunder
;        CALL_PROC actors,#ActorType::THUNDER, #0, #0
      :
    :
    rts
  .endproc
.endscope
