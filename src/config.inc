;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; In-Game configurations
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.define MUSIC_ENABLED                   0
.define SFX_ENABLED                     1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FamiStudio audio engine configuration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.define FAMISTUDIO_CA65_ZP_SEGMENT      ZEROPAGE
.define FAMISTUDIO_CA65_RAM_SEGMENT     RAM
.define FAMISTUDIO_CA65_CODE_SEGMENT    CODE

FAMISTUDIO_CFG_EXTERNAL                 = 1
FAMISTUDIO_CFG_DPCM_SUPPORT             = 0
FAMISTUDIO_CFG_SFX_SUPPORT              = 1
FAMISTUDIO_CFG_SFX_STREAMS              = 2
FAMISTUDIO_CFG_EQUALIZER                = 1
FAMISTUDIO_USE_VOLUME_TRACK             = 1
FAMISTUDIO_USE_PITCH_TRACK              = 1
FAMISTUDIO_USE_SLIDE_NOTES              = 1
FAMISTUDIO_USE_VIBRATO                  = 0
FAMISTUDIO_USE_ARPEGGIO                 = 0
FAMISTUDIO_CFG_SMOOTH_VIBRATO           = 0
FAMISTUDIO_USE_RELEASE_NOTES            = 1
FAMISTUDIO_DPCM_OFF                     = $E000
