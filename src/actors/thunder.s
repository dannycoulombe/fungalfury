    PUSH_REGS

    lda ActorsArray+Actor::Clock,x
    bne @ActorThunderLoopNext
      BUFFER_SOUND #0,FAMISTUDIO_SFX_CH0          ; Buffer thunder sound effect
@ActorThunderLoopNext:

    lda ActorsArray+Actor::Clock,x
    cmp #6
    bne @ActorThunderLoopContinue
      CHANGE_PALETTE_COLOR_AT_RUNTIME $3F02,#$11  ; Thunder active color
      SET_CURRENT_ACTOR_METASPRITE ThunderMetadata
      jmp ActorThunderEnd
@ActorThunderLoopContinue:

    lda ActorsArray+Actor::Clock,x
    cmp #24
    bne ActorThunderEnd
      CHANGE_PALETTE_COLOR_AT_RUNTIME $3F02,#$0F  ; Thunder unactive color
      CLEAR_CURRENT_ACTOR ThunderMetadata
      jmp ActorThunderEnd                         ; Make sure to skip metadata

ThunderMetadata:
	.byte    0,$00,1,0
	.byte    0,$01,1,8
	.byte    8,$02,1,0
	.byte    8,$03,1,8
	.byte   16,$04,1,0
	.byte   16,$05,1,8
	.byte $80

ActorThunderEnd:
    PULL_REGS
