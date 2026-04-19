#include "room.h"

/**
 * @file room.c
 * @brief 实现房间与 NPC 实例的具体定义与空间连接逻辑。
 */

/* 1. 全局变量的引出定义 */
extern room_loc_t player_spawn;
extern room_loc_t player_spawn_tree_hole;
extern room_npc_t player_spawn_npc_guider;
extern item_t     player_spawn_treehole_sword;
/**
 * @brief 初始出生点：青溪渡新手村
 */
room_loc_t player_spawn = {
    .room_id = 0,
    .room_name = "新手村",
    .room_desc = SPAWN_DECS,                 /* 引用 text/room_spawn.h 中的宏 */
    .down = &player_spawn_tree_hole,         /* 向下指向树洞 */
    .room_npc[0] = &player_spawn_npc_guider, /* 安置引导 NPC */
};

/**
 * @brief 场景：树洞内部
 */
room_loc_t player_spawn_tree_hole = {
    .room_id = 1,
    .room_name = "树洞",
    .room_desc = SPAWN_TREEHOLE_DESC, /* 引用 text/room_spawn_treehole.h 中的宏 */
    .up = &player_spawn,              /* 向上返回新手村 */
    .room_item[0] = &weapon_sword,
};

/**
 * @brief NPC 实例：新手引导员
 */
room_npc_t player_spawn_npc_guider = {
    .room_npc_name = "向导",
    .room_npc_id = "guider",
    .room_npc_dialog = SPAWN_GUIDER_DIALOG,
    .room_npc_desc = SPAWN_GUIDER_DESC,
};
