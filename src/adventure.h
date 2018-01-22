/*******************************************************************************
 * Stefan Bylund 2017
 *
 * Interface to the adventure game engine.
 ******************************************************************************/

#ifndef _ADVENTURE_H
#define _ADVENTURE_H

typedef enum command
{
    NORTH,
    SOUTH,
    WEST,
    EAST,
    NW,
    NE,
    SW,
    SE,
    UP,
    DOWN,
    LOOK,
    QUIT,
    NOP,
    UNKNOWN
} command_t;

void init_hardware(void);

void wait_key(void);

void show_screen(const char *screen_file, uint8_t border_color);

void play_music(const char *music_file);

void stop_music(void);

void create_game_screen(uint8_t border_color,
                        uint8_t layer2_background_color,
                        uint8_t timex_hires_colors,
                        const char *generic_image_file);

void create_game_world(void);

command_t read_input(void);

void handle_command(command_t command);

#endif
