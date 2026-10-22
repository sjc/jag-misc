;
; sound.s
; Functions for using the U235 sound engine
; https://www.u-235.co.uk/developer/sound-engine/documentation/v0-24/
;

    .include    'jaguar.inc'
    .include    'u235/u235se-rel.inc'

; these are defined in sound_data.s in the example code
    .extern     sample_bank
    .extern     mod_table

; number of words of random stored
RANDLISTSIZE    .equ    32

; for reloading _rand_read and _rand_write
RANDLISTEND     .equ    ((RANDLISTSIZE-1)*2)

;
; Initialised U235 for the current console region
;

    .extern     _VIDpal     ; from startup.s

_sound_init::

    lea     u235sebin,a1

    tst.w   _VIDpal         ; 1 = PAL, 0 = NTSC
    bne     .sound_init_PAL

.sound_init_NTSC:
    jsr     2(a1) ;U235SE_initNTSC ; will return from here
    bra     .sound_init_done
.sound_init_PAL:
    jsr     4(a1) ;U235SE_initPAL  ; will return from here
    ;rts
.sound_init_done:

    ; setup random stuff
    clr.w   _rand_read
    clr.w   _rand_write

    clr.w   _sfx_queue_count     ; prep this now

    lea     _playlist,a0        ; we'll only use one sample bank, so add the header to the playlist
    move.l  #sample_bank,d0     ; Put address of sample bank in d0
    lsl.l   #4, d0              ; Move the address over for the command to fit
    or.l    #$b,d0              ; add Set Sample Bank instruction
    move.l  d0,(a0)+            ; Write Set bank to playlist

    move.l  #U235SE_16KHZ,U235SE_playback_rate
    move.l  #U235SE_16KHZ_PERIOD,U235SE_playback_period

    move.l #(1 << U235SE_SR_PAD_ENABLED) | (1 << U235SE_SR_RNG_ENABLED),U235SE_ctrl_reg
    
    ; clear the longword at $0 because U235 will attempt to process
    ;   command lists even if their pointer is 0
    clr.l   0.w

    move.w  #$100,JOYSTICK      ; enable sound now U235 is loaded
    move.l  #D_RAM,D_PC
    move.l  #RISCGO,D_CTRL

    move.l  U235SE_playbackmode,d0  
    andi.l  #$FFFFFFFC,d0               ; set music and sfx to mono
    ori.l   #U235SE_STEREO_MUSIC,d0     ; then change our mind and play music in stereo
    move.l  d0,U235SE_playbackmode

    ; try to get the audio levels sounding right
    moveq   #32,d0
    move.l  d0,U235SE_music_vol
    move.w  d0,_music_vol_current

    move.l  #40,U235SE_sfx_vol

    ; setup mod changing
    clr.w   _music_vol_muted
    clr.w   _music_vol_restore
    clr.w   _music_fade_out
    move.w  #-1,_mod_queued
    clr.w   _shhh_count

    rts


;
; mods
;

_play_mod::

.if ^^defined SILENT
    rts
.endif

    move.w  4(sp),d0        ; index of mod to play
    lsl.w   #2,d0           ; offset into mod_table
    lea     mod_table,a0
    add.w   d0,a0

    move.l  (a0),a0

    moveq   #0,d0           ; not entirely sure if this takes a parameter
    lea     u235sebin,a1
    jsr     (a1)            ; modinit()

    move.l  #U235SE_PLAYSTEREO,U235SE_playmod     ; Enable mod playback

    rts


;
; stop and reset module playback stuff
;

_stop_mod::

    move.l  #U235SE_NOMOD,U235SE_playmod

    ; and this is the trick, silence the mod playback channels
    move.l  #_vol_shhh_playlist,U235SE_sfxplaylist_ptr

    lea     U235SE_modregdump,a0
    moveq   #0,d0
    move.l  d0,(a0)+
    move.l  d0,(a0)+
    move.l  d0,(a0)+
    move.l  d0,(a0)+

    rts

;
; SFX
;

;
; add a sample to the queue to be played later
;

_queue_sfx::

.if ^^defined SILENT
    rts
.endif

    move.w  _sfx_queue_count,d0
    cmp.w   #4,d0               ; max queue size
    beq     .queue_sfx_done

    lea     _sfx_queue,a0
    add.w   d0,a0
    add.w   d0,a0

    addq.w  #1,d0
    move.w  d0,_sfx_queue_count

    move.w  4(sp),d0    ; change sample index to offset into sample table
    lsl.w   #8,d0
    move.w  d0,(a0)

.queue_sfx_done:

    rts

;
; play as many queued samples as we have free voices
;

_play_queued_sfx::

    movem.l     d2-d4,-(sp)

    move.w  _sfx_queue_count,d0
    beq     .play_queued_sfx_rts    ; nothing to do

    move.l  U235SE_voice_status,d1  ; ready the status of the voices
    andi.w  #$F0,d1                 ; issolate SFX voices
    cmp.w   #$F0,d1
    beq     .play_queued_sfx_done   ; all voices in use

    lea     _sfx_queue,a0
    adda.w  d0,a0               ; update list pointer, will be moving backwards from the last entry
    adda.w  d0,a0

    subq.w  #1,d0               ; ready for looping

    lea     _playlist+4,a1      ; 1st command to set sample bank exists, so we skip it

    moveq   #0,d3   ; to store count of successfully-queued samples
    moveq   #3,d2   ; before the first voice to test
                    ; this is one less so that the addition below makes sense

