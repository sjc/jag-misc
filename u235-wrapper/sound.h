//
// sound.h
//

#ifndef SOUND_H
#define SOUND_H

//
// U235 sound engine
//

extern void sound_init(void);

//
// To be used when pausing the game
// Don't actually pause anything, just mutes and un-mutes
//
extern void pause_sound(void);
extern void resume_sound(void);

//
// Sound-related input (muting, volume)
// Handles required 0, # and * input
// Also handles fading, so should be called every frame
//
void process_sound_input(void);

//
// mods
//

//
// If another mod is playing, fade that out before starting the new mod
// If no mod is playing, start it right away
//
void queue_mod(short mod_idx);

//
// If a mod is playing, fade it out and then stop playback
//
void fade_out_music(void);

//
// sfx
//

//
// Request that a SFX is played, adding it to a queue which play_queued_sfx()
//  will process
// Doing it this way allows you potentially start multiple samples from different
//  bits of code and have starting them batched
//
extern void queue_sfx(short sfx_id);

//
// Attempt to play all queued SFX, free channels allowing
// This should be called once per frame
//
extern void play_queued_sfx(void);

//
// random
//

//
// Get a 16-bit random number direct from the DSP
//
//extern short get_random(void);

//
// Store the next random number from the DSP to the random list
// We do this so that there will be multiple different random numbers
//  available in a single frame
// Call this every frame to build up the pool of values
//
extern void store_random(void);

//
// Return the next random number from the list
//
//extern short random(void);

//
// Return the next random number from the list AND'd with the param
//
extern short random_and(short mask);

//
// After reading a pad, the _DOWN and _PRESS macros can be used:
//  _DOWN if a button is currently pressed
//  _PRESS if a button has just been pressed eg. it was UP last time
//      read_pad_x() was called but is now DOWN. _PRESS will not
//      repeat until the button is released
//

extern long read_pad_one(void);
extern long pad_one;
extern long pad_one_xor;
extern long pad_one_prev;

extern long read_pad_two(void);
extern long pad_two;
extern long pad_two_xor;
extern long pad_two_prev;

#define U235SE_BUT_UP     1         // Up
#define U235SE_BUT_DOWN   2         // Down
#define U235SE_BUT_LEFT   4         // Left
#define U235SE_BUT_RIGHT  8         // Right
#define U235SE_BUT_HASH   16        // Hash (#)
#define U235SE_BUT_9      32        // 9
#define U235SE_BUT_6      64        // 6
#define U235SE_BUT_3      0x80       // 3
#define U235SE_BUT_PAUSE  0x100      // Pause
#define U235SE_BUT_A      0x200      // A button
#define U235SE_BUT_OPTION 0x800      // Option
#define U235SE_BUT_STAR   0x1000     // Star 
#define U235SE_BUT_7      0x2000     // 7
#define U235SE_BUT_4      0x4000     // 4
#define U235SE_BUT_1      0x8000     // 1
#define U235SE_BUT_0      0x10000    // 0 (zero)
#define U235SE_BUT_8      0x20000    // 8
#define U235SE_BUT_5      0x40000    // 5
#define U235SE_BUT_2      0x80000    // 2
#define U235SE_BUT_B      0x200000   // B button
#define U235SE_BUT_C      0x2000000  // C button

#define PAD1_A_DOWN     (pad_one & U235SE_BUT_A)
#define PAD1_B_DOWN     (pad_one & U235SE_BUT_B)
#define PAD1_C_DOWN     (pad_one & U235SE_BUT_C)

#define PAD1_X_DOWN     (pad_one & U235SE_BUT_9)
#define PAD1_Y_DOWN     (pad_one & U235SE_BUT_8)
#define PAD1_Z_DOWN     (pad_one & U235SE_BUT_7)

#define PAD1_L_DOWN     (pad_one & U235SE_BUT_4)    // left trigger on pro controller
#define PAD1_R_DOWN     (pad_one & U235SE_BUT_6)    // right trigger on pro controller

#define PAD1_STAR_DOWN  (pad_one & U235SE_BUT_STAR)
#define PAD1_HASH_DOWN  (pad_one & U235SE_BUT_HASH)

#define PAD1_OPTION_DOWN    (pad_one & U235SE_BUT_OPTION)

// these detect if the press has just started
#define PAD1_A_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_A))
#define PAD1_B_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_B))
#define PAD1_C_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_C))

#define PAD1_LEFT_PRESS     (pad_one_xor && (pad_one & U235SE_BUT_LEFT))
#define PAD1_RIGHT_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_RIGHT))
#define PAD1_UP_PRESS       (pad_one_xor && (pad_one & U235SE_BUT_UP))
#define PAD1_DOWN_PRESS     (pad_one_xor && (pad_one & U235SE_BUT_DOWN))

#define PAD1_PAUSE_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_PAUSE))
#define PAD1_OPTION_PRESS   (pad_one_xor && (pad_one & U235SE_BUT_OPTION))

#define PAD1_0_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_0))
#define PAD1_1_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_1))
#define PAD1_2_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_2))
#define PAD1_3_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_3))
#define PAD1_4_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_4))
#define PAD1_5_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_5))
#define PAD1_6_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_6))
#define PAD1_7_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_7))
#define PAD1_8_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_8))
#define PAD1_9_PRESS    (pad_one_xor && (pad_one & U235SE_BUT_9))

#define PAD1_STAR_PRESS     (pad_one_xor && (pad_one & U235SE_BUT_STAR))
#define PAD1_HASH_PRESS     (pad_one_xor && (pad_one & U235SE_BUT_HASH))

#endif // SOUND_H
