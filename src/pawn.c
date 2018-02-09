/*******************************************************************************
 * Stefan Bylund 2017
 *
 * The Pawn demo program for Sinclair ZX Spectrum Next.
 ******************************************************************************/

#include <arch/zxn.h>
#include <stdbool.h>
#include "adventure.h"

#pragma output CRT_ORG_CODE = 0x8184
#pragma output REGISTER_SP = 0xE000
#pragma output CLIB_MALLOC_HEAP_SIZE = 0
#pragma output CLIB_STDIO_HEAP_SIZE = 0
#pragma printf = "%s"

int main(void)
{
    init_hardware();

    play_music("sound/music.pt3");
    show_screen("gfx/title.nxi", INK_BLACK);
    wait_key();
    show_screen("gfx/credits.nxi", INK_BLACK);
    wait_key();
    stop_music();

    create_game_screen(INK_BLACK, 0x00, IO_TVM_HIRES_WHITE, "gfx/generic.nxi");
    create_game_world();

    while (true)
    {
        command_t command = read_input();
        handle_command(command);

        if (command == QUIT)
        {
            break;
        }
    }

    return 0;
}