.play_queued_sfx_find_voice:

    addq.w  #1,d2
    cmp.w   #8,d2
    beq     .play_queued_sfx_no_more_voices  ; no more voices to check

    btst    d2,d1               ; is this voice active?
    bne     .play_queued_sfx_find_voice

    ; build the playlist command for this sample

    move.l  d2,d4               ; we've ensures this whole long is clear
    lsl.w   #4,d4               ; shift voice into place
    ori.w   #U235SE_CMD_PLAY_SAMPLE,d4
    or.w   -(a0),d4

    move.l  d4,(a1)+            ; add to playlist
    addq.w  #1,d3               ; record that we added a sample

    dbra    d0,.play_queued_sfx_find_voice

.play_queued_sfx_no_more_voices:

    tst.b   d3                      ; did we add anything?
    beq     .play_queued_sfx_done   ; no

    move.l  #U235SE_CMD_LIST_TERMINATOR,(a1)    ; terminate the end of the list
    move.l  #_playlist,U235SE_sfxplaylist_ptr   ; submit the playlist

.play_queued_sfx_done:

    clr.w   _sfx_queue_count     ; reset the queue count now we've player these

.play_queued_sfx_rts:

    movem.l     (sp)+,d2-d4
    rts

;
; 'pause' sfx and music playback
; actually just mute both, because i can't work out how to pause
;   U235 and then restart from the exact same point
;

_pause_sound::

    moveq   #0,d0
    move.l  U235SE_music_vol,_paused_mod_vol
    move.l  d0,U235SE_music_vol
    move.l  U235SE_sfx_vol,_paused_sfx_vol
    move.l  d0,U235SE_sfx_vol

    rts

;
; 'resume' sfx and music playback
;

_resume_sound::

    move.l  _paused_mod_vol,U235SE_music_vol
    move.l  _paused_sfx_vol,U235SE_sfx_vol

    rts


;
; random
;

;
; Read a random number directly from the DSP
;

;_get_random::
;
;    move.l     U235SE_rng,d0   ; d0 will contain a pseudo random number
;    rts

;
; Store a random number to our internal list
;

_store_random::

    move.w  _rand_write,d0
    subq.w  #2,d0
    bpl     .idx_ok
    move.w  #RANDLISTEND,d0
.idx_ok:
    move.w  d0,_rand_write
    lea     _rand_list,a0

    move.l  U235SE_rng,d1
    move.w  d1,(a0,d0.w)

    rts

;
; Read a random number
;

;_random::
;
;    move.l  a0,-(sp)
;
;    move.w  _rand_read,d0
;    subq.w  #2,d0
;    bpl     .idx_ok
;    move.w  #RANDLISTEND,d0
;.idx_ok:
;    move.w  d0,_rand_read
;    lea     _rand_list,a0
;    adda.w  d0,a0
;
;    move.w  (a0),d0
;
;    move.l  (sp)+,a0
;    rts

;
; Read a random number and return the masked value
;

_random_and::

    move.w  _rand_read,d1
    subq.w  #2,d1
    bpl     .idx_ok
    move.w  #RANDLISTEND,d1
.idx_ok:
    move.w  d1,_rand_read
    lea     _rand_list,a0

    move.w  4(sp),d0
    and.w   (a0,d1.w),d0

    rts


;
; joypads
;

;
; Read the joypads
;

_read_pad_one::

    move.l  _pad_one,d1
    move.l  d1,_pad_one_prev
    move.l  U235SE_pad1,d0
    eor.l   d0,d1
    move.l  d0,_pad_one
    move.l  d1,_pad_one_xor

    rts

_read_pad_two::

    move.l  _pad_two,d1
    move.l  d1,_pad_two_prev
    move.l  U235SE_pad2,d0
    eor.l   d0,d1
    move.l  d0,_pad_two
    move.l  d1,_pad_two_xor

    rts


    .data
;
; The U235 sound engine
;

.LONG
u235sebin:
    .INCBIN "u235/u235se-rel.bin"


    .even
_vol_shhh_playlist:
    .dc.l   $00000003   ; silence the mod playback channels
    .dc.l   $00000013
    .dc.l   $00000023
    .dc.l   $00000033
    .dc.l   0

    .bss
    .long

;
; the sfx playlist
;

_playlist:  .ds.l   8   ; guess at how much space we'll need

_music_vol_current::    .ds.w   1
_music_vol_restore::    .ds.w   1
_music_vol_muted::      .ds.w   1
_mod_queued::           .ds.w   1
_shhh_count::           .ds.w   1
_music_fade_out::       .ds.w   1

_paused_sfx_vol:    .ds.l   1
_paused_mod_vol:    .ds.l   1

;
; joypad state
;

_pad_one::      .ds.l   1
_pad_one_xor::  .ds.l   1
_pad_one_prev:: .ds.l   1
_pad_two::      .ds.l   1
_pad_two_xor::  .ds.l   1
_pad_two_prev:: .ds.l   1

;
; sfx control
;

_sfx_queue_count::  .ds.w   1
_sfx_queue::        .ds.w   4   ; assume this is the maximum we'd ever want to try to play

;
; random
; _rand_read and _rand_write should be used as an index and
;   then updated (not the other way around)

_rand_write:    .ds.w   1
_rand_read:     .ds.w   1
_rand_list:     .ds.w   RANDLISTSIZE
