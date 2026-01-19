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
 * @file      driver_mcp9808_register_test.c
 * @brief     driver mcp9808 register test source file
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
#include <stdlib.h>

static mcp9808_handle_t gs_handle;        /**< mcp9808 handle */

/**
 * @brief     register test
 * @param[in] addr iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mcp9808_register_test(mcp9808_address_t addr)
{
    uint8_t res;
    uint8_t revision;
    uint16_t reg;
    uint16_t reg_check;
    float temperature_deg;
    float temperature_deg_check;
    mcp9808_info_t info;
    mcp9808_address_t addr_pin;
    mcp9808_bool_t enable;
    mcp9808_hysteresis_t hysteresis;
    mcp9808_alert_output_select_t output_select;
    mcp9808_alert_output_polarity_t polarity;
    mcp9808_alert_output_mode_t mode;
    mcp9808_resolution_t resolution;

    /* link interface function */
    DRIVER_MCP9808_LINK_INIT(&gs_handle, mcp9808_handle_t); 
    DRIVER_MCP9808_LINK_IIC_INIT(&gs_handle, mcp9808_interface_iic_init);
    DRIVER_MCP9808_LINK_IIC_DEINIT(&gs_handle, mcp9808_interface_iic_deinit);
    DRIVER_MCP9808_LINK_IIC_READ(&gs_handle, mcp9808_interface_iic_read);
    DRIVER_MCP9808_LINK_IIC_WRITE(&gs_handle, mcp9808_interface_iic_write);
    DRIVER_MCP9808_LINK_DELAY_MS(&gs_handle, mcp9808_interface_delay_ms);
    DRIVER_MCP9808_LINK_DEBUG_PRINT(&gs_handle, mcp9808_interface_debug_print);
    DRIVER_MCP9808_LINK_RECEIVE_CALLBACK(&gs_handle, mcp9808_interface_receive_callback);
    
    /* get information */
    res = mcp9808_info(&info);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
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
    
    /* start register test */
    mcp9808_interface_debug_print("mcp9808: start register test.\n");
    
    /* mcp9808_set_addr/mcp9808_get_addr test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_addr/mcp9808_get_addr test.\n");
    
    /* set address 0 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_000);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 0.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_000) ? "ok" : "error");
    
    /* set address 1 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_001);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 1.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_001) ? "ok" : "error");
    
    /* set address 2 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_010);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 2.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_010) ? "ok" : "error");
    
    /* set address 3 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_011);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 3.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_011) ? "ok" : "error");
    
    /* set address 4 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_100);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 4.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_100) ? "ok" : "error");
    
    /* set address 5 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_101);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 5.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_101) ? "ok" : "error");
    
    /* set address 6 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_110);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 6.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_110) ? "ok" : "error");
    
    /* set address 7 */
    res = mcp9808_set_addr(&gs_handle, MCP9808_ADDRESS_A2A1A0_111);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set address 7.\n");
    res = mcp9808_get_addr(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get addr failed.\n");
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check addr %s.\n", (addr_pin == MCP9808_ADDRESS_A2A1A0_111) ? "ok" : "error");
    
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
    
    /* mcp9808_set_temperature_threshold_hysteresis/mcp9808_get_temperature_threshold_hysteresis test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_temperature_threshold_hysteresis/mcp9808_get_temperature_threshold_hysteresis test.\n");
    
    /* set temperature threshold hysteresis 0C */
    res = mcp9808_set_temperature_threshold_hysteresis(&gs_handle, MCP9808_HYSTERESIS_0P0);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis 0C.\n");
    res = mcp9808_get_temperature_threshold_hysteresis(&gs_handle, &hysteresis);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature threshold hysteresis %s.\n", (hysteresis == MCP9808_HYSTERESIS_0P0) ? "ok" : "error");
    
    /* set temperature threshold hysteresis 1.5C */
    res = mcp9808_set_temperature_threshold_hysteresis(&gs_handle, MCP9808_HYSTERESIS_1P5);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis 1.5C.\n");
    res = mcp9808_get_temperature_threshold_hysteresis(&gs_handle, &hysteresis);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature threshold hysteresis %s.\n", (hysteresis == MCP9808_HYSTERESIS_1P5) ? "ok" : "error");
    
    /* set temperature threshold hysteresis 3.0C */
    res = mcp9808_set_temperature_threshold_hysteresis(&gs_handle, MCP9808_HYSTERESIS_3P0);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis 3.0C.\n");
    res = mcp9808_get_temperature_threshold_hysteresis(&gs_handle, &hysteresis);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature threshold hysteresis %s.\n", (hysteresis == MCP9808_HYSTERESIS_3P0) ? "ok" : "error");
    
    /* set temperature threshold hysteresis 6.0C */
    res = mcp9808_set_temperature_threshold_hysteresis(&gs_handle, MCP9808_HYSTERESIS_6P0);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature threshold hysteresis 6.0C.\n");
    res = mcp9808_get_temperature_threshold_hysteresis(&gs_handle, &hysteresis);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature threshold hysteresis failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature threshold hysteresis %s.\n", (hysteresis == MCP9808_HYSTERESIS_6P0) ? "ok" : "error");
    
    /* mcp9808_set_shutdown/mcp9808_get_shutdown test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_shutdown/mcp9808_get_shutdown test.\n");
    
    /* enable shutdown */
    res = mcp9808_set_shutdown(&gs_handle, MCP9808_BOOL_TRUE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set shutdown failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: enable shutdown.\n");
    res = mcp9808_get_shutdown(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get shutdown failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check shutdown %s.\n", (enable == MCP9808_BOOL_TRUE) ? "ok" : "error");
    
    /* disable shutdown */
    res = mcp9808_set_shutdown(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set shutdown failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: disable shutdown.\n");
    res = mcp9808_get_shutdown(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get shutdown failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check shutdown %s.\n", (enable == MCP9808_BOOL_FALSE) ? "ok" : "error");
    
    /* mcp9808_clear_interrupt test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_clear_interrupt test.\n");
    
    /* clear interrupt */
    res = mcp9808_clear_interrupt(&gs_handle);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: clear interrupt failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check clear interrupt %s.\n", (res == 0) ? "ok" : "error");
    
    /* mcp9808_get_alert_output_asserted test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_get_alert_output_asserted test.\n");
    
    /* mcp9808_get_alert_output_asserted */
    res = mcp9808_get_alert_output_asserted(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output asserted failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: alert output asserted is %s.\n", (enable == MCP9808_BOOL_TRUE) ? "true" : "false");
    
    /* mcp9808_set_alert_output/mcp9808_get_alert_output test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_alert_output/mcp9808_get_alert_output test.\n");
    
    /* enable alert output */
    res = mcp9808_set_alert_output(&gs_handle, MCP9808_BOOL_TRUE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: enable alert output.\n");
    res = mcp9808_get_alert_output(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output %s.\n", (enable == MCP9808_BOOL_TRUE) ? "ok" : "error");
    
    /* disable alert output */
    res = mcp9808_set_alert_output(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: disable alert output.\n");
    res = mcp9808_get_alert_output(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output %s.\n", (enable == MCP9808_BOOL_FALSE) ? "ok" : "error");
    
    /* mcp9808_set_alert_output_select/mcp9808_get_alert_output_select test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_alert_output_select/mcp9808_get_alert_output_select test.\n");
    
    /* set alert output select all */
    res = mcp9808_set_alert_output_select(&gs_handle, MCP9808_ALERT_OUTPUT_SELECT_ALL);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output select failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set alert output select all.\n");
    res = mcp9808_get_alert_output_select(&gs_handle, &output_select);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output select failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output select %s.\n", (output_select == MCP9808_ALERT_OUTPUT_SELECT_ALL) ? "ok" : "error");
    
    /* set alert output select only critical temperature */
    res = mcp9808_set_alert_output_select(&gs_handle, MCP9808_ALERT_OUTPUT_SELECT_ONLY_CRIT);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output select failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set alert output select only critical temperature.\n");
    res = mcp9808_get_alert_output_select(&gs_handle, &output_select);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output select failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output select %s.\n", (output_select == MCP9808_ALERT_OUTPUT_SELECT_ONLY_CRIT) ? "ok" : "error");
    
    /* mcp9808_set_alert_output_polarity/mcp9808_get_alert_output_polarity test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_alert_output_polarity/mcp9808_get_alert_output_polarity test.\n");
    
    /* set alert output polarity low */
    res = mcp9808_set_alert_output_polarity(&gs_handle, MCP9808_ALERT_OUTPUT_POLARITY_LOW);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output polarity failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set alert output polarity low.\n");
    res = mcp9808_get_alert_output_polarity(&gs_handle, &polarity);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output polarity failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output polarity %s.\n", (polarity == MCP9808_ALERT_OUTPUT_POLARITY_LOW) ? "ok" : "error");
    
    /* set alert output polarity high */
    res = mcp9808_set_alert_output_polarity(&gs_handle, MCP9808_ALERT_OUTPUT_POLARITY_HIGH);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output polarity failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set alert output polarity high.\n");
    res = mcp9808_get_alert_output_polarity(&gs_handle, &polarity);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output polarity failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output polarity %s.\n", (polarity == MCP9808_ALERT_OUTPUT_POLARITY_HIGH) ? "ok" : "error");
    
    /* mcp9808_set_alert_output_mode/mcp9808_get_alert_output_mode test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_alert_output_mode/mcp9808_get_alert_output_mode test.\n");
    
    /* set alert comparator output mode */
    res = mcp9808_set_alert_output_mode(&gs_handle, MCP9808_ALERT_OUTPUT_MODE_COMPARATOR);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output mode failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set alert comparator output mode.\n");
    res = mcp9808_get_alert_output_mode(&gs_handle, &mode);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output mode failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output mode %s.\n", (mode == MCP9808_ALERT_OUTPUT_MODE_COMPARATOR) ? "ok" : "error");
    
    /* set alert interrupt output mode */
    res = mcp9808_set_alert_output_mode(&gs_handle, MCP9808_ALERT_OUTPUT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set alert output mode failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set alert interrupt output mode.\n");
    res = mcp9808_get_alert_output_mode(&gs_handle, &mode);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get alert output mode failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check alert output mode %s.\n", (mode == MCP9808_ALERT_OUTPUT_MODE_INTERRUPT) ? "ok" : "error");
    
    /* mcp9808_set_temperature_high_threshold/mcp9808_get_temperature_high_threshold test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_temperature_high_threshold/mcp9808_get_temperature_high_threshold test.\n");
    
    reg = rand() % 0x07FFU;
    res = mcp9808_set_temperature_high_threshold(&gs_handle, reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature high threshold failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature high threshold %d.\n", reg);
    res = mcp9808_get_temperature_high_threshold(&gs_handle, &reg_check);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature high threshold failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature high threshold %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* mcp9808_set_temperature_low_threshold/mcp9808_get_temperature_low_threshold test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_temperature_low_threshold/mcp9808_get_temperature_low_threshold test.\n");
    
    reg = rand() % 0x07FFU;
    res = mcp9808_set_temperature_low_threshold(&gs_handle, reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature low threshold failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature low threshold %d.\n", reg);
    res = mcp9808_get_temperature_low_threshold(&gs_handle, &reg_check);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature low threshold failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature low threshold %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* mcp9808_set_critical_temperature/mcp9808_get_critical_temperature test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_critical_temperature/mcp9808_get_critical_temperature test.\n");
    
    reg = rand() % 0x07FFU;
    res = mcp9808_set_critical_temperature(&gs_handle, reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set critical temperature failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set critical temperature %d.\n", reg);
    res = mcp9808_get_critical_temperature(&gs_handle, &reg_check);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get critical temperature failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check critical temperature %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* mcp9808_get_device_revision test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_get_device_revision test.\n");
    
    /* get device revision */
    res = mcp9808_get_device_revision(&gs_handle, &revision);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get device revision failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: revision is 0x%02X.\n", revision);
    
    /* mcp9808_set_resolution/mcp9808_get_resolution test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_resolution/mcp9808_get_resolution test.\n");
    
    /* set resolution 0.5C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P5);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set resolution 0.5C.\n");
    res = mcp9808_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check resolution %s.\n", (resolution == MCP9808_RESOLUTION_0P5) ? "ok" : "error");
    
    /* set resolution 0.25C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P25);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set resolution 0.25C.\n");
    res = mcp9808_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check resolution %s.\n", (resolution == MCP9808_RESOLUTION_0P25) ? "ok" : "error");
    
    /* set resolution 0.125C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P125);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set resolution 0.125C.\n");
    res = mcp9808_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check resolution %s.\n", (resolution == MCP9808_RESOLUTION_0P125) ? "ok" : "error");
    
    /* set resolution 0.0625C */
    res = mcp9808_set_resolution(&gs_handle, MCP9808_RESOLUTION_0P0625);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set resolution 0.0625C.\n");
    res = mcp9808_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get resolution failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check resolution %s.\n", (resolution == MCP9808_RESOLUTION_0P0625) ? "ok" : "error");
    
    /* mcp9808_temperature_convert_to_register/mcp9808_temperature_convert_to_data test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_temperature_convert_to_register/mcp9808_temperature_convert_to_data test.\n");
    
    temperature_deg = (float)(rand() % 1000) / 100.0f;
    res = mcp9808_temperature_convert_to_register(&gs_handle, temperature_deg, &reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: temperature convert to register failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature %0.3fC.\n", temperature_deg);
    res = mcp9808_temperature_convert_to_data(&gs_handle, reg, &temperature_deg_check);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: temperature convert to data failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature %0.3fC.\n", temperature_deg_check);
    
    temperature_deg = -(float)(rand() % 1000) / 100.0f;
    res = mcp9808_temperature_convert_to_register(&gs_handle, temperature_deg, &reg);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: temperature convert to register failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: set temperature %0.3fC.\n", temperature_deg);
    res = mcp9808_temperature_convert_to_data(&gs_handle, reg, &temperature_deg_check);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: temperature convert to data failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature %0.3fC.\n", temperature_deg_check);
    
    /* mcp9808_set_critical_temperature_lock/mcp9808_get_critical_temperature_lock test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_critical_temperature_lock/mcp9808_get_critical_temperature_lock test.\n");
    
    /* disable critical temperature lock */
    res = mcp9808_set_critical_temperature_lock(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set critical temperature lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: disable critical temperature lock.\n");
    res = mcp9808_get_critical_temperature_lock(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get critical temperature lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check critical temperature lock %s.\n", (enable == MCP9808_BOOL_FALSE) ? "ok" : "error");
    
    /* enable critical temperature lock */
    res = mcp9808_set_critical_temperature_lock(&gs_handle, MCP9808_BOOL_TRUE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set critical temperature lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: enable critical temperature lock.\n");
    res = mcp9808_get_critical_temperature_lock(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get critical temperature lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check critical temperature lock %s.\n", (enable == MCP9808_BOOL_TRUE) ? "ok" : "error");
    
    /* mcp9808_set_temperature_threshold_lock/mcp9808_get_temperature_threshold_lock test */
    mcp9808_interface_debug_print("mcp9808: mcp9808_set_temperature_threshold_lock/mcp9808_get_temperature_threshold_lock test.\n");
    
    /* disable temperature threshold lock */
    res = mcp9808_set_temperature_threshold_lock(&gs_handle, MCP9808_BOOL_FALSE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: disable temperature threshold lock.\n");
    res = mcp9808_get_temperature_threshold_lock(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature threshold lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature threshold lock %s.\n", (enable == MCP9808_BOOL_FALSE) ? "ok" : "error");
    
    /* enable temperature threshold lock */
    res = mcp9808_set_temperature_threshold_lock(&gs_handle, MCP9808_BOOL_TRUE);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: set temperature threshold lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: enable temperature threshold lock.\n");
    res = mcp9808_get_temperature_threshold_lock(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9808_interface_debug_print("mcp9808: get temperature threshold lock failed.\n");
        (void)mcp9808_deinit(&gs_handle);
        
        return 1;
    }
    mcp9808_interface_debug_print("mcp9808: check temperature threshold lock %s.\n", (enable == MCP9808_BOOL_TRUE) ? "ok" : "error");
    
    /* output */
    mcp9808_interface_debug_print("mcp9808: if you run the register test, please power up again.\n");
    
    /* finish register */
    mcp9808_interface_debug_print("mcp9808: finish register test.\n");
    (void)mcp9808_deinit(&gs_handle);
    
    return 0;
}
