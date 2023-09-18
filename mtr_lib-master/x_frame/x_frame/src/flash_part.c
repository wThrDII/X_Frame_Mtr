


#include "x_frame.h"
#include "xh_flash.h"

static list_t flash_part_head_list = LIST_HEAD_INIT(flash_part_head_list);

typedef struct
{
	flash_part_core_t *p_flash_part;
	part_item_t	*p_part_item;
}flash_part_info;

#define FLASH_PART_ERR 0xffffffff

static void flash_part_init(void)
{
	flash_part_core_t *p_flash_part;
    uint32_t i = 0;
//    INIT_LIST_HEAD(&flash_part_head_list);
    while(1) {
        p_flash_part = (flash_part_core_t*)auto_reg_get_func_cont_by_index(AUTO_REG_FLASH_PART, i);
        if(p_flash_part == NULL) {
            break;
        }
        INIT_LIST_HEAD(&p_flash_part->list);
        list_add_tail(&p_flash_part->list, &flash_part_head_list);
		i++;
        
//		DBG_I_BLOCK(20,"add flash part drv: %d",p_flash_part->drv_id);
		p_flash_part->p_part_item[0].part_address = p_flash_part->start_address;
        uint16_t item = 1;
		for (item = 1; item < p_flash_part->part_num; item++)
		{
			p_flash_part->p_part_item[item].part_address =
				p_flash_part->p_part_item[item - 1].part_address + p_flash_part->p_part_item[item - 1].part_size;
		}
	}
}

REG_INIT_MOD_SETUP(flash_part_init);

part_item_t *get_flash_item_by_part_id(uint32_t part_id)
{
	uint32_t i = 0;
	flash_part_core_t *p_flash_part;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_part_head_list;
    list_for_each(cur_list, tag_list_head) {
        p_flash_part = list_entry(cur_list, flash_part_core_t, list);
		for (i = 0; i < p_flash_part->part_num; i++)
		{
			if (part_id == p_flash_part->p_part_item[i].part_id) { return &p_flash_part->p_part_item[i]; }
		}
    }
    DBG_E_LIMIT(1000, "no part id: %d", part_id);
	return NULL;
}


static bool get_flash_info_by_part_id(uint32_t part_id,flash_part_info *p_part_info)
{
	uint32_t i = 0;
	flash_part_core_t *p_flash_part_core;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_part_head_list;
    list_for_each(cur_list, tag_list_head) {
        p_flash_part_core = list_entry(cur_list, flash_part_core_t, list);
		for (i = 0; i < p_flash_part_core->part_num; i++)
		{
			if (part_id == p_flash_part_core->p_part_item[i].part_id) {
				p_part_info->p_flash_part = p_flash_part_core;
				p_part_info->p_part_item = &p_flash_part_core->p_part_item[i];
				return true;
			}
		}
    }
    DBG_E_LIMIT(1000,"no part id: %d", part_id);
	return false;
}

bool flash_part_is_exit(uint32_t part_id)
{
	part_item_t *p_part_item = NULL;
	p_part_item = get_flash_item_by_part_id(part_id);
	if(p_part_item == NULL){
		return false;
	}
	return true;
}

uint32_t flash_part_get_part_size(uint32_t part_id)
{
	part_item_t *p_part_item = NULL;
	p_part_item = get_flash_item_by_part_id(part_id);
	if(p_part_item == NULL){
		return 0;
	}
	return p_part_item->part_size;
}

uint32_t flash_part_get_sector_size(uint32_t part_id)
{
	flash_part_info part_info;
    if(get_flash_info_by_part_id(part_id,&part_info) == false){
		return 0;
	}
	return part_info.p_flash_part->block_size;
}


bool flash_part_erase(uint32_t part_id)
{
	flash_part_info part_info;
    if(get_flash_info_by_part_id(part_id,&part_info) == false){
		return false;
	}
    xh_flash_erase(part_info.p_flash_part->drv_id, part_info.p_part_item->part_address, part_info.p_part_item->part_size);
	return true;
} 

bool flash_part_erase_len(uint32_t part_id, uint32_t offset, uint32_t len)
{
	uint32_t addr = 0;
	flash_part_info part_info;
    if(get_flash_info_by_part_id(part_id,&part_info) == false){
		return false;
	}
	if((offset + len) > part_info.p_part_item->part_size){
		return false;
	}
	addr = part_info.p_part_item->part_address + offset;
    xh_flash_erase(part_info.p_flash_part->drv_id, addr, len);
	
	return true;
}

bool flash_part_write(uint32_t part_id, uint32_t offset, uint8_t *pbuf, uint32_t len)
{
    flash_part_info part_info;
    if(get_flash_info_by_part_id(part_id,&part_info) == false){
		return false;
	}
	if((offset + len) > part_info.p_part_item->part_size){
		return false;
	}
	offset = offset + part_info.p_part_item->part_address;
	
    xh_flash_write(part_info.p_flash_part->drv_id, offset, pbuf, len);
	return true;
}

