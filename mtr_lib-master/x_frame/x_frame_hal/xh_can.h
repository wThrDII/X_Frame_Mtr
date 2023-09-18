/**
 * @brief can 驱动接口
 * @file xh_can.h
 * @version 1.0
 * @author juntong.chen
 * @date 2022-07-13 14:08:17
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-13 14:08:17 <td>1.0       <td>juntong.chen      <td>Created function
 */
#ifndef xh_can_h
#define xh_can_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef bool (*xh_can_write_obj)(void *var, uint32_t pack_type, uint32_t id, uint8_t *p_pack, uint32_t pack_len);
    typedef void (*xh_can_irq_cb_obj)(uint32_t pack_type, uint32_t id, uint8_t *p_pack, uint32_t pack_len);
    typedef bool (*xh_can_reg_rx_irq_cb_obj)(void *var, xh_can_irq_cb_obj irq_cb);
    typedef void (*xh_can_set_baudrate_obj)(void *var, uint32_t baudrate);

    typedef struct _xh_can_t
    {
        xh_comm_t core;
        xh_can_write_obj write;
        xh_can_reg_rx_irq_cb_obj reg_rx_irq_cb;
        xh_can_set_baudrate_obj set_baudrate;
    } xh_can_t, *p_xh_can_t;

#define REG_XH_CAN_DRV(id, var, prio, auto_init, init, deinit, write, reg_rx_irq_cb, set_baudrate) \
    REG_XH_DEV(xh_can_t, id, var, prio, auto_init, init, deinit,                                   \
               (xh_can_write_obj)write,                                                            \
               (xh_can_reg_rx_irq_cb_obj)reg_rx_irq_cb,                                            \
               (xh_can_set_baudrate_obj)set_baudrate, )

#define EXT_XH_CAN_DRV(id) EXT_XH_DEV(xh_can_t, id)

    /**
     * @brief can写数据
     *
     * @param [in] id               设备ID
     * @param [in] pack_type        数据包类型
     *                              0：标准帧
     *                              1：扩展帧
     * @param [in] pack_id          can id
     * @param [in] p_pack           数据地址
     * @param [in] pack_len         数据长度
     *
     * @return true
     *  @retval 成功
     * @return false
     *  @retval 失败
     * @details 特殊说明:
     * @par eg:以标准帧的方式发送8个数据 id为2
     * @code
     * uint8_t res;
     * uint8_t tx_data[8] = {0xAA,1,2,3,4,5,6,7};
     * res = xh_can_write(DRV_CAN0,0, 2, tx_data, 8);
     * @endcode
     */
    static inline bool xh_can_write(void *const id, uint32_t pack_type, uint32_t pack_id, uint8_t *p_pack, uint32_t pack_len)
    {
        xh_can_t *p = (xh_can_t *)id;
        return p->write(p->core.var, pack_type, pack_id, p_pack, pack_len);
    }
    /**
     * @brief 注册回调函数
     *
     * @param [in] id               设备ID
     * @param [in] irq_cb           回调函数
     *
     * @return true
     *  @retval 成功
     * @return false
     *  @retval 失败
     * @details 特殊说明:
     * 回调函数类型：
     * typedef void (*xh_can_irq_cb_obj)(uint32_t pack_type, uint32_t id, uint8_t *p_pack, uint32_t pack_len);
     * pack_type：数据包类型
     *      0：标准帧
     *      1：数据帧
     * id：接收到数据的can id
     * p_pack:接收到的数据指针
     * pack_len：接收到数据长度
     * @par eg: 注册一个can回调函数
     * @code
     * void can_test_cb_id(uint32_t pack_type, uint32_t id, uint8_t *p_pack, uint32_t pack_len)
     * {
     *     return 0;
     * }
     * xh_can_reg_rx_irq_cb(DRV_CAN0,can_test_cb_id);
     * @endcode
     */
    static inline bool xh_can_reg_rx_irq_cb(void *const id, xh_can_irq_cb_obj irq_cb)
    {
        xh_can_t *p = (xh_can_t *)id;
        return p->reg_rx_irq_cb(p->core.var, irq_cb);
    }

    /**
     * @brief 设置can波特率
     *
     * @param [in] id            驱动id
     * @param [in] baudrate      需要设置的波特率
     *
     *
     * @details
     */

    /**
     * @brief 设置can波特率
     *
     * @param [in] id               设备ID
     * @param [in] baudrate         波特率
     *
     * @details 特殊说明:
     * 波特率可选 1000000/500000/250000/100000
     * @par eg:将can的波特率改为500KHZ
     * @code
     *  xh_can_set_baudrate(DRV_CAN0,500000);
     * @endcode
     */
    static inline void xh_can_set_baudrate(void *const id, uint32_t baudrate)
    {
        xh_can_t *p = (xh_can_t *)id;
        p->set_baudrate(p->core.var, baudrate);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_can_h */
