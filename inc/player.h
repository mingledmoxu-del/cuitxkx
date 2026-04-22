#ifndef PLAYER_H
#define PLAYER_H

#include <util.h>

#include "room.h"
#include "skill.h"

typedef struct player_def_s {
    const char       *player_name;
    const char       *player_id;
    int               hp, max_hp; // 增加当前血量和上限
    int               mp, max_mp;
    int               spirit, max_spirit;
    int               player_money;
    item_t           *weapon;
    item_t           *armor_head;
    item_t           *armor_body;
    item_t           *armor_leg;
    room_loc_t       *player_cur_loc;
    inventory_node_t *player_bag;
    skill_node_t     *player_skill;
    player_skill_t   *active_skill;
    room_npc_t       *fight_target;
    time_t            last_atk_time;
} player_def_t;

extern player_def_t omo;

void player_add_item(player_def_t *player, item_t *item);
int  player_get_atk(player_def_t *player_id);
int  player_get_def(player_def_t *player_id);
void combat_tick(player_def_t *player_id);

#endif