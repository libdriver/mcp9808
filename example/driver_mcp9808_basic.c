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
 * @file      driver_mcp9808_basic.c
 * @brief     driver mcp9808 basic source file
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

#include "driver_mcp9808_basic.h"

static mcp9808_handle_t gs_handle;        /**< mcp9808 handle */

/**
 * @brief     basic example init
 * @param[in] addr iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t mcp9808_basic_init(mcp9808_address_t addr)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_MCP9808_LINK_INIT(&gs_handle, mcp9808_handle_t); 
    DRIVER_MCP9808_LINK_IIC_INIT(&gs_handle, mcp9808_interface_iic_init);
    DRIVER_MCP9808_LINK_IIC_DEINIT(&gs_handle, mcp9808_interface_iic_deinit);
    DRIVER_MCP9808_LINK_IIC_READ(&gs_handle, mcp9808_interface_iic_read);
    DRIVER_MCP9808_LINK_IIC_WRITE(&gs_handle, mcp9808_interface_iic_write);
    DRIVER_MCP9808_LINK_DELAY_MS(&gs_handle, mcp9808_interface_delay_ms);
    DRIVER_MCP9808_LINK_DEBUG_PRINT(&gs_handle, mcp9808_interface_debug_print);
    DRIVER_MCP9808_LINK_RECEIVE_CALLBACK(&gs_handle, mcp9808_interface_receive_callback);

    /* set addr */
    res = mcp9808_set_addr(&gs_handle, addr);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr pin failed.\n");
        
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
    
    /* set default temperature threshold hysteresis */
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
    
    /* disable alert output */
    res = mcp9808_set_alert_output(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default resolution */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_BASIC_DEFAULT_RESOLUTION);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *temperature_deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mcp9808_basic_read(float *temperature_deg)
{
    int16_t raw;
   
    /* read data */
    if (mcp9808_read(&gs_handle, &raw, temperature_deg) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mcp9808_basic_deinit(void)
{
    /* close mcp9808 */
    if (mcp9808_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
