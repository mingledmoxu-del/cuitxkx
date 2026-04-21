#include "skill.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

int player_fix_status(player_def_t *player_id, player_res_type_t status_type, int status_num) {
    switch (status_type) {
    case (HP_TYPE):

        player_id->hp += status_num;
        if (player_id->hp > player_id->max_hp) {
            player_id->hp = player_id->max_hp;
            return 0;
        }
        break;
    case (MP_TYPE):

        player_id->mp += status_num;
        if (player_id->mp > player_id->max_mp) {
            player_id->mp = player_id->max_mp;
            return 0;
        }
        break;
    case (SP_TYPE):

        player_id->spirit += status_num;
        if (player_id->spirit > player_id->max_spirit) {
            player_id->spirit = player_id->max_spirit;
            return 0;
        }
        break;
    default:
        break;
    }
    return 0;
}

int player_add_skill(player_def_t *player_id, player_skill_t *skill) {
    skill_node_t *curr_node;
    curr_node = player_id->player_skill;
    while (curr_node != NULL) {
        if (strcmp(curr_node->skill->skill_id, skill->skill_id) == 0) {
            printf("已学会该技能。\n");
            return 0;
        }
        curr_node = curr_node->next;
    }

    skill_node_t *new_skill_node = (skill_node_t *)malloc(sizeof(skill_node_t));
    new_skill_node->skill = skill;
    new_skill_node->next = player_id->player_skill;
    player_id->player_skill = new_skill_node;
    player_id->player_skill->skill->skill_level += 1;

    printf("恭喜！你成功领悟了新招式：『%s』。\n", skill->skill_name);
    return 0;
}

player_skill_t *player_find_skill(player_def_t *player_id, const char *skill_id) {
    skill_node_t *curr_node;
    curr_node = player_id->player_skill;
    while (curr_node != NULL) {
    }
}

void logic_zazen(player_skill_t *skill, void *caller, void *target) {
    player_def_t *player_id = (player_def_t *)caller;
    printf("你盘膝而坐，运转内息……\n");

    int heal_hp_count = 10 + skill->skill_level * 1;
    player_fix_status(&omo, HP_TYPE, heal_hp_count);
    printf("一口浊气吐出，你感觉气血恢复了点。\n");
}

player_skill_t skill_zazen = {
    .skill_name = "打坐",
    .skill_id = "zazen",
    .skill_desc = "静心凝气，引导内息运转，以缓慢恢复伤势",
    .skill_cast_type_t = {.active =
                              {
                                  .active_skill_cd = 10,
                              }},
    .skill_mp_cost = 15,
    .skill_type = SKILL_TYPE_ACTIVE,
    .skill_cast = logic_zazen,
};