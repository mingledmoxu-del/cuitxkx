#ifndef ROOM_H
#define ROOM_H

#include <util.h>

#include "room_spawn.h"
#include "room_spawn_treehole.h"

typedef struct room_npc_s room_npc_t;

typedef struct room_loc_s {
    int         room_id;
    const char *room_name;
    const char *room_desc;

    /* 出口指针：指向相邻的房间结构体 */
    struct room_loc_s *north;
    struct room_loc_s *south;
    struct room_loc_s *east;
    struct room_loc_s *west;
    struct room_loc_s *up;
    struct room_loc_s *down;

    room_npc_t *room_npc[3];
} room_loc_t;

typedef struct room_npc_s {
    const char *room_npc_name;
    const char *room_npc_id;     // 用于指令触发，如 "chief"
    const char *room_npc_desc;   // 它的样子
    int         room_npc_hp;     // 生命值
    const char *room_npc_dialog; // 默认对话内容
} room_npc_t;

extern room_loc_t *current_loc; // 注意这里有个 *

extern room_loc_t player_spawn;
extern room_loc_t player_spawn_tree_hole;

#endif
