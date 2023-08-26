.scope Actors

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Subroutine to add new actor to the array in the first empty slot found
  ;; Params = Type, XPos, YPos
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  .proc push
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

  AddNewActorToArray:                               ; Here we add a new actor at index [x] of the array
    lda Param1                                    ; Fetch parameter "actor type" from RAM
    sta ActorsArray+Actor::Type,x
    lda Param2                                    ; Fetch parameter "actor position X" from RAM
    sta ActorsArray+Actor::XPos,x
    lda Param3                                    ; Fetch parameter "actor position Y" from RAM
    sta ActorsArray+Actor::YPos,x
    lda #ActorState::IDLE
    sta ActorsArray+Actor::State,x                ; Every actor are idles
    lda #0
    sta ActorsArray+Actor::Frame,x                ; Every actor are idles
    lda #0
    sta ActorsArray+Actor::Clock,x                ; Every actor are idles
  EndRoutine:
    rts
  .endproc

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Subroutine to update the actor states
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  .proc update

    ; Initialize sprite pointer to $0200
    lda #$02
    sta SprPtr+1
    lda #$00
    sta SprPtr

    ldx #0
  ActorsLoop:
    lda ActorsArray+Actor::Type,x
    cmp #ActorType::NULL
    beq @NextActor

    cmp #ActorType::THUNDER                       ; Thunder
    bne :+
      .include "thunder.update.s"
    :

  @NextActor:                                     ; Fetch the next actor from the array
    inc ActorsArray+Actor::Clock,x

    txa
    clc
    adc #.sizeof(Actor)
    tax
    cmp #MAX_ACTORS * .sizeof(Actor)
    bne ActorsLoop

    rts
  .endproc

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Subroutine to loop all actors and send their tiles to the OAM-RAM at $200
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  .proc render
      ldx #0                             ; Counts how many actors we are looping
  ActorsLoop:
      lda ActorsArray+Actor::Type,x

      cmp #ActorType::THUNDER
      bne :+
        .include "thunder.render.s"
      :

;    NextActor:
;      txa
;      clc
;      adc #.sizeof(Actor)
;      tax
;      cmp #MAX_ACTORS * .sizeof(Actor)
;      bne ActorsLoop
;      tya
;      pha                                ; Save the Y register to the stack

      rts
  .endproc
.endscope
