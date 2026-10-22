;
; See U235 documentation for layout of sample bank nd MOD table
; 
;

    .globl sample_bank
    .globl mod_table

;
; sample bank
;

    .data
    .dphrase
sample_bank:

;
; - normalise to 0db
;

; Player laser sound
    dc.l    sample0     ; base address of sample 2
    dc.l    sample1     ; next samples
    dc.l    0           ; RBASE
    dc.l    0           ; REND
    dc.l    64          ; <null word> | Fine tune | Volume
    dc.l    8000        ; Default play freq (only used in SFX)

; Boundary
    dc.l    sample1     ; base address of sample 2
    dc.l    sample2     ; next sample
    dc.l    0           ; RBASE
    dc.l    0           ; REND
    dc.l    64          ; <null word> | Fine tune | Volume
    dc.l    8000        ; Default play freq (only used in SFX)

; Player explosion
    dc.l    sample2     ; base address of sample 2
    dc.l    sample_end  ; End of samples
    dc.l    0           ; RBASE
    dc.l    0           ; REND
    dc.l    64          ; <null word> | Fine tune | Volume
    dc.l    8000        ; Default play freq (only used in SFX)

sample0:
.INCBIN "sfx/sfx_wpn_laser8.raw"
sample1:
.INCBIN "sfx/sfx_sounds_Blip2.raw"
sample2:
.INCBIN "sfx/sfx_exp_odd2.raw"
sample_end:

;
; modules
;

.LONG
mod_table:
    dc.l   module0
    dc.l   module1

.LONG
module0:
.INCBIN "mods/once_is_not_enough.mod"

.LONG
module1:
.INCBIN "mods/drozerix_neon_techno.mod"
