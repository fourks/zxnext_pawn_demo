/*******************************************************************************
 * Stefan Bylund 2017
 *
 * The game world of The Pawn, its rooms and the connections between the rooms.
 ******************************************************************************/

#include "world.h"

// Kitchen
static room_t room01 =
{
    "room01.txt"
};

// Store
static room_t room02 =
{
    "room02.txt"
};

// Rank Forest
static room_t room03 =
{
    "room03.txt",
    "forest.nxi"
};

// Lift
static room_t room04 =
{
    "room04.txt"
};

// Rockface
static room_t room05 =
{
    "room05.txt"
};

// Lounge
static room_t room06 =
{
    "room06.txt",
    "lounge.nxi"
};

// Room of Incarceration (princess)
static room_t room07 =
{
    "room07.txt",
    "princess.nxi"
};

// Landing
static room_t room08 =
{
    "room08.txt"
};

// Inside the Tower
static room_t room09 =
{
    "room09.txt"
};

// Uncultivated Hills
static room_t room10 =
{
    "room10.txt",
    "forest.nxi"
};

// Rank Forest
static room_t room11 =
{
    "room11.txt",
    "forest.nxi"
};

// Rank Forest
static room_t room12 =
{
    "room12.txt",
    "forest.nxi"
};

// Passage
static room_t room13 =
{
    "room13.txt"
};

// Passage
static room_t room14 =
{
    "room14.txt"
};

// Wall
static room_t room15 =
{
    "room15.txt"
};

// Plateau
static room_t room16 =
{
    "room16.txt"
};

// Plateau (snowman)
static room_t room17 =
{
    "room17.txt",
    "snowman.nxi"
};

// Inside the Hut (guru)
static room_t room18 =
{
    "room18.txt",
    "guru.nxi"
};

// Large Hill
static room_t room19 =
{
    "room19.txt"
};

// Rolling Hills
static room_t room20 =
{
    "room20.txt"
};

// Rank Forest
static room_t room21 =
{
    "room21.txt",
    "forest.nxi"
};

// Forest Clearing
static room_t room22 =
{
    "room22.txt",
    "clearing.nxi"
};

// Rank Forest (tree)
static room_t room23 =
{
    "room23.txt",
    "tree.nxi"
};

// Up the Tree
static room_t room24 =
{
    "room24.txt"
};

// Crossroads
static room_t room25 =
{
    "room25.txt"
};

// Plateau
static room_t room26 =
{
    "room26.txt"
};

// Plateau
static room_t room27 =
{
    "room27.txt"
};

// Narrow Track
static room_t room28 =
{
    "room28.txt"
};

// Narrow Track
static room_t room29 =
{
    "room29.txt"
};

// Mountainous Hills
static room_t room30 =
{
    "room30.txt"
};

// Rank Forest
static room_t room31 =
{
    "room31.txt",
    "forest.nxi"
};

// Rank Forest
static room_t room32 =
{
    "room32.txt",
    "forest.nxi"
};

// Rank Forest
static room_t room33 =
{
    "room33.txt",
    "forest.nxi"
};

// Tree Trunk Room
static room_t room34 =
{
    "room34.txt"
};

// Staircase Room
static room_t room35 =
{
    "room35.txt",
    "stair.nxi"
};

// Low Ceiling Room
static room_t room36 =
{
    "room36.txt"
};

// Circular Platform
static room_t room37 =
{
    "room37.txt"
};

// Workshop
static room_t room38 =
{
    "room38.txt",
    "workshop.nxi"
};

// Chamber (dragon)
static room_t room39 =
{
    "room39.txt",
    "chamber.nxi"
};

// Narrow Track
static room_t room40 =
{
    "room40.txt",
    "track.nxi"
};

// Cavemouth
static room_t room41 =
{
    "room41.txt"
};

// Foothills
static room_t room42 =
{
    "room42.txt",
    "foothill.nxi"
};

// On the Path
static room_t room43 =
{
    "room43.txt",
    "path.nxi"
};

// On the Path
static room_t room44 =
{
    "room44.txt",
    "path.nxi"
};

// On the Path (first room)
static room_t room45 =
{
    "room45.txt",
    "path.nxi"
};

// On the Path
static room_t room46 =
{
    "room46.txt",
    "path.nxi"
};

// On the Path
static room_t room47 =
{
    "room47.txt",
    "path.nxi"
};

// Office
static room_t room48 =
{
    "room48.txt"
};

// White Room
static room_t room49 =
{
    "room49.txt"
};

// High Ledge
static room_t room50 =
{
    "room50.txt"
};

// Rope Bridge
static room_t room51 =
{
    "room51.txt",
    "rope.nxi"
};

// Rope Bridge
static room_t room52 =
{
    "room52.txt",
    "rope.nxi"
};

// High Ledge
static room_t room53 =
{
    "room53.txt"
};

// Cavern
static room_t room54 =
{
    "room54.txt"
};

// Grassy Plain
static room_t room55 =
{
    "room55.txt",
    "plain.nxi"
};

// Grassy Wilderness
static room_t room56 =
{
    "room56.txt",
    "grassy.nxi"
};

// Grassy Plain
static room_t room57 =
{
    "room57.txt",
    "grassy.nxi"
};

// Voting Booth
static room_t room58 =
{
    "room58.txt",
    "voting.nxi"
};

// Room
static room_t room59 =
{
    "room59.txt",
    "graffiti.nxi"
};

// Tunnel
static room_t room60 =
{
    "room60.txt",
    "tunnel.nxi"
};

// Passage
static room_t room61 =
{
    "room61.txt"
};

// Cavern (stalagmites and stalactites)
static room_t room62 =
{
    "room62.txt",
    "cavern.nxi"
};

// Cliff
static room_t room63 =
{
    "room63.txt"
};

// Stone Bridge
static room_t room64 =
{
    "room64.txt",
    "bridge.nxi"
};

// Bank
static room_t room65 =
{
    "room65.txt",
    "bank.nxi"
};

// Ledge
static room_t room66 =
{
    "room66.txt",
    "ledge.nxi"
};

// Passage
static room_t room67 =
{
    "room67.txt"
};

// Laboratory
static room_t room68 =
{
    "room68.txt",
    "lab.nxi"
};

// Damp Passage
static room_t room69 =
{
    "room69.txt"
};

// Small Cave
static room_t room70 =
{
    "room70.txt"
};

// Entrance
static room_t room71 =
{
    "room71.txt"
};

// Corridor
static room_t room72 =
{
    "room72.txt"
};

// Computer Room
static room_t room73 =
{
    "room73.txt"
};

// On the Bridge
static room_t room74 =
{
    "room74.txt",
    "bridge.nxi"
};

// In the Shed
static room_t room75 =
{
    "room75.txt"
};

// Shaft
static room_t room76 =
{
    "room76.txt",
    "shaft.nxi"
};

// Ledge
static room_t room77 =
{
    "room77.txt"
};

// Store Room
static room_t room78 =
{
    "room78.txt",
    "store.nxi"
};

// Corridor
static room_t room79 =
{
    "room79.txt"
};

// Palace Gardens
static room_t room80 =
{
    "room80.txt",
    "palace.nxi"
};

// Annexe
static room_t room81 =
{
    "room81.txt",
    "annexe.nxi"
};

// Shaft
static room_t room82 =
{
    "room82.txt",
    "shaft.nxi"
};

// Cavern
static room_t room83 =
{
    "room83.txt"
};

// Lava River
static room_t room84 =
{
    "room84.txt"
};

// Gateway
static room_t room85 =
{
    "room85.txt",
    "palace.nxi"
};

// Cavern (demons)
static room_t room86 =
{
    "room86.txt",
    "demons.nxi"
};

// Chamber
static room_t room87 =
{
    "room87.txt"
};

// Circular Room
static room_t room88 =
{
    "room88.txt"
};

// Irrelevant Maze
static room_t room89 =
{
    "room89.txt"
};

// Vertical Shaft
static room_t room90 =
{
    "room90.txt"
};

// Hell
static room_t room91 =
{
    "room91.txt",
    "hell.nxi"
};

// Riverside Chamber
static room_t room92 =
{
    "room92.txt"
};

// Lift (this extra room represents taking the lift down in room 4)
static room_t room93 =
{
    "room93.txt"
};

void create_world(room_t **first_room)
{
    room01.east = &room06;

    room02.east = &room09;

    room03.east = &room10;

    room04.south = &room13;
    room04.down = &room93;

    room93.up = &room04;
    room93.south = &room05;

    room05.north = &room93;

    room06.west = &room01;
    room06.east = &room14;

    room07.east = &room15;
    room07.south = &room08;

    room08.north = &room07;
    room08.down = &room09;

    room09.west = &room02;
    room09.sw = &room17;
    room09.up = &room08;

    room10.west = &room03;
    room10.east = &room21;
    room10.south = &room11;

    room11.north = &room10;
    room11.east = &room22;
    room11.south = &room12;

    room12.north = &room11;
    room12.east = &room23;

    room13.north = &room04;
    room13.se = &room25;

    room14.ne = &room25;
    room14.west = &room06;

    room15.west = &room07;
    room15.up = &room07;
    room15.down = &room27;

    room16.east = &room26;
    room16.south = &room17;

    room17.north = &room16;
    room17.ne = &room09;
    room17.east = &room27;

    room18.south = &room19;

    room19.north = &room18;
    room19.south = &room20;

    room20.north = &room19;
    room20.east = &room30;
    room20.south = &room21;

    room21.north = &room20;
    room21.west = &room10;
    room21.east = &room31;
    room21.south = &room22;

    room22.north = &room21;
    room22.west = &room11;
    room22.east = &room32;
    room22.south = &room23;

    room23.north = &room22;
    room23.west = &room12;
    room23.east = &room33;
    room23.up = &room24;

    room24.east = &room34;
    room24.down = &room23;

    room25.nw = &room13;
    room25.east = &room36;
    room25.sw = &room14;

    room26.west = &room16;
    room26.south = &room27;
    room26.down = &room40;

    room27.north = &room26;
    room27.west = &room17;

    room28.ne = &room40;
    room28.east = &room41;
    room28.up = &room40;
    room28.down = &room29;

    room29.up = &room28;
    room29.se = &room42;

    room30.west = &room20;
    room30.east = &room44;
    room30.south = &room31;

    room31.north = &room30;
    room31.west = &room21;
    room31.east = &room45;
    room31.south = &room32;

    room32.north = &room31;
    room32.west = &room22;
    room32.east = &room46;
    room32.south = &room33;

    room33.north = &room32;
    room33.west = &room23;
    room33.east = &room47;

    room34.west = &room24;
    room34.down = &room35;

    room35.up = &room34;
    room35.down = &room36;

    room36.west = &room25;
    room36.east = &room49;
    room36.up = &room35;

    room37.south = &room38;
    room37.down = &room88;

    room38.north = &room37;
    room38.south = &room39;

    room39.north = &room38;
    room39.se = &room50;

    room40.up = &room26;
    room40.sw = &room28;
    room40.down = &room28;

    room41.west = &room28;
    room41.east = &room54;

    room42.nw = &room29;
    room42.south = &room43;

    room43.north = &room42;
    room43.south = &room44;

    room44.north = &room43;
    room44.west = &room30;
    room44.south = &room45;

    room45.north = &room44;
    room45.west = &room31;
    room45.east = &room55;
    room45.south = &room46;

    room46.north = &room45;
    room46.west = &room32;
    room46.east = &room56;
    room46.south = &room47;

    room47.north = &room46;
    room47.west = &room33;
    room47.east = &room57;

    room48.south = &room49;

    room49.north = &room48;
    room49.west = &room36;
    room49.east = &room58;

    room50.nw = &room39;
    room50.ne = &room60;
    room50.south = &room51;

    room51.north = &room50;
    room51.south = &room52;

    room52.north = &room51;
    room52.south = &room53;

    room53.north = &room52;
    room53.east = &room61;

    room54.west = &room41;
    room54.east = &room62;

    room55.west = &room45;
    room55.east = &room63;
    room55.south = &room56;

    room56.north = &room55;
    room56.west = &room46;
    room56.east = &room64;
    room56.south = &room57;

    room57.north = &room56;
    room57.west = &room47;
    room57.east = &room65;

    room58.west = &room49;

    room59.down = &room66;
    room59.south = &room60;

    room60.north = &room59;
    room60.sw = &room50;

    room61.west = &room53;
    room61.se = &room68;

    room62.west = &room54;
    room62.down = &room70;

    room63.west = &room55;
    room63.south = &room64;

    room64.north = &room63;
    room64.west = &room56;
    room64.east = &room74;
    room64.south = &room65;

    room65.north = &room64;
    room65.west = &room57;

    room66.up = &room59;
    room66.south = &room67;

    room67.north = &room66;
    room67.west = &room66;
    room67.east = &room76;

    room68.nw = &room61;
    room68.ne = &room78;
    room68.south = &room69;

    room69.north = &room68;
    room69.south = &room70;

    room70.north = &room69;
    room70.east = &room79;
    room70.south = &room71;
    room70.up = &room62;

    room71.north = &room70;
    room71.south = &room72;

    room72.north = &room71;
    room72.south = &room73;

    room73.north = &room72;

    room74.west = &room64;
    room74.east = &room80;

    room75.ne = &room80;

    room76.west = &room67;
    room76.south = &room77;
    room76.down = &room82;

    room77.north = &room76;

    room78.sw = &room68;

    room79.west = &room70;
    room79.east = &room84;

    room80.west = &room74;
    room80.east = &room85;
    room80.sw = &room75;

    room81.east = &room86;
    room81.south = &room82;

    room82.north = &room81;
    room82.east = &room87;
    room82.up = &room76;

    room83.ne = &room89;
    room83.south = &room84;

    room84.north = &room83;
    room84.west = &room79;
    room84.up = &room90;

    room85.west = &room80;

    room86.west = &room81;
    room86.east = &room91;
    room86.south = &room87;

    room87.north = &room86;
    room87.west = &room82;
    room87.south = &room88;

    room88.north = &room87;

    room89.sw = &room83;

    room90.up = &room92;
    room90.down = &room84;

    room91.west = &room86;

    room92.down = &room90;

    *first_room = &room45;
}
