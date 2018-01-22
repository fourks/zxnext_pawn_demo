/*******************************************************************************
 * Stefan Bylund 2017
 *
 * API for creating the game world, its rooms and the connections between the
 * rooms.
 ******************************************************************************/

#ifndef _WORLD_H
#define _WORLD_H

#define MAX_FILENAME_LENGTH 12

typedef struct room
{
    char description_file[MAX_FILENAME_LENGTH + 1];
    char image_file[MAX_FILENAME_LENGTH + 1];
    struct room *north;
    struct room *south;
    struct room *west;
    struct room *east;
    struct room *nw;
    struct room *ne;
    struct room *sw;
    struct room *se;
    struct room *up;
    struct room *down;
} room_t;

void create_world(room_t **first_room);

#endif
