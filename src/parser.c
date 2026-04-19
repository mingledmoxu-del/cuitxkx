#include "parser.h"

/**
 * @brief 内部辅助：绘制彩色进度条
 * @param label 标签名称（如 HP/MP）
 * @param cur 当前值
 * @param max 最大值
 * @param color 颜色宏
 */
/**
 * @brief 内部辅助：绘制彩色进度条
 *
 * 这是一个通用的 UI 函数，根据当前值与最大值的百分比，生成一串形如 [#####-----] 的字符串。
 * 它能够根据传入的颜色宏显示不同的颜色（如 HP 显红，MP 显蓝）。
 *
 * @param label 标签名称（显示在进度条最左侧，如 "气血"）
 * @param cur 当前数值
 * @param max 最大数值
 * @param color 颜色宏（定义在 util.h 中）
 */
static void print_status_bar(const char *label, int cur, int max, const char *color) {
    int bar_width = 20; // 进度条的总字符宽度
    // 计算填充格子的数量，注意防止被零除
    int filled = (max > 0) ? (cur * bar_width / max) : 0;

    // 边界安全检查，防止进度条溢出
    if (filled > bar_width)
        filled = bar_width;

    printf("%s: [", label);
    printf("%s", color); // 设置颜色
    for (int i = 0; i < bar_width; i++) {
        if (i < filled)
            printf("#"); // 已填充的部分
        else
            printf("-"); // 未填充的部分
    }
    printf(CLR_RESET "] %d/%d\n", cur, max); // 重置颜色并打印数值
}

static item_t *player_find_item(player_def_t *player_id, const char *item_id) {
    inventory_node_t *curr_item = player_id->player_bag;
    while (curr_item != NULL) {
        if (strcmp(curr_item->item->item_id, item_id) == 0) {
            return curr_item->item;
        }
        curr_item = curr_item->next;
    }
    if (player_id->weapon && strcmp(item_id, player_id->weapon->item_id) == 0)
        return player_id->weapon;
    if (player_id->armor_body && strcmp(item_id, player_id->armor_body->item_id) == 0)
        return player_id->armor_body;
    if (player_id->armor_leg && strcmp(item_id, player_id->armor_leg->item_id) == 0)
        return player_id->armor_leg;
    if (player_id->armor_head && strcmp(item_id, player_id->armor_head->item_id) == 0)
        return player_id->armor_head;
    return NULL;
}

static int player_equip_item(player_def_t *player_id, item_t *item) {
    item_t **slot = NULL;
    switch (item->item_type) {
    case ITEM_TYPE_WEAPON:
        slot = &player_id->weapon;
        break;
    case ITEM_TYPE_ARMOR_LEG:
        slot = &player_id->armor_leg;
        break;
    case ITEM_TYPE_ARMOR_BODY:
        slot = &player_id->armor_body;
        break;
    case ITEM_TYPE_ARMOR_HEAD:
        slot = &player_id->armor_head;
        break;
    default:
        printf("这个东西没法穿戴。\n");
        return 0;
    }

    if (*slot != NULL) {
        printf("你脱下了『%s』，并将其收入囊中。\n", (*slot)->item_name);
        player_add_item(player_id, *slot);
    }

    *slot = item;
    printf("你装备了『%s』。\n", (*slot)->item_name);
    return 1;
}