bool flash_part_read(uint32_t part_id, uint32_t offset, uint8_t *pbuf, uint32_t len)
{
    flash_part_info part_info;
    if(get_flash_info_by_part_id(part_id,&part_info) == false){
		return false;
	}
	if((offset + len) > part_info.p_part_item->part_size){
		return false;
	}
	offset = offset + part_info.p_part_item->part_address;
    xh_flash_read(part_info.p_flash_part->drv_id, offset, pbuf, len);
	return true;
}

#if TINY_FUNC_CODE == 0

// int32_t show_flash_part(uint8_t argc, uint8_t *argv[])
// {
// 	uint32_t i = 0;
// 	if(get_flash_info_by_part_id(part_id,&part_info) == false){
// 		return 0;
// 	}
// 	flash_part_core_t *p_flash_part = p_flash_part;
//     for(i=0;i<p_flash_part->part_num;i++)
// 	{
// 		DBG_I("FLASH_PART: %d   addr:0x%x   size:%8d   name:%s", p_flash_part->p_part_item[i].part_id,p_flash_part->p_part_item[i].part_address,p_flash_part->p_part_item[i].part_size,p_flash_part->p_part_item[i].part_name  );
// 	}

//     return RET_OK;
// }

// REG_SHELL_CMD_X(show_flash_part, 0, show_flash_part, "show_flash_part!!", "show_flash_part \r\n ");


// int32_t flash_part_addr(uint8_t argc, uint8_t *argv[])
// {
// 	if(get_flash_info_by_part_id(part_id,&part_info) == false){
// 		return 0;
// 	}
// 	flash_part_core_t *p_flash_part = p_flash_part;
// 	if(argc == 2)
//     {
//         uint32_t part_id = 0;
// 		part_item_t* part_item = NULL;
//         sscanf((const char *)argv[1], "%d", &part_id);
//         part_item = get_flash_item_by_part_id(part_id);
// 		if(part_item!=NULL)
// 		{
// 			DBG_I("part id:%d  addr:0x%x",part_id, part_item->part_address);
// 		}
//     }
//     return RET_OK;
// }

// REG_SHELL_CMD(flash_part_addr, 0, flash_part_addr, "flash_part_addr!!", "flash_part_addr \r\n ");


// int32_t flash_part_e(uint8_t argc, uint8_t *argv[])
// {
//     uint32_t part_id = 0;

//     if(argc >= 2)
//     {
//         sscanf((const char *)argv[1], "%d", &part_id);

//         DBG_I("flash_part_e part:%d", part_id);
//         flash_part_erase(part_id);
//     }
//     else
//     {
//         DBG_W("flash_part_e param error! eg: flash_part_e part_id");
//     }
//     return RET_OK;
// }

// REG_SHELL_CMD(flash_part_e, 0, flash_part_e, "flash_part_e !!", "eg:flash_part_e part_id \r\n ");

// int32_t flash_part_w(uint8_t argc, uint8_t *argv[])
// {
//     uint32_t part_id = 0;
// 	uint32_t offset  = 0;
// 	uint32_t len = 0;
// 	uint8_t  read_buf[100];

//     if(argc >= 4)
//     {
//         sscanf((const char *)argv[1], "%d", &part_id);
// 		sscanf((const char *)argv[2], "%d", &offset);
//         len = strlen((const char *)argv[3]);

//         flash_part_write(part_id, offset,argv[3],len);
// 		len = (len<90)?len:90;
// 		flash_part_read(part_id, offset,read_buf,len);
//         DBG_I("flash_part_w part_id:%d offset: %d ", part_id, offset);
// 		show_buf(read_buf,len);
//     }
//     else
//     {
//         DBG_W("flash_part_w param error! eg: flash_part_w part_id offset str");
//     }
//     return RET_OK;
// }

// REG_SHELL_CMD(flash_part_w, 0, flash_part_w, "flash_part_w!!", "eg:flash_part_w part_id offset str \r\n ");


// int32_t flash_part_r(uint8_t argc, uint8_t *argv[])
// {
//     uint32_t part_id = 0;
// 	uint32_t offset  = 0;
// 	uint32_t len = 0;
// 	uint8_t  read_buf[100];

//     if(argc >= 4)
//     {
//         sscanf((const char *)argv[1], "%d", &part_id);
// 		sscanf((const char *)argv[2], "%d", &offset);
// 		sscanf((const char *)argv[3], "%d", &len);
// 		len = (len<90)?len:90;
// 		flash_part_read(part_id, offset,read_buf,len);
//         DBG_I("flash_part_r part_id:%d offset: %d  ", part_id, offset);
// 		show_buf(read_buf,len);
//     }
//     else
//     {
//         DBG_W("flash_part_r ! eg: flash_part_r id  len");
//     }
//     return RET_OK;
// }

// REG_SHELL_CMD(flash_part_r, 0, flash_part_r, "flash_part_r read!!", "eg:flash_part_r id  len \r\n ");

#endif


