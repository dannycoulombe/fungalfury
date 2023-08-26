    lda ActorsArray+Actor::Clock,x
    bne :+
      PUSH_REGS
      PLAY_SOUND #0,FAMISTUDIO_SFX_CH0            ; Play thunder sound effect
      PULL_REGS
    :
    cmp #120
    bne :+
      lda #ActorType::NULL
      sta ActorsArray+Actor::Type,x
    :
