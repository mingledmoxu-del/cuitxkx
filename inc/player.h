#ifndef PLAYER_H
#define PLAYER_H

#include <util.h>

#include "room.h"

typedef struct player_def_s {
    const char       *player_name;
    const char       *player_id;
    int               hp, max_hp; // 增加当前血量和上限
    int               mp, max_mp;
    item_t           *weapon;
    item_t           *armor_head;
    item_t           *armor_body;
    item_t           *armor_leg;
    room_loc_t       *player_cur_loc;
    inventory_node_t *player_bag;
} player_def_t;

extern player_def_t omo;

void player_add_item(player_def_t *player, item_t *item);

#endif