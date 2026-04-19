#include "player.h"

player_def_t omo = {
    .player_name = "傲谕",
    .player_id = "omo",
    .hp = 100,
    .max_hp = 100,
    .mp = 100,
    .max_mp = 100,
    .spirit = 100,
    .max_spirit = 100,
    .player_cur_loc = &player_spawn,
};

void player_add_item(player_def_t *player, item_t *item) {
    if (player == NULL || item == NULL)
        return;
    inventory_node_t *new_node = (inventory_node_t *)malloc(sizeof(inventory_node_t));
    if (new_node != NULL) {
        new_node->item = item;
        new_node->next = player->player_bag;
        player->player_bag = new_node;
    }
}

int player_get_atk(player_def_t *player_id) {
    int player_base_atk = 8;
    if (player_id->weapon != NULL) {
        player_base_atk += player_id->weapon->item_u.item_weapon.weapon_atk;
        return player_base_atk;
    }
    return player_base_atk;
}

int player_get_def(player_def_t *player_id) {
    int player_base_def = 3;
    if (player_id->armor_body != NULL) {
        player_base_def += player_id->armor_body->item_u.item_armor.armor_def;
    }
    if (player_id->armor_head != NULL) {
        player_base_def += player_id->armor_head->item_u.item_armor.armor_def;
    }
    if (player_id->armor_leg != NULL) {
        player_base_def += player_id->armor_leg->item_u.item_armor.armor_def;
    }
    return player_base_def;
}