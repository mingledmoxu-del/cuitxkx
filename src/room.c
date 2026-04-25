#include "room.h"

/**
 * @file room.c
 * @brief 实现房间与 NPC 实例的具体定义与空间连接逻辑。
 */

/* 1. 全局变量的引出定义 */
extern room_loc_t player_spawn;
extern room_loc_t player_spawn_tree_hole;
extern room_npc_t player_spawn_npc_guider;
extern room_loc_t room_test;
extern item_t     player_spawn_treehole_sword;

/**
 * @brief 初始出生点：青溪渡新手村
 */
room_loc_t player_spawn = {
    .room_id = 0,
    .room_name = "青溪渡",
    .room_desc = SPAWN_DECS,         /* 引用 text/room_spawn.h 中的宏 */
    .down = &player_spawn_tree_hole, /* 向下指向树洞 */
    .east = &player_spawn_east_forest,
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

room_loc_t player_spawn_east_forest = {
    .room_id = 2,
    .room_name = "快活林",
    .room_desc = "一片树林\n",
    .west = &player_spawn,
};

/**
 * @brief NPC 实例：新手引导员
 */
room_npc_t player_spawn_npc_guider = {
    .room_npc_name = "向导",
    .room_npc_id = "guider",
    .room_npc_dialog = SPAWN_GUIDER_DIALOG,
    .room_npc_desc = SPAWN_GUIDER_DESC,
    .room_npc_type = NPC_TYPE_STATIC,
};

room_npc_t room_test_enemy = {
    .room_npc_name = "史莱姆",
    .room_npc_id = "slam",
    .room_npc_atk = 5,
    .room_npc_def = 3,
    .room_npc_max_hp = 50,
    .room_npc_hp = 50,
    .room_fight_interval = 3,
    .room_npc_type = NPC_TYPE_ENEMY,
    .room_npc_desc = "一托史莱姆",
};

room_loc_t room_test = {
    .room_id = 1021,
    .room_name = "练功房",
    .room_desc = ROOM_TEST_DESC,
    .room_npc[0] = &player_spawn_npc_guider,
    .room_npc[1] = &room_test_enemy,
    .room_item = &weapon_sword,
};