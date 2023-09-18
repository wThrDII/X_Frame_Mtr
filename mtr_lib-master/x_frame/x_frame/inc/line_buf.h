#ifndef __LINE_BUF_H__
#define __LINE_BUF_H__


#include <stdint.h>


typedef struct
{
    const char	*name;
    uint8_t	*pbuf;
    uint16_t 	size;
    uint16_t	r_index;
    uint16_t	w_index;
    uint8_t 	state;

} linebuf_t;

/*不能在写入的时候同时读取会导致内存错乱*/

#define LB_STATE_IS_W	(0x1<<0)
#define LB_STATE_IS_R	(0x1<<1)
#define LB_STATE_IS_M	(0x1<<2) //move buf

/**
 * @brief 初始化指定line buff的基础信息
 *
 * @param [in] name line buff的名称
 * @param [in] p_lb line buff的结构体指针
 * @param [in] pbuf line buff的数组指针
 * @param [in] size line buff的大小
 *
 * @return int32_t 返回0
 *  @retval
 * @details:
 * @par e.g:
 * @code
 *      linebuf_t *lb_test=NULL;
 *      uint8_t buff[3];
 *
 *      user_val = lb_init("user_name", lb_test, buff, 2);
 * @endcode
 */
int32_t lb_init(const char 	*name,
                linebuf_t	*p_lb,
                uint8_t		*pbuf,
                uint16_t		size);
/**
 * @brief 获取指定line buff中的空闲空间长度
 *
 * @param [in] p_lb line buff的结构体指针
 *
 * @return int32_t 返回line buff中的空闲空间长度
 *  @retval
 * @details:
 * @par e.g:
 * @code
 *      int32_t user_val;
 *
 *      user_val = lb_get_rest_len(lb_test);//lb_test已初始化过
 * @endcode
 */
int32_t 	lb_get_rest_len	(linebuf_t	*p_lb);
/**
 * @brief 获取指定line buff中有效数据长度
 *
 * @param [in] p_lb line buff的结构体指针
 *
 * @return int32_t 返回line buff中的有效数据长度
 *  @retval
 * @details:
 * @par e.g:
 * @code
 *      int32_t user_val;
 *
 *      user_val = lb_get_data_len(lb_test);//lb_test已初始化过
 * @endcode
 */
int32_t 	lb_get_data_len	(linebuf_t	*p_lb);
/**
 * @brief 获取指定line buff的状态
 *
 * @param [in] p_lb line buff的结构体指针
 *
 * @return uint8_t 返回line buff的状态
 *  @retval 有以下几种可能:
 *              1.LB_STATE_IS_W  向line buff中写入数据
 *              2.LB_STATE_IS_R  读取line buff中的数据
 *              3.LB_STATE_IS_M  移动line buff中的数据
 * @details:
 * @par e.g:
 * @code
 *      uint8_t user_val;
 *
 *      user_val = lb_get_statues(lb_test);//lb_test已初始化过
 * @endcode
 */
uint8_t 	lb_get_statues	(linebuf_t	*p_lb);
/**
 * @brief 向指定line buff中写入数据
 *
 * @param [in] p_lb line buff的结构体指针
 * @param [in] p_src 向line buff中写入的数据
 * @param [in] len 向line buff中写入数据的长度
 *
 * @return int32_t 实际写入line buff中数据的长度
 *  @retval
 *  -1:line buff正处于其他操作中，写入数据失败
 *  其他数据: 实际写入line buff中数据的长度,line buff空闲长度和
 *            写入数据的最小值
 * @details:
 * @par e.g:
 * @code
 *      uint8_t buff[3];
 *      int32_t user_val;
 *
 *      user_val = lb_push(ld_test, buff, 2);//lb_test已初始化过
 * @endcode
 */
int32_t 	lb_push			(linebuf_t	*p_lb, uint8_t *p_src, uint16_t len);
/**
 * @brief 读取指定line buffz中的指定长度数据
 *
 * @param [in] p_lb line buff的结构体指针
 * @param [in] p_dest 获取数据的buff
 * @param [in] len  需要获取数据的长度
 *
 * @return int32_t 实际获取数据的长度
 *  @retval
 *  -1:line buff正处于其他操作中，写入数据失败
 *  其他数据: 实际读取到line buff中数据的长度,line buff有效数据长度和
 *            读取数据的最小值
 * @details:
 * @par e.g:
 * @code
 *      uint8_t buff[3]={ ... }
 *      int32_t user_val;
 *
 *      user_val = lb_pull(ld_test, buff, 2);//lb_test已初始化过
 * @endcode
 */
int32_t 	lb_pull			(linebuf_t	*p_lb, uint8_t *p_dest, uint16_t len);
/**
 * @brief 移动line buff中的数据
 *
 * @param [in] p_lb line buff的结构体指针
 * @param [in] len
 *
 * @return int32_t 实际移动数据的长度
 *  @retval
 *  -1:line buff正处于其他操作中，写入数据失败
 *  其他数据: 实际读取到line buff中数据的长度,line buff有效数据长度和
 *            读取数据的最小值
 * @details:
 *          三种情况
 *          1： len 小于 r_index,w_index
 *          2： len 大于 r_index,小于 w_index
 *          3： len 大于 r_index,w_index
 * @par e.g:
 * @code
 *      int32_t user_val;
 *
 *      user_val = lb_move(ld_test, 2);//lb_test已初始化过
 * @endcode
 */
int32_t 	lb_move			(linebuf_t	*p_lb, uint16_t len);
/**
 * @brief 获取line buff读数据索引的地址
 *
 * @param [in] p_lb line buff的结构体指针
 *
 * @return uint8_t* 返回读数据索引的地址
 *  @retval
 * @details:
 * @par e.g:
 * @code
 *      uint8_t *pUser = NULL;
 *      pUser = lb_get_r_ptr(ld_test);//lb_test已初始化过
 *
 * @endcode
 */
uint8_t 	*lb_get_r_ptr	(linebuf_t	*p_lb);
/**
 * @brief 重置line buff
 *
 * @param [in] p_lb line buff的结构体指针
 *
 * @details:
 * @par e.g:
 * @code
 *      lb_reset(ld_test);//lb_test已初始化过
 * @endcode
 */
void lb_reset(linebuf_t	*p_lb);


#endif  //__LINE_BUF_H__
