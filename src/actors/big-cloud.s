
    SET_CURRENT_ACTOR_METASPRITE BigCloudMetadata
    jmp ActorBigCloudNext                        	; Make sure to skip metadata

BigCloudMetadata:
	.byte  0,$0c,3, 0
	.byte  0,$0d,3, 8
	.byte  0,$0e,3,16
	.byte  0,$0f,3,24
	.byte  8,$1c,3, 0
	.byte  8,$1d,3, 8
	.byte  8,$1e,3,16
	.byte  8,$1f,3,24
	.byte  $80

ActorBigCloudNext:

		clc
		lda ActorsArray+Actor::XVel,x
		adc #6
		sta ActorsArray+Actor::XVel,x
		bcs :+
      jmp ActorBigCloudEnd
		:
    clc
    inc ActorsArray+Actor::XPos,x                 ; Make cloud move to the right

ActorBigCloudEnd:
