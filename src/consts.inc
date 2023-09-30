;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Constants for PPU registers
;; https://www.nesdev.org/wiki/PPU_registers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PPU_CTRL              = $2000                     ; VPHB SINN  - NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN)
PPU_MASK              = $2001
PPU_STATUS            = $2002
OAM_ADDR              = $2003
OAM_DATA              = $2004
PPU_SCROLL            = $2005
PPU_ADDR              = $2006
PPU_DATA              = $2007
PPU_OAM_DMA           = $4014

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; APU ports
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
APU_FLAGS             = $4015

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Controllers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
JOYPAD1               = $4016
JOYPAD2               = $4017

BUTTON_A              = $80                       ; 10000000
BUTTON_B              = $40                       ; 01000000
BUTTON_SELECT         = $20                       ; 00100000
BUTTON_START          = $10                       ; 00010000
BUTTON_UP             = $08                       ; 00001000
BUTTON_DOWN           = $04                       ; 00000100
BUTTON_LEFT           = $02                       ; 00000010
BUTTON_RIGHT          = $01                       ; 00000001

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Game configuration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NMI_BIT_SEQUENCE      = %10010000
PPU_MASK_BIT_SEQUENCE = %00011110
ANIM_BANK_POSITION    = 5                         ; CHR bank at $1c00

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; In-Game constants
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MAX_ACTORS            = 12                        ; Max amount of meta-sprites (actors) on-screen
