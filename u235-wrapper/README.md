# U-236 Wrapper

Helper code for using the [U-235 Sound Engine](https://www.u-235.co.uk/developer/sound-engine/), mostly from C.

### Features

* Setup function
* MOD playback, including cross-fading between tracks
* SFX playback, with queue to batch multiple SFX per frame
* Handling of standard Jaguar 0, # and * volume controls
* Extra macros to help with joypad reading and button detection
* Random number pooling

The functions available are documented in `sound.h` and demonstrated in the example `main.c`.

## Using the code

The code, and the example project, are intended to be built via the [Jaguar SDK](https://github.com/cubanismo/jaguar-sdk), or at the very least using an assembler which recognises RMAC syntax.

### Files

To use this code in your own project, you will need to include the following files:

* `sound.h`
* `sound.s`
* `sound_utils.c`

You will also need `u235se-rel.bin` and `u235se-rel.inc`. Either include them in a `u235` subfolder or update the references in `sound.s` to point to them.

You will also also need to expose `mod_table` and `sample_bank` from somewhere in your project. The included `sound_data.s` is one example of how you could do this.

### Symbols

The library expects to find `_VIDpal` available to link with. This should have a value of 1 on PAL systems and 0 on NTSC. In the example code it is set in `startup.s`, and many of the other Atari example projects already do the same.

### Compiler flags

The assembly code in the project expects `short` (`word`) values passed by C on the stack to occupy only 2 bytes (rather than be padded to 4). Since this is not the default, the `-mshort` flag should be passed to GCC.

### SILENT flag

If the symbol `SILENT` is defined, the library will compile with playing modules and samples disabled, but all other code intact. This may be useful if you're sick to the back teeth of hearing your game's bloody music over and over and over again. The included `Makefile` has code to show how you can pass this on from an invocation of `make SILENT=1`.

## Example Assets

* ["Once is not enough"](https://modarchive.org/index.php?request=view_by_moduleid&query=170002) by [Zilly Mike](https://modarchive.org/index.php?request=view_profile&query=69027)
* ["Neon Techno"](https://modarchive.org/index.php?request=view_by_moduleid&query=178172) by [Drozerix](https://modarchive.org/index.php?request=view_profile&query=84702)
* SFX from ["Essential Retro Video Game Sound FX"](https://subspaceaudio.itch.io/1000-retro-sound-effects) by SubspaceAudio
