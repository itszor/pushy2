Pushy II (R-Fred-W / FishNet Software, 1996; freeware) - static recompilation
=============================================================================

`!PushyII` is the application as distributed (v2.12): `!RunImage` (an AIF
image written in assembler, 30K), graphics for three kinds of screen mode
(GFX8Lo, GFX8Hi, GFX24), two level sets (Levels.LegoLev, Levels.Pushy1),
sound and fade tables (Data). `!Help` and `Diary` are the author's notes.

    analysis/run.sh     SuperSpider (~/stuff/superspider) on !RunImage:
                        analysis/pushy2.json
    recomp/             make regen (analysis + superspider gen), make, ./pushy2

Running
-------

    cd recomp && make && ./pushy2 [--mode=N] [--levels=Pushy1] [--no-sound] [--scale=N] [--fullscreen]
    ./pushy2 --mode=list      the screen modes (as the game's Mode menu offers them)
    ./pushy2 --archimedes     an A3000 instead of a Risc PC: MODE 13, the lo-res graphics
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

The screen mode: the machine is a Risc PC (libroger's `vidc20`), so the
game offers all eight of its modes and, unless `--mode` picks one from
its Mode menu, takes the first the machine can show:

    --mode=0  640x400, 24 bit, flip   (the default: GFX24, two banks)
    --mode=1  640x480, 24 bit         (GFX24; the extra 80 lines black)
    --mode=2  640x400, 24 bit
    --mode=3  640x400, 8 bit, flip    (GFX8Hi)
    --mode=4  640x480, 8 bit, flip
    --mode=5  320x256, 8 bit, flip    (GFX8Lo)
    --mode=6  MODE 28                 (GFX8Hi)
    --mode=7  MODE 13                 (GFX8Lo)

It recognises a Risc PC by the Wimp's version (3.50 or later) and by
OS_SWINumberFromString knowing OS_ScreenMode and OS_Memory; then it
selects its modes with OS_ScreenMode and mode selectors, checks VRAM with
OS_Memory 8, and fades through VIDC20's gamma tables (PaletteV 9). With
`--archimedes` (a Wimp 3.16, as on an A3000) it only considers MODE 28
and MODE 13, and an Archimedes on a 50Hz monitor has only MODE 13: the
GFX8Lo graphics, faded by remapping pixels through Data.FadeTable. Either
way it enlarges the screen's dynamic area (OS_ChangeDynamicArea) for two
banks where it flips between them, and its memory grows with
Wimp_SlotSize as it loads graphics.

It reads the keyboard from key transition events (event 11) through its
EventV handler, and plays its sound effects with its own voice
generator (one sound per channel, durations counted by the voice).

Not exercised: the Info window (its template is loaded but never
opened), the diagnostics menu (`-d` on the command line).

The code
--------

SuperSpider finds 5524 code words in 165 functions, with no unresolved
jumps. Play-testing (title menu, both level sets, moves, the retry/quit
menu, quitting to the desktop; each of the eight modes) runs no
untranslated code. Under 1% of
instructions are interpreted: the sprite plotters modify their own inner
loops (the non-StrongARM versions; the game
switches to unmodified copies only when OS_PlatformFeatures reports
separate caches, which RISC OS 3.1 doesn't have).

What it took
------------

libroger: a Risc PC (`vidc20`: VIDC20's modes and mode selectors up to
32 bits per pixel, OS_ScreenMode, OS_Memory 8, gamma tables), the Wimp
calls above, Wimp_SlotSize, OS_ReadDynamicArea and
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
