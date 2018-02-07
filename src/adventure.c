/*******************************************************************************
 * Stefan Bylund 2017
 *
 * Adventure game engine.
 ******************************************************************************/

#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>
#include <z80.h>
#include <intrinsic.h>
#include <input.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <errno.h>

#include "adventure.h"
#include "world.h"
#include "str_util.h"
#include "gfx_util.h"
#include "zxnext_layer2.h"
#include "vt_sound.h"

#define FD_OUT 2
#define FD_IN 1

#define MAX_IMAGE_HEIGHT 134
#define MAX_TEXT_LENGTH 1096
#define MAX_LINE_LENGTH 256
#define MAX_FILEPATH_LENGTH 20

#define TEXT_DIR "text/"
#define GFX_DIR "gfx/"
#define VT_SOUND_BIN "vt_sound.bin"

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

static void init_isr(void);

static void reset_hardware(void);

static size_t load_file(const char *filename, uint8_t *address);

static void show_current_room(bool load_image);

static void goto_room(room_t *next_room);

static void load_room_image(const char *filename);

static void load_room_text(const char *filename, char *buffer);

static void print_dirs(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

// Output terminal window (fd 2).
extern FILE *window_out;

// Input/output terminal window (fd 1).
extern FILE *window_in;

static char text_buffer[MAX_TEXT_LENGTH];

static char line_buffer[MAX_LINE_LENGTH];

static room_t *current_room;

static uint8_t layer2_bg_color;

static char generic_image[16];

static const char *dir_names[] =
{
    "north",
    "south",
    "west",
    "east",
    "nw",
    "ne",
    "sw",
    "se",
    "up",
    "down"
};

/*******************************************************************************
 * Functions
 ******************************************************************************/

void init_hardware(void)
{
    // Put Z80 in 14 MHz turbo mode.
    ZXN_WRITE_REG(REG_PERIPHERAL_2, ZXN_READ_REG(REG_PERIPHERAL_2) | RP2_ENABLE_TURBO);
    ZXN_WRITE_REG(__REG_TURBO_MODE, RTM_14MHZ);

    // Enable Timex modes.
    ZXN_WRITE_REG(REG_PERIPHERAL_3, ZXN_READ_REG(REG_PERIPHERAL_3) | RP3_ENABLE_TIMEX);

    // TODO: This is not set as default in some emulators.
    layer2_set_main_screen_ram_bank(8);

    // Initialize IM2 interrupt service routine.
    init_isr();
}

void wait_key(void)
{
    in_wait_nokey();
    in_wait_key();
    in_wait_nokey();
}

void show_screen(const char *screen_file, uint8_t border_color)
{
    zx_border(border_color);
    layer2_configure(true, false, false, 0);
    layer2_clear_screen(0x00, NULL);
    layer2_set_rw_palette(true);
    layer2_load_screen(screen_file, NULL, 7, true);
}

void play_music(const char *music_file)
{
    size_t vt_sound_size;
    uint8_t *music_module;

    if (music_file == NULL)
    {
        return;
    }

    // Load the vt_sound binary at address 0x4000 (ULA screen).
    // This is done in order to avoid using memory between 0x8000 and 0xFFFF.
    vt_sound_size = load_file(VT_SOUND_BIN, (uint8_t *) 0x4000);

    // Load the music file right after the vt_sound binary.
    music_module = (uint8_t *) (0x4000 + vt_sound_size);
    load_file(music_file, music_module);

    // Initialize vt_sound with the music module.
    vt_init(music_module);

    // Install the vt_play_isr() interrupt service routine.
    // The music will now play in the background.
    intrinsic_di();
    z80_bpoke(0x8181, 0xC3);
    z80_wpoke(0x8182, (uint16_t) vt_play_isr);
    intrinsic_ei();
}

void stop_music(void)
{
    // Disable the vt_play_isr() interrupt service routine and mute the sound.
    vt_set_play_isr_enabled(false);
    vt_mute();

    // Install an empty interrupt service routine.
    intrinsic_di();
    z80_bpoke(0x8181, 0xFB);
    z80_bpoke(0x8182, 0xED);
    z80_bpoke(0x8183, 0x4D);
    intrinsic_ei();

    // Clear the ULA screen to remove the vt_sound binary and music module.
    zx_cls(INK_BLACK | PAPER_BLACK);
}

void create_game_screen(uint8_t border_color,
                        uint8_t layer2_background_color,
                        uint8_t timex_hires_colors,
                        const char *generic_image_file)
{
    // Set border color.
    /*
     * TODO: In the Timex high-resolution graphics mode, ZEsarUX picks the
     * border color from ULA palette index 0 to 7 (inks) while according to the
     * specification it should pick it from ULA palette index 128 to 135 (papers).
     */
    zx_border(border_color);

    // Set foreground and background colors for Timex high-resolution graphics mode.
    // TODO: Setting this via ts_vmod(), i.e. port 0xFF, doesn't work in the emulators.
    set_timex_hires_colors(0xFF, 0x00);

    // Enable layer 2 screen (by default on top of ULA screen) and clear it.
    // Set the first layer 2 palette as the read/write palette and reset its
    // colors to the standard colors.
    // The main top part is cleared with the given background color and the
    // bottom part is cleared with the transparency color so that the ULA screen
    // will show through.
    layer2_configure(true, false, false, 0);
    layer2_clear_screen(0x00, NULL);
    layer2_set_rw_palette(true);
    layer2_reset_palette();
    layer2_fill_rect(0, 0, 256, MAX_IMAGE_HEIGHT, layer2_background_color, NULL);
    layer2_fill_rect(0, MAX_IMAGE_HEIGHT, 256, 192 - MAX_IMAGE_HEIGHT, 0xE3, NULL);
    layer2_bg_color = layer2_background_color;

    // Store the generic image filename, if any. If NULL, the image area for
    // rooms without an image is filled with the layer2_background_color instead.
    if (generic_image_file != NULL)
    {
        strcpy(generic_image, generic_image_file);
    }

    // Enable Timex high-resolution graphics mode for the ULA screen with the
    // given ink/paper color combination and clear the screen.
    // Note: Memory areas 0x4000 - 0x57FF and 0x6000 - 0x77FF are used.
    ts_vmod(TVM_HIRES | ((timex_hires_colors & 0x07) << 3));
    memset((void *) 0x4000, 0, 0x1800);
    memset((void *) 0x6000, 0, 0x1800);
    // FIXME: Use tshr_cls_pix() instead of memset() when supported by SCCZ80.
    //tshr_cls_pix(0);

    // Clear output and input terminals.
    ioctl(FD_OUT, IOCTL_OTERM_CLS);
    ioctl(FD_IN, IOCTL_OTERM_CLS);
}

void create_game_world(void)
{
    create_world(&current_room);
    show_current_room(true);
}

command_t read_input(void)
{
    char *result;
    char *word;

    ioctl(FD_IN, IOCTL_OTERM_CLS);
    fprintf(window_in, "> ");
    fflush(stdin);

    // Wait for and read input until Enter is pressed.
    memset(line_buffer, 0, sizeof(line_buffer));
    result = fgets(line_buffer, sizeof(line_buffer), stdin);

    if ((result == NULL) || (strlen(result) <= 1))
    {
        return NOP;
    }

    str_lower_case(line_buffer);
    word = strtok(line_buffer, " \n");

    if (word != NULL)
    {
        if (!strcmp(word, "north") || !strcmp(word, "n"))
        {
            return NORTH;
        }
        else if (!strcmp(word, "south") || !strcmp(word, "s"))
        {
            return SOUTH;
        }
        else if (!strcmp(word, "west") || !strcmp(word, "w"))
        {
            return WEST;
        }
        else if (!strcmp(word, "east") || !strcmp(word, "e"))
        {
            return EAST;
        }
        else if (!strcmp(word, "northwest") || !strcmp(word, "nw"))
        {
            return NW;
        }
        else if (!strcmp(word, "northeast") || !strcmp(word, "ne"))
        {
            return NE;
        }
        else if (!strcmp(word, "southwest") || !strcmp(word, "sw"))
        {
            return SW;
        }
        else if (!strcmp(word, "southeast") || !strcmp(word, "se"))
        {
            return SE;
        }
        else if (!strcmp(word, "up") || !strcmp(word, "u"))
        {
            return UP;
        }
        else if (!strcmp(word, "down") || !strcmp(word, "d"))
        {
            return DOWN;
        }
        else if (!strcmp(word, "look") || !strcmp(word, "l"))
        {
            return LOOK;
        }
        else if (!strcmp(word, "quit"))
        {
            return QUIT;
        }
   }

   return UNKNOWN;
}

void handle_command(command_t command)
{
    switch (command)
    {
        case NORTH:
            goto_room(current_room->north);
            break;
        case SOUTH:
            goto_room(current_room->south);
            break;
        case WEST:
            goto_room(current_room->west);
            break;
        case EAST:
            goto_room(current_room->east);
            break;
        case NW:
            goto_room(current_room->nw);
            break;
        case NE:
            goto_room(current_room->ne);
            break;
        case SW:
            goto_room(current_room->sw);
            break;
        case SE:
            goto_room(current_room->se);
            break;
        case UP:
            goto_room(current_room->up);
            break;
        case DOWN:
            goto_room(current_room->down);
            break;
        case LOOK:
            show_current_room(false);
            break;
        case QUIT:
            reset_hardware();
            break;
        case NOP:
            break;
        case UNKNOWN:
            // Fall through
        default:
            fprintf(window_out, "\nI don't understand.");
            break;
    }
}

static void init_isr(void)
{
    // Set up IM2 interrupt service routine:
    // Put Z80 in IM2 mode with a 257-byte interrupt vector table located
    // at 0x8000 (before CRT_ORG_CODE) filled with 0x81 bytes. Install an
    // empty interrupt service routine at the interrupt service routine
    // entry at address 0x8181.

    intrinsic_di();
    im2_init((void *) 0x8000);
    memset((void *) 0x8000, 0x81, 257);
    z80_bpoke(0x8181, 0xFB);
    z80_bpoke(0x8182, 0xED);
    z80_bpoke(0x8183, 0x4D);
    intrinsic_ei();
}

static void reset_hardware(void)
{
    // Disable layer 2 screen.
    layer2_configure(false, false, false, 0);

    // Disable Timex high-resolution graphics mode.
    ts_vmod(TVM_SPECTRUM);
    ZXN_WRITE_REG(REG_PERIPHERAL_3, RP3_ENABLE_SPEAKER); // Default settings

    zx_border(INK_WHITE);
    zx_cls(INK_BLACK | PAPER_WHITE);
}

static size_t load_file(const char *filename, uint8_t *address)
{
    uint8_t filehandle;
    struct esxdos_stat stat;
    size_t size = 0;

    errno = 0;
    filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (!errno)
    {
        esxdos_f_fstat(filehandle, &stat);
        size = (size_t) stat.size;
        esxdos_f_read(filehandle, address, size);
        esxdos_f_close(filehandle);
    }

    return size;
}

static void show_current_room(bool load_image)
{
    // Load and display room image.

    if (load_image)
    {
        load_room_image(current_room->image_file);
    }

    // Load and display room description.

    memset(text_buffer, 0, sizeof(text_buffer));
    load_room_text(current_room->description_file, text_buffer);

    str_remove_cr(text_buffer);
    str_word_wrap(window_out, text_buffer);
    str_remove_trailing_lf(text_buffer);

    ioctl(FD_OUT, IOCTL_OTERM_CLS);
    ioctl(FD_OUT, IOCTL_OTERM_PAUSE, 1);
    ioctl(FD_OUT, IOCTL_OTERM_PAGE, 1);
    fprintf(window_out, text_buffer);
    ioctl(FD_OUT, IOCTL_OTERM_PAUSE, 0);
    ioctl(FD_OUT, IOCTL_OTERM_PAGE, 0);
}

static void goto_room(room_t *next_room)
{
    if (next_room != NULL)
    {
        current_room = next_room;
        show_current_room(true);
    }
    else
    {
        fprintf(window_out, "\nYou can't go that way.");
        print_dirs();
    }
}

static void load_room_image(const char *filename)
{
    uint8_t transparency_color[2] = {0xE3, 0};

    intrinsic_halt();

    /*
     * Load the layer 2 image, if any, by temporarily using MMU slot 3 where
     * half of the Timex hi-res screen resides. We can safely do this in the
     * vertical blanking interval without disturbing the display.
     */

    if (strlen(filename) > 0)
    {
        char filepath[MAX_FILEPATH_LENGTH];
        strcpy(filepath, GFX_DIR);
        strcat(filepath, filename);
        gfx_load_screen(filepath, MAX_IMAGE_HEIGHT);
        layer2_set_palette((uint16_t *) transparency_color, 1, 0xE3);
    }
    else if (strlen(generic_image) > 0)
    {
        layer2_fill_rect(0, 0, 256, MAX_IMAGE_HEIGHT, layer2_bg_color, NULL);
        gfx_load_screen(generic_image, MAX_IMAGE_HEIGHT);
        layer2_set_palette((uint16_t *) transparency_color, 1, 0xE3);
    }
    else
    {
        layer2_fill_rect(0, 0, 256, MAX_IMAGE_HEIGHT, layer2_bg_color, NULL);
        layer2_reset_palette();
    }
}

static void load_room_text(const char *filename, char *buffer)
{
    char filepath[MAX_FILEPATH_LENGTH];
    uint8_t filehandle;

    strcpy(filepath, TEXT_DIR);
    strcat(filepath, filename);

    errno = 0;
    filehandle = esxdos_f_open(filepath, ESXDOS_MODE_R | ESXDOS_MODE_OE);

    if (!errno)
    {
        struct esxdos_stat stat;
        size_t size;

        esxdos_f_fstat(filehandle, &stat);
        size = (stat.size > MAX_TEXT_LENGTH) ? MAX_TEXT_LENGTH : stat.size;
        esxdos_f_read(filehandle, buffer, size);
        esxdos_f_close(filehandle);
    }
}

static void print_dirs(void)
{
    command_t dirs[10];
    uint8_t num_dirs = 0;
    char buf[50];
    char *buf_ptr = buf;
    uint8_t i;

    memset(buf, 0, sizeof(buf));

    if (current_room->north != NULL)
    {
        dirs[num_dirs++] = NORTH;
    }
    if (current_room->south != NULL)
    {
        dirs[num_dirs++] = SOUTH;
    }
    if (current_room->west != NULL)
    {
        dirs[num_dirs++] = WEST;
    }
    if (current_room->east != NULL)
    {
        dirs[num_dirs++] = EAST;
    }
    if (current_room->nw != NULL)
    {
        dirs[num_dirs++] = NW;
    }
    if (current_room->ne != NULL)
    {
        dirs[num_dirs++] = NE;
    }
    if (current_room->sw != NULL)
    {
        dirs[num_dirs++] = SW;
    }
    if (current_room->se != NULL)
    {
        dirs[num_dirs++] = SE;
    }
    if (current_room->up != NULL)
    {
        dirs[num_dirs++] = UP;
    }
    if (current_room->down != NULL)
    {
        dirs[num_dirs++] = DOWN;
    }

    for (i = 0; i < num_dirs; i++)
    {
        if (i > 0)
        {
            const char *sep = (i < (num_dirs - 1)) ? ", " : " or ";
            strcpy(buf_ptr, sep);
            buf_ptr += strlen(sep);
        }

        strcpy(buf_ptr, dir_names[dirs[i]]);
        buf_ptr += strlen(dir_names[dirs[i]]);
    }

    if (num_dirs > 0)
    {
        fprintf(window_out, "\nYou can only go %s.", buf);
    }
}
