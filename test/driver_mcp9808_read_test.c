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
 * @file      driver_mcp9808_read_test.c
 * @brief     driver mcp9808 read test source file
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

#include "driver_mcp9808_read_test.h"

static mcp9808_handle_t gs_handle;        /**< mcp9808 handle */

/**
 * @brief     read test
 * @param[in] addr iic device address
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mcp9808_read_test(mcp9808_address_t addr, uint32_t times)
{
    uint8_t res;
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
    DRIVER_MCP9808_LINK_RECEIVE_CALLBACK(&gs_handle, mcp9808_interface_receive_callback);
    
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
    
    /* start read test */
    mcp9808_interface_debug_print("mcp9808: start read test.\n");
    
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
    
    /* set temperature threshold hysteresis 0.0C */
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
    
    /* disable temperature threshold lock */
    res = mcp9808_set_temperature_threshold_lock(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold lock failed.\n");
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
    
    /* disable alert output */
    res = mcp9808_set_alert_output(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output failed.\n");
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
    
    /* set resolution 0.5C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P5);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set resolution 0.5C */
    mcp9808_interface_debug_print("mcp9808: set resolution 0.5C.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float temperature_deg;
        
        /* delay 1000ms */
        mcp9808_interface_delay_ms(1000);
        
        /* read data */
        res = mcp9808_read(&gs_handle, &raw, &temperature_deg);
        if (res != 0)
        {
            mcp9808_interface_debug_print("mcp9808: read failed.\n");
            (void)mcp9808_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
    }
    
    /* set resolution 0.25C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P25);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set resolution 0.25C */
    mcp9808_interface_debug_print("mcp9808: set resolution 0.25C.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float temperature_deg;
        
        /* delay 1000ms */
        mcp9808_interface_delay_ms(1000);
        
        /* read data */
        res = mcp9808_read(&gs_handle, &raw, &temperature_deg);
        if (res != 0)
        {
            mcp9808_interface_debug_print("mcp9808: read failed.\n");
            (void)mcp9808_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
    }
    
    /* set resolution 0.125C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P125);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set resolution 0.125C */
    mcp9808_interface_debug_print("mcp9808: set resolution 0.125C.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float temperature_deg;
        
        /* delay 1000ms */
        mcp9808_interface_delay_ms(1000);
        
        /* read data */
        res = mcp9808_read(&gs_handle, &raw, &temperature_deg);
        if (res != 0)
        {
            mcp9808_interface_debug_print("mcp9808: read failed.\n");
            (void)mcp9808_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
    }
    
    /* set resolution 0.0625C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P0625);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set resolution 0.0625C */
    mcp9808_interface_debug_print("mcp9808: set resolution 0.0625C.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float temperature_deg;
        
        /* delay 1000ms */
        mcp9808_interface_delay_ms(1000);
        
        /* read data */
        res = mcp9808_read(&gs_handle, &raw, &temperature_deg);
        if (res != 0)
        {
            mcp9808_interface_debug_print("mcp9808: read failed.\n");
            (void)mcp9808_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        mcp9808_interface_debug_print("mcp9808: temperature is %0.2fC.\n", temperature_deg);
    }
    
    /* finish read test */
    mcp9808_interface_debug_print("mcp9808: finish read test.\n");
    (void)mcp9808_deinit(&gs_handle);
    
    return 0;
}
