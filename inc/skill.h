#ifndef SKILL_H
#define SKILL_H

#include <util.h>

typedef enum skill_type_e {
    SKILL_TYPE_COMMON = 0,
    SKILL_TYPE_FIGHT,
    SKILL_TYPE_TREAT,
    SKILL_TYPE_PASSIVE,
} skill_type_t;

typedef struct player_skill_s {
    char        *skill_name;
    char        *skill_id;
    int          skill_mp_cost;
    int          skill_level;
    int          skill_exp;
    int          skill_max_exp;
    int          skill_power;
    skill_type_t skill_type;
} player_skill_t;

typedef struct skill_node_s {
    player_skill_t       skill;
    struct skill_node_s *next;
} skill_node_t;

#endif
