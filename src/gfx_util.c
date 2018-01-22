/*******************************************************************************
 * Stefan Bylund 2017
 *
 * Graphics utility functions.
 ******************************************************************************/

#include <arch/zxn/esxdos.h>
#include <z80.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "gfx_util.h"
#include "zxnext_registers.h"
#include "zxnext_layer2.h"

#define SCREEN_ADDRESS ((uint8_t *) 0)

static void set_palette_color(uint8_t index, uint8_t color);

// From zxnext_layer2.lib.
extern uint8_t buf_256[];

void set_timex_hires_colors(uint8_t fg_color, uint8_t bg_color)
{
    // Set the first ULA palette as the read/write palette.
    z80_outp(REGISTER_NUMBER_PORT, PALETTE_CONTROL_REGISTER);
    z80_outp(REGISTER_VALUE_PORT, 0);

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

    if ((filename == NULL) || (num_lines == 0))
    {
        return;
    }

    if (num_lines > 192)
    {
        num_lines = 192;
    }

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

    // Load screen.
    if (num_lines <= 64)
    {
        layer2_configure(true, true, false, LAYER2_SCREEN_TOP);
        esxdos_f_read(filehandle, SCREEN_ADDRESS, num_lines << 8);
    }
    else if (num_lines <= 128)
    {
        layer2_configure(true, true, false, LAYER2_SCREEN_TOP);
        esxdos_f_read(filehandle, SCREEN_ADDRESS, 16384);
        if (errno)
        {
            goto end;
        }

        layer2_configure(true, true, false, LAYER2_SCREEN_MIDDLE);
        esxdos_f_read(filehandle, SCREEN_ADDRESS, (num_lines - 64) << 8);
    }
    else
    {
        layer2_configure(true, true, false, LAYER2_SCREEN_TOP);
        esxdos_f_read(filehandle, SCREEN_ADDRESS, 16384);
        if (errno)
        {
            goto end;
        }

        layer2_configure(true, true, false, LAYER2_SCREEN_MIDDLE);
        esxdos_f_read(filehandle, SCREEN_ADDRESS, 16384);
        if (errno)
        {
            goto end;
        }

        layer2_configure(true, true, false, LAYER2_SCREEN_BOTTOM);
        esxdos_f_read(filehandle, SCREEN_ADDRESS, (num_lines - 128) << 8);
    }

end:
    layer2_configure(true, false, false, 0);
    esxdos_f_close(filehandle);
}

static void set_palette_color(uint8_t index, uint8_t color)
{
    z80_outp(REGISTER_NUMBER_PORT, PALETTE_INDEX_REGISTER);
    z80_outp(REGISTER_VALUE_PORT, index);

    z80_outp(REGISTER_NUMBER_PORT, PALETTE_VALUE_8BIT_REGISTER);
    z80_outp(REGISTER_VALUE_PORT, color);
}
