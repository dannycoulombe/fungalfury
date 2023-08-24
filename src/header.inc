;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; iNES Header (16 bytes)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.segment "HEADER"
.byte $4E,$45,$53,$1A           ; 4 bytes with the characters NES + \n
.byte $02                       ; How many 16KB of PRG-ROM
.byte $01                       ; How may 8KB of CHR-ROM

;76543210
;||||||||
;|||||||+- Mirroring: 0: horizontal (vertical arrangement) (CIRAM A10 = PPU A11)
;|||||||              1: vertical (horizontal arrangement) (CIRAM A10 = PPU A10)
;||||||+-- 1: Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory
;|||||+--- 1: 512-byte trainer at $7000-$71FF (stored before PRG data)
;||||+---- 1: Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM
;++++----- Lower nybble of mapper number
.byte %01000001                 ; Mapper 004, Vertical mirroring

;76543210
;||||||||
;|||||||+- VS Unisystem
;||||||+-- PlayChoice-10 (8 KB of Hint Screen data stored after CHR data)
;||||++--- If equal to 2, flags 8-15 are in NES 2.0 format
;++++----- Upper nybble of mapper number
.byte %00000000                 ; None of that..

.byte $00                       ; No PRG-RAM
.byte $00                       ; NTSC TV format
.byte $00                       ; Extra flags for TV format and PRG-RAM
.byte $00,$00,$00,$00,$00       ; Unused paddin to complete 16 bytes of header

