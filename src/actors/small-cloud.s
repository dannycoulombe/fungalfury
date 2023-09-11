    PUSH_REGS

    SET_CURRENT_ACTOR_METASPRITE SmallCloudMetadata
    jmp ActorSmallCloudEnd                        ; Make sure to skip metadata

SmallCloudMetadata:
	.byte   0,$0a,1,0
	.byte   0,$0b,1,8
	.byte   $80

ActorSmallCloudEnd:

    lda Frame
    ror a
    beq :+
      inc ActorsArray+Actor::XPos,x                 ; Make cloud move to the right
    :

    PULL_REGS
