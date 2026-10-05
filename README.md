Pushy II (R-Fred-W / FishNet Software, 1996; freeware) - static recompilation
=============================================================================

`!PushyII` is the application as distributed (v2.12): `!RunImage` (an AIF
image written in assembler, 30K), graphics for three kinds of screen mode
(GFX8Lo, GFX8Hi, GFX24), two level sets (Levels.LegoLev, Levels.Pushy1),
sound and fade tables (Data). `!Help` and `Diary` are the author's notes.

    analysis/run.sh     SuperSpider (~/stuff/superspider) on !RunImage:
                        analysis/pushy2.json(.pickle), analysis/out/
    recomp/             make regen (analysis + gen_c.py), make, ./pushy2

Running
-------

    cd recomp && make && ./pushy2 [--levels=Pushy1] [--no-sound] [--scale=N] [--fullscreen]
    ./pushy2 --trace-gaps     interpret untranslated code instead of stopping (none found)
    ./pushy2 --limit=N        headless, deterministic: Return (Start Game), cursor keys, Escape

Keys (from !Help): menus - cursor up/down and Return or Space; in the
game - cursor keys, Escape for the retry/quit menu. Quit Game on the
title menu goes back to the desktop, and the run ends there.

What runs
---------

Pushy II is a desktop application. !Run sets PushyII$Dir, puts the
sprites in the Wimp's pool and runs !RunImage, which puts an icon on the
icon bar and waits in Wimp_Poll. Clicking it with Select takes over the
screen in a mode of its own choice; its menu (Menu on the icon) has Info,
Mode (the screen mode), Levels (one entry per file in Levels) and Quit.

libroger has no desktop: Wimp_Poll returns events game.c's wimp_idle hook
queues. The first time, that's the Levels menu choice if `--levels` was
given, then the Select click; when the game comes back to the icon bar,
Message_Quit (as the Task Manager sends it).

The screen mode: with a Wimp older than 3.50 (libroger's is 3.16, as on an
A3000) the game only considers its two non-RiscPC modes, MODE 28
(640x480, 256 colours) and MODE 13 (320x256, 256 colours), and takes the
first OS_CheckModeValid accepts. libroger has only the Archimedes' 50Hz
modes, so it's MODE 13 with the GFX8Lo graphics - the A3000 version. It
enlarges the screen's dynamic area (OS_ChangeDynamicArea) for two banks
and flips between them; its memory grows with Wimp_SlotSize as it loads
graphics.

It reads the keyboard from key transition events (event 11) through its
EventV handler, and plays its sound effects with its own voice
generator (one sound per channel, durations counted by the voice).

Not exercised: the RiscPC modes and their 24-bit and hi-res 256-colour
graphics, the gamma-table fades (PaletteV), the Info window (its template
is loaded but never opened).

The code
--------

SuperSpider finds 5524 code words in 165 functions, with no unresolved
jumps. Play-testing (title menu, both level sets, moves, the retry/quit
menu, quitting to the desktop) runs no untranslated code. Under 1% of
instructions are interpreted: the sprite plotters modify their own inner
loops (the non-StrongARM versions; the game
switches to unmodified copies only when OS_PlatformFeatures reports
separate caches, which RISC OS 3.1 doesn't have).

What it took
------------

libroger: the Wimp calls above, Wimp_SlotSize, OS_ReadDynamicArea and
OS_ChangeDynamicArea for the screen, OS_CheckModeValid,
OS_SWINumberFromString (no names known: the game probes for OS_ScreenMode
and OS_Memory), key transition events, OS_GBPB 9/10/12 (directory
listings: the Levels menu).

SuperSpider (see its pushy/PUSHY.md): the game's routines are reached
through variables set by setter routines (the main loop calls whatever
the current screen has installed), tables indexed by values from its
workspace, and menu objects holding their handlers. Before, SuperSpider
found 1684 code words and the recompilation interpreted half its
instructions.
