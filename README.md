# CUITXKX - 模块化文字江湖引擎 (MUD Engine)

CUITXKX 是一个基于 C 语言开发的简约、模块化 MUD (Multi-User Dungeon) 游戏引擎骨架。它旨在为文字冒险游戏开发者提供一个坚实的基础，强调代码的可读性、扩展性以及现代化的构建流程。

## 🌟 核心特色

- **模块化设计**：逻辑、数据、解析器完全分离，方便扩展房间与 NPC。
- **CMake 构建**：支持标准 CMake 工作流，轻松跨平台。
- **ANSI 动态色彩**：内置颜色宏支持，提供沉浸式的文字体验。
- **指令系统**：基于函数指针的高效指令分发机制。

## 🛠️ 核心功能

- **`look`**：观察当前环境，高亮显示出口与周边的 NPC。
- **`go [方向]`**：在江湖中穿梭（支持：东、西、南、北、上、下）。
- **`talk [ID]`**：与 NPC 进行互动对话。
- **`see [ID]`**：细致观察房间里的某个人或物。
- **`help [指令]`**：内置详细的中文帮助手册。

## 🚀 快速开始

### 1. 环境准备
确保你的系统已安装：
- **GCC** (或 Clang)
- **CMake** (3.10+)
- **Make**

### 2. 获取代码并构建
```bash
mkdir build
cd build
cmake ..
make
```

### 3. 运行游戏
```bash
./game
```

## 📂 项目结构

```text
.
├── CMakeLists.txt      # 顶层 CMake 配置文件
├── inc/                # 头文件目录 (.h)
│   ├── parser.h        # 指令解析器定义
│   ├── room.h          # 房间与 NPC 数据结构
│   └── util.h          # 工具宏（如颜色控制）
├── src/                # 源码目录 (.c)
│   ├── main.c          # 游戏主循环入口
│   ├── parser.c        # 指令逻辑实现
│   └── room.c          # 房间实例定义
└── text/               # 数据与文本资源
    └── room_spawn.h    # 初始化场景数据
```

## 📜 开发者指南

本项目遵循 **Teaching-First** 原则。如果你想添加一个新房间：
1. 在 `text/room_spawn.h` 中定义新的 `room_loc_t` 结构体。
2. 在 `room.c` 中配置其指针连接。
3. 重新执行 `make` 即可。

---

> [!TIP]
> **提示**：在 Linux 下运行效果最佳，建议使用支持 ANSI 颜色的终端（如 GNOME Terminal, Alacritty 等）。
