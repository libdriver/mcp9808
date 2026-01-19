[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9808

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9808/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The MCP9808 digital temperature sensor converts temperatures between -40°C and +125°C to a digital word with ±0.5°C (max.) accuracy. The MCP9808 comes with user-programmable registers that provide flexibility for temperature sensing applications. The registers allow user-selectable settings such as Shutdown or low-power modes and the specification of temperature Event and Critical output boundaries. When the temperature changes beyond the specified boundary limits, the MCP9808 outputs an Event signal. The user has the option of setting the event output signal polarity as an active-low or active-high comparator output for thermostat operation, or as temperature event interrupt output for microprocessor-based systems. The event output can also be configured as a Critical temperature output. This sensor has an industry standard 2-wire, SMBus and Standard I2C™Compatible compatible (100kHz/400kHz bus clock) serial interface, allowing up to eight sensors to be controlled in a single serial bus. These features make the MCP9808 ideal for sophisticated multi-zone temperature- monitoring applications.

LibDriver MCP9808 is a full-featured driver for MCP9808, launched by LibDriver.It provides temperature reading, threshold interrupt and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver MCP9808 source files.

/interface includes LibDriver MCP9808 IIC platform independent template.

/test includes LibDriver MCP9808 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver MCP9808 sample code.

/doc includes LibDriver MCP9808 offline document.

/datasheet includes MCP9808 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

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

### Document

Online documents: [https://www.libdriver.com/docs/mcp9808/index.html](https://www.libdriver.com/docs/mcp9808/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.