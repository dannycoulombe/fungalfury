    pha
    txa
    pha

    lda ActorsArray+Actor::Clock,x
    bne @LoopNext
      BUFFER_SOUND #0,FAMISTUDIO_SFX_CH0          ; Buffer thunder sound effect
@LoopNext:

    lda ActorsArray+Actor::Clock,x
    cmp #15
    bne @LoopContinue
      lda ThunderMetadata
      sta Param1
      lda ActorsArray+Actor::YPos,x
      sta Param2
      lda ActorsArray+Actor::XPos,x
      sta Param3
      jsr SetMetasprite
      jmp ActorThunderEnd
@LoopContinue:

    lda ActorsArray+Actor::Clock,x
    cmp #120
    bne @LoopEnd
      lda #ActorType::NULL
      sta ActorsArray+Actor::Type,x
      jmp ActorThunderEnd
@LoopEnd:

    jmp ActorThunderEnd                           ; Make sure to skip metadata

ThunderMetadata:
	.byte   4
	.byte   0, 0,$01,1
	.byte   8, 0,$02,1
	.byte   0, 8,$03,1
	.byte   8, 8,$04,1

ActorThunderEnd:
    pla
    tax
    pla