static item_t *player_remove_item(player_def_t *player_id, const char *item_id) {
    inventory_node_t *curr_node = player_id->player_bag;
    inventory_node_t *prev_node = NULL;

    while (curr_node != NULL) {
        if (strcmp(curr_node->item->item_id, item_id) == 0) {
            if (prev_node == NULL) {
                player_id->player_bag = curr_node->next;
            } else {
                prev_node->next = curr_node->next;
            }
            item_t *target_item = curr_node->item;
            free(curr_node);
            return target_item;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    return NULL;
}

/**
 * @brief 指令条目结构体定义
 */
typedef struct cmd_entry_e {
    const char     *cmd_name; /* 指令触发名称（用户输入） */
    command_handler cmd_func; /* 对应的处理函数指针 */
    const char     *cmd_desc; /* 简短的功能描述 */
    const char     *cmd_help; /* 详细的使用帮助文本 */
} cmd_entry_t;

/**
 * @brief 指令映射表
 * 集中管理游戏中所有可用的指令。新增指令需在此添加条目。
 */
static cmd_entry_t cmd_table[] = {
    {"quit", cmd_quit, "退出游戏", "用法：直接输入 quit 离开江湖"},
    {"help", cmd_help, "帮助列表", "用法：help [指令名] 获取具体教程"},
    {"go", cmd_go, "前往地点", "用法：go [方向] (由 north/south/east/west/up/down 组成)"},
    {"look", cmd_look, "观察环境", "用法：直接输入 look 查看当前房间与出口"},
    {"talk", cmd_talk, "与人对话", "用法：talk [人物ID] 与房间内的 NPC 攀谈"},
    {"see", cmd_see, "仔细检视", "用法：see [人物ID] 观察对方的容貌或状态"},
    {"get", cmd_get, "拿起某物", "用法：get [物品ID] 将其收入背包"},
    {"wear", cmd_wear, "装备某物", "用法：wear [物品ID] 将其装备在身上"},
    {"check", cmd_check, "检视某物", "用法：check [物品ID] 查看背包内的物品"},
    {NULL, NULL, NULL, NULL},
};

/**
 * @brief 处理「quit」指令
 */
int cmd_quit(int argc, char **argv) {
    printf("青山不改，绿水长流，我们江湖有缘再见！\n");
    return 1; /* 返回 1 会导致主循环 break */
}

/**
 * @brief 处理「help」指令
 * 如果不带参数，列出所有指令；如果带参数，显示该指令的详细用法。
 */
int cmd_help(int argc, char **argv) {

    /* 情况 A：用户只输入了 help */
    if (argc == 1) {
        printf("=== 已学会的江湖指令 ===\n");
        for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
            printf("%s \t %s \n", cmd_table[i].cmd_name, cmd_table[i].cmd_desc);
        }

        printf("\n提示：使用 'help [指令名]' 来获取该指令的秘籍（用法）。\n");
    }
    /* 情况 B：用户输入了 help [某指令] */
    else {
        int found = 0;
        for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
            if (strcmp(argv[1], cmd_table[i].cmd_name) == 0) {
                printf("【%s 指令秘籍】\n%s\n", cmd_table[i].cmd_name, cmd_table[i].cmd_help);
                found = 1;
                break;
            }
        }

        if (!found)
            printf("抱歉，你尚未领悟名为「%s」的招式。\n", argv[1]);
    }
    return 0;
}

/**
 * @brief 处理「go」指令
 * 实现玩家在不同房间指针间的跳转。
 */
int cmd_go(int argc, char **argv) {
    /* 1. 参数完整性检查 */
    if (argc < 2) {
        printf("你想往哪儿走？\n");
        printf("用法提示：输入 'go [方向]' (例如: go east)\n");
        return 0;
    }

    room_loc_t *next_loc = NULL;

    /* 2. 匹配移动方向，查找当前房间对应的出口指针 */
    if (strcmp(argv[1], "down") == 0) {
        next_loc = omo.player_cur_loc->down;
    } else if (strcmp(argv[1], "up") == 0) {
        next_loc = omo.player_cur_loc->up;
    } else if (strcmp(argv[1], "east") == 0) {
        next_loc = omo.player_cur_loc->east;
    } else if (strcmp(argv[1], "west") == 0) {
        next_loc = omo.player_cur_loc->west;
    } else if (strcmp(argv[1], "north") == 0) {
        next_loc = omo.player_cur_loc->north;
    } else if (strcmp(argv[1], "south") == 0) {
        next_loc = omo.player_cur_loc->south;
    }

    /* 3. 边界检查：如果目标方向没有路（指针为 NULL） */
    if (next_loc == NULL) {
        printf("前面已经没有路了，你张望了一阵，决定留在原地。\n");
        return 0;
    }

    /* 4. 执行移动过程中的氛围描写 */
    if (strcmp(argv[1], "up") == 0)
        printf("你顺着粗糙的树干小心攀爬，借着枝节一点点向上移动。\n");
    else if (strcmp(argv[1], "down") == 0)
        printf("你弯下身子，顺着狭窄的洞口缓缓向下钻入，周围的光线一点点被黑暗吞没。\n");
    else
        printf("你向着 %s 方向出发，脚下的路在迷雾中延伸...\n", argv[1]);

    /* 模拟行路时间 */
    sleep(1);

    /* 核心逻辑：更新玩家当前所在的房间指针 */
    omo.player_cur_loc = next_loc;

    /* 5. 到达新地点后自动执行「look」指令，展示新环境 */
    cmd_look(1, NULL);
    return 0;
}

