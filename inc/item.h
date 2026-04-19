#ifndef ITEM_H
#define ITEM_H

#include <util.h>

#include "con_desc.h"
#include "weapon_desc.h"

typedef struct item_weapon_s {
    int weapon_atk;
} item_weapon_t;

typedef struct item_armor_s {
    int armor_def;
} item_armor_t;

typedef struct item_consumable_s {
    int con_hp_heal;
    int con_mp_add;
    int con_sp_add;
} item_consumable_t;

// 物品类型枚举
typedef enum item_type_e {
    ITEM_TYPE_NONE = 0,
    ITEM_TYPE_WEAPON,     // 武器
    ITEM_TYPE_CONSUMABLE, // 消耗品（如草药）
    ITEM_TYPE_ARMOR,      // 防具
    ITEM_TYPE_ARMOR_HEAD,
    ITEM_TYPE_ARMOR_BODY,
    ITEM_TYPE_ARMOR_LEG,
    ITEM_TYPE_SHIELD
} item_type_t;

typedef struct item_s {
    const char *item_name;
    const char *item_id;
    char       *item_pre;
    char       *item_desc;
    item_type_t item_type;

    union {
        item_weapon_t     item_weapon;
        item_consumable_t item_con;
        item_armor_t      item_armor;
    } item_u;

} item_t;

typedef struct inventory_node_s {
    item_t                  *item;
    struct inventory_node_s *next;
} inventory_node_t;

extern item_t weapon_sword;
extern item_t con_wound_medic;
extern item_t con_grand_medic;
extern item_t con_foucus_medic;
#endif
