/* Pushy II (R-Fred-W / FishNet Software, 1996; freeware): what libroger
 * needs to know about the game, and the desktop it is started from.
 *
 * !Run: Set PushyII$Dir <Obey$Dir>, IconSprites, WimpSlot -min 64k, Run
 * !RunImage. !RunImage is a Wimp task: it grows its slot as it loads
 * graphics, puts an icon on the icon bar and takes over the screen when
 * the icon is clicked, in the best mode the machine has - a Risc PC here,
 * so 640x400 at 24 bits per pixel (see ../README.md). */
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <roger.h>

extern const struct roger_image recomp_image;      /* gen/entries.c (gen_c.py) */

static const char *const prefixes[] = {"<PushyII$Dir>.", "<Obey$Dir>.", NULL};
/* PushyII$Dir is set (the game reads it only as a filename prefix,
 * stripped above) */
static const char *const variables[] = {"PushyII$Dir", "!PushyII", NULL};

#define APPDIR "../!PushyII"

/* --levels=NAME: the level set chosen from the icon bar menu's Levels
 * submenu, which lists the files in Levels (in RISC OS's order, names
 * compared without case): its item index, or -1 for the default */
static int levels_item = -1;

static int name_cmp(const void *a, const void *b) { return strcasecmp(*(char *const *)a, *(char *const *)b); }

/* --mode=N: the screen mode chosen from the icon bar menu's Mode submenu
 * (item N, below), -1 for the game's own choice (the first the machine
 * can show, in this order) */
static int mode_item = -1;
static const char *const mode_names[] = {
    "640x400, 24 bit, flip", "640x480, 24 bit", "640x400, 24 bit", "640x400, 8 bit, flip",
    "640x480, 8 bit, flip", "320x256, 8 bit, flip", "MODE 28", "MODE 13"};

static struct roger_config config;

static int arg(const char *a)
{
    if (!strcmp(a, "--archimedes")) { config.vidc20 = 0; return 1; }
    if (!strncmp(a, "--mode=", 7)) {
        mode_item = a[7] >= '0' && a[7] <= '7' && !a[8] ? a[7] - '0' : -1;
        if (mode_item < 0) {
            for (int k = 0; k < 8; k++) fprintf(stderr, "--mode=%d  %s\n", k, mode_names[k]);
            exit(1);
        }
        return 1;
    }
    if (strncmp(a, "--levels=", 9)) return 0;
    char *names[64]; int n = 0;
    DIR *d = opendir(APPDIR "/Levels");
    struct dirent *e;
    while (d && (e = readdir(d)) && n < 64)
        if (e->d_name[0] != '.') names[n++] = strdup(e->d_name);
    if (d) closedir(d);
    qsort(names, n, sizeof *names, name_cmp);
    for (int k = 0; k < n; k++)
        if (!strcasecmp(names[k], a + 9)) levels_item = k;
    if (levels_item < 0) {
        fprintf(stderr, "no level set %s; there are:", a + 9);
        for (int k = 0; k < n; k++) fprintf(stderr, " %s", names[k]);
        fprintf(stderr, "\n");
        exit(1);
    }
    return 1;
}

/* The desktop: !RunImage puts its icon on the icon bar and waits. Choose
 * the screen mode and the level set from its menu if asked (Mode and
 * Levels are the main menu's second and third items), then click the icon with Select to start the game; when the game
 * is left (Quit on its own menu, or Escape), it's back on the icon bar,
 * and is told to quit. */
static void wimp_idle(void)
{
    static int step;
    switch (step++) {
    case 0:
        if (mode_item >= 0) roger_wimp_menu((const int[]){1, mode_item, -1});
        if (levels_item >= 0) roger_wimp_menu((const int[]){2, levels_item, -1});
        roger_wimp_click(-2, 0, 4);
        break;
    default:
        roger_wimp_quit();
    }
}

/* Headless runs (--limit): Return on the title menu (Start Game), then
 * cursor keys in the first level, then Escape (the retry/quit menu). Key
 * numbers are INKEY's: Return 74, cursor left 26, right 122, up 58, down
 * 42, Escape 113. */
static const struct roger_key keys[] = {
    {74, 30000000, 31000000},
    {122, 60000000, 62000000}, {42, 64000000, 66000000}, {26, 68000000, 70000000},
    {58, 72000000, 74000000}, {113, 80000000, 80500000},
};

/* A Risc PC (vidc20): the game then offers its 24-bit and hi-res 256-colour
 * modes and graphics, and takes 640x400 at 24 bits per pixel; with
 * --archimedes it's an A3000 with RISC OS 3.1 (MODE 13, the lo-res
 * graphics). */
static struct roger_config config = {
    .title = "Pushy II (recompiled)",
    .image_path = APPDIR "/!RunImage",
    .image = &recomp_image,
    .appdir = APPDIR,
    .dir_prefixes = prefixes,
    .variables = variables,
    .initial_mode = 12,                   /* the desktop's; the game chooses MODE 13 */
    .wimpslot = 64 * 1024,                /* !Run's; Wimp_SlotSize grows it */
    .vidc20 = 1,                          /* --archimedes: 0 */
    .keys = keys,
    .nkeys = sizeof keys / sizeof keys[0],
    .wimp_idle = wimp_idle,
    .arg = arg,
    .usage = "  --levels=NAME     level set (a file in !PushyII.Levels: LegoLev, Pushy1)\n"
             "  --mode=N          screen mode from the game's Mode menu (--mode=list: the list)\n"
             "  --archimedes      an A3000 (MODE 13, lo-res graphics), not a Risc PC\n",
};

int main(int argc, char **argv) { roger_main(argc, argv, &config); }
