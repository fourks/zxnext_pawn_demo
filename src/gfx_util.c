/*******************************************************************************
 * Stefan Bylund 2017
 *
 * Graphics utility functions.
 ******************************************************************************/

#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "gfx_util.h"
#include "zxnext_layer2.h"

#define SCREEN_ADDRESS ((uint8_t *) 0xE000)

static void set_palette_color(uint8_t index, uint8_t color);

// From zxnext_layer2.lib.
extern uint8_t buf_256[];

void set_timex_hires_colors(uint8_t fg_color, uint8_t bg_color)
{
    // Set the first ULA palette as the read/write palette.
    ZXN_WRITE_REG(REG_PALETTE_CONTROL, 0);

    /*
     * NOTE: The foreground and background colors in the Timex high-resolution
     * graphics mode are selected differently in ZEsarUX and CSpect!
     *
     * ZEsarUX: Foreground color is at ULA palette index 0 and background color
     * is at ULA palette index 128.
     *
     * CSpect: Foreground color is at ULA palette index 7 and background color
     * is at ULA palette index 16.
     *
     * FIXME: How is the foreground and background color set on a real Spectrum Next?
     */

    // ZEsarUX foreground and background color.
    set_palette_color(0, fg_color);
    set_palette_color(128, bg_color);

    // CSpect foreground and background color.
    set_palette_color(7, fg_color);
    set_palette_color(16, bg_color);
}

void gfx_load_screen(const char *filename, uint8_t num_lines)
{
    uint8_t filehandle;
    uint8_t screen_start_page;

    if ((filename == NULL) || (num_lines == 0))
    {
        return;
    }

    if (num_lines > 192)
    {
        num_lines = 192;
    }

    // Disable layer 2 screen writing at bottom 16 KB prior to ESXDOS usage.
    layer2_configure(true, false, false, 0);

    errno = 0;
    filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (errno)
    {
        return;
    }

    // Load palette.
    esxdos_f_read(filehandle, buf_256, 256);
    if (errno)
    {
        goto end;
    }
    layer2_set_palette((uint16_t *) buf_256, 128, 0);
    esxdos_f_read(filehandle, buf_256, 256);
    if (errno)
    {
        goto end;
    }
    layer2_set_palette((uint16_t *) buf_256, 128, 128);

    // Load screen in max 8 KB chunks using MMU slot 7 at address 0xE000.

    screen_start_page = layer2_get_main_screen_ram_bank() << 1;

    if (num_lines <= 32)
    {
        ZXN_WRITE_MMU7(screen_start_page);
        esxdos_f_read(filehandle, SCREEN_ADDRESS, num_lines << 8);
    }
    else
    {
        uint8_t num_pages = num_lines / 32;
        uint8_t rest_lines = num_lines % 32;
        uint8_t page;

        for (page = screen_start_page; page < screen_start_page + num_pages; page++)
        {
            ZXN_WRITE_MMU7(page);
            esxdos_f_read(filehandle, SCREEN_ADDRESS, 8192);
            if (errno)
            {
                goto end;
            }
        }

        if (rest_lines > 0)
        {
            ZXN_WRITE_MMU7(page);
            esxdos_f_read(filehandle, SCREEN_ADDRESS, rest_lines << 8);
        }
    }

end:
    // Restore original page in MMU slot 7.
    // TODO: The MMU slot registers are not readable in CSpect. Hence we cannot
    // read the original MMU slot 7 value and restore it page. Instead, we must
    // set it to its default page which is correct in this case.
    ZXN_WRITE_MMU7(1);
    esxdos_f_close(filehandle);
}

static void set_palette_color(uint8_t index, uint8_t color)
{
    ZXN_WRITE_REG(REG_PALETTE_INDEX, index);
    ZXN_WRITE_REG(REG_PALETTE_VALUE_8, color);
}
