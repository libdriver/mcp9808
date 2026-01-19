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
 * @file      driver_mcp9808_interrupt_test.c
 * @brief     driver mcp9808 interrupt test source file
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

#include "driver_mcp9808_interrupt_test.h"

static mcp9808_handle_t gs_handle;        /**< mcp9808 handle */
volatile static uint8_t gs_flag;          /**< flag */

/**
 * @brief     interface receive callback
 * @param[in] type interrupt type
 * @note      none
 */
static void a_callback(uint8_t type)
{
    int16_t raw;
    float temperature_deg = 0.0f;
    
    switch (type)
    {
        case MCP9808_STATUS_CRIT:
        {
            mcp9808_interface_debug_print("mcp9808: irq critical temperature.\n");
            gs_flag = 1;
            
            /* read temperature */
            (void)mcp9808_read(&gs_handle, &raw, &temperature_deg);
            
            /* output */
            mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
            
            break;
        }
        case MCP9808_STATUS_T_UPPER:
        {
            mcp9808_interface_debug_print("mcp9808: irq temperature upper boundary.\n");
            gs_flag = 1;
            
            /* read temperature */
            (void)mcp9808_read(&gs_handle, &raw, &temperature_deg);
            
            /* output */
            mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
            
            break;
        }
        case MCP9808_STATUS_T_LOWER:
        {
            mcp9808_interface_debug_print("mcp9808: irq temperature lower boundary.\n");
            gs_flag = 1;
            
            /* read temperature */
            (void)mcp9808_read(&gs_handle, &raw, &temperature_deg);
            
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
 * @brief  interrupt test irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t mcp9808_interrupt_test_irq_handler(void)
{
    if (mcp9808_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     interrupt test
 * @param[in] addr iic device address
 * @param[in] low temperature low threshold
 * @param[in] high temperature high threshold
 * @param[in] critical_temperature critical temperature
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mcp9808_interrupt_test(mcp9808_address_t addr, float low, float high,
                               float critical_temperature, uint32_t times)
{
    uint8_t res;
    uint16_t reg;
    uint32_t i;
    mcp9808_info_t info;
    
    /* link interface function */
    DRIVER_MCP9808_LINK_INIT(&gs_handle, mcp9808_handle_t); 
    DRIVER_MCP9808_LINK_IIC_INIT(&gs_handle, mcp9808_interface_iic_init);
    DRIVER_MCP9808_LINK_IIC_DEINIT(&gs_handle, mcp9808_interface_iic_deinit);
    DRIVER_MCP9808_LINK_IIC_READ(&gs_handle, mcp9808_interface_iic_read);
    DRIVER_MCP9808_LINK_IIC_WRITE(&gs_handle, mcp9808_interface_iic_write);
    DRIVER_MCP9808_LINK_DELAY_MS(&gs_handle, mcp9808_interface_delay_ms);
    DRIVER_MCP9808_LINK_DEBUG_PRINT(&gs_handle, mcp9808_interface_debug_print);
    DRIVER_MCP9808_LINK_RECEIVE_CALLBACK(&gs_handle, a_callback);
    
    /* mcp9808 info */
    res = mcp9808_info(&info);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        mcp9808_interface_debug_print("mcp9808: chip is %s.\n", info.chip_name);
        mcp9808_interface_debug_print("mcp9808: manufacturer is %s.\n", info.manufacturer_name);
        mcp9808_interface_debug_print("mcp9808: interface is %s.\n", info.interface);
        mcp9808_interface_debug_print("mcp9808: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        mcp9808_interface_debug_print("mcp9808: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        mcp9808_interface_debug_print("mcp9808: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        mcp9808_interface_debug_print("mcp9808: max current is %0.2fmA.\n", info.max_current_ma);
        mcp9808_interface_debug_print("mcp9808: max temperature is %0.1fC.\n", info.temperature_max);
        mcp9808_interface_debug_print("mcp9808: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start interrupt test */
    mcp9808_interface_debug_print("mcp9808: start interrupt test.\n");
    
    /* set address pin */
    res = mcp9808_set_addr(&gs_handle, addr);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    
    /* mcp9808 init */
    res = mcp9808_init(&gs_handle);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: init failed.\n");
        
        return 1;
    }
    
    /* disable shutdown */
    res = mcp9808_set_shutdown(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set shutdown failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature threshold hysteresis 0C */
    res = mcp9808_set_temperature_threshold_hysteresis(&gs_handle, MCP9808_HYSTERESIS_0P0);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable critical temperature lock */
    res = mcp9808_set_critical_temperature_lock(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set critical temperature lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable temperature temperature lock */
    res = mcp9808_set_temperature_threshold_lock(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature temperature lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set resolution 0.0625C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P0625);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert output select all */
    res = mcp9808_set_alert_output_select(&gs_handle, MCP9808_ALERT_OUTPUT_SELECT_ALL);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output select failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert output polarity low */
    res = mcp9808_set_alert_output_polarity(&gs_handle, MCP9808_ALERT_OUTPUT_POLARITY_LOW);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output polarity failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert interrupt output mode */
    res = mcp9808_set_alert_output_mode(&gs_handle, MCP9808_ALERT_OUTPUT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output mode failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* temperature convert to register */
    res = mcp9808_temperature_convert_to_register(&gs_handle, high, &reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: temperature convert to register failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature high threshold */
    res = mcp9808_set_temperature_high_threshold(&gs_handle, reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature high threshold failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* temperature convert to register */
    res = mcp9808_temperature_convert_to_register(&gs_handle, low, &reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: temperature convert to register failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature low threshold */
    res = mcp9808_set_temperature_low_threshold(&gs_handle, reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature low threshold failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* temperature convert to register */
    res = mcp9808_temperature_convert_to_register(&gs_handle, critical_temperature, &reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: temperature convert to register failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set critical temperature */
    res = mcp9808_set_critical_temperature(&gs_handle, reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set critical temperature failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear interrupt */
    res = mcp9808_clear_interrupt(&gs_handle);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: clear interrupt failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable alert output */
    res = mcp9808_set_alert_output(&gs_handle, MCP9808_BOOL_TRUE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
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
    
    /* finish interrupt test */
    mcp9808_interface_debug_print("mcp9808: finish interrupt test.\n");
    (void)mcp9808_deinit(&gs_handle);
    
    return 0;
}
