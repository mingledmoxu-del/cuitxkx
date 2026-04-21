#ifndef SKILL_H
#define SKILL_H

#include <util.h>

typedef enum player_res_type_e {
    HP_TYPE = 0,
    MP_TYPE,
    SP_TYPE,
} player_res_type_t;

struct player_def_s;
typedef struct player_def_s player_def_t;

typedef struct skill_passive_s {

} skill_passive_t;

typedef struct skill_active_s {
    int active_skill_cd;
} skill_active_t;

typedef struct skill_fight_s {
} skill_fight_t;

typedef enum skill_type_e {
    SKILL_TYPE_COMMON = 0,
    SKILL_TYPE_FIGHT,
    SKILL_TYPE_ACTIVE,
    SKILL_TYPE_PASSIVE,
} skill_type_t;

typedef struct player_skill_s {
    char        *skill_name;
    char        *skill_id;
    char        *skill_desc;
    int          skill_mp_cost;
    int          skill_sp_cost;
    int          skill_hp_cost;
    int          skill_level;
    int          skill_exp;
    int          skill_max_exp;
    int          skill_power;
    skill_type_t skill_type;
    time_t       skill_last_cast_time;

    union skill_cast_type_u {
        skill_active_t  active;
        skill_fight_t   fight;
        skill_passive_t passive;
    } skill_cast_type_t;

    int (*skill_cast)(struct player_skill_s *skill_id, void *caller, void *target);

} player_skill_t;

typedef struct skill_node_s {
    player_skill_t      *skill;
    struct skill_node_s *next;
} skill_node_t;

int             player_add_skill(player_def_t *player_id, player_skill_t *skill);
player_skill_t *player_find_skill(player_def_t *player_id, const char *skill_id);
void            player_skill_exp_add(player_def_t *player_id, player_skill_t *player_skill);

extern player_skill_t skill_zazen;

#endif
