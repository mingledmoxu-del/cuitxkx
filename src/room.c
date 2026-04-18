#include "room.h"

// 1. 在文件顶部前置声明所有房间变量
// 这样下面初始化时，编译器就知道这些名字代表的地址了
extern room_loc_t player_spawn;
extern room_loc_t player_spawn_tree_hole;
extern room_npc_t player_spawn_npc_guider;

room_loc_t player_spawn = {
    .room_id = 0,
    .room_name = "新手村",
    .room_desc = SPAWN_DECS,
    .down = &player_spawn_tree_hole,
    .room_npc[0] = &player_spawn_npc_guider,
};

room_loc_t player_spawn_tree_hole = {
    .room_id = 1,
    .room_name = "树洞",
    .room_desc = SPAWN_TREEHOLE_DESC,
    .up = &player_spawn,
};

room_npc_t player_spawn_npc_guider = {
    .room_npc_name = "向导",
    .room_npc_id = "guider",
    .room_npc_dialog = SPAWN_GUIDER_DIALOG,
    .room_npc_desc = SPAWN_GUIDER_DESC,

};
