.macro UPDATE_THUNDER
  lda ActorsArray+Actor::Clock,x
  cmp #15
  bne :+
    ; PLAY SOUND EFFECT
    lda #0
    ldx #FAMISTUDIO_SFX_CH0
    jsr famistudio_sfx_play        ; Play thunder sound effect
  :
  cmp #120
  bne :+
    lda #ActorType::NULL
    sta ActorsArray+Actor::Type,x
  :
.endmacro

.macro RENDER_THUNDER

.endmacro