/**
 * @brief 处理「look」指令
 * 展示当前房间的名称、描述、可见出口以及存在的 NPC。
 */
int cmd_look(int argc, char **argv) {
    /* 1. 打印房间名与详细描述（带颜色高亮） */
    printf(CLR_GREEN CLR_BOLD "【%s】" CLR_RESET "\n", omo.player_cur_loc->room_name);
    printf("%s\n", omo.player_cur_loc->room_desc);

    /* 2. 扫描并打印有效的出口信息 */
    printf(CLR_CYAN "\n这里明显的出口有：" CLR_RESET);
    printf(CLR_BOLD CLR_YELLOW);
    int count = 0;
    if (omo.player_cur_loc->north) {
        printf("北 ");
        count++;
    }
    if (omo.player_cur_loc->south) {
        printf("南 ");
        count++;
    }
    if (omo.player_cur_loc->east) {
        printf("东 ");
        count++;
    }
    if (omo.player_cur_loc->west) {
        printf("西 ");
        count++;
    }
    if (omo.player_cur_loc->up) {
        printf("上 ");
        count++;
    }
    if (omo.player_cur_loc->down) {
        printf("下 ");
        count++;
    }

    if (count == 0)
        printf("无");
    printf(CLR_RESET "\n");

    /* 3. 遍历 NPC 列表并显示 */
    if (omo.player_cur_loc->room_npc[0] != NULL) {
        printf(CLR_CYAN "你注意到了这里还有：" CLR_RESET);
        printf(CLR_YELLOW CLR_BOLD);
        for (int i = 0; i < 3 && omo.player_cur_loc->room_npc[i] != NULL; i++) {
            printf("%s(%s) ", omo.player_cur_loc->room_npc[i]->room_npc_name,
                   omo.player_cur_loc->room_npc[i]->room_npc_id);
        }
        printf(CLR_RESET "\n");
    }

    int item_has_flag = 0;
    for (int i = 0; i < 5; i++) {
        if (omo.player_cur_loc->room_item[i] != NULL) {
            item_has_flag = 1;
            break;
        }
    }
    if (item_has_flag) {
        printf(CLR_CYAN "这里放着：" CLR_RESET);
        printf(CLR_YELLOW CLR_BOLD);
        for (int i = 0; i < 5; i++) {
            if (omo.player_cur_loc->room_item[i] != NULL) {
                // 打印前缀和名字，如：[一把锈迹斑斑的铁剑]
                printf("『%s』(%s) ", omo.player_cur_loc->room_item[i]->item_name,
                       omo.player_cur_loc->room_item[i]->item_id);
            }
        }
        printf(CLR_RESET "\n");
    }

    return 0;
}

/**
 * @brief 处理「talk」指令
 * 与当前房间内的 NPC 进行基本对话。
 */
