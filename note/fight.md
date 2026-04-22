# 实施方案：MUD 实时战斗系统 (Combat System)

## 核心设计决策
- **战斗模型**：采用半实时制。玩家开启战斗后，每隔 N 秒自动进行一次普通攻击。
- **手动控制**：玩家可以在自动攻击的间隙，手动输入技能指令。
- **解决阻塞**：为了让战斗“动起来”，我们将通过计算时间增量（Delta Time）来驱动自动攻击逻辑。

## Proposed Changes

### 1. 基础数据定义 (Headers)

#### [MODIFY] [player.h](file:///media/omo/新加卷/code/cuitxkx/inc/player.h)
- 增加 `room_npc_t *fighting_target;` 成员。
- 增加 `time_t last_attack_time;` 记录上次普攻时间。

#### [MODIFY] [room.h](file:///media/omo/新加卷/code/cuitxkx/inc/room.h)
- 为 `room_npc_t` 增加简单的 AI 标记（如是否主动攻击）。

---

### 2. 战斗核心逻辑 (Implementation)

#### [NEW] [combat.c](file:///media/omo/新加卷/code/cuitxkx/src/combat.c)
- `combat_perform_attack(attacker, defender)`：核心伤害结算函数。计算防御抵扣，扣除 HP，并打印中文战斗描述。
- `combat_check_death(target)`：检查角色是否死亡。如果是 NPC 死亡，处理掉落并将其从房间移除；如果是玩家死亡，处理重生。

#### [MODIFY] [parser.c](file:///media/omo/新加卷/code/cuitxkx/src/parser.c)
- **`cmd_kill`**：设置战斗标志，更新 `last_attack_time` 为当前时间。
- **`cmd_cast`**：调整逻辑，允许在战斗中直接对 `fighting_target` 施放技能。

---

### 3. 世界驱动 (Main Loop)

#### [MODIFY] [main.c](file:///media/omo/新加卷/code/cuitxkx/src/main.c)
- 在 `while(1)` 循环中，调用 `combat_tick()` 函数。
- **可选优化**：引入非阻塞输入，让 NPC 即使在玩家发呆时也能持续攻击。

---

## 验证计划

### 自动化测试
1. 执行 `kill slam`，观察是否每隔几秒自动跳出伤害提示。
2. 在自动攻击期间输入 `cast zazen` 或其他伤害技能，确认技能伤害能立刻生效。

### 手动验证
1. 验证 NPC 死亡后是否正确从房间消失。
2. 验证玩家死亡后是否有提示并停止战斗。
