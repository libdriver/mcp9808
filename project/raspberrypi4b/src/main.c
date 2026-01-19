/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2026-01-25
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2026/01/25  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mcp9808_register_test.h"
#include "driver_mcp9808_read_test.h"
#include "driver_mcp9808_interrupt_test.h"
#include "driver_mcp9808_basic.h"
#include "driver_mcp9808_interrupt.h"
#include "gpio.h"
#include <getopt.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */
volatile static uint8_t gs_flag;           /**< flag */

/**
 * @brief     interface receive callback
 * @param[in] type interrupt type
 * @note      none
 */
static void a_callback(uint8_t type)
{
    float temperature_deg;
    
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

/**
 * @brief     mcp9808 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t mcp9808(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"crit", required_argument, NULL, 2},
        {"high", required_argument, NULL, 3},
        {"low", required_argument, NULL, 4},
        {"times", required_argument, NULL, 5},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    float low_threshold = 25.0f;
    float high_threshold = 30.0f;
    float critical_temperature = 27.0f;
    mcp9808_address_t addr = MCP9808_ADDRESS_A2A1A0_000;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_000;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_001;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_010;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_011;
                }
                else if (strcmp("4", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_100;
                }
                else if (strcmp("5", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_101;
                }
                else if (strcmp("6", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_110;
                }
                else if (strcmp("7", optarg) == 0)
                {
                    addr = MCP9808_ADDRESS_A2A1A0_111;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* critical temperature */
            case 2 :
            {
                /* critical temperature */
                critical_temperature = (float)atof(optarg);
                
                break;
            } 
            
            /* high threshold */
            case 3 :
            {
                /* high threshold */
                high_threshold = (float)atof(optarg);
                
                break;
            } 
            
            /* low threshold */
            case 4 :
            {
                /* low threshold */
                low_threshold = (float)atof(optarg);
                
                break;
            } 
            
            /* running times */
            case 5 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run functions */
    if (strcmp("t_reg", type) == 0)
    {
        /* run register test */
        if (mcp9808_register_test(addr) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (mcp9808_read_test(addr, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        g_gpio_irq = mcp9808_interrupt_test_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* run interrupt test */
        if (mcp9808_interrupt_test(addr, low_threshold, high_threshold,
                                   critical_temperature, times) != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        
        /* basic init */
        res = mcp9808_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
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
            mcp9808_interface_debug_print("mcp9808: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature);
        }
        
        /* basic deinit */
        (void)mcp9808_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* gpio init */
        g_gpio_irq = mcp9808_interrupt_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* run interrupt test */
        if (mcp9808_interrupt_init(addr, low_threshold, high_threshold,
                                   critical_temperature, a_callback) != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        gs_flag = 0;
        for (i = 0; i < times; i++)
        {
            while (gs_flag == 0)
            {
                gs_flag = 0;
            }
            
            /* output */
            mcp9808_interface_debug_print("mcp9808: find interrupt.\n");
        }
        
        /* gpio deinit */
        (void)mcp9808_interrupt_deinit();
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        mcp9808_interface_debug_print("Usage:\n");
        mcp9808_interface_debug_print("  mcp9808 (-i | --information)\n");
        mcp9808_interface_debug_print("  mcp9808 (-h | --help)\n");
        mcp9808_interface_debug_print("  mcp9808 (-p | --port)\n");
        mcp9808_interface_debug_print("  mcp9808 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]\n");
        mcp9808_interface_debug_print("  mcp9808 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--times=<num>]\n");
        mcp9808_interface_debug_print("  mcp9808 (-t int | --test=int) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]\n");
        mcp9808_interface_debug_print("                                [--crit=<temperature>] [--high=<threshold>] [--low=<threshold>] [--times=<num>]\n");
        mcp9808_interface_debug_print("  mcp9808 (-e read | --example=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--times=<num>]\n");
        mcp9808_interface_debug_print("  mcp9808 (-e int | --example=int) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]\n");
        mcp9808_interface_debug_print("                                   [--crit=<temperature>] [--high=<threshold>] [--low=<threshold>] [--times=<num>]\n");
        mcp9808_interface_debug_print("\n");
        mcp9808_interface_debug_print("Options:\n");
        mcp9808_interface_debug_print("      --addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>\n");
        mcp9808_interface_debug_print("                        Set the addr.([default: 0])\n");
        mcp9808_interface_debug_print("      --crit=<temperature>\n");
        mcp9808_interface_debug_print("                        Set the critical temperature.([default: 27.0])\n");
        mcp9808_interface_debug_print("  -e <read | int>, --example=<read | int>\n");
        mcp9808_interface_debug_print("                        Run the driver example.\n");
        mcp9808_interface_debug_print("      --low=<threshold>\n");
        mcp9808_interface_debug_print("                        Set the low temperature threshold.([default: 25.0])\n");
        mcp9808_interface_debug_print("  -h, --help            Show the help.\n");
        mcp9808_interface_debug_print("      --high=<threshold>\n");
        mcp9808_interface_debug_print("                        Set the high temperature threshold.([default: 30.0])\n");
        mcp9808_interface_debug_print("  -i, --information     Show the chip information.\n");
        mcp9808_interface_debug_print("  -p, --port            Display the pin connections of the current board.\n");
        mcp9808_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        mcp9808_interface_debug_print("                        Run the driver test.\n");
        mcp9808_interface_debug_print("      --times=<num>     Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        mcp9808_info_t info;
        
        /* print mcp9808 info */
        mcp9808_info(&info);
        mcp9808_interface_debug_print("mcp9808: chip is %s.\n", info.chip_name);
        mcp9808_interface_debug_print("mcp9808: manufacturer is %s.\n", info.manufacturer_name);
        mcp9808_interface_debug_print("mcp9808: interface is %s.\n", info.interface);
        mcp9808_interface_debug_print("mcp9808: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        mcp9808_interface_debug_print("mcp9808: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        mcp9808_interface_debug_print("mcp9808: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        mcp9808_interface_debug_print("mcp9808: max current is %0.2fmA.\n", info.max_current_ma);
        mcp9808_interface_debug_print("mcp9808: max temperature is %0.1fC.\n", info.temperature_max);
        mcp9808_interface_debug_print("mcp9808: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        mcp9808_interface_debug_print("mcp9808: SCL connected to GPIO3(BCM).\n");
        mcp9808_interface_debug_print("mcp9808: SDA connected to GPIO2(BCM).\n");
        mcp9808_interface_debug_print("mcp9808: ALERT connected to GPIO17(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = mcp9808(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        mcp9808_interface_debug_print("mcp9808: run failed.\n");
    }
    else if (res == 5)
    {
        mcp9808_interface_debug_print("mcp9808: param is invalid.\n");
    }
    else
    {
        mcp9808_interface_debug_print("mcp9808: unknown status code.\n");
    }

    return 0;
}
