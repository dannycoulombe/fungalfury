;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Subroutine to add new actor to the array in the first empty slot found
;; Params = Type, XPos, YPos
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.proc PushActor
    ldx #0                                        ; X = 0
  ArrayLoop:
    cpx #MAX_ACTORS * .sizeof(Actor)              ; Reached maximum number of actors allowed in the array?
    beq EndRoutine                                ; Then we skip and don't add a new actor
    lda ActorsArray+Actor::Type,x
    cmp #ActorType::NULL                          ; If the actor type of this array position is NULL
    beq AddNewActorToArray                        ; Then: we found an empty slot, proceed to add actor to position [x]
  NextActor:
    txa
    clc
    adc #.sizeof(Actor)                           ; Otherwise, we offset to the check the next actor in the array
    tax                                           ; X += sizeof(Actor)
    jmp ArrayLoop

  AddNewActorToArray:                             ; Here we add a new actor at index [x] of the array
    lda Param1                                    ; Fetch parameter "actor type" from RAM
    sta ActorsArray+Actor::Type,x
    lda Param2                                    ; Fetch parameter "actor position X" from RAM
    sta ActorsArray+Actor::YPos,x
    lda Param3                                    ; Fetch parameter "actor position Y" from RAM
    sta ActorsArray+Actor::XPos,x
    lda #ActorState::IDLE
    sta ActorsArray+Actor::State,x                ; Every actor are idles
    lda #0
    sta ActorsArray+Actor::Frame,x                ; Every actor are idles
    lda #0
    sta ActorsArray+Actor::Clock,x                ; Every actor are idles
  EndRoutine:
    rts
.endproc

.scope Actors

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Subroutine to update the actor states
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  .proc update

    ; Initialize sprite pointer to $0200
    lda #$02
    sta SprPtr+1
    lda #$00
    sta SprPtr

    ; Loop through all actors
    ldx #0                                        ; Actor index
  ActorsLoop:
    lda ActorsArray+Actor::Type,x
    cmp #ActorType::NULL
    bne :+
      jmp NextActor
    :

    lda ActorsArray+Actor::Type,x
    cmp #ActorType::THUNDER
    beq :+
      jmp AfterActorThunder
    :
    .include "thunder.s"
  AfterActorThunder:

    lda ActorsArray+Actor::Type,x
    cmp #ActorType::SMALL_CLOUD
    beq :+
      jmp AfterActorSmallCloud
    :
    .include "small-cloud.s"
  AfterActorSmallCloud:

    inc ActorsArray+Actor::Clock,x
  NextActor:                                      ; Fetch the next actor from the array
    txa
    clc
    adc #.sizeof(Actor)
    tax
    cmp #MAX_ACTORS * .sizeof(Actor)
    beq :+
      jmp ActorsLoop
    :

    rts
  .endproc

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Subroutine to loop all actors and send their tiles to the OAM-RAM at $200
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  .proc render
    lda #$02                                      ; Every frame, we copy sprite data starting at $02**.
    sta $4014                                     ; The OAM-DMA copy starts when we write to $4014.
    rts
  .endproc
.endscope
