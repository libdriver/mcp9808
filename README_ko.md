[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9808

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9808/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MCP9808 디지털 온도 센서는 -40°C ~ +125°C 범위의 온도를 ±0.5°C(최대)의 정확도로 디지털 워드로 변환합니다. MCP9808은 사용자가 프로그래밍할 수 있는 레지스터를 제공하여 온도 감지 애플리케이션에 유연성을 제공합니다. 이 레지스터를 통해 사용자는 종료 또는 저전력 모드와 같은 설정을 선택하고 온도 이벤트 및 임계 출력 경계를 지정할 수 있습니다. 온도가 지정된 경계를 벗어나면 MCP9808은 이벤트 신호를 출력합니다. 사용자는 이벤트 출력 신호의 극성을 서모스탯 작동을 위한 액티브 로우 또는 액티브 하이 비교기 출력으로 설정하거나 마이크로프로세서 기반 시스템을 위한 온도 이벤트 인터럽트 출력으로 설정할 수 있습니다. 이벤트 출력은 임계 온도 출력으로도 구성할 수 있습니다. 이 센서는 업계 표준 2선식, SMBus 및 표준 I2C™ 호환(100kHz/400kHz 버스 클록) 직렬 인터페이스를 지원하여 단일 직렬 버스에서 최대 8개의 센서를 제어할 수 있습니다. 이러한 특징 덕분에 MCP9808은 정교한 다중 구역 온도 모니터링 애플리케이션에 이상적입니다.

LibDriver MCP9808은 LibDriver에서 출시한 MCP9808용 완벽한 기능을 갖춘 드라이버입니다. 온도 판독, 임계값 인터럽트 및 기타 기능을 제공합니다. LibDriver는 MISRA 규정을 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver MCP9808의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver MCP9808용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver MCP9808드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver MCP9808프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver MCP9808오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 MCP9808데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/mcp9808/index.html](https://www.libdriver.com/docs/mcp9808/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.