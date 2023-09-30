
    SET_CURRENT_ACTOR_METASPRITE SmallCloudMetadata
    jmp ActorSmallCloudNext                        ; Make sure to skip metadata

SmallCloudMetadata:
    .byte   0,$0a,%00100011,0
    .byte   0,$0b,%00100011,8
    .byte   $80

ActorSmallCloudNext:

    clc
		lda ActorsArray+Actor::XVel,x
		adc #32
		sta ActorsArray+Actor::XVel,x
		bcs :+
      jmp ActorSmallCloudEnd
		:
    clc
    inc ActorsArray+Actor::XPos,x                 ; Make cloud move to the right

ActorSmallCloudEnd:

