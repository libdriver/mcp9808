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
 * @file      driver_mcp9808.h
 * @brief     driver mcp9808 header file
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

#ifndef DRIVER_MCP9808_H
#define DRIVER_MCP9808_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup mcp9808_driver mcp9808 driver function
 * @brief    mcp9808 driver modules
 * @{
 */

/**
 * @addtogroup mcp9808_base_driver
 * @{
 */

/**
 * @brief mcp9808 address enumeration definition
 */
typedef enum
{
    MCP9808_ADDRESS_A2A1A0_000 = (0x18 << 1),        /**< address 0 */
    MCP9808_ADDRESS_A2A1A0_001 = (0x19 << 1),        /**< address 1 */
    MCP9808_ADDRESS_A2A1A0_010 = (0x1A << 1),        /**< address 2 */
    MCP9808_ADDRESS_A2A1A0_011 = (0x1B << 1),        /**< address 3 */
    MCP9808_ADDRESS_A2A1A0_100 = (0x1C << 1),        /**< address 4 */
    MCP9808_ADDRESS_A2A1A0_101 = (0x1D << 1),        /**< address 5 */
    MCP9808_ADDRESS_A2A1A0_110 = (0x1E << 1),        /**< address 6 */
    MCP9808_ADDRESS_A2A1A0_111 = (0x1F << 1),        /**< address 7 */
} mcp9808_address_t;

/**
 * @brief mcp9808 bool enumeration definition
 */
typedef enum
{
    MCP9808_BOOL_FALSE = 0x00,        /**< false */
    MCP9808_BOOL_TRUE  = 0x01,        /**< true */
} mcp9808_bool_t;

/**
 * @brief mcp9808 hysteresis enumeration definition
 */
typedef enum
{
    MCP9808_HYSTERESIS_0P0 = 0x00,        /**< 0.0C */
    MCP9808_HYSTERESIS_1P5 = 0x01,        /**< +1.5C */
    MCP9808_HYSTERESIS_3P0 = 0x02,        /**< +3.0C */
    MCP9808_HYSTERESIS_6P0 = 0x03,        /**< +6.0C */
} mcp9808_hysteresis_t;

/**
 * @brief mcp9808 status enumeration definition
 */
typedef enum
{
    MCP9808_STATUS_CRIT    = 0x00,        /**< critical temperature boundary */
    MCP9808_STATUS_T_UPPER = 0x01,        /**< temperature upper boundary */
    MCP9808_STATUS_T_LOWER = 0x02,        /**< temperature lower boundary */
} mcp9808_status_t;

/**
 * @brief mcp9808 resolution enumeration definition
 */
typedef enum
{
    MCP9808_RESOLUTION_0P5    = 0x00,        /**< 0.5C */
    MCP9808_RESOLUTION_0P25   = 0x01,        /**< 0.25C */
    MCP9808_RESOLUTION_0P125  = 0x02,        /**< 0.125C */
    MCP9808_RESOLUTION_0P0625 = 0x03,        /**< 0.0625C */
} mcp9808_resolution_t;

/**
 * @brief mcp9808 alert output select enumeration definition
 */
typedef enum
{
    MCP9808_ALERT_OUTPUT_SELECT_ALL       = 0x00,        /**< all */
    MCP9808_ALERT_OUTPUT_SELECT_ONLY_CRIT = 0x01,        /**< only critical temperature */
} mcp9808_alert_output_select_t;

/**
 * @brief mcp9808 alert output polarity enumeration definition
 */
typedef enum
{
    MCP9808_ALERT_OUTPUT_POLARITY_LOW  = 0x00,        /**< active low */
    MCP9808_ALERT_OUTPUT_POLARITY_HIGH = 0x01,        /**< active high */
} mcp9808_alert_output_polarity_t;

/**
 * @brief mcp9808 alert output mode enumeration definition
 */
typedef enum
{
    MCP9808_ALERT_OUTPUT_MODE_COMPARATOR = 0x00,        /**< comparator output mode */
    MCP9808_ALERT_OUTPUT_MODE_INTERRUPT  = 0x01,        /**< interrupt output mode */
} mcp9808_alert_output_mode_t;

/**
 * @brief mcp9808 handle structure definition
 */
typedef struct mcp9808_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    uint8_t iic_addr;                                                                   /**< iic device address */
    uint8_t inited;                                                                     /**< inited flag */
} mcp9808_handle_t;

/**
 * @brief mcp9808 information structure definition
 */
typedef struct mcp9808_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} mcp9808_info_t;

/**
 * @}
 */

/**
 * @defgroup mcp9808_link_driver mcp9808 link driver function
 * @brief    mcp9808 link driver modules
 * @ingroup  mcp9808_driver
 * @{
 */

/**
 * @brief     initialize mcp9808_handle_t structure
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] STRUCTURE mcp9808_handle_t
 * @note      none
 */
#define DRIVER_MCP9808_LINK_INIT(HANDLE, STRUCTURE)               memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_MCP9808_LINK_IIC_INIT(HANDLE, FUC)                (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_MCP9808_LINK_IIC_DEINIT(HANDLE, FUC)              (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_MCP9808_LINK_IIC_READ(HANDLE, FUC)                (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_MCP9808_LINK_IIC_WRITE(HANDLE, FUC)               (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_MCP9808_LINK_DELAY_MS(HANDLE, FUC)                (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_MCP9808_LINK_DEBUG_PRINT(HANDLE, FUC)             (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to a mcp9808 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_MCP9808_LINK_RECEIVE_CALLBACK(HANDLE, FUC)        (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup mcp9808_base_driver mcp9808 base driver function
 * @brief    mcp9808 base driver modules
 * @ingroup  mcp9808_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a mcp9808 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mcp9808_info(mcp9808_info_t *info);

/**
 * @brief     set the iic address
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] addr chip iic address
 * @return    status code
 *            - 0 success
 *            - 1 set addr failed
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mcp9808_set_addr(mcp9808_handle_t *handle, mcp9808_address_t addr);

/**
 * @brief      get the iic address
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *addr pointer to a chip iic address buffer
 * @return      status code
 *              - 0 success
 *              - 1 get addr failed
 *              - 2 handle is NULL
 * @note        none
 */
