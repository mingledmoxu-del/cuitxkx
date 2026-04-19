#include "player.h"

player_def_t omo = {
    .player_name = "傲谕",
    .player_id = "omo",
    .hp = 100,
    .max_hp = 100,
    .mp = 100,
    .max_mp = 100,
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
