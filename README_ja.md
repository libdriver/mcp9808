[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9808

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9808/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MCP9808 デジタル温度センサは、-40°C ～ +125°C の温度を ±0.5°C (最大) の精度でデジタルデータに変換します。MCP9808 には、温度検知アプリケーションに柔軟性を提供するユーザプログラマブルレジスタが搭載されています。レジスタにより、シャットダウンまたは低電力モードなどの設定や、温度イベントおよびクリティカル出力境界の指定をユーザが選択できます。温度が指定された境界制限を超えると、MCP9808 はイベント信号を出力します。イベント出力信号の極性は、サーモスタット動作の場合はアクティブローまたはアクティブハイのコンパレータ出力、マイクロプロセッサベースのシステムの場合は温度イベント割り込み出力として設定できます。イベント出力は、クリティカル温度出力としても構成できます。このセンサは、業界標準の 2 線式、SMBus、および標準 I2C™ 互換 (バスクロック 100kHz/400kHz) シリアルインタフェースを備えており、単一のシリアルバスで最大 8 個のセンサを制御できます。これらの機能により、MCP9808 は高度なマルチゾーン温度監視アプリケーションに最適です。

LibDriver MCP9808は、LibDriverがリリースしたMCP9808用のフル機能ドライバです。温度読み取り、閾値割り込み、その他の追加機能を提供します。LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MCP9808のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MCP9808用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MCP9808ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MCP9808プログラミング例が含まれています。

/ docディレクトリには、LibDriver MCP9808オフラインドキュメントが含まれています。

/ datasheetディレクトリには、MCP9808データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/mcp9808/index.html](https://www.libdriver.com/docs/mcp9808/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。