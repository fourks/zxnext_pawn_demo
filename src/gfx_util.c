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

// From zxnext_layer2.lib.
extern uint8_t buf_256[];

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
