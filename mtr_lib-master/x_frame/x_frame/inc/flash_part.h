/**
 * @brief flash_part.c.xtemp header file
 * @file flash_part.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:38:21
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:38:21
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __FLASH_PART_H__
#define __FLASH_PART_H__

enum
{
    SYS_LOADER_PART=1,
    SYS_APP_PART,
    SYS_BOOT_CFG_PART,
    
    //--------user part
    USER_REC_PART,
    SYS_APP_CFG_PART,
    SYS_PRODUCT_PART,
    SYS_HW_VER_PART,
	//--------backup part
    SYS_BACKUP_PART,
    //--------app custom
    FLASH_PART_APP_CUSTOM = 10,
};

/**
 * @brief 注册FLASH分区
 *
 * @param [in]  drv_id			FLASH驱动ID
 * 				start_addr      FLASH起始地址
 * 				flash_size      FLASH大小
 * 				block_size      FLASH块大小(最小擦除大小)
 * 			    part_item...    分区信息
 * 					part_name; 	///< 分区名字
 *					part_id;  	///< 分区ID
 *					part_size; 	///< 分区大小
 *					part_address; ///< 分区起始地址
 * @details 1、外部FLASH和内部FLASH一样的用法
 * 			2、注意 block_size 需要符合芯片
 * @par 示例:注册FLASH分区
 * @code
 *
	#define FLASH_ADDR                      (0x8000000) ///< FLASH起始地址
	#define FLASH_BLOCK_SIZE                (1024 * 2)  ///< 块大小
	#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 128) ///< FLASH大小
	///< 分区大小
	#define SYS_LOADER_PART_SIZE            (12  *  FLASH_BLOCK_SIZE)
	#define SYS_APP_PART_SIZE 		        (100  *  FLASH_BLOCK_SIZE)
	#define SYS_BOOT_CFG_PART_SIZE          (1   *  FLASH_BLOCK_SIZE)
	#define USER_REC_PART_SIZE  	        (2   *  FLASH_BLOCK_SIZE)
	#define SYS_APP_CFG_PART_SIZE           (11   *  FLASH_BLOCK_SIZE)
	#define SYS_PRODUCT_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)
	#define SYS_HW_VER_PART_SIZE            (1   *  FLASH_BLOCK_SIZE)

///< 分区ID枚举
enum
{
	SYS_LOADER_PART=1,
	SYS_APP_PART,
	SYS_BOOT_CFG_PART,

	//--------user part
	USER_REC_PART,
	SYS_APP_CFG_PART,
	SYS_PRODUCT_PART,
	SYS_HW_VER_PART,

	//--------app custom
	FLASH_PART_APP_CUSTOM = 10,
};

REG_FLASH_PART(DRV_FLASH,FLASH_ADDR,FLASH_SIZE,FLASH_BLOCK_SIZE,
	///< 分区名        			分区ID     			 分区大小
		{"loader",          SYS_LOADER_PART,       SYS_LOADER_PART_SIZE},
		{"app",             SYS_APP_PART,          SYS_APP_PART_SIZE},
		{"boot_cfg",        SYS_BOOT_CFG_PART,     SYS_BOOT_CFG_PART_SIZE},

		{"rec",             USER_REC_PART,         USER_REC_PART_SIZE},
		{"app_cfg",         SYS_APP_CFG_PART,      SYS_APP_CFG_PART_SIZE},
		{"product_info",    SYS_PRODUCT_PART,      SYS_PRODUCT_PART_SIZE},
		{"product_info",    SYS_HW_VER_PART,       SYS_HW_VER_PART_SIZE},
	);
 * @endcode
 */


