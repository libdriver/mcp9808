[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9808

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9808/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MCP9808数字温度传感器可将-40°C至+125°C之间的温度转换为数字字，精度最高可达±0.5°C。MCP9808配备了用户可编程寄存器，为温度传感应用提供了灵活性。这些寄存器允许用户进行设置，如选择关机或低功耗模式，以及指定温度事件和临界输出边界。当温度变化超出指定边界限制时，MCP9808会输出一个事件信号。用户可以选择将事件输出信号极性设置为低电平有效或高电平有效的比较器输出，以用于恒温器操作，或设置为基于微处理器的系统的温度事件中断输出。事件输出还可以配置为临界温度输出。该传感器具有行业标准的2线、SMBus和标准I2C™兼容（100kHz/400kHz总线时钟）串行接口，最多可在单个串行总线上控制八个传感器。这些特性使MCP9808成为复杂多区域温度监测应用的理想选择。

LibDriver MCP9808是LibDriver推出的MCP9808全功能驱动程序，提供温度读取和阈值中断等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver MCP9808的源文件。

/interface目录包含了LibDriver MCP9808与平台无关的IIC总线模板。

/test目录包含了LibDriver MCP9808驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver MCP9808编程范例。

/doc目录包含了LibDriver MCP9808离线文档。

/datasheet目录包含了MCP9808数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_mcp9808_basic.h"

uint8_t res;
uint32_t i;
float temperature;

/* basic init */
res = mcp9808_basic_init(MCP9808_ADDRESS_A2A1A0_000);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
{
    /* delay 1000ms */
    mcp9808_interface_delay_ms(1000);

    /* read data */
    res = mcp9808_basic_read((float *)&temperature);
    if (res != 0)
    {
        (void)mcp9808_basic_deinit();

        return 1;
    }

    /* output */
    mcp9808_interface_debug_print("mcp9808: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)3);
    mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature);
    
    ...
}

...
    
/* basic deinit */
(void)mcp9808_basic_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_mcp9808_interrupt.h"

uint8_t res;
uint32_t i;
float low_threshold = 25.0f;
float high_threshold = 30.0f;
float critical_temperature = 27.0f;
uint8_t (*g_gpio_irq)(void) = NULL;
volatile static uint8_t gs_flag;

static void a_callback(uint8_t type)
{
    float temperature_deg = 0.0f;
    
    switch (type)
    {
        case MCP9808_STATUS_CRIT:
        {
            mcp9808_interface_debug_print("mcp9808: irq critical temperature.\n");
            gs_flag = 1;
            
            /* read temperature */
            (void)mcp9808_interrupt_read(&temperature_deg);
            
            /* output */
            mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
            
            break;
        }
        case MCP9808_STATUS_T_UPPER:
        {
            mcp9808_interface_debug_print("mcp9808: irq temperature upper boundary.\n");
            gs_flag = 1;
            
            /* read temperature */
            (void)mcp9808_interrupt_read(&temperature_deg);
            
            /* output */
            mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
            
            break;
        }
        case MCP9808_STATUS_T_LOWER:
        {
            mcp9808_interface_debug_print("mcp9808: irq temperature lower boundary.\n");
            gs_flag = 1;
            
            /* read temperature */
            (void)mcp9808_interrupt_read(&temperature_deg);
            
            /* output */
            mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
            
            break;
        }
        default:
        {
            mcp9808_interface_debug_print("mcp9808: unknown code.\n");
            
            break;
        }
    }
}

/* gpio init */
g_gpio_irq = mcp9808_interrupt_irq_handler;
res = gpio_interrupt_init();
if (res != 0)
{
    g_gpio_irq = NULL;

    return 1;
}

/* run interrupt test */
if (mcp9808_interrupt_init(MCP9808_ADDRESS_A2A1A0_000, low_threshold, high_threshold,
                           critical_temperature, a_callback) != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...
    
gs_flag = 0;
for (i = 0; i < times; i++)
{
    while (gs_flag == 0)
    {
        gs_flag = 0;
    }

    /* output */
    mcp9808_interface_debug_print("mcp9808: find interrupt.\n");
    
    ...
}

...
    
/* gpio deinit */
(void)mcp9808_interrupt_deinit();
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/mcp9808/index.html](https://www.libdriver.com/docs/mcp9808/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。