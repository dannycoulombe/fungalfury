;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Enumeration with the different types of actors
;; of our game
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Struct to hold the data for one actor
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.struct Actor
    Type   .byte
    XPos   .byte
    YPos   .byte
    State  .byte                                  ; The ActorState enum
    Frame  .byte                                  ; The frame to display
    Clock  .byte                                  ; Counting every frame since init
.endstruct