#define REG_FLASH_PART(drv_id,start_addr,flash_size,block_size,part_item...)  									\
			part_item_t g_flash_part_item_##drv_id[] = { part_item };																					\
			flash_part_core_t g_flash_part##drv_id={PTR_TYPE_XH_DEV(drv_id),start_addr,flash_size,block_size,ARRY_ITEMS_NUM(g_flash_part_item_##drv_id),g_flash_part_item_##drv_id} ;   \
			AUTO_REG_ITEM(reg_flash_part_##drv_id,AUTO_REG_FLASH_PART,(void*)&g_flash_part##drv_id)

/**
* @brief 分区的信息
* @details 结构体使用REG_FLASH_PART()注册分区表时填充
*/
typedef struct
{
	const char *		part_name; 	///< 分区名字
	uint32_t			part_id;  	///< 分区ID
	uint32_t			part_size; 	///< 分区大小
	uint32_t 			part_address; ///< 分区起始地址
}part_item_t;

typedef struct
{
    void*     const     drv_id;
	uint32_t   			start_address;
	uint32_t 			flash_size;
	uint32_t			block_size;
	uint32_t   			part_num;
	part_item_t*		p_part_item;
	list_t				list;
}flash_part_core_t;

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

/**
 * @brief 获取flash分区的信息
 *
 * @param [in] part_id       分区的ID
 *
 * @return part_item_t*
 *  @retval 返回值 NULL:未注册的分区
 * 				  非NULL:该分区的信息结构体指针
 * @details 特殊说明:
 * @par eg:获取分区 SYS_LOADER_PART的信息
 * @code
 *
///< flash分区的枚举
enum
{
    SYS_LOADER_PART=1,
    SYS_APP_PART,
    SYS_BOOT_CFG_PART,
	...///< 其他FLASH分区
};

part_item_t *flash_item = NULL;
void test_init(void)
{
    flash_item = get_flash_item_by_part_id(SYS_LOADER_PART);
    if(flash_item != NULL)
    {
        ///< 成功获取分区信息
    }
	else
	{
		///< 获取分区信息失败
	}
}
 * @endcode
 */
part_item_t *get_flash_item_by_part_id(uint32_t part_id);

/**
 * @brief 获取分区是否注册
 *
 * @param [in] part_id       分区的ID
 *
 * @return true
 *  @retval 分区存在
 * @return false
 *  @retval 分区不存在
 * @details 特殊说明:
 * @par eg:获取FLASH_PART_APP_CUSTOM是否存在
 * @code
enum
{
    SYS_LOADER_PART=1,
    SYS_APP_PART,
    SYS_BOOT_CFG_PART,
	...///< 其他FLASH分区
	FLASH_PART_APP_CUSTOM =10,
};
if(flash_part_is_exit(FLASH_PART_APP_CUSTOM) == false)
{
	///< 分区不存在 打印对应信息
}
else
{
	///< 分区存在
}
 * @endcode
 */
bool flash_part_is_exit(uint32_t part_id);

/**
 * @brief 获取分区的大小
 *
 * @param [in] part_id       分区的ID
 *
 * @return uint32_t
 *  @retval >0 :分区的大小
 * 			=0 :分区不存在
 * @details 特殊说明: 返回值为0 表示分区没有使用REG_FLASH_PART注册
 * @par eg:获取SYS_LOADER_PART的大小
 * @code
enum
{
    SYS_LOADER_PART=1,
    SYS_APP_PART,
    SYS_BOOT_CFG_PART,
	...///< 其他FLASH分区
};
uint32_t part_size; ///< 分区大小
part_size = flash_part_get_part_size(SYS_LOADER_PART);
if(part_size != 0)
{
	///< 分区存在
}
else
{
	///< 分区不存在
}
 * @endcode
 */
uint32_t flash_part_get_part_size(uint32_t part_id);

/**
 * @brief 获取FLASH块大小
 *
 * @param [in] part_id       分区的ID
 *
 * @return uint32_t
 *  @retval >0 :flash的块大小
 * 			=0 :分区不存在
 * @details flash的最小的擦除大小
 * 			part_id：传入任意已经注册的分区即可
 * @par eg:获取flash块大小
 * @code
uint32_t sector_size;
sector_size = flash_part_get_sector_size(SYS_LOADER_PART);
if(sector_size > 0)
{
	///< 获取块大小成功
}
 * @endcode
 */
uint32_t flash_part_get_sector_size(uint32_t part_id);

/**
 * @brief 擦除分区
 *
 * @param [in] part_id       分区的ID
 *
 * @return true
 *  @retval 擦除成功
 * @return false
 *  @retval 擦除失败(分区不存在)
 * @details 分区没有注册会擦除失败
 * @par eg:擦除SYS_LOADER_PART分区
 * @code
 * bool res;
 * res = flash_part_erase(SYS_LOADER_PART);
 * if(res)
 * {
 * 		///< 擦除成功
 * }
 * else
 * {
 * 		///< 擦除失败(分区不存在)
 * }
 * @endcode
 */
bool flash_part_erase(uint32_t part_id);

/**
 * @brief 擦除指定分区的部分内容
 *
 * @param [in] part_id       分区的ID
 * @param [in] offset        距离分区起始地址的长度
 * @param [in] len           擦除的长度
 *
 * @return true
 *  @retval 擦除成功
 * @return false
 *  @retval 擦除失败
 * @details 特殊说明: 擦除最小单位为块，不满一块擦除一块
 * @par eg:擦除SYS_LOADER_PART第0x20开始的0x500个字节
 * @code
 * bool res;
 * uint32_t offset = 0x20;
 * uint32_t len = 0x500;
 * res = flash_part_erase_len(SYS_LOADER_PART,offset,len);
 * if(res)
 * {
 * 		///< 擦除成功
 * }
 * else
 * {
 * 		///< 擦除失败(分区不存在)
 * }
 * @endcode
 */
bool flash_part_erase_len(uint32_t part_id, uint32_t offset, uint32_t len);

/**
 * @brief 将数据写入分区
 *
 * @param [in] part_id       分区的ID
 * @param [in] offset        距离分区起始地址的长度
 * @param [in] pbuf          写入的数据
 * @param [in] len           写入的长度
 *
 * @return true
 *  @retval 写入成功
 * @return false
 *  @retval 写入失败(分区不存在)
 * @details 特殊说明: 写入之前需要先擦除
 * @par eg:对SYS_LOADER_PART的0x20地址写入数据
 * @code
 * uint32_t wbuf[20]={0x12,0x34,0x56,0x12,0x44,0x65};///< 写入的数据
 * uint32_t add = 0x20; ///< 写入的地址
 * bool res;
 * res = flash_part_write(SYS_LOADER_PART,wbuf,sizeof(wbuf));
 * if(res)
 * {
 * 		///< 写入成功
 * }
 * else
 * {
 * 		///< 写入失败
 * }
 * @endcode
 */
bool flash_part_write(uint32_t part_id, uint32_t offset, uint8_t *pbuf, uint32_t len);

/**
 * @brief 读取分区内的数据
 *
 * @param [in] part_id       分区的ID
 * @param [in] offset        距离分区起始地址的长度
 * @param [in] pbuf          存放数据缓存
 * @param [in] len           读取的长度
 *
 * @return true
 *  @retval 读取成功
 * @return false
 *  @retval 读取失败(分区不存在)
 * @details 特殊说明:
 * @par eg:读取SYS_LOADER_PART的0x20地址开始都20个字节
 * @code
 * uint32_t rbuf[20];///< 读数据的缓存
 * uint32_t add = 0x20; ///< 读的地址
 * bool res;
 * res = flash_part_read(SYS_LOADER_PART,rbuf,20);
 * if(res)
 * {
 * 		///< 读取成功
 * }
 * else
 * {
 * 		///< 读取失败
 * }
 * @endcode
 */
bool flash_part_read(uint32_t part_id, uint32_t offset, uint8_t *pbuf, uint32_t len);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __FLASH_PART_H__
