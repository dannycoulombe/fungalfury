;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Enumeration with the different types of actors
;; of our game
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.enum ActorType
    NULL = 0
    PLAYER = 2
    THUNDER = 3
    SMALL_CLOUD = 4
    BIG_CLOUD = 5
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
    Type        .byte
    XPos        .byte
    YPos        .byte
    XVel        .byte
    YVel        .byte
    State       .byte                             ; The ActorState enum
    Clock       .byte                             ; Counting every frame since init
.endstruct
