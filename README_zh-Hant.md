[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9808

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9808/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MCP9808數位溫度感測器可將-40°C至+125°C之間的溫度轉換為數位字，精度最高可達±0.5°C。MCP9808配備了使用者可編程寄存器，為溫度感測應用提供了靈活性。這些寄存器允許使用者進行設置，如選擇關機或低功耗模式，以及指定溫度事件和臨界輸出邊界。當溫度變化超出指定邊界限制時，MCP9808會輸出一個事件信號。使用者可以選擇將事件輸出信號極性設置為低電平有效或高電平有效的比較器輸出，以用於恆溫器操作，或設置為基於微處理器的系統的溫度事件中斷輸出。事件輸出還可以配置為臨界溫度輸出。該感測器具有行業標準的2線、SMBus和標準I2C™兼容（100kHz/400kHz總線時鐘）串行介面，最多可在單個串行總線上控制八個感測器。這些特性使MCP9808成為複雜多區域溫度監測應用的理想選擇。

LibDriver MCP9808是LibDriver推出的MCP9808全功能驅動程式，提供溫度讀取和閾值中斷等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MCP9808的源文件。

/interface目錄包含了LibDriver MCP9808與平台無關的IIC總線模板。

/test目錄包含了LibDriver MCP9808驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MCP9808編程範例。

/doc目錄包含了LibDriver MCP9808離線文檔。

/datasheet目錄包含了MCP9808數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/mcp9808/index.html](https://www.libdriver.com/docs/mcp9808/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。