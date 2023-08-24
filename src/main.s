.include "consts.inc"
.include "actors/actors.inc"
.include "header.inc"
.include "reset.inc"
.include "utils.inc"

.segment "ZEROPAGE"
Frame:          .res 1       ; Total amount of frame counted
Clock60:        .res 1       ; Total amount of (60 frames) counted
BgPtr:          .res 2       ; Pointer to background address - 16bits (lo,hi)
SprPtr:         .res 2       ; Pointer to the sprite address - 16bits (lo,hi)
param1:         .res 1
param2:         .res 1
param3:         .res 1
ActorsArray:    .res MAX_ACTORS * .sizeof(Actor)

.segment "CODE"
.include "config.inc"
.include "routines.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FamiStudio audio engine configuration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.define FAMISTUDIO_CA65_ZP_SEGMENT   ZEROPAGE
.define FAMISTUDIO_CA65_RAM_SEGMENT  RAM
.define FAMISTUDIO_CA65_CODE_SEGMENT CODE

FAMISTUDIO_CFG_EXTERNAL       = 1
FAMISTUDIO_CFG_DPCM_SUPPORT   = 1
FAMISTUDIO_CFG_SFX_SUPPORT    = 1
FAMISTUDIO_CFG_SFX_STREAMS    = 2
FAMISTUDIO_CFG_EQUALIZER      = 1
FAMISTUDIO_USE_VOLUME_TRACK   = 1
FAMISTUDIO_USE_PITCH_TRACK    = 1
FAMISTUDIO_USE_SLIDE_NOTES    = 1
FAMISTUDIO_USE_VIBRATO        = 1
FAMISTUDIO_USE_ARPEGGIO       = 1
FAMISTUDIO_CFG_SMOOTH_VIBRATO = 1
FAMISTUDIO_USE_RELEASE_NOTES  = 1
FAMISTUDIO_DPCM_OFF           = $E000

.include "lib/audioengine.s"
.include "states/titlescreen.s"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reset
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Reset:
  INIT_NES

Main:
  LOAD_PALETTE $3F00,TitlescreenPaletteData,#32
  LOAD_NAMETABLE TitlescreenData
;    PRINT_SPRITE ThunderMetaData

  ldx #<sounds
  ldy #>sounds
  jsr famistudio_sfx_init

  jsr Titlescreen::init

EnableRendering:
    lda #%10010000           ; Enable NMI and set background to use the 2nd pattern table (at $1000)
    sta PPU_CTRL
    lda #0
    sta PPU_SCROLL           ; Disable scroll in X
    sta PPU_SCROLL           ; Disable scroll in Y
    lda #%00011110
    sta PPU_MASK             ; Set PPU_MASK bits to render the background

  InfiniteLoop:
    jmp InfiniteLoop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; NMI: Non-Maskable Interrupt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NMI:
  PUSH_REGS                   ; Push registers to the stack

  inc Frame                   ; Increment the frame counter

  CalculateClocks:
    lda Frame
    cmp #60
    bne :+
      lda #0
      sta Frame
      inc Clock60
    :

  jsr famistudio_update       ; Call audio engine update (to progress audio frame per frame)
  jsr Actors::update
  jsr Actors::render
  jsr Titlescreen::update

  PULL_REGS                   ; Pull register back from the stack
  rti

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Interrupt Handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IRQ:
  rti

TitlescreenData:
  .incbin "levels/titlescreen.nam"
TitlescreenPaletteData:
  .incbin "levels/titlescreen.pal"
  .incbin "levels/titlescreen.pal"
ThunderMetaData:
  	.byte   0,  0,$01,1
  	.byte   8,  0,$02,1
  	.byte   0,  8,$03,1
  	.byte   8,  8,$04,1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Here goes the encoded music data that was exported by FamiStudio
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MusicData:
.include "musics/titan.s"
SfxData:
.include "musics/sfx.s"

.segment "CHARS"
.incbin "bin/titlescreen.chr"

.segment "VECTORS"
.word NMI
.word Reset
.word IRQ
