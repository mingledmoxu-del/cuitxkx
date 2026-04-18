#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* ANSI 颜色代码定义 */
#define CLR_RESET  "\033[0m"
#define CLR_BOLD   "\033[1m"
#define CLR_CYAN   "\033[36m"
#define CLR_GREEN  "\033[32m"
#define CLR_YELLOW "\033[33m"
#define CLR_RED    "\033[31m"

/* 方向枚举 */
typedef enum {
    NORTH = 0,
    SOUTH,
    EAST,
    WEST,
    DIR_COUNT
} Direction;

/* 房间结构体 */
typedef struct {
    const char *name;
    const char *description;
    int exits[DIR_COUNT]; // 存储相邻房间在 world 数组中的索引，-1 表示无出口
} Room;

/* 玩家结构体 */
typedef struct {
    char name[32];
    int hp;
    int attack;
    int defense;
    int current_room;
} Player;

/* 全局世界地图 */
Room world[] = {
    {
        "新手村",
        "一个安静的小村庄。",
        {-1, -1, 1, 2} // north, south, east (森林), west (山路)
    },
    {
        "森林",
        "树木茂密，偶尔传来野兽叫声。",
        {-1, -1, -1, 0} // west -> 新手村
    },
    {
        "山路",
        "一条通往山寨的崎岖小路。",
        {-1, -1, 0, -1} // east -> 新手村
    }
};

/* 辅助函数：获取方向名称 */
const char* get_dir_name(Direction d) {
    switch (d) {
        case NORTH: return "north";
        case SOUTH: return "south";
        case EAST:  return "east";
        case WEST:  return "west";
        default:    return "unknown";
    }
}

/* 功能：查看当前房间 */
void do_look(Player *p) {
    Room *r = &world[p->current_room];
    printf("\n%s你现在位于：%s%s%s\n", CLR_BOLD, CLR_CYAN, r->name, CLR_RESET);
    printf("%s\n", r->description);
    
    printf("出口：");
    bool first = true;
    for (int i = 0; i < DIR_COUNT; i++) {
        if (r->exits[i] != -1) {
            if (!first) printf(" ");
            printf("%s%s%s", CLR_YELLOW, get_dir_name(i), CLR_RESET);
            first = false;
        }
    }
    printf("\n\n");
}

/* 功能：查看人物状态 */
void do_status(Player *p) {
    printf("\n%s姓名：%s%s\n", CLR_BOLD, CLR_RESET, p->name);
    printf("%s生命：%s%d\n", CLR_RED, CLR_RESET, p->hp);
    printf("%s攻击：%s%d\n", CLR_GREEN, CLR_RESET, p->attack);
    printf("%s防御：%s%d\n\n", CLR_YELLOW, CLR_RESET, p->defense);
}

/* 功能：移动 */
void do_go(Player *p, const char *dir_str) {
    Room *r = &world[p->current_room];
    Direction dir = -1;

    if (strcmp(dir_str, "north") == 0) dir = NORTH;
    else if (strcmp(dir_str, "south") == 0) dir = SOUTH;
    else if (strcmp(dir_str, "east") == 0) dir = EAST;
    else if (strcmp(dir_str, "west") == 0) dir = WEST;

    if (dir == -1) {
        printf("那个方向是哪？（请输入 north/south/east/west）\n");
        return;
    }

    int next_room = r->exits[dir];
    if (next_room == -1) {
        printf("这个方向走不通。\n");
    } else {
        p->current_room = next_room;
        printf("你来到：%s%s%s\n", CLR_CYAN, world[p->current_room].name, CLR_RESET);
        do_look(p);
    }
}

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    #ifdef _WIN32
    // 设置控制台输出代码页为 UTF-8 (65001) 以解决中文乱码
    SetConsoleOutputCP(65001);
    #endif

    Player player = {"player", 100, 10, 5, 0};
    char input[256];
    char cmd[32], arg[32];

    printf("欢迎来到简化版文字江湖！\n");
    do_look(&player);

    while (true) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // 简单的指令解析
        int num = sscanf(input, "%s %s", cmd, arg);
        if (num <= 0) continue;

        if (strcmp(cmd, "quit") == 0) {
            printf("游戏结束。\n");
            break;
        } else if (strcmp(cmd, "help") == 0) {
            printf("\n%s可用指令列表：%s\n", CLR_BOLD, CLR_RESET);
            printf("  %slook%s          - 查看当前房间信息\n", CLR_GREEN, CLR_RESET);
            printf("  %sgo <方向>%s     - 向指定方向移动 (north, south, east, west)\n", CLR_GREEN, CLR_RESET);
            printf("  %sstatus%s        - 查看人物属性状态\n", CLR_GREEN, CLR_RESET);
            printf("  %shelp%s          - 显示此帮助列表\n", CLR_GREEN, CLR_RESET);
            printf("  %squit%s          - 退出游戏\n\n", CLR_GREEN, CLR_RESET);
        } else if (strcmp(cmd, "look") == 0) {
            do_look(&player);
        } else if (strcmp(cmd, "status") == 0) {
            do_status(&player);
        } else if (strcmp(cmd, "go") == 0) {
            if (num < 2) {
                printf("去哪里？(go <direction>)\n");
            } else {
                do_go(&player, arg);
            }
        } else {
            printf("你自言自语，但没人理你。\n");
        }
    }

    return 0;
}