#include "skill.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

int player_fix_status(player_def_t *player_id, player_res_type_t status_type, int status_num) {
    int tmp_status;
    switch (status_type) {
    case (HP_TYPE):
        tmp_status = player_id->hp;
        player_id->hp += status_num;
        if (player_id->hp > player_id->max_hp) {
            player_id->hp = player_id->max_hp;
            return player_id->max_hp - tmp_status;
        }
        return player_id->hp - tmp_status;
        break;

    case (MP_TYPE):
        tmp_status = player_id->mp;
        player_id->mp += status_num;
        if (player_id->mp > player_id->max_mp) {
            player_id->mp = player_id->max_mp;
            return player_id->max_mp - tmp_status;
        }
        return player_id->mp - tmp_status;
        break;

    case (SP_TYPE):
        tmp_status = player_id->spirit;
        player_id->spirit += status_num;
        if (player_id->spirit > player_id->max_spirit) {
            player_id->spirit = player_id->max_spirit;
            return player_id->max_spirit - tmp_status;
        }
        return player_id->spirit - tmp_status;
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
            return 1;
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
        if (strcmp(skill_id, curr_node->skill->skill_id) == 0) {
            return curr_node->skill;
        }
        curr_node = curr_node->next;
    }
    return NULL;
}

void player_skill_exp_add(player_def_t *player_id, player_skill_t *player_skill) {
    player_skill->skill_exp += 10 + player_skill->skill_level;
    if (player_skill->skill_exp >= player_skill->skill_max_exp) {
        player_skill->skill_level++;
        player_skill->skill_exp -= player_skill->skill_max_exp;
        printf(CLR_YELLOW "【领悟】你的『%s』精进到了第 %d 层！" CLR_RESET "\n", player_skill->skill_name,
               player_skill->skill_level);
        player_skill->skill_max_exp = (int)(player_skill->skill_max_exp * 1.2);
    }
}

int logic_zazen(player_skill_t *skill, void *caller, void *target) {
    player_def_t *player_id = (player_def_t *)caller;
    if (player_id->hp >= player_id->max_hp) {
        printf("你气血充溢，并不需要打坐调息。\n");
        return 0;
    }
    printf("你盘膝而坐，运转内息……\n");

    int heal_hp_count = 10 + skill->skill_level * 1;
    heal_hp_count = player_fix_status(&omo, HP_TYPE, heal_hp_count);
    player_fix_status(&omo, MP_TYPE, -skill_zazen.skill_mp_cost);
    printf("一口浊气吐出，你感觉气血恢复了点。\n");
    printf("" CLR_RED CLR_BOLD "+ %d hp " CLR_RESET " \n", heal_hp_count);
    return 1;
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
    .skill_max_exp = 100,
    .skill_type = SKILL_TYPE_ACTIVE,
    .skill_cast = logic_zazen,
};