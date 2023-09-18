/**
 * @brief flash_param.c.xtemp header file
 * @file flash_param.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:51:32
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:51:32
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __FLASH_PARAM_H__
#define __FLASH_PARAM_H__


/**
 * @brief 注册普通参数
 *
 * @param [in]  part_id        分区的ID
 * 				param_name     FLASH管理的名字
 *				p_param        缓存指针(可以为NULL)
 * 				p_def_param    默认参数指针(可以为NULL)
 * 				param_len      参数长度
 * @details 1、内存管理依赖flash_part
 * 			2、part_id为使用REG_FLASH_PART注册的分区ID
 * 			3、选择的分区最小需要有两个FLASH块大小
 * @par 示例:注册内存管理
 * @code
	uint8_t param_data[4];///< 缓存
	uint8_t param_def_data[4]={0x12,0x33,0x11,0x54};
	//注册普通参数
	///< SYS_APP_CFG_PART :分区ID
	///< TEST_NORMAL :FLASH管理的名字
	REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,TEST_NORMAL,param_data,param_def_data,sizeof(param_data));
 * @endcode
 */
#define REG_FLASH_NORMAL_PARAM(part_id, param_name, p_param, p_def_param, param_len)		\
		DEF_X_UID(param_name)													\
		FLASH_PARAM_SECTOR_OBJ fp_##param_name##_sec_tab[2];					\
		FLASH_PARAM_OBJ fp_##param_name = {										\
			FLASH_NORMAL_PARAM,													\
			part_id,															\
			#param_name,														\
			sizeof(#param_name),												\
			X_UID_VAL(param_name),												\
			p_param,															\
            p_def_param,                                                        \
			param_len,															\
			fp_##param_name##_sec_tab,											\
			2,																	\
		};																		\
		AUTO_REG_ITEM(reg_fp_##param_name,AUTO_REG_FLASH_PARAM,(void*)&fp_##param_name);

/**
 * @brief 注册队列参数
 *
 * @param [in]  part_id        分区的ID
 * 				param_name     FLASH管理的名字
 *				p_param        缓存指针(可以为NULL)
 * 				p_def_param    默认参数指针(可以为NULL)
 * 				param_len      参数长度
 * 				queue_size     队列大小，即需要保存的参数个数
 * 				block_size     保存区域的块大小
 *
 * @details 1、内存管理依赖flash_part
 * 			2、part_id为使用REG_FLASH_PART注册的分区ID
 * 			3、选择的分区最小需要有两个FLASH块大小
 * @par 示例:注册内存管理
 * @code
	uint8_t param_data[4];///< 缓存
	uint8_t param_def_data[4]={0x12,0x33,0x11,0x54};
	//注册普通参数
	///< SYS_APP_CFG_PART :分区ID
	///< QUEUE_PARAM :FLASH管理的名字
	REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,QUEUE_PARAM,param_data,param_def_data,sizeof(param_data),6,FLASH_BLOCK_SIZE);
 * @endcode
 */
#define REG_FALSH_QUEUE_PARAM(part_id, param_name, p_param, p_def_param, param_len, queue_size, block_size)						\
		DEF_X_UID(param_name)																					\
		FLASH_PARAM_SECTOR_OBJ fp_##param_name##_sec_tab[1 + CAL_DIVISION_MIN_RESULT(queue_size,(block_size - CAL_FLASH_ALIGN_LEN(sizeof(#param_name) + sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ))) / CAL_FLASH_ALIGN_LEN(param_len + 2))];				\
		FLASH_PARAM_OBJ fp_##param_name = {											\
			FLASH_QUEUE_PARAM,														\
			part_id,																\
			#param_name,															\
			sizeof(#param_name),													\
			X_UID_VAL(param_name),											        \
			p_param,																\
            p_def_param,                                                            \
			param_len,																\
			fp_##param_name##_sec_tab,												\
			ARRY_ITEMS_NUM(fp_##param_name##_sec_tab),								\
		};																			\
		AUTO_REG_ITEM(reg_fp_##param_name,AUTO_REG_FLASH_PARAM,(void*)&fp_##param_name);

#define REG_LINK_FLASH_PARAM_ACTION(link_id)   REG_LINK_ACTION(link_id, FUNC_CMDSET_REC, CMD_ID_FLASH_PARAM_RD, 		rd_flash_param_all);

#define EXT_FLASH_PARAM_NAME(param_name)	EXT_X_UID(param_name)

#define flash_param_save_all()                                         										real_flash_param_save_all()
#define flash_param_rd_all()                                         										    real_flash_param_rd_all()
#define flash_param_read_all()                                         										real_flash_param_read_all()
#define flash_param_id_save_all(part_id)                               										real_flash_param_id_save_all(part_id)
#define flash_param_id_rd_all(part_id)                               										    real_flash_param_id_rd_all(part_id)
#define flash_param_id_read_all(part_id)                               										real_flash_param_id_read_all(part_id)
#define flash_param_save(param_name,p_buf,len)        	 														real_flash_param_save(param_name,p_buf,len)
#define flash_param_rd(param_name)        	 														            real_flash_param_rd(param_name)
#define flash_param_read(param_name,p_buf,len)        	 														real_flash_param_read(param_name,p_buf,len)
#define flash_queue_param_read_history(param_name,history_index,p_buf,len)										real_flash_queue_param_read_history(param_name,history_index,p_buf,len)

#define FLASH_PARAM_PART_ID_MAX_NUM			10

#define FLASH_PARAM_BYTE_ALIGN		4		/* 字节对齐 */
#define FLASH_PARAM_NAME_MAX_LEN	100

#define CAL_DIVISION_MIN_RESULT(dividend, divison)	(((dividend) + (divison) - 1) / (divison))		/* 计算除法最小结果，余数进1 */
#define CAL_ALIGN_LEN(all, align_num)	(((all) + (align_num) - 1) / (align_num) * (align_num))		/* 计算对齐长度 */
#define CAL_FLASH_ALIGN_LEN(len)		CAL_ALIGN_LEN(len,FLASH_PARAM_BYTE_ALIGN)					/* 计算FLASH对齐后的长度 */

typedef enum {
    FLASH_NORMAL_PARAM = 0,
	FLASH_QUEUE_PARAM,
}FLASH_PARAM_TYPE;

typedef enum {
    FLASH_PARAM_SUCCESS = 0,        /* 成功 */
    FLASH_PARAM_NO_PART,            /* 没有区域 */
	FLASH_PARAM_NO_DATA,			/* 没有数据 */
    FLASH_PARAM_NO_PARAM,           /* 没有参数 */
    FLASH_PARAM_NO_DEF_PARAM,       /* 没有默认参数 */
	FLASH_PARAM_CRC_ERR,			/* CRC校验错误 */
	FLASH_PARAM_NO_SECTOR,			/* 扇区不存在 */
    FLASH_PARAM_LEN_LIMIT,          /* 参数长度受限 */
}FLASH_PARAM_STA_ID;

typedef struct _FLASH_PARAM_PART_INFO_OBJ{
    uint32_t part_id;
}FLASH_PARAM_PART_INFO_OBJ, *P_FLASH_PARAM_PART_INFO_OBJ;

typedef struct _FLASH_PARAM_PART_OBJ{
    FLASH_PARAM_PART_INFO_OBJ p_part_tab[FLASH_PARAM_PART_ID_MAX_NUM];
	uint16_t part_tab_size;
}FLASH_PARAM_PART_OBJ, *P_FLASH_PARAM_PART_OBJ;

/* 参数定义 */

typedef struct _FLASH_PARAM_SECTOR_INFO_HEAD_OBJ{
	uint16_t head_crc;
    uint16_t info_area_size;
    uint16_t save_index;
    uint16_t data_area_size;
	uint16_t data_len;
    uint16_t name_len;
	uint16_t name_crc;
}FLASH_PARAM_SECTOR_INFO_HEAD_OBJ, *P_FLASH_PARAM_SECTOR_INFO_HEAD_OBJ;

typedef struct _FLASH_PARAM_SECTOR_INFO_OBJ{
    FLASH_PARAM_SECTOR_INFO_HEAD_OBJ info_head;
	char *p_name;
}FLASH_PARAM_SECTOR_INFO_OBJ, *P_FLASH_PARAM_SECTOR_INFO_OBJ;

typedef struct _FLASH_PARAM_SECTOR_OBJ{
	uint8_t init;
	uint32_t sector_offset;
	uint16_t data_num;
    FLASH_PARAM_SECTOR_INFO_HEAD_OBJ info;
}FLASH_PARAM_SECTOR_OBJ, *P_FLASH_PARAM_SECTOR_OBJ;

typedef struct _FLASH_PARAM_OBJ{
    FLASH_PARAM_TYPE type;
	uint32_t part_id;
    char *p_name;
	uint16_t name_len;
    _X_UID_TYPE param_name;
    void *p_param;
    void const *p_def_param;
    uint32_t param_len;
    P_FLASH_PARAM_SECTOR_OBJ p_sec_tab;
    uint16_t sec_tab_size;
	struct {
		uint32_t save_index;
		uint32_t save_sector;
	}exec;
	list_t   list;
}FLASH_PARAM_OBJ, *P_FLASH_PARAM_OBJ;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

/**
 * @brief 保存全部的flash参数
 * @details 特殊说明: 有缓存的才能使用这个保存
 * @par eg:保存全部参数
 * @code
 *    flash_param_save_all();
 * @endcode
 */
void real_flash_param_save_all(void);

/**
 * @brief 恢复全部的默认参数
 * @details 特殊说明: 有默认参数的使用这个才生效
 * @par eg:恢复全部的默认参数
 * @code
 *    real_flash_param_rd_all();
 * @endcode
 */
void real_flash_param_rd_all(void);

/**
 * @brief 读取全部的参数到自己的缓存
 * @details 特殊说明: 有缓存的使用这个才生效
 * @par eg:读取全部的参数到自己的缓存
 * @code
 *    real_flash_param_read_all();
 * @endcode
 */
void real_flash_param_read_all(void);

/**
 * @brief 保存指定分区ID的所有flash参数
 *
 * @param [in] part_id       分区ID
 *
 * @details 1、保存使用这个分区ID的参数
 * 			2、将缓存的内容写进flash
 * @par eg:保存 SYS_APP_CFG_PART 的flash参数
 * @code
uint8_t param_data[4];///< 缓存
REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,TEST_NORMAL,param_data,NULL,sizeof(param_data));
void func(void)
{
	real_flash_param_id_save_all(SYS_APP_CFG_PART);
}
 * @endcode
 */
void real_flash_param_id_save_all(uint32_t part_id);

/**
 * @brief 将指定分区ID的设置为默认值
 *
 * @param [in] part_id       分区ID
 *
 * @details 1、保存使用这个分区ID的参数
 * 			2、将缓存的内容写进flash
 * @par eg:将指定分区SYS_APP_CFG_PART的设置为默认值
 * @code
uint8_t param_data[4];///< 缓存
uint8_t param_def_data[4]={0x12,0x33,0x11,0x54};//默认值
REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,TEST_NORMAL,param_data,param_def_data,sizeof(param_data));
void func(void)
{
	real_flash_param_id_rd_all(SYS_APP_CFG_PART);
}
 * @endcode
 */
void real_flash_param_id_rd_all(uint32_t part_id);

/**
 * @brief 读取指定分区ID的参数
 *
 * @param [in] part_id       分区ID
 *
 * @details 1、读取使用这个分区ID的参数
 * 			2、将参数读入缓存
 * @par eg:读取SYS_APP_CFG_PART保存的参数
 * @code
uint8_t param_data[4];///< 缓存
uint8_t param_def_data[4]={0x12,0x33,0x11,0x54};//默认值
REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,TEST_NORMAL,param_data,param_def_data,sizeof(param_data));
void func(void)
{
	real_flash_param_id_read_all(SYS_APP_CFG_PART);
}
 * @endcode
 */
void real_flash_param_id_read_all(uint32_t part_id);

/**
 * @brief 保存参数
 *
 * @param [in] param_name    参数管理名字
 * @param [in] p_buf         需要保存的数据
 * @param [in] len           数据长度
 *
 * @return FLASH_PARAM_STA_ID
 *  @retval 参考FLASH_PARAM_STA_ID枚举的说明
 * @details 特殊说明:保存的数据长度需要和注册时的参数长度一致
 * @par eg:保存参数
 * @code
uint8_t param_data[4];
uint8_t param_def_data[20]={0x12,0x33,0x11,0x54};
uint8_t wbuf[4] = {0x1,0x2,0x3,0x4};
uint32_t len = 4; ///< 保存的数据长度 和注册的参数长度一样
uint8_t res;
//注册普通参数
REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,TEST_NORMAL,param_data,param_def_data,sizeof(param_data));
void func(void)
{
    res = real_flash_param_save(TEST_NORMAL,wbuf,len);
	if(res == FLASH_PARAM_SUCCESS)
	{
		///< 保存成功
	}
	else
	{
		///< 保存失败
	}
}
 * @endcode
 */
FLASH_PARAM_STA_ID real_flash_param_save(_X_UID_TYPE param_name, uint8_t *p_buf, uint32_t len);

/**
 * @brief 恢复初始值
 * @param [in] param_name    参数管理名字
 * @return FLASH_PARAM_STA_ID
 *  @retval 参考FLASH_PARAM_STA_ID枚举的说明
 * @details 特殊说明:有默认参数的才有效
 * @par eg:恢复初始值
 * @code
uint8_t param_data[4];
uint8_t param_def_data[20]={0x12,0x33,0x11,0x54};
uint8_t wbuf[4] = {0x1,0x2,0x3,0x4};
uint8_t res;
//注册普通参数
REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,TEST_NORMAL,param_data,param_def_data,sizeof(param_data));
void func(void)
{
    res = real_flash_param_rd(TEST_NORMAL);
	if(res == FLASH_PARAM_SUCCESS)
	{
		///< 恢复成功
	}
	else
	{
		///< 恢复失败
	}
}
 * @endcode
 */
FLASH_PARAM_STA_ID real_flash_param_rd(_X_UID_TYPE param_name);


/**
 * @brief 读取保存的参数内容
 *
 * @param [in] param_name    参数管理名字
 * @param [in] p_buf         读的数据缓存
 * @param [in] len           数据长度
 *
 * @return FLASH_PARAM_STA_ID
 *  @retval 参考FLASH_PARAM_STA_ID枚举的说明
 * @details 特殊说明:最多读取参数长度的数据
 * @par eg:读取TEST_NORMAL的参数
 * @code
uint8_t param_data[4];
uint8_t param_def_data[20]={0x12,0x33,0x11,0x54};
uint8_t rbuf[4];
uint32_t len = 4; ///< 需要读取的数据长度
uint8_t res;
//注册普通参数
REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART,TEST_NORMAL,param_data,param_def_data,sizeof(param_data));
void func(void)
{
    res = real_flash_param_read(TEST_NORMAL,rbuf,len);
	if(res == FLASH_PARAM_SUCCESS)
	{
		///< 读取成功
	}
	else
	{
		///< 读取失败
	}
}
 * @endcode
 */
FLASH_PARAM_STA_ID real_flash_param_read(_X_UID_TYPE param_name, uint8_t *p_buf, uint32_t len);

/**
 * @brief 读取历史数据
 *
 * @param [in] param_name    参数管理名字
 * @param [in] history_index 历史索引
 * @param [in] p_buf         读的数据缓存
 * @param [in] len           数据长度
 *
 * @return FLASH_PARAM_STA_ID
 *  @retval 参考FLASH_PARAM_STA_ID枚举的说明
 * @details 1、队列参数专用
 * 			2、history_index :历史索引,从0开始，越小数据越新
 * @par eg:读取保存的上一个数据
 * @code
uint8_t rbuf[4];
uint32_t len = 4; ///< 需要读取的数据长度
REG_FALSH_QUEUE_PARAM(SYS_APP_CFG_PART,QUEUE_PARAM,NULL,NULL,4,4,FLASH_BLOCK_SIZE)
void func(void)
{
    res = real_flash_queue_param_read_history(QUEUE_PARAM,1,rbuf,len);
	if(res == FLASH_PARAM_SUCCESS)
	{
		///< 读取成功
	}
	else
	{
		///< 读取失败
	}
}
 * @endcode
 */
FLASH_PARAM_STA_ID real_flash_queue_param_read_history(_X_UID_TYPE param_name, uint16_t history_index, uint8_t *p_buf, uint32_t len);
int32_t rd_flash_param_all(hal_frame_info_t *p_frame_info);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __FLASH_PARAM_H__
