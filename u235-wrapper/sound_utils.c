//
// sound_utils.c
//

#include "sound.h"

//
// keep these in-sync with any changes in u235/u235se-rel.inc
//
#define U235SE_music_vol    *(long *)0x00f1be8c
#define U235SE_playmod      *(long *)0x00f1b5d0

#define MAXVOL  63
#define NO_MOD  -1

extern short music_vol_current;
extern short music_vol_restore;
extern short music_vol_muted;
extern short mod_queued;
extern short shhh_count;

extern short music_fade_out;

#define MUSIC_FADE_FAST 2
#define MUSIC_FADE_SLOW 1

extern short VIDpal;

#define FADE_DURATION (VIDpal ? 10 : 12) // 1/5 of a second

//
// Start playing a mod
// Favour using queue_mod()
//
extern void play_mod(short mod_idx);

//
// Stop playing the current mod
// Favour using fade_out_music()
//
extern void stop_mod(void);

void queue_mod(short mod_idx) {
    if (U235SE_playmod == 0) {
        // not playing anything
        play_mod(mod_idx);
    } else {
        music_fade_out = MUSIC_FADE_FAST;
        mod_queued = mod_idx;
        music_vol_restore = music_vol_current;
        shhh_count = FADE_DURATION;
    }
}

void fade_out_music() {
    music_fade_out = MUSIC_FADE_SLOW;
    shhh_count = FADE_DURATION;
}

void process_sound_input() {

    if (music_fade_out) {
        if (music_vol_current) {
            // keep reducing the volume until we're silent
            music_vol_current -= music_fade_out;
            if (music_vol_current < 0) music_vol_current = 0;

            if (music_vol_current == 0) stop_mod();
        
        } else if (shhh_count) {
            // pause between playing tracks
            shhh_count--;
        
        } else {
            
            // finished fade-out
            music_fade_out = 0;
            // turn the volume back up
            music_vol_current = music_vol_restore;

            if (mod_queued != NO_MOD) {
                // start playing the new mod
                play_mod(mod_queued);
                // cleanup
                mod_queued = NO_MOD;
            }
        }

        // assume we've chaged volume somehow
        if (!music_vol_muted) {
            U235SE_music_vol = (long)music_vol_current;
        }

        return; // don't process input further
    }

    if (PAD1_0_PRESS) {
        if (music_vol_muted) {
            // unmute
            music_vol_muted = 0;
            // restore old volume
            U235SE_music_vol = (long)music_vol_current;
        } else {
            // mute
            music_vol_muted = 1;
            // shhh
            U235SE_music_vol = 0;
        }
    
    } else if (PAD1_STAR_DOWN) {
        if (!music_vol_muted && music_vol_current > 0) {
            // turn the volume down
            music_vol_current--;
            U235SE_music_vol = (long)music_vol_current;
        }
    } else if (PAD1_HASH_DOWN) {
        if (music_vol_current < MAXVOL) {
            // turn the volum up
            // this will also unmute
            music_vol_current++;
            U235SE_music_vol = (long)music_vol_current;
        }
    }
}
