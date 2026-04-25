#include "player.h"

player_def_t omo = {
    .player_name = "傲谕",
    .player_id = "omo",
    .hp = 75,
    .max_hp = 100,
    .mp = 100,
    .max_mp = 100,
    .spirit = 100,
    .max_spirit = 100,
    .fight_interval = 2,
    .player_cur_loc = &room_test,
};

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