.include "consts.inc"
.include "header.inc"
.include "reset.inc"
.include "utils.inc"
.include "interfaces.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Zeropage (Fast-memory)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.segment "ZEROPAGE"
Frame:            .res    1                       ; Total amount of frame counted
Clock60:          .res    1                       ; Total amount of (60 frames) counted
VBlankCompleted:  .res    1                       ; Flag to indicate when VBlank is done drawing
BgPtr:            .res    2                       ; Pointer to background address - 16bits (lo,hi)
SprPtr:           .res    2                       ; Pointer to the sprite address - 16bits (lo,hi)
Param1:           .res    1                       ; Non-exclusive parameter (can be used anywhere)
Param2:           .res    1                       ; Non-exclusive parameter (can be used anywhere)
Param3:           .res    1                       ; Non-exclusive parameter (can be used anywhere)
GameStage:        .res    1                       ; Current game stage of the player
ActorsArray:      .res    MAX_ACTORS * .sizeof(Actor) ; Array of actors (sprites) on-screen

.segment "CODE"
.include "config.inc"
.include "routines.inc"
.include "lib/audioengine.s"
.include "stages/titlescreen.s"
.include "actors/actors.s"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reset Interrupt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Reset:
    INIT_NES

    ; Initialize variables
    lda #0
    sta GameStage
    sta Frame
    sta Clock60

    ; Enable sound engine if defined in
    ; configurations
    .if MUSIC_ENABLED = 1 .or SFX_ENABLED = 1
      lda #1                                      ; NTSC
      jsr famistudio_init
    .endif

    ; Load sound banks if defined in
    ; configurations
    .if SFX_ENABLED = 1
      ldx #<sounds
      ldy #>sounds
      jsr famistudio_sfx_init
    .endif

    ; Load current game stage
    lda GameStage
    bne :+
      jsr Titlescreen::init
    :

EnableRendering:
    lda #%10010000                                ; Enable NMI and set background to use the 2nd pattern table (at $1000)
    sta PPU_CTRL
    lda #0
    sta PPU_SCROLL                                ; Disable scroll in X
    sta PPU_SCROLL                                ; Disable scroll in Y
    lda #%00011110
    sta PPU_MASK                                  ; Set PPU_MASK bits to render the background

GameLoop:
    jsr famistudio_update                         ; Call audio engine update (to progress audio frame per frame)
    jsr Titlescreen::update
    jsr Actors::update

WaitForVBlank:                                    ; We lock the execution of the game logic here
    lda VBlankCompleted                           ; Here we check and only perform a game loop call once NMI is done drawing
    beq WaitForVBlank                             ; Otherwise, we keep looping
      lda #0
      sta VBlankCompleted                         ; Once we're done, we set the DrawComplete flag back to 0

    jmp GameLoop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; NMI: Non-Maskable Interrupt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NMI:
    PUSH_REGS                                     ; Push registers to the stack

    inc Frame                                     ; Increment the frame counter
CalculateClock:
    lda Frame
    cmp #60
    bne :+
      lda #0
      sta Frame
      inc Clock60
    :

Render:
    jsr Actors::render

SetDrawComplete:
    lda #1
    sta VBlankCompleted                           ; Set the DrawComplete flag to indicate we are done drawing to the PPU

    PULL_REGS                                     ; Pull register back from the stack
    rti

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Interrupt Handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IRQ:
    rti

TitlescreenData:  .incbin "levels/titlescreen.nam"
TitlescreenPaletteData:
.incbin "levels/titlescreen.pal"
.incbin "levels/titlescreen.pal"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Here goes the encoded music data that was exported by FamiStudio
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MusicData:        .include "musics/titlescreen.s"
SfxData:          .include "musics/sfx.s"

.segment "CHARS"
.incbin "bin/titlescreen.chr"

.segment "VECTORS"
.word NMI
.word Reset
.word IRQ
