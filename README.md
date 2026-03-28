# esp-numworks
This is numworks caculation porting on esp32s3 device

# Numworks 移植项目

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Numworks-red.svg)](https://www.numworks.com/)

本项目是针对 Numworks 计算器的系统移植与定制开发，旨在将自定义固件或第三方操作系统移植到 Numworks 硬件平台，并提供完整的构建与烧录工具链。

## 📖 项目简介

Numworks 是一款基于 ARM Cortex-M4 的开源计算器，其硬件设计、操作系统（Epsilon）和配套工具链均开放源代码。本项目在此基础上进行移植开发，支持：

- 自定义内核与应用程序
- 跨平台交叉编译环境
- 一键烧录脚本
- 仿真器调试支持

## 🏗️ 系统架构

下图为移植后的系统软件架构，展示了从硬件抽象层到应用层的模块关系。

![](.\doc\main.png)

## 🔄 移植流程

整个移植过程分为硬件适配、引导加载、系统集成和测试验证四个阶段，具体流程如下：

图表

![](.\doc\step.png)

## 🚀 快速开始

### 1. 环境准备

- 操作系统：Ubuntu 20.04+ / macOS 12+ / Windows WSL2
- 编译工具链：`arm-none-eabi-gcc` (≥ 10.2)
- 烧录工具：`dfu-util` 或 `OpenOCD`

安装命令（Ubuntu）：

bash

```
sudo apt update
sudo apt install build-essential git dfu-util
sudo apt install gcc-arm-none-eabi
```



### 2. 克隆仓库

bash

```
git clone https://github.com/your-username/numworks-port.git
cd numworks-port
```



### 3. 构建固件

bash

```
make clean
make all
```



编译成功后会在 `build/` 目录下生成 `epsilon.bin` 或 `firmware.dfu` 文件。

### 4. 烧录到设备

将 Numworks 计算器通过 USB 连接电脑，按住 **6** 键（或根据机型）进入 DFU 模式，然后执行：

bash

```
make flash
```



或手动使用 `dfu-util`：

bash

```
dfu-util -a 0 -s 0x08000000:leave -D build/firmware.dfu
```



### 5. 运行仿真器（可选）

bash

```
make simulator
```



## 📂 目录结构

text

```
.
├── src/            # 移植相关源代码
├── drivers/        # 硬件驱动适配层
├── kernel/         # 内核修改部分
├── tools/          # 辅助脚本
├── build/          # 编译输出目录
├── Makefile        # 主构建文件
├── README.md       # 本文件
└── LICENSE         # 开源许可证
```



## 🧪 测试状态

| 模块        | 状态     | 备注                     |
| :---------- | :------- | :----------------------- |
| 屏幕显示    | ✅ 通过   | 支持 320x240 灰度/彩色   |
| 键盘输入    | ✅ 通过   | 完整按键扫描             |
| USB 通信    | ✅ 通过   | DFU 烧录 / 虚拟串口      |
| 电池管理    | 🟡 部分   | 充电检测未完全适配       |
| Python 引擎 | ✅ 通过   | MicroPython 移植成功     |
| 第三方应用  | 🚧 开发中 | 正在移植外部应用程序框架 |

## 🤝 贡献指南

欢迎通过 Issue 和 Pull Request 参与贡献。请确保：

1. 代码符合项目编码规范（`clang-format`）
2. 提交前进行完整测试
3. 更新相关文档

## 📄 许可证

本项目基于 [MIT 许可证](https://license/) 开源。

## 📚 参考资料

- [Numworks 官方网站](https://www.numworks.com/)
- [Numworks 开源固件仓库](https://github.com/numworks/epsilon)
- [STM32F412 参考手册](https://www.st.com/resource/en/reference_manual/dm00180369.pdf)
- [ARM Cortex-M4 编程手册](https://developer.arm.com/documentation/ddi0439/latest/)