int cmd_talk(int argc, char **argv) {
    if (argc == 1) {
        printf("你想和谁聊天？需要指定对方的身分 ID。\n");
        return 0;
    }

    /* 搜索房间内的 NPC 列表 */
    if (argc == 2) {
        int found = 0;
        for (int i = 0; i < 3 && omo.player_cur_loc->room_npc[i] != NULL; i++) {
            /* 匹配 NPC 的英文 ID */
            if (strcmp(argv[1], omo.player_cur_loc->room_npc[i]->room_npc_id) == 0) {
                printf(CLR_YELLOW CLR_BOLD "%s" CLR_RESET "对你说：\n", omo.player_cur_loc->room_npc[i]->room_npc_name);
                printf("%s\n", omo.player_cur_loc->room_npc[i]->room_npc_dialog);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("这里并没有叫「" CLR_YELLOW CLR_BOLD "%s" CLR_RESET "」的人，莫非你产生了幻听？\n", argv[1]);
        }
    }

    return 0;
}

/**
 * @brief 处理「see」指令
 * 详细观察（检视）房间内的 NPC、物品，或者查看自己的状态看板。
 */
int cmd_see(int argc, char **argv) {
    if (argc == 1) {
        printf("你要仔细查看谁？（提示：输入 'see self' 或 'see %s' 查看自己）\n", omo.player_id);
        return 0;
    }

    /* 1. 特色分支：查看玩家自身状态看板（指令：see self 或 see [玩家ID]） */
    if (strcmp(argv[1], "self") == 0 || strcmp(argv[1], omo.player_id) == 0) {
        printf(CLR_CYAN CLR_BOLD " === 江湖档案簿 : %s (%s) === " CLR_RESET "\n", omo.player_name, omo.player_id);

        /* 状态条展示 */
        print_status_bar("气血 (HP)", omo.hp, omo.max_hp, CLR_RED);
        print_status_bar("内力 (MP)", omo.mp, omo.max_mp, CLR_BLUE);
        print_status_bar("精神 (SP)", omo.spirit, omo.max_spirit, CLR_MAGENTA);

        /* 动态属性展示 */
        printf(CLR_CYAN "【 战斗属性 】" CLR_RESET "\n");
        printf("  攻击力：%d\n", player_get_atk(&omo));
        printf("  防御力：%d\n", player_get_def(&omo));

        /* 展示身上的装备位（头、身、下装、武器） */
        /* 使用了指针判空 logic：如果没穿装备则提示（空） */
        printf(CLR_CYAN "【 穿戴物 】" CLR_RESET "\n");
        printf("  头戴：%s\n", omo.armor_head ? omo.armor_head->item_name : CLR_WHITE "（空）" CLR_RESET);
        if (omo.armor_head)
            printf("        ID: %s\n", omo.armor_head->item_id);

        printf("  身着：%s\n", omo.armor_body ? omo.armor_body->item_name : CLR_WHITE "（空）" CLR_RESET);
        if (omo.armor_body)
            printf("        ID: %s\n", omo.armor_body->item_id);

        printf("  下装：%s\n", omo.armor_leg ? omo.armor_leg->item_name : CLR_WHITE "（空）" CLR_RESET);
        if (omo.armor_leg)
            printf("        ID: %s\n", omo.armor_leg->item_id);

        printf("  手持：%s\n", omo.weapon ? omo.weapon->item_name : CLR_WHITE "（空）" CLR_RESET);
        if (omo.weapon)
            printf("        ID: %s\n", omo.weapon->item_id);

        /* 【核心升级】动态遍历背包链表 */
        /* 这种方式支持“无限容量背包”，每一个新增物品都是链表的一个新节点 */
        printf(CLR_CYAN "【 随身行囊 】" CLR_RESET "\n  ");
        inventory_node_t *node = omo.player_bag;
        int               count = 0;
        while (node != NULL) {
            printf("『%s』(%s) ", node->item->item_name, node->item->item_id);
            node = node->next; // 移动到链表的下一个物品
            count++;
        }
        if (count == 0)
            printf("（行囊空空如也）");
        printf("\n");

        printf(CLR_CYAN "【 当前位置 】" CLR_RESET " %s\n", omo.player_cur_loc->room_name);
        return 0;
    }

    if (argc == 2) {
        /* 2. 扫描环境：查找 NPC */
        for (int i = 0; i < 3 && omo.player_cur_loc->room_npc[i] != NULL; i++) {
            if (strcmp(argv[1], omo.player_cur_loc->room_npc[i]->room_npc_id) == 0) {
                printf("%s\n", omo.player_cur_loc->room_npc[i]->room_npc_desc);
                return 0;
            }
        }

        /* 3. 扫描环境：查找 房间地上的物品 */
        for (int i = 0; i < 5; i++) {
            if (omo.player_cur_loc->room_item[i] != NULL) {
                if (strcmp(argv[1], omo.player_cur_loc->room_item[i]->item_id) == 0) {
                    printf("%s\n", omo.player_cur_loc->room_item[i]->item_desc);
                    return 0;
                }
            }
        }

        printf("此处四下空旷，并没有所谓「" CLR_YELLOW CLR_BOLD "%s" CLR_RESET "」的踪影。\n", argv[1]);
    }
    return 0;
}

int cmd_get(int argc, char **argv) {
    if (argc == 1) {
        printf("你要拿起什么？\n");
        return 0;
    }

    if (argc == 2) {
        int found_idx = -1;
        for (int i = 0; i < 5; i++) {
            if (omo.player_cur_loc->room_item[i] != NULL) {
                if (strcmp(argv[1], omo.player_cur_loc->room_item[i]->item_id) == 0) {
                    found_idx = i;
                    break;
                }
            }
        }

        if (found_idx != -1) {
            item_t *target = omo.player_cur_loc->room_item[found_idx];
            printf("你捡起了 " CLR_YELLOW CLR_BOLD "『%s%s』" CLR_RESET "，并将其收入行囊。\n", target->item_pre,
                   target->item_name);

            /* 【核心集成】物理转移至背包链表 */
            /* 这里不再只是打印文字，而是调用 player_add_item 函数在内存中创建新节点 */
            player_add_item(&omo, target);

            /* 从地面移除：这是为了防止“量子分身”，必须将房间指针设为 NULL */
            omo.player_cur_loc->room_item[found_idx] = NULL;
        } else {
            printf("这里并没有「" CLR_YELLOW CLR_BOLD "%s" CLR_RESET "」这样东西。\n", argv[1]);
        }
    }

    return 0;
}

int cmd_wear(int argc, char **argv) {
    if (argc == 1)
        return 0;
    if (argc == 2) {
        item_t *item = player_remove_item(&omo, argv[1]);

        if (item == NULL) {
            printf("你的背包没有『%s』这样东西。\n", argv[1]);
            return 0;
        } else {
            if (player_equip_item(&omo, item) == 0) {
                player_add_item(&omo, item);
            }
        }
    }
    return 0;
}

int cmd_check(int argc, char **argv) {
    if (argc < 2) {
        printf("你要查看什么？\n");
        return 0;
    }

    item_t *item = player_find_item(&omo, argv[1]);
    if (item != NULL) {
        printf(CLR_YELLOW CLR_BOLD "『%s』" CLR_RESET "\n", item->item_desc);
        switch (item->item_type) {
        case ITEM_TYPE_WEAPON:
            printf("攻击力增加：" CLR_BLUE CLR_BOLD "%d" CLR_RESET "\n", item->item_u.item_weapon.weapon_atk);
            break;
        case ITEM_TYPE_ARMOR_BODY:
        case ITEM_TYPE_ARMOR_LEG:
        case ITEM_TYPE_ARMOR_HEAD:
        case ITEM_TYPE_SHIELD:
            printf("防御力增加：" CLR_BLUE CLR_BOLD "%d" CLR_RESET "\n", item->item_u.item_armor.armor_def);
            break;

        case ITEM_TYPE_CONSUMABLE:
            if (item->item_u.item_con.con_hp_heal > 0)
                printf("恢复气血：" CLR_GREEN "%d" CLR_RESET "\n", item->item_u.item_con.con_hp_heal);
            if (item->item_u.item_con.con_mp_add > 0)
                printf("恢复内力：" CLR_BLUE "%d" CLR_RESET "\n", item->item_u.item_con.con_mp_add);
            if (item->item_u.item_con.con_sp_add > 0)
                printf("恢复精力：" CLR_BLUE "%d" CLR_RESET "\n", item->item_u.item_con.con_sp_add);

            break;
        default:
            break;
        }
    }
    return 0;
}

/**
 * @brief 实现自定义的提示符输入
 */
char *parser_input(char *buf, int size) {
    /* 显示带有当前房间名称的交互提示符 */
    printf(CLR_GREEN CLR_BOLD "%s" CLR_RESET " > ", omo.player_cur_loc->room_name);

    if (fgets(buf, size, stdin)) {
        /* 去除输入行尾的换行符 \n */
        buf[strcspn(buf, "\n")] = 0;
        return buf;
    }

    return NULL;
}

/**
 * @brief 基于空格的词法拆分
 */
int parser_split(char *line, char **argv, int max_args) {
    int count = 0;

    /* 使用 strtok 进行切词 */
    char *token = strtok(line, " ");

    while (token != NULL && count < max_args) {
        argv[count] = token;
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}

/**
 * @brief 核心分发逻辑
 * 遍历指令表，匹配用户输入的第一个单词并执行对应的函数。
 */
int parser_execute(int argc, char **argv) {
    if (argc == 0)
        return 0;

    for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
        if (strcmp(argv[0], cmd_table[i].cmd_name) == 0) {
            /* 找到匹配指令，调用对应的函数接口 */
            return cmd_table[i].cmd_func(argc, argv);
        }
    }

    /* 未匹配到任何指令时的默认反馈（增加仪式感） */
    printf("你对着空气自言自语，却始终没有人回应，周围只有山风在轻轻回响。\n");
    return 0;
}
