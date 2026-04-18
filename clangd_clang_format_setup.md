# VS Code clangd 与 clang-format 配置总结

## 目标

在 VS Code 中实现：

- 使用 clangd 进行 C/C++ 语法跳转、补全、诊断。
- 保存 `.c` / `.cpp` 文件时自动格式化。
- 连续变量声明自动对齐。
- 连续 `#define` 宏定义自动对齐。
- 带中文行尾注释的声明也尽量保持对齐。

## 需要的依赖

系统命令：

```bash
clangd
clang-format
clang
```

建议安装：

```bash
sudo apt install clangd clang-format clang
```

VS Code 扩展：

```text
llvm-vs-code-extensions.vscode-clangd
xaver.clang-format
ms-vscode.cpptools
```

其中：

- `clangd` 扩展负责语法跳转、补全、诊断。
- `xaver.clang-format` 扩展负责保存时格式化。
- Microsoft C/C++ 扩展保留，但 IntelliSense 已关闭，避免和 clangd 抢语言服务。

## 已修改的 VS Code 用户配置

配置文件位置：

```text
/home/omo/.config/Code/User/settings.json
```

关键配置如下：

```json
{
    "editor.formatOnSave": true,
    "[c]": {
        "editor.defaultFormatter": "xaver.clang-format",
        "editor.formatOnSave": true
    },
    "[cpp]": {
        "editor.defaultFormatter": "xaver.clang-format",
        "editor.formatOnSave": true
    },
    "clang-format.executable": "/usr/bin/clang-format",
    "clang-format.style": "{BasedOnStyle: LLVM, AlignConsecutiveMacros: Consecutive, AlignConsecutiveDeclarations: Consecutive, IndentWidth: 4, PointerAlignment: Right, AllowShortFunctionsOnASingleLine: None, BreakBeforeBraces: Attach, ColumnLimit: 120}",
    "clang-format.fallbackStyle": "LLVM",
    "C_Cpp.clang_format_style": "{BasedOnStyle: LLVM, AlignConsecutiveMacros: Consecutive, AlignConsecutiveDeclarations: Consecutive, IndentWidth: 4, PointerAlignment: Right, AllowShortFunctionsOnASingleLine: None, BreakBeforeBraces: Attach, ColumnLimit: 120}",
    "C_Cpp.clang_format_fallbackStyle": "LLVM",
    "C_Cpp.intelliSenseEngine": "disabled",
    "clangd.arguments": [
        "--header-insertion=never",
        "--fallback-style=LLVM"
    ]
}
```

## 已修改的 clang-format 配置

配置文件位置：

```text
/home/omo/.clang-format
```

内容重点：

```yaml
BasedOnStyle: LLVM
AlignConsecutiveDeclarations: Consecutive
AlignConsecutiveMacros: Consecutive
IndentWidth: 4
PointerAlignment: Right
AllowShortFunctionsOnASingleLine: None
BreakBeforeBraces: Attach
ColumnLimit: 120
```

说明：

- `AlignConsecutiveDeclarations: Consecutive` 用于对齐连续变量声明。
- `AlignConsecutiveMacros: Consecutive` 用于对齐连续 `#define`。
- `ColumnLimit: 120` 用于避免带中文行尾注释时，因为默认 80 列限制导致声明不对齐。

## 修复过的问题

### 1. clangd 启动失败

原配置里有：

```json
"--clang-format-style=file"
```

但当前系统的 `clangd 14` 不支持这个参数，导致 VS Code 中 clangd 启动后直接崩溃。

日志错误类似：

```text
clangd: Unknown command line argument '--clang-format-style=file'
Server process exited with code 1.
The Clang Language Server server crashed 5 times
```

处理方式：

删除该参数。现在 `clangd.arguments` 只保留：

```json
[
    "--header-insertion=never",
    "--fallback-style=LLVM"
]
```

### 2. V4L2 类型不能跳转

示例中的这些类型：

```c
struct v4l2_format
struct v4l2_capability
struct v4l2_requestbuffers
struct v4l2_buffer
enum v4l2_buf_type
```

定义在：

```c
#include <linux/videodev2.h>
```

如果文件没有包含这个头文件，clangd 会认为这些类型不完整，跳转会失败。

### 3. 变量声明没有完全对齐

示例：

```c
struct v4l2_format video_fmt; // 使用中文版格式结构体
struct v4l2_requestbuffers video_req; // 使用中文版申请缓冲区结构体
struct v4l2_buffer video_buf; // 使用中文版缓冲区详情结构体
```

默认 `ColumnLimit: 80` 时，带中文注释和函数体缩进后可能超过 80 列，clang-format 会放弃部分对齐。

处理方式：

```yaml
ColumnLimit: 120
```

格式化后：

```c
struct v4l2_format         video_fmt; // 使用中文版格式结构体
struct v4l2_requestbuffers video_req; // 使用中文版申请缓冲区结构体
struct v4l2_buffer         video_buf; // 使用中文版缓冲区详情结构体
```

### 4. `#define` 没有对齐

处理方式：

```yaml
AlignConsecutiveMacros: Consecutive
```

格式化后：

```c
#define VIDEO_WIDTH  1280
#define VIDEO_HEIGHT 800
#define VIDEO_PATH   "/dev/video2"
```

## 使用方式

修改配置后，在 VS Code 中执行：

```text
Developer: Reload Window
```

然后打开 `.c` 或 `.cpp` 文件，保存时会自动格式化。

如果某个文件没有按预期格式化：

1. 右键文件。
2. 选择 `Format Document With...`。
3. 确认默认 formatter 是：

```text
Clang-Format
xaver.clang-format
```

## 验证命令

验证 clangd 是否能解析文件：

```bash
clangd --header-insertion=never --fallback-style=LLVM --check=main.c
```

验证格式化输出：

```bash
/usr/bin/clang-format --style='{BasedOnStyle: LLVM, AlignConsecutiveMacros: Consecutive, AlignConsecutiveDeclarations: Consecutive, IndentWidth: 4, PointerAlignment: Right, AllowShortFunctionsOnASingleLine: None, BreakBeforeBraces: Attach, ColumnLimit: 120}' main.c
```

## 注意事项

- clangd 负责语法跳转，不负责这次的全局格式化策略。
- 保存格式化现在主要由 `xaver.clang-format` 扩展执行。
- 如果项目目录里有自己的 `.clang-format`，可能会覆盖或影响格式化结果。
- 对于复杂项目，建议生成 `compile_commands.json`，否则 clangd 只能使用 fallback 编译参数，跳转可能不稳定。

