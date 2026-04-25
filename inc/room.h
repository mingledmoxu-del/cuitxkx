#ifndef ROOM_H
#define ROOM_H

#include <util.h>

#include "item.h"
#include "room_spawn.h"
#include "room_spawn_treehole.h"

/* NPC 结构体的前置声明 */
typedef struct room_npc_s room_npc_t;
typedef struct room_loc_s room_loc_t;

extern room_loc_t player_spawn_tree_hole;
extern room_loc_t player_spawn_east_forest;
extern room_loc_t player_spawn;
extern room_loc_t room_test;

typedef enum room_npc_type_e     room_npc_type_t;
typedef struct room_npc_neut_s   room_npc_neut_t;
typedef struct room_npc_static_s room_npc_static_t;
typedef struct room_npc_enemy_s  room_npc_enemy_t;

typedef enum room_npc_type_e {
    NPC_TYPE_NONE = 0,
    NPC_TYPE_STATIC,
    NPC_TYPE_NEUT,
    NPC_TYPE_ENEMY,
} room_npc_type_t;

/* NPC 身份类型 */
typedef struct room_npc_neut_s {

} room_npc_neut_t;

typedef struct room_npc_static_s {

} room_npc_static_t;

typedef struct room_npc_enemy_s {

} room_npc_enemy_t;

/**
 * @brief 房间/地点结构体
 * 包含房间的基本信息以及与其他房间的连接关系（出口）
 */
typedef struct room_loc_s {
    int         room_id;   /* 房间唯一标识符 */
    const char *room_name; /* 房间名称 */
    const char *room_desc; /* 房间的详细中文描述 */

    /* 出口指针：指向相邻的房间结构体。如果为 NULL，表示该方向不可行 */
    struct room_loc_s *north; /* 北方 */
    struct room_loc_s *south; /* 南方 */
    struct room_loc_s *east;  /* 东方 */
    struct room_loc_s *west;  /* 西方 */
    struct room_loc_s *up;    /* 上方（如上楼、爬树） */
    struct room_loc_s *down;  /* 下方（如地洞、下楼） */

    room_npc_t *room_npc[10]; /* 房间内存在的 NPC，最多支持 10 个 */
    item_t     *room_item[5];
} room_loc_t;

/**
 * @brief NPC（非玩家角色）结构体
 * 定义了 NPC 的基本属性和互动内容
 */
typedef struct room_npc_s {
    const char *room_npc_name;   /* NPC 姓名 */
    const char *room_npc_id;     /* 英文 ID，用户输入指令时使用（如 "guider"） */
    const char *room_npc_desc;   /* 检视 (see) 时显示的详细描述 */
    const char *room_npc_dialog; /* 进行对话 (talk) 时显示的默认内容 */

    int    room_npc_hp, room_npc_max_hp; /* 生命值（为战斗系统预留） */
    int    room_npc_atk, room_npc_def;
    time_t room_last_fight_time;
    time_t room_fight_interval;

    room_npc_type_t room_npc_type;

    union {
        room_npc_neut_t   npc_neutual;
        room_npc_static_t npc_static;
        room_npc_enemy_t  npc_enemy;
    } room_npc_type_u;

} room_npc_t;

/* --- 全局变量声明 --- */

/* 玩家当前所在的房间指针 */
extern room_loc_t *current_loc;

/* 预定义的房间实例 */
extern room_loc_t player_spawn;           /* 初始出生点（青溪渡） */
extern room_loc_t player_spawn_tree_hole; /* 树洞场景 */

#endif
