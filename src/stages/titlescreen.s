.scope Titlescreen
    thunder = 0                                   ; Flag to determine if thunder animation is playing or not

  .proc init
    LOAD_PALETTE TitlescreenPaletteData,#32       ; Load the titlescreen palette to $3F00
    LOAD_NAMETABLE TitlescreenData                ; Load the titlescreen nametable
    PLAY_MUSIC music_data_titlescreen             ; Play the titlescreen music

;    CALL_PROC PushActor,#ActorType::SMALL_CLOUD, #24, #88
    rts
  .endproc

  .proc update
    lda Clock60
    cmp #03                                       ; Wait for 3 seconds
    bne @TitlescreenUpdateEnd
      lda thunder
      bne @TitlescreenUpdateEnd                   ; If the thunder flag is not set yet, we can proceed with animation
        lda #01
        sta thunder
        CALL_PROC PushActor,#ActorType::THUNDER, #0, #136 ; Start playing the thunder animation
    @TitlescreenUpdateEnd:
    rts
  .endproc
.endscope
