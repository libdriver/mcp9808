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
 * @file      driver_mcp9808.c
 * @brief     driver mcp9808 source file
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

#include "driver_mcp9808.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Microchip MCP9808"        /**< chip name */
#define MANUFACTURER_NAME         "Microchip"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.7f                       /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                       /**< chip max supply voltage */
#define MAX_CURRENT               0.4f                       /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                     /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                     /**< chip max operating temperature */
#define DRIVER_VERSION            1000                       /**< driver version */

/**
 * @brief chip reg definition
 */
#define MCP9808_REG_CONFIG                 0x01        /**< config register */
#define MCP9808_REG_ALERT_UPPER            0x02        /**< alert temperature upper boundary trip register */
#define MCP9808_REG_ALERT_LOWER            0x03        /**< alert temperature lower boundary trip register */
#define MCP9808_REG_CRIT                   0x04        /**< critical temperature trip register */
#define MCP9808_REG_TEMPERATURE            0x05        /**< temperature register */
#define MCP9808_REG_MANUFACTURER_ID        0x06        /**< manufacturer id register */
#define MCP9808_REG_DEVICE_ID              0x07        /**< device id register */
#define MCP9808_REG_RESOLUTION             0x08        /**< resolution register */

/**
 * @brief      read bytes
 * @param[in]  *handle pointer to a mcp9808 handle structure
 * @param[in]  reg iic register address
 * @param[out] *data pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_mcp9808_iic_read(mcp9808_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (handle->iic_read(handle->iic_addr, reg, data, len) != 0)        /* read the register */
    {
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle pointer to a mcp9808 handle structure
 * @param[in] reg iic register address
 * @param[in] *data pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_mcp9808_iic_write(mcp9808_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (handle->iic_write(handle->iic_addr, reg, data, len) != 0)        /* write the register */
    {
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t mcp9808_irq_handler(mcp9808_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_TEMPERATURE, buf, 2);        /* get temperature */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("mcp9808: get temperature failed.\n");            /* get temperature failed */
        
        return 1;                                                             /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                        /* set raw data */
    if ((prev & (1 << 15)) != 0)                                              /* check critical temperature bit */
    {
        if (handle->receive_callback != NULL)                                 /* not null */
        {
            handle->receive_callback((uint8_t)MCP9808_STATUS_CRIT);           /* run the callback */
        }
    }
    if ((prev & (1 << 14)) != 0)                                              /* check temperature upper bit */
    {
        if (handle->receive_callback != NULL)                                 /* not null */
        {
            handle->receive_callback((uint8_t)MCP9808_STATUS_T_UPPER);        /* run the callback */
        }
    }
    if ((prev & (1 << 13)) != 0)                                              /* check temperature lower bit */
    {
        if (handle->receive_callback != NULL)                                 /* not null */
        {
            handle->receive_callback((uint8_t)MCP9808_STATUS_T_LOWER);        /* run the callback */
        }
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);             /* get config */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");                 /* get config failed */
        
        return 1;                                                             /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                        /* set raw data */
    prev &= ~(1 << 5);                                                        /* clear settings */
    prev |= 1 << 5;                                                           /* set enable */
    buf[0] = (prev >> 8) & 0xFF;                                              /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                              /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);            /* set config */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");                 /* set config failed */
        
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t mcp9808_init(mcp9808_handle_t *handle)
{
    uint8_t res;
    uint16_t id;
    uint8_t buf[2];
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->debug_print == NULL)                                              /* check debug_print */
    {
        return 3;                                                                 /* return error */
    }
    if (handle->iic_init == NULL)                                                 /* check iic_init */
    {
        handle->debug_print("mcp9808: iic_init is null.\n");                      /* iic_init is null */
       
        return 3;                                                                 /* return error */
    }
    if (handle->iic_deinit == NULL)                                               /* check iic_deinit */
    {
        handle->debug_print("mcp9808: iic_deinit is null.\n");                    /* iic_deinit is null */
       
        return 3;                                                                 /* return error */
    }
    if (handle->iic_read == NULL)                                                 /* check iic_read */
    {
        handle->debug_print("mcp9808: iic_read is null.\n");                      /* iic_read is null */
       
        return 3;                                                                 /* return error */
    }
    if (handle->iic_write == NULL)                                                /* check iic_write */
    {
        handle->debug_print("mcp9808: iic_write is null.\n");                     /* iic_write is null */
       
        return 3;                                                                 /* return error */
    }
    if (handle->delay_ms == NULL)                                                 /* check delay_ms */
    {
        handle->debug_print("mcp9808: delay_ms is null.\n");                      /* delay_ms is null */
       
        return 3;                                                                 /* return error */
    }
    if (handle->receive_callback == NULL)                                         /* check receive_callback */
    {
        handle->debug_print("mcp9808: receive_callback is null.\n");              /* receive_callback is null */
       
        return 3;                                                                 /* return error */
    }
    
    if (handle->iic_init() != 0)                                                  /* iic init */
    {
        handle->debug_print("mcp9808: iic init failed.\n");                       /* iic init failed */
       
        return 1;                                                                 /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_MANUFACTURER_ID, buf, 2);        /* get manufacturer id */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("mcp9808: get manufacturer id failed.\n");            /* get manufacturer id failed */
        (void)handle->iic_deinit();                                               /* iic deinit */
        
        return 4;                                                                 /* return error */
    }
    id = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                              /* set id */
    if (id != 0x0054U)                                                            /* check id */
    {
        handle->debug_print("mcp9808: manufacturer id is invalid.\n");            /* manufacturer id is invalid */
        (void)handle->iic_deinit();                                               /* iic deinit */
        
        return 5;                                                                 /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_DEVICE_ID, buf, 2);              /* get device id */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("mcp9808: get device id failed.\n");                  /* get device id failed */
        (void)handle->iic_deinit();                                               /* iic deinit */
        
        return 4;                                                                 /* return error */
    }
    id = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                              /* set id */
    if ((id & 0xFF00U) != 0x0400U)                                                /* check id */
    {
        handle->debug_print("mcp9808: device id is invalid.\n");                  /* device id is invalid */
        (void)handle->iic_deinit();                                               /* iic deinit */
        
        return 5;                                                                 /* return error */
    }
    
    handle->inited = 1;                                                           /* flag finish initialization */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t mcp9808_deinit(mcp9808_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 4;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 8);                                                    /* clear settings */
    prev |= 1 << 8;                                                       /* set bool */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 4;                                                         /* return error */
    }
    if (handle->iic_deinit() != 0)                                        /* iic deinit */
    {
        handle->debug_print("mcp9808: iic deinit failed.\n");             /* iic deinit failed */
       
        return 1;                                                         /* return error */
    }
    handle->inited = 0;                                                   /* flag close */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_addr(mcp9808_handle_t *handle, mcp9808_address_t addr)
{
    if (handle == NULL)                      /* check handle */
    {
        return 2;                            /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr;        /* set address */
    
    return 0;                                /* success return 0 */
}

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
uint8_t mcp9808_get_addr(mcp9808_handle_t *handle, mcp9808_address_t *addr)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    
    *addr = (mcp9808_address_t)(handle->iic_addr);        /* get address */
    
    return 0;                                           /* success return 0 */
}

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
uint8_t mcp9808_temperature_convert_to_register(mcp9808_handle_t *handle, float temperature_deg, uint16_t *reg)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    if (temperature_deg > 0.0f)                                        /* > 0 */
    {
        *reg = (uint16_t)(temperature_deg * 4.0f);                     /* convert real data to register data */
    }
    else                                                               /* < 0 */
    {
        *reg = (uint16_t)(-temperature_deg * 4.0f) | (1 << 10);        /* convert real data to register data */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t mcp9808_temperature_convert_to_data(mcp9808_handle_t *handle, uint16_t reg, float *temperature_deg)
{
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    }
    
    if ((reg & (1 << 10)) != 0)                          /* check signed bit */
    {
        reg &= ~(1 << 10);                               /* clear sign bit */
        *temperature_deg = -(float)(reg) * 0.25f;        /* convert raw data to real data */
    }
    else
    {
        *temperature_deg = (float)(reg) * 0.25f;         /* convert raw data to real data */
    }
    
    return 0;                                            /* success return 0 */
}

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
uint8_t mcp9808_set_temperature_high_threshold(mcp9808_handle_t *handle, uint16_t raw)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (raw > 0x7FF)                                                                     /* check result */
    {
        handle->debug_print("mcp9808: raw > 0x7FF.\n");                                  /* raw > 0x7FF */
        
        return 4;                                                                        /* return error */
    }
    
    prev = (raw & 0x7FF) << 2;                                                           /* set data */
    buf[0] = (prev >> 8) & 0xFF;                                                         /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                                         /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_ALERT_UPPER, buf, 2);                  /* set threshold */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mcp9808: set temperature high threshold failed.\n");        /* set temperature high threshold failed */
        
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mcp9808_get_temperature_high_threshold(mcp9808_handle_t *handle, uint16_t *raw)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_ALERT_UPPER, buf, 2);                   /* get threshold */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mcp9808: get temperature high threshold failed.\n");        /* get temperature high threshold failed */
        
        return 1;                                                                        /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                                   /* set raw data */
    *raw = (prev >> 2) & 0x7FF;                                                          /* set output data */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mcp9808_set_temperature_low_threshold(mcp9808_handle_t *handle, uint16_t raw)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (raw > 0x7FF)                                                                     /* check result */
    {
        handle->debug_print("mcp9808: raw > 0x7FF.\n");                                  /* raw > 0x7FF */
        
        return 4;                                                                        /* return error */
    }
    
    prev = (raw & 0x7FF) << 2;                                                           /* set data */
    buf[0] = (prev >> 8) & 0xFF;                                                         /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                                         /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_ALERT_LOWER, buf, 2);                  /* set threshold */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mcp9808: set temperature low threshold failed.\n");         /* set temperature low threshold failed */
        
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mcp9808_get_temperature_low_threshold(mcp9808_handle_t *handle, uint16_t *raw)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_ALERT_LOWER, buf, 2);                   /* get threshold */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mcp9808: get temperature low threshold failed.\n");         /* get temperature low threshold failed */
        
        return 1;                                                                        /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                                   /* set raw data */
    *raw = (prev >> 2) & 0x7FF;                                                          /* set output data */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mcp9808_set_critical_temperature(mcp9808_handle_t *handle, uint16_t raw)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (raw > 0x7FF)                                                               /* check result */
    {
        handle->debug_print("mcp9808: raw > 0x7FF.\n");                            /* raw > 0x7FF */
        
        return 4;                                                                  /* return error */
    }
    
    prev = (raw & 0x7FF) << 2;                                                     /* set data */
    buf[0] = (prev >> 8) & 0xFF;                                                   /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                                   /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CRIT, buf, 2);                   /* set temperature */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mcp9808: set critical temperature failed.\n");        /* set critical temperature failed */
        
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t mcp9808_get_critical_temperature(mcp9808_handle_t *handle, uint16_t *raw)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CRIT, buf, 2);                    /* get temperature */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mcp9808: get critical temperature failed.\n");        /* get critical temperature failed */
        
        return 1;                                                                  /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                             /* set raw data */
    *raw = (prev >> 2) & 0x7FF;                                                    /* set output data */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t mcp9808_read(mcp9808_handle_t *handle, int16_t *raw, float *temperature_deg)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_TEMPERATURE, buf, 2);        /* get temperature */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("mcp9808: get temperature failed.\n");            /* get temperature failed */
        
        return 1;                                                             /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                        /* set raw data */
    prev &= ~(7 << 13);                                                       /* clear flags */
    if ((prev & (1 << 12)) != 0)                                              /* check signed bit */
    {
        *raw = (int16_t)((uint16_t)(prev | (uint16_t)(0x7 << 13)));           /* save data and set signed bits */
    }
    else
    {
        *raw = (int16_t)prev;                                                 /* save data */
    }
    *temperature_deg = (float)(*raw) * 0.0625f;                               /* convert temperature */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t mcp9808_get_device_revision(mcp9808_handle_t *handle, uint8_t *revision)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_DEVICE_ID, buf, 2);        /* get device revision */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("mcp9808: get device revision failed.\n");      /* get device revision failed */
        
        return 1;                                                           /* return error */
    }
    *revision = buf[1];                                                     /* set revision */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t mcp9808_set_resolution(mcp9808_handle_t *handle, mcp9808_resolution_t resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_RESOLUTION, &prev, 1);        /* get resolution */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9808: get resolution failed.\n");              /* get resolution failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(3 << 0);                                                         /* clear settings */
    prev |= resolution << 0;                                                   /* set resolution */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_RESOLUTION, &prev, 1);       /* set resolution */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9808: set resolution failed.\n");              /* set resolution failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9808_get_resolution(mcp9808_handle_t *handle, mcp9808_resolution_t *resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_RESOLUTION, &prev, 1);        /* get resolution */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9808: get resolution failed.\n");              /* get resolution failed */
        
        return 1;                                                              /* return error */
    }
    *resolution = (mcp9808_resolution_t)((prev >> 0) & 0x03);                  /* set resolution */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9808_set_temperature_threshold_hysteresis(mcp9808_handle_t *handle, mcp9808_hysteresis_t hysteresis)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(3 << 9);                                                    /* clear settings */
    prev |= hysteresis << 9;                                              /* set hysteresis */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_temperature_threshold_hysteresis(mcp9808_handle_t *handle, mcp9808_hysteresis_t *hysteresis)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *hysteresis = (mcp9808_hysteresis_t)((prev >> 9) & 0x03);             /* set hysteresis */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_shutdown(mcp9808_handle_t *handle, mcp9808_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 8);                                                    /* clear settings */
    prev |= enable << 8;                                                  /* set bool */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    handle->delay_ms(10);                                                 /* delay 10ms */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_shutdown(mcp9808_handle_t *handle, mcp9808_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *enable = (mcp9808_bool_t)((prev >> 8) & 0x01);                       /* set bool */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_critical_temperature_lock(mcp9808_handle_t *handle, mcp9808_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 7);                                                    /* clear settings */
    prev |= enable << 7;                                                  /* set bool */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_critical_temperature_lock(mcp9808_handle_t *handle, mcp9808_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *enable = (mcp9808_bool_t)((prev >> 7) & 0x01);                       /* set bool */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_temperature_threshold_lock(mcp9808_handle_t *handle, mcp9808_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 6);                                                    /* clear settings */
    prev |= enable << 6;                                                  /* set bool */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_temperature_threshold_lock(mcp9808_handle_t *handle, mcp9808_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *enable = (mcp9808_bool_t)((prev >> 6) & 0x01);                       /* set bool */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_clear_interrupt(mcp9808_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 5);                                                    /* clear settings */
    prev |= 1 << 5;                                                       /* set enable */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_alert_output_asserted(mcp9808_handle_t *handle, mcp9808_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *enable = (mcp9808_bool_t)((prev >> 4) & 0x01);                       /* set bool */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_alert_output(mcp9808_handle_t *handle, mcp9808_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 3);                                                    /* clear settings */
    prev |= enable << 3;                                                  /* set bool */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_alert_output(mcp9808_handle_t *handle, mcp9808_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *enable = (mcp9808_bool_t)((prev >> 3) & 0x01);                       /* set bool */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_alert_output_select(mcp9808_handle_t *handle, mcp9808_alert_output_select_t select_output)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 2);                                                    /* clear settings */
    prev |= select_output << 2;                                           /* set select */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_alert_output_select(mcp9808_handle_t *handle, mcp9808_alert_output_select_t *select_output)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);                /* get config */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");                    /* get config failed */
        
        return 1;                                                                /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                           /* set raw data */
    *select_output = (mcp9808_alert_output_select_t)((prev >> 2) & 0x01);        /* set select */
    
    return 0;                                                                    /* success return 0 */
}

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
uint8_t mcp9808_set_alert_output_polarity(mcp9808_handle_t *handle, mcp9808_alert_output_polarity_t polarity)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 1);                                                    /* clear settings */
    prev |= polarity << 1;                                                /* set polarity */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_alert_output_polarity(mcp9808_handle_t *handle, mcp9808_alert_output_polarity_t *polarity)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *polarity = (mcp9808_alert_output_polarity_t)((prev >> 1) & 0x01);    /* set polarity */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_alert_output_mode(mcp9808_handle_t *handle, mcp9808_alert_output_mode_t mode)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    prev &= ~(1 << 0);                                                    /* clear settings */
    prev |= mode << 0;                                                    /* set mode */
    buf[0] = (prev >> 8) & 0xFF;                                          /* set msb */
    buf[1] = (prev >> 0) & 0xFF;                                          /* set lsb */
    res = a_mcp9808_iic_write(handle, MCP9808_REG_CONFIG, buf, 2);        /* set config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: set config failed.\n");             /* set config failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_get_alert_output_mode(mcp9808_handle_t *handle, mcp9808_alert_output_mode_t *mode)
{
    uint8_t res;
    uint16_t prev;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, MCP9808_REG_CONFIG, buf, 2);         /* get config */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9808: get config failed.\n");             /* get config failed */
        
        return 1;                                                         /* return error */
    }
    prev = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                    /* set raw data */
    *mode = (mcp9808_alert_output_mode_t)((prev >> 0) & 0x01);            /* set mode */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9808_set_reg(mcp9808_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }
    
    res = a_mcp9808_iic_write(handle, reg, buf, len);           /* write data */
    if (res != 0)                                               /* check result */
    {
        handle->debug_print("mcp9808: write failed.\n");        /* write failed */
        
        return 1;                                               /* return error */
    }
    
    return 0;                                                   /* success return 0 */
}

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
uint8_t mcp9808_get_reg(mcp9808_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    if (handle->inited != 1)                                   /* check handle initialization */
    {
        return 3;                                              /* return error */
    }
    
    res = a_mcp9808_iic_read(handle, reg, buf, len);           /* read data */
    if (res != 0)                                              /* check result */
    {
        handle->debug_print("mcp9808: read failed.\n");        /* read failed */
        
        return 1;                                              /* return error */
    }
    
    return 0;                                                  /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a mcp9808 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mcp9808_info(mcp9808_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(mcp9808_info_t));                        /* initialize mcp9808 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
