.scope Titlescreen

  thunder = 0

  .proc init
    PLAY_MUSIC music_data_titan
    rts
  .endproc

  .proc update
    lda Clock60
    cmp #02
    bne :+
      lda thunder
      bne :++
        lda #01
        sta thunder
        ADD_ACTOR #ActorType::THUNDER, #0, #0
      :
    :
    rts
  .endproc
.endscope