uint8_t mcp9808_get_addr(mcp9808_handle_t *handle, mcp9808_address_t *addr);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_irq_handler(mcp9808_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 read id failed
 *            - 5 id is invalid
 * @note      none
 */
uint8_t mcp9808_init(mcp9808_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 shut down failed
 * @note      none
 */
uint8_t mcp9808_deinit(mcp9808_handle_t *handle);

/**
 * @brief      read temperature
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *raw pointer to a raw data buffer
 * @param[out] *temperature_deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_read(mcp9808_handle_t *handle, int16_t *raw, float *temperature_deg);

/**
 * @brief     set temperature threshold hysteresis
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] hysteresis input hysteresis
 * @return    status code
 *            - 0 success
 *            - 1 set temperature threshold hysteresis failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_temperature_threshold_hysteresis(mcp9808_handle_t *handle, mcp9808_hysteresis_t hysteresis);

/**
 * @brief      get temperature threshold hysteresis
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *hysteresis pointer to a hysteresis buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature threshold hysteresis failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_temperature_threshold_hysteresis(mcp9808_handle_t *handle, mcp9808_hysteresis_t *hysteresis);

/**
 * @brief     enable or disable shutdown
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set shutdown failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_shutdown(mcp9808_handle_t *handle, mcp9808_bool_t enable);

/**
 * @brief      get shutdown status
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shutdown failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_shutdown(mcp9808_handle_t *handle, mcp9808_bool_t *enable);

/**
 * @brief     enable or disable critical temperature lock
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set critical temperature lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_critical_temperature_lock(mcp9808_handle_t *handle, mcp9808_bool_t enable);

/**
 * @brief      get critical temperature lock status
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get critical temperature lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_critical_temperature_lock(mcp9808_handle_t *handle, mcp9808_bool_t *enable);

/**
 * @brief     enable or disable temperature threshold lock
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set temperature threshold lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_temperature_threshold_lock(mcp9808_handle_t *handle, mcp9808_bool_t enable);

/**
 * @brief      get temperature threshold lock status
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature threshold lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_temperature_threshold_lock(mcp9808_handle_t *handle, mcp9808_bool_t *enable);

/**
 * @brief     clear interrupt
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_clear_interrupt(mcp9808_handle_t *handle);

/**
 * @brief      get alert output asserted status
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert output asserted failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_alert_output_asserted(mcp9808_handle_t *handle, mcp9808_bool_t *enable);

/**
 * @brief     enable or disable alert output
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set alert output failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_alert_output(mcp9808_handle_t *handle, mcp9808_bool_t enable);

/**
 * @brief      get alert output status
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert output failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_alert_output(mcp9808_handle_t *handle, mcp9808_bool_t *enable);

/**
 * @brief     set alert output select
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] select_output alert output select
 * @return    status code
 *            - 0 success
 *            - 1 set alert output select failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_alert_output_select(mcp9808_handle_t *handle, mcp9808_alert_output_select_t select_output);

/**
 * @brief      get alert output select
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *select_output pointer to an alert output select buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert output select failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_alert_output_select(mcp9808_handle_t *handle, mcp9808_alert_output_select_t *select_output);

/**
 * @brief     set alert output polarity
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] polarity alert output polarity
 * @return    status code
 *            - 0 success
 *            - 1 set alert output polarity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_alert_output_polarity(mcp9808_handle_t *handle, mcp9808_alert_output_polarity_t polarity);

/**
 * @brief      get alert output polarity
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *polarity pointer to an alert output polarity buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert output polarity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_alert_output_polarity(mcp9808_handle_t *handle, mcp9808_alert_output_polarity_t *polarity);

/**
 * @brief     set alert output mode
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] mode alert output mode
 * @return    status code
 *            - 0 success
 *            - 1 set alert output mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_alert_output_mode(mcp9808_handle_t *handle, mcp9808_alert_output_mode_t mode);

/**
 * @brief      get alert output mode
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *mode pointer to an alert output mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert output mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_alert_output_mode(mcp9808_handle_t *handle, mcp9808_alert_output_mode_t *mode);

/**
 * @brief     set temperature high threshold
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] raw raw data
 * @return    status code
 *            - 0 success
 *            - 1 set temperature high threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 raw > 0x7FF
 * @note      none
 */
uint8_t mcp9808_set_temperature_high_threshold(mcp9808_handle_t *handle, uint16_t raw);

/**
 * @brief      get temperature high threshold
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *raw pointer to a raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature high threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_temperature_high_threshold(mcp9808_handle_t *handle, uint16_t *raw);

/**
 * @brief     set temperature low threshold
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] raw raw data
 * @return    status code
 *            - 0 success
 *            - 1 set temperature low threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 raw > 0x7FF
 * @note      none
 */
uint8_t mcp9808_set_temperature_low_threshold(mcp9808_handle_t *handle, uint16_t raw);

/**
 * @brief      get temperature low threshold
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *raw pointer to a raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature low threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_temperature_low_threshold(mcp9808_handle_t *handle, uint16_t *raw);

/**
 * @brief     set critical temperature
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] raw raw data
 * @return    status code
 *            - 0 success
 *            - 1 set critical temperature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 raw > 0x7FF
 * @note      none
 */
uint8_t mcp9808_set_critical_temperature(mcp9808_handle_t *handle, uint16_t raw);

/**
 * @brief      get critical temperature
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *raw pointer to a raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get critical temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_critical_temperature(mcp9808_handle_t *handle, uint16_t *raw);

/**
 * @brief      get device revision
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *revision pointer to a revision buffer
 * @return     status code
 *             - 0 success
 *             - 1 get device revision failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_device_revision(mcp9808_handle_t *handle, uint8_t *revision);

/**
 * @brief     set resolution
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] resolution temperature resolution
 * @return    status code
 *            - 0 success
 *            - 1 set resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_resolution(mcp9808_handle_t *handle, mcp9808_resolution_t resolution);

/**
 * @brief      get resolution
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[out] *resolution pointer to a temperature resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 get resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_resolution(mcp9808_handle_t *handle, mcp9808_resolution_t *resolution);

/**
 * @brief      convert the temperature to the register raw data
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[in]  temperature_deg temperature in degrees celsius
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_temperature_convert_to_register(mcp9808_handle_t *handle, float temperature_deg, uint16_t *reg);

/**
 * @brief      convert the register raw data to the temperature
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[in]  reg register raw data
 * @param[out] *temperature_deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_temperature_convert_to_data(mcp9808_handle_t *handle, uint16_t reg, float *temperature_deg);

/**
 * @}
 */

/**
 * @defgroup mcp9808_extend_driver mcp9808 extend driver function
 * @brief    mcp9808 extend driver modules
 * @ingroup  mcp9808_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9808_set_reg(mcp9808_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9808_get_reg(mcp9808_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
