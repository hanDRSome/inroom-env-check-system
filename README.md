# 基于 STM32的室内环境检测系统

## 1.技术与框架

### 1.1 软件使用
- STM32CubeMx
- Vscode 
### 1.2 环境框架
- Cmake
- arm-none-eabi
### 1.3 存储方式
- git 
- 本地

## 2.软件与硬件
### 2.1 项目框架

```text
project/
├── CMakeLists.txt              # 顶层构建配置
├── cmake/                      # CMake 模块和工具链配置
├── User/                       # 编译代码
│   ├── app
│   ├── driver
│   └── bsp
├── Core/                       # MCU 核心代码
│   ├── Inc/
│   ├── Src/
│   ├── startup/
│   └── cmsis/
├── Drivers/                    # STM32 HAL / 外设驱动
│   └── STM32xxxx_HAL_Driver/
├── docs/                       # 文档和开发说明
├── tools/                      # 脚本、烧录工具
├── build/                      # 编译输出目录
├── .gitignore
├── README.md
└── .vscode                     # 环境配置
```
### 2.2 用户工作区
├── User/       
│   ├── app
│   ├── driver
│   └── bsp
### 2.3 硬件配置
- MQ-2
    data_pin: PA0
- SR501
    data_pin: PA1
- DHT11
    data_pin: PA2
- BH1750
    scl_pin: PB0
    sda_pin: PB1
- OLED
    scl_pin: PB10
    sda_pin: PB11
- fan
    pwm_pin: PA6
- DCDC-5V
- LDO-3.3V
- BATTERY
    3.7V锂电池
- OSC 
    8MHz
- LED
    硬件设计
- 按键
    同上
（不要冲突设计）

## 3.目的
### 3.1 特点
- 可移植
- 高效率

### 3.2 要求
无要求
