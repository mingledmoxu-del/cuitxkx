#include "item.h"

item_t player_spawn_treehole_sword = {
    .item_name = "铁剑",
    .item_id = "sword",
    .item_pre = "一把锈迹斑斑的",
    .item_type = ITEM_TYPE_WEAPON,
    .item_desc = "这是一把锈迹斑斑的铁剑，剑身布满暗红色的锈斑，边缘已有些钝化，却依稀能看出昔日锋芒的轮廓。",
    .item_u = {.item_weapon = {
                   .weapon_atk = 10,
               }}};

item_t con_wound_medic = {
    .item_name = "金疮药",
    .item_id = "wound_medic",
    .item_pre = "一瓶",
    .item_type = ITEM_TYPE_CONSUMABLE,
    .item_desc =
        "这是一瓶简陋的金疮药，瓷瓶略显粗糙，封口以布条缠紧，散发出淡淡的草药清香，虽不显眼，却足以应对寻常伤势。",
    .item_u = {.item_con = {
                   .con_hp_heal = 20,
               }}};

item_t con_grand_medic = {.item_name = "大还丹",
                          .item_id = "grand_medic",
                          .item_pre = "一枚",
                          .item_type = ITEM_TYPE_CONSUMABLE,
                          .item_desc = "这是一枚色泽温润的大还丹，丹体圆润如玉，隐隐散发出淡淡药香，握在手中微微生暖，"
                                       "据说可迅速恢复内外伤势，乃难得的疗伤之物。",
                          .item_u = {.item_con = {
                                         .con_hp_heal = 20,
                                     }}};
