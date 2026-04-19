#include "item.h"

item_t weapon_sword = {.item_name = "铁剑",
                       .item_id = "sword",
                       .item_pre = "一把锈迹斑斑的",
                       .item_type = ITEM_TYPE_WEAPON,
                       .item_desc = WP_SWORD_DESC,
                       .item_u = {.item_weapon = {
                                      .weapon_atk = 5,
                                  }}};

item_t con_wound_medic = {.item_name = "金疮药",
                          .item_id = "wound_medic",
                          .item_pre = "一瓶",
                          .item_type = ITEM_TYPE_CONSUMABLE,
                          .item_desc = CON_WOUND_MEDIC_DESC,
                          .item_u = {.item_con = {
                                         .con_hp_heal = 20,
                                     }}};

item_t con_grand_medic = {.item_name = "大还丹",
                          .item_id = "grand_medic",
                          .item_pre = "一枚",
                          .item_type = ITEM_TYPE_CONSUMABLE,
                          .item_desc = CON_GRAND_MEDIC_DESC,
                          .item_u = {.item_con = {
                                         .con_hp_heal = 20,
                                     }}};

item_t con_foucus_medic = {.item_name = "凝神散",
                           .item_id = "focus_medic",
                           .item_pre = "一包",
                           .item_desc = CON_FOCUS_MEDIC_DESC,
                           .item_type = ITEM_TYPE_CONSUMABLE,
                           .item_u = {.item_con = {
                                          .con_sp_add = 15,
                                      }}};
