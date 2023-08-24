;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Enumeration with the different types of actors of our game
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.enum ActorType
  NULL = 0
  PLAYER = 1
  THUNDER = 2
.endenum

.enum ActorState
  IDLE = 0
  ACTIVE = 1
  WALKING = 2
  RUNNING = 3
  JUMPING = 4
.endenum

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Struct to hold the data for one actor
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.struct Actor
  Type   .byte
  XPos   .byte
  YPos   .byte
  State  .byte          ; The ActorState enum
  Frame  .byte          ; The frame to display
  Clock  .byte          ; Counting every frame since init
.endstruct

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Subroutine to add new actor to the array in the first empty slot found
;; Params = ParamType, ParamXPos, ParamYPos
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro ADD_ACTOR type, xPos, yPos
  lda type
  sta param1
  lda xPos
  sta param2
  lda yPos
  sta param3
  jsr AddActor          ; Add new actor to array
.endmacro
.proc AddActor
    ldx #0                            ; X = 0
  ArrayLoop:
    cpx #MAX_ACTORS * .sizeof(Actor)  ; Reached maximum number of actors allowed in the array?
    beq EndRoutine                    ; Then we skip and don't add a new actor
    lda ActorsArray+Actor::Type,x
    cmp #ActorType::NULL              ; If the actor type of this array position is NULL
    beq AddNewActorToArray            ;   Then: we found an empty slot, proceed to add actor to position [x]
   NextActor:
    txa
    clc
    adc #.sizeof(Actor)               ; Otherwise, we offset to the check the next actor in the array
    tax                               ; X += sizeof(Actor)
    jmp ArrayLoop

  AddNewActorToArray:                 ; Here we add a new actor at index [x] of the array
    lda param1                     ; Fetch parameter "actor type" from RAM
    sta ActorsArray+Actor::Type,x
    lda param2                     ; Fetch parameter "actor position X" from RAM
    sta ActorsArray+Actor::XPos,x
    lda param3                     ; Fetch parameter "actor position Y" from RAM
    sta ActorsArray+Actor::YPos,x
    lda #ActorState::IDLE
    sta ActorsArray+Actor::State,x   ; Every actor are idles
    lda #0
    sta ActorsArray+Actor::Frame,x   ; Every actor are idles
    lda #0
    sta ActorsArray+Actor::Clock,x   ; Every actor are idles
EndRoutine:
    rts
.endproc

.scope Actors

  .include "thunder.s"

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Subroutine to update the actor states
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  .proc update
    ldx #0
    ActorsLoop:
      lda ActorsArray+Actor::Type,x

      cmp #ActorType::NULL
      beq :+
        inc ActorsArray+Actor::Clock,x
      :

      cmp #ActorType::THUNDER
      bne :+
        UPDATE_THUNDER
        jmp NextActor
      :

      ; Fetch the next actor from the array
      NextActor:
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
;    lda #$02
;    sta SprPtr+1
;    lda #$00
;    sta SprPtr                         ; Point SprPtr to $0200
;
;    ldy #0                             ; Count how many tiles we are sending
;    ldx #0                             ; Counts how many actors we are looping
;    ActorsLoop:
;;      lda ActorsArray+Actor::Screen,x
;;      sta ParamScreen
;
;      lda ActorsArray+Actor::Type,x
;
;      cmp #ActorType::THUNDER
;      bne :+
;        lda ActorsArray+Actor::XPos,x
;        sta ParamXPos
;        lda ActorsArray+Actor::YPos,x
;        sta ParamYPos
;        lda #$60
;        sta ParamTileNum
;        lda #%00000000
;        sta ParamAttribs
;        lda #4
;        sta ParamNumTiles
;        jsr DrawSprite                 ; Call routine to draw 4 PLAYER tiles to the OAM
;        jmp NextActor
;      :
;      NextActor:
;        txa
;        clc
;        adc #.sizeof(Actor)
;        tax
;        cmp #MAX_ACTORS * .sizeof(Actor)
;
;        beq :+
;          jmp ActorsLoop               ; Use absolute jump to avoid branch limit of [-128..127]
;        :
;
;        tya
;        pha                            ; Save the Y register to the stack
;
;      LoopTrailingTiles:
;        cpy PrevOAMCount
;        bcs :+
;          lda #$FF
;          sta (SprPtr),y               ; Set Y position to $FF (to hide tile)
;          iny
;          sta (SprPtr),y               ; Set tile number as $FF
;          iny
;          sta (SprPtr),y               ; Set attribs as $FF
;          iny
;          sta (SprPtr),y               ; Set X position to $FF (to hide tile)
;          iny
;          jmp LoopTrailingTiles
;        :
;
;        pla                            ; Save the previous value of Y into PrevOAMCount
;        sta PrevOAMCount               ; This is the total number of bytes that we just sent to the OAM
;
    rts
  .endproc
.endscope
