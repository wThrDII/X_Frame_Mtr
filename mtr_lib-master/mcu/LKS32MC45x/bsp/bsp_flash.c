/**
 * @brief flash驱动
 * @file bsp_flash.c
 * @version 1.0
 * @author boyu.qin 
 * @date 2023-01-11 10:34:39
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 * 
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-11 10:34:39      <td>boyu.qin      <td>创建文件
 */

#include "bsp_flash.h"
#include "chip_cfg.h"
#include "string.h"

/**
 * @brief flash写
 *
 * @param [in] var           ///< 预留
 * @param [in] addr          ///< 写首地址
 * @param [in] pbuf          ///< 待写入数据
 * @param [in] len           ///< 写入的数据长度
 *
 * @return uint32_t          ///< 返回0: 写入成功，返回1：写入失败
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
uint32_t bsp_flash_write(void *var, uint32_t addr, uint8_t *pbuf, uint32_t len)
{	
	if(len == 0){return 0;}
	uint32_t write_cnt = (((addr%16)+15)/16)+((len-(addr%16)+15)/16);
	uint32_t write_len = len; 

	uint32_t WD_32[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
	uint32_t DEF_DATA[4] =  {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};

	uint32_t i = 0;        
	uint32_t write_addr = addr;

	if(addr%16 != 0 && write_cnt != 0)
	{        
		// 最小写入单元的写入地址，需从此地址先读出原内容
		uint32_t read_addr = addr - addr%16;

		// 最小读取单元的读取地址
		uint32_t start_write_addr = addr - addr%4;

		/*** 首次写入 ***/
		for(i = 0; i <= (start_write_addr - read_addr)/4; i++)
		{
			WD_32[i] |=  FLASH_Read((addr%16) + i*4);
		}			
		i--;
		
		// 头写入
		if(addr%4 != 0)
		{
			uint32_t check_len = addr%4;
			uint8_t WD_8[4] = {0xff,0xff,0xff,0xff};

			if(write_len > check_len)
			{
				memcpy(&WD_8[check_len], pbuf, check_len);
				write_len -= check_len;
				pbuf += check_len;
			}
			else 
			{
				memcpy(&WD_8[check_len], pbuf, write_len);
				memcpy(&WD_32[i], &WD_8, sizeof(uint32_t));
				FLASH_Program(addr, WD_32, 4);
				return len;
			}
			memcpy(&WD_32[i], &WD_8, sizeof(uint32_t));
			i++;
		}

		for(; i<4; i++)
		{
			if(write_len > 4 )
			{
				memcpy(&WD_32[i], pbuf+(addr%4), sizeof(uint32_t));
				write_len -=4;
			}
			else
			{
				memcpy(&WD_32[i], pbuf+(addr%4), write_len);
				FLASH_Program(addr, WD_32, 4);
				return len;
			}
			pbuf += 4;
		}
		FLASH_Program(addr, WD_32, 4);
		write_cnt--;
		write_addr = (addr-addr%16)+16;
	}

	/*** 多次写入 ***/
	while(write_cnt)
	{
		for(i=0; i<4; i++)
		{
			if(write_len > 4 )
			{
				memcpy(&WD_32[i], pbuf, sizeof(uint32_t));
				write_len -=4;
				pbuf += 4;
			}
			else
			{
				memcpy(&WD_32[i], pbuf, write_len);
				FLASH_Program(write_addr, WD_32, 4);
				return len;
			}
		}                
		FLASH_Program(write_addr, WD_32, 4);
		write_cnt--;
		write_addr +=16;
		memcpy(WD_32,DEF_DATA,sizeof(WD_32));
	}

	return len;
}

/**
 * @brief flash读
 *
 * @param [in] var          ///< 预留
 * @param [in] addr         ///< 读地址
 * @param [in] p_dest       ///< 读出数据目标地址
 * @param [in] len          ///< 读出长度
 *
 * @return uint32_t         ///< 读出长度
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
uint32_t bsp_flash_read(void *var, uint32_t addr, uint8_t *p_dest, uint32_t len)
{
	if(len == 0){return 0;}

	uint32_t i = 0;
	uint32_t read_addr = addr;
	uint32_t read_cnt = (((addr%4)+3)/4)+(len-(addr%4)+3/4);
	uint32_t read_len = len;
	uint32_t DATA_32 = 0;

	// 头读取	
	if(addr%4 != 0)
	{
		uint32_t first_read_len = addr%4;
		DATA_32 = FLASH_Read(addr+4*i);
		if(read_len > first_read_len)
		{
			for(i=0; i<first_read_len; i++)
			{
				p_dest[i] = DATA_32;
				DATA_32 >>= 8;			
			}
			read_len -= first_read_len;
			p_dest += first_read_len;
		}
		else
		{
			for(i=0; i<read_len; i++)
			{
				p_dest[i] = DATA_32;
				DATA_32 >>= 8;			
			}
			return len;
		}
		read_cnt--;
		read_addr += first_read_len;
	}

	// 多次读取
	while(read_cnt)
	{
		DATA_32 = FLASH_Read(read_addr);
		if(read_len > 4)
		{
			for(i=0; i<4; i++)
			{
				p_dest[i] = DATA_32;
				DATA_32 >>= 8;			
			}
			read_len -= 4;
			p_dest += 4;
		}
		else
		{
			for(i=0; i<read_len; i++)
			{
				p_dest[i] = DATA_32;
				DATA_32 >>= 8;			
			}
			return len;
		}		
		read_cnt--;
		read_addr += 4;
	}

	return len;
}


/**
 * @brief
 *
 * @param [in] var           ///< 预留
 * @param [in] address       ///< 擦除起始地址
 * @param [in] len           ///< 擦除长度
 *
 * @return uint32_t          ///< 擦除长度
 *  @retval
 * @details 特殊说明:        ///< 最小擦除单元为页擦除（一页大小为1K）
 * @par eg:
 * @code
 *
 * @endcode
 */
uint32_t bsp_flash_erase(void *var, uint32_t address, uint32_t len)
{
    uint8_t erase_block_num = 0;
	uint32_t i;

    /* 判断擦除地址是否在片区范围内 */
    if ((address < FLASH_ADDR) || (address > FLASH_ADDR + FLASH_SIZE))
    {
        //        DBG_E("bsp_flash_erase error : Address is exceeded:0x%x ", address);
        return 1;
    }
    if ((address + len) > FLASH_ADDR + FLASH_SIZE)
    {
        //        DBG_E("bsp_flash_erase error : erase len is exceeded:0x%x ", (address + len));
        return 1;
    }

    /* 擦除页数计算，不够一页的按一页计算 */
	if((address % FLASH_BLOCK_SIZE) == 0)
	{
		erase_block_num = (len-1) / FLASH_BLOCK_SIZE;
		for(i=0; i<= erase_block_num; i++)
		{
			FLASH_EraseSector(address+i*FLASH_BLOCK_SIZE);
		}
	}else {
		erase_block_num = ((len-1) + (address % FLASH_BLOCK_SIZE)) / FLASH_BLOCK_SIZE;
		if (erase_block_num == 0)
		{
			return 1;
		}
		for(i=0; i< erase_block_num; i++)
		{
			FLASH_EraseSector(address+(i+1)*FLASH_BLOCK_SIZE);
		}
	}
    return 0;
}

/**
 * @brief 获取flash存储单元长度
 *
 *
 * @return uint32_t          // 返回2：2字节，返回1：1字节
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
uint32_t bsp_flash_get_byte_len(void *var)
{
    return 1;
}



