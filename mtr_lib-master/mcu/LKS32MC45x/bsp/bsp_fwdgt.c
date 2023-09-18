/**
 * @brief 看门狗驱动
 * @file bsp_fwdgt.c
 * @version 1.0
 * @author boyu.qin 
 * @date 2023-01-11 10:36:44
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 * 
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-11 10:36:44      <td>boyu.qin      <td>创建文件
 */


#include "bsp_fwdgt.h"

__attribute__((weak)) bool g_bsp_fwdgt_enable = false;

/**
 * @brief 看门狗初始化
 *
 * @param [in] time
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_fwdgt_init(P_BSP_FWDGT fwdgt)
{
    IWDG_Disable();
	uint32_t time_ms = fwdgt->timeout;
	uint32_t reload_cnt =0;
    if (g_bsp_fwdgt_enable == false)
    {
        return;
    }
	time_ms = (time_ms > 65408) ? 65408 : time_ms;
    time_ms = (time_ms < 128) ? 128 : time_ms;	
	reload_cnt = (time_ms/128)*4096;
    IWDG_SetResetCnt(reload_cnt);
	IWDG_Enable();
}

/**
 * @brief 看门狗反初始化
 *
 * @param [in] fwdgt
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_fwdgt_deinit(P_BSP_FWDGT fwdgt)
{
    IWDG_Disable();
	IWDG_SetResetCnt(0x17000);             // 默认看门狗间隔为 3s;
}

/**
 * @brief 看门狗复位标志获取
 *
 * @param [in] var      ///< 预留
 *
 * @return uint32_t     ///< 看门狗复位标志
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
uint32_t bsp_get_fwdgt_is_reset(P_BSP_FWDGT fwdgt)
{
    return 0;
}

/**
 * @brief 重载看门狗
 *
 * @param [in] var       ///< 预留
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_reload_fwdgt(P_BSP_FWDGT fwdgt)
{
    //    DBG_I("重载看门狗a");
    IWDG_FeedDog();
}



