.scope Background

  .proc switchBankCheck
    lda CountTo32
    bne :+
      lda #ANIM_BANK_POSITION                     ; Set CHR bank at $1c00
      sta $8000
      lda #12                                     ; 8x2k chars
      sta $8001
    :
    cmp #16
    bne :+
      lda #ANIM_BANK_POSITION                     ; Set CHR bank at $1c00
      sta $8000
      lda #13                                     ; 8x2k chars
      sta $8001
    :
    cmp #32
    bne :+
      lda #ANIM_BANK_POSITION                     ; Set CHR bank at $1c00
      sta $8000
      lda #14                                     ; 8x2k chars
      sta $8001
    :
    cmp #48
    bne :+
      lda #ANIM_BANK_POSITION                     ; Set CHR bank at $1c00
      sta $8000
      lda #15                                     ; 8x2k chars
      sta $8001
    :
    rts
  .endproc

.endscope
