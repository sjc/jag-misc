#include <jaguar.h>
#include "draw.h"
#include "sound.h"

/* jaghello */

extern void *vidmem;

extern void VIDsync(void);

#define SETBG(c) *(short *)(0xF00058) = c;

void __main(void) {
  unsigned short bg = 0;

  /* load U235 into the DSP and setup variables used by the wrapper code */
  sound_init();

  SetScreenAddress((unsigned char *)&vidmem);

  *(unsigned short int *) 0xf00400 = 0x0000; /* Color reg 0 = black */
  *(unsigned short int *) 0xf00402 = 0xffff; /* Color reg 1 = white */

  DrawString(0, 0,"U235 wrapper example");
  DrawString(4, 9,"0,#,* - Mute, volume up and down");
  DrawString(4,18,"1 - Play first mod");
  DrawString(4,27,"2 - Play second mod");
  DrawString(4,36,"3 - Fade out playing mod");
  DrawString(4,45,"4 - Play first sound effect");
  DrawString(4,54,"5 - Play second sound effect");
  DrawString(4,63,"6 - Play third sound effect");

  while(1) {
    VIDsync();
    SETBG(bg++);

    /* take a random number from U235 and store it in a circular buffer, so
        we will always have a source of multiple random numbers available */
    store_random();

    /* U235 has already read the pad for us. Here we take the value it read
        and so the extra calculations so we can tell if a button has just
        been pressed (the _PRESS macros used below) */
    read_pad_one();

    /* check for the *, # and 0 music commands, and also process fading */
    process_sound_input();

    if (PAD1_1_PRESS) {
      queue_mod(0);
    } else if (PAD1_2_PRESS) {
      queue_mod(1);
    } else if (PAD1_3_PRESS) {
      fade_out_music();
    } else if (PAD1_4_PRESS) {
      queue_sfx(0);
    } else if (PAD1_5_PRESS) {
      queue_sfx(1);
    } else if (PAD1_6_PRESS) {
      queue_sfx(2);
    }

    /* check if any SFX has been queued (by queue_sfx()) this frame, match
        them with free sound channels, and pass them together to U235 */
    play_queued_sfx();
  }
}



