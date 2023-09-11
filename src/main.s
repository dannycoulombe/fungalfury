.include "consts.inc"
.include "header.inc"
.include "interfaces.inc"
.include "utils.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Zeropage (Fast-memory)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.segment "ZEROPAGE"
Frame:           .res 1                           ; Total amount of frame counted
Clock60:         .res 1                           ; Total amount of (60 frames) counted
VBlankCompleted: .res 1                           ; Flag to indicate when VBlank is done drawing
BgPtr:           .res 2                           ; Pointer to background address - 16bits (lo,hi)
SprPtr:          .res 2                           ; Pointer to the sprite address - 16bits (lo,hi)
PlaySound:       .res 1                           ; Bit 1-4 represent channels that must be played (1) or not (0)
SfxBuffer:       .res 2                           ; Each byte represent a channel with an index of sound effect
Param1:          .res 1                           ; Non-exclusive parameter (can be used anywhere)
Param2:          .res 1                           ; Non-exclusive parameter (can be used anywhere)
Param3:          .res 1                           ; Non-exclusive parameter (can be used anywhere)
ParamPtr1:       .res 2                           ; Non-exclusive 16bits (lo,hi) pointer parameter (can be used anywhere)
GameStage:       .res 1                           ; Current game stage of the player
ActorsArray:     .res MAX_ACTORS * .sizeof(Actor) ; Array of actors (sprites) on-screen
xScroll:         .res 1                           ; Horizontal background scroll
yScroll:         .res 1                           ; Vertical background scroll

.segment "CODE"
.include "config.inc"
.include "reset.inc"
.include "routines.inc"
.include "buffer.inc"
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
    sta PlaySound
    sta SfxBuffer

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
    lda #NMI_BIT_SEQUENCE                         ; Enable NMI and set background to use the 2nd pattern table (at $1000)
    sta PPU_CTRL
    lda #PPU_MASK_BIT_SEQUENCE
    sta PPU_MASK                                  ; Set PPU_MASK bits to render the background

GameLoop:
    jsr Titlescreen::update
    jsr Actors::update                            ; Update all actors
    jsr Buffer::update                            ; Perform all actions in buffer

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
    sta VBlankCompleted                           ; Set the VBlankCompleted flag to indicate we are done drawing to the PPU

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

.segment "SPRITES"
.incbin "bin/sprites.chr"

.segment "BACKGROUNDS"
.incbin "bin/titlescreen.chr"
.incbin "bin/water.anim.chr"
.incbin "bin/stars.anim.chr"
.incbin "bin/offset.chr"

.segment "VECTORS"
.word NMI
.word Reset
.word IRQ
