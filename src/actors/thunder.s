    PUSH_REGS

    lda ActorsArray+Actor::Clock,x
    bne @LoopNext
      BUFFER_SOUND #0,FAMISTUDIO_SFX_CH0          ; Buffer thunder sound effect
@LoopNext:

    lda ActorsArray+Actor::Clock,x
    cmp #6
    bne @LoopContinue
      PUSH_REGS

      SET_CURRENT_ACTOR_METAOBJECT ThunderMetadata
      CHANGE_PALETTE_COLOR_AT_RUNTIME $3F02,#$11

      PULL_REGS
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
	.byte   0,$01,1,0
	.byte   8,$02,1,0
	.byte   0,$03,1,8
	.byte   8,$04,1,8
	.byte   $80

ActorThunderEnd:
    PULL_REGS
