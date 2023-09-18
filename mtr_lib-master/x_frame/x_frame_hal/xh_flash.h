/**
 * @brief
 * @file xh_flash.h
 * @version 1.0
 * @author boyu.qin
 * @date 2022-09-06 18:21:22
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-09-06 18:21:22 <td>1.0       <td>boyu.qin      <td>Created function
 */
#ifndef xh_flash_h
#define xh_flash_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef uint32_t (*xh_flash_write_obj)(void *var, uint32_t addr, uint8_t *pbuf, uint32_t len);
    typedef void (*xh_flash_read_obj)(void *var, uint32_t addr, uint8_t *pbuf, uint32_t len);
    typedef uint32_t (*xh_flash_write_16_obj)(void *var, uint32_t addr, uint16_t *pbuf, uint32_t len);
    typedef void (*xh_flash_read_16_obj)(void *var, uint32_t addr, uint16_t *pbuf, uint32_t len);
    typedef void (*xh_flash_erase_obj)(void *var, uint32_t addr, uint32_t len);
    typedef uint32_t (*xh_flash_get_byte_len_obj)(void *var);

    typedef struct _xh_flash_t
    {
        xh_comm_t comm;
        xh_flash_read_obj read;
        xh_flash_write_obj write;
        xh_flash_read_16_obj read_16;
        xh_flash_write_16_obj write_16;
        xh_flash_erase_obj erase;
        xh_flash_get_byte_len_obj get_byte_len;
    } xh_flash_t, *p_xh_flash_t;

#define REG_XH_FLASH_DRV(id, var, prio, auto_init, init, deinit, read, write, read_16, write_16, erase, get_byte_len) \
    REG_XH_DEV(xh_flash_t, id, var, prio, auto_init, init, deinit,                                                    \
               (xh_flash_read_obj)read,                                                                               \
               (xh_flash_write_obj)write,                                                                             \
               (xh_flash_read_16_obj)read_16,                                                                         \
               (xh_flash_write_16_obj)write_16,                                                                       \
               (xh_flash_erase_obj)erase,                                                                             \
               (xh_flash_get_byte_len_obj)get_byte_len)

#define EXT_XH_FLASH_DRV(id) EXT_XH_DEV(xh_flash_t, id)

    /**
     * @brief flash写
     *
     * @param [in] id            驱动ID
     * @param [in] addr          写入起始地址
     * @param [in] pbuf          存放写入内容的buf指针
     * @param [in] len           写入的数据长度
     *
     * @details 特殊说明:
     * @par eg:     往flash地址0x800f000写入8个8字节的数据
     * @code
     *    uint8_t wr_data[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
     *    xh_flash_write(DRV_FLASH, 0x800f000, wr_data, 8);
     * @endcode
     */
    static inline uint32_t xh_flash_write(void *const id, uint32_t addr, uint8_t *pbuf, uint32_t len)
    {
        xh_flash_t *p = (xh_flash_t *)id;
        p->write(p->comm.var, addr, pbuf, len);
        if(len % 2 != 0)
            len = (len + 1)/2;
        else
            len = len/2;
        if(len%4 != 0)
            len += 4 - (len%4);
        return len;
    }

    /**
     * @brief flash读
     *
     * @param [in] id          驱动ID
     * @param [in] addr        读取起始地址
     * @param [in] pbuf        存放读取内容的buf指针
     * @param [in] len         读取的数据长度
     *
     * @details 特殊说明:
     * @par eg:     从flash地址0x800f000读取8个8字节的数据放到re_data中
     * @code
     *    uint8_t re_data[8]={0};
     *    xh_flash_read(DRV_FLASH, 0x800f000, re_data, 8);
     * @endcode
     */
    static inline void xh_flash_read(void *const id, uint32_t addr, uint8_t *pbuf, uint32_t len)
    {
        xh_flash_t *p = (xh_flash_t *)id;
        p->read(p->comm.var, addr, pbuf, len);
    }

    /**
     * @brief flash写 半字
     *
     * @param [in] id            驱动ID
     * @param [in] addr          写入起始地址
     * @param [in] pbuf          存放写入内容的buf指针
     * @param [in] len           写入的数据长度
     *
     * @details 特殊说明:
     * @par eg:     往flash地址0x800f000写入8个16字节的数据
     * @code
     *    uint16_t wr_data[8]={0x0102,0x0304,0x0506,0x0708,0x090A,0x0B0C,0x0D0E,0x0F10};
     *    xh_flash_write(DRV_FLASH, 0x800f000, wr_data, 8);
     * @endcode
     */
    static inline uint32_t xh_flash_write_16(void *const id, uint32_t addr, uint16_t *pbuf, uint32_t len)
    {
        xh_flash_t *p = (xh_flash_t *)id;
        p->write_16(p->comm.var, addr, pbuf, len);
        if(len % 4 != 0)
            len += 4 - (len%4);
        return len;
    }
    /**
     * @brief flash读 半字
     *
     * @param [in] id          驱动ID
     * @param [in] addr        读取起始地址
     * @param [in] pbuf        存放读取内容的buf指针
     * @param [in] len         读取的数据长度
     *
     * @details 特殊说明:
     * @par eg:     从flash地址0x800f000读取8个16字节的数据放到re_data中
     * @code
     *    uint16_t re_data[8]={0};
     *    xh_flash_read(DRV_FLASH, 0x800f000, re_data, 8);
     * @endcode
     */
    static inline void xh_flash_read_16(void *const id, uint32_t addr, uint16_t *pbuf, uint32_t len)
    {
        xh_flash_t *p = (xh_flash_t *)id;
        p->read_16(p->comm.var, addr, pbuf, len);
    }

    /**
     * @brief flash擦除
     *
     * @param [in] id         驱动ID
     * @param [in] addr       擦除起始地址
     * @param [in] len        擦除长度
     *
     * @details 特殊说明:      擦除最小单元为2KB
     * @par eg:     从flash地址0x800f000开始擦除4097字节
     * @code
     *    xh_flash_erase(DRV_FLASH, 0x800f000, 4097);      //实际擦除的大小为4KB（4KB>4097>2KB）
     * @endcode
     */
    static inline void xh_flash_erase(void *const id, uint32_t addr, uint32_t len)
    {
        xh_flash_t *p = (xh_flash_t *)id;
        p->erase(p->comm.var, addr, len);
    }

    /**
     * @brief 获取flash存储最小单元长度
     *
     * @param [in] id            //  设备ID
     *
     * @return uint32_t         // 返回 2：2字节，返回 1：1字节
     *  @retval
     * @details 特殊说明:
     * @par eg:
     * @code
     *          uint32_t res;
     *          res = get_flash_byte_len(DRV_FLASH);
     * @endcode
     */
    static inline uint32_t get_flash_byte_len(void *const id)
    {
        xh_flash_t *p = (xh_flash_t *)id;
        return p->get_byte_len(p->comm.var);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_flash_h */
