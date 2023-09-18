/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** Created by:              Chen JunTong
** Created date:            2022-03-13
** Version:                 V1.0.0
** Descriptions:            flash 参数管理
**
**------File Info----------------------------------------------
** File name:               flash_param.c
** Latest modified date:    2022-03-13
** Latest version:          V1.0.0
** Description:             flash 参数管理
**
*************************************************************/
#include "x_frame.h"


typedef struct _FLASH_PARAM_HEAD_OBJ{
    list_t     param_head;
    list_t     part_id;
}FLASH_PARAM_HEAD_OBJ, *P_FLASH_PARAM_HEAD_OBJ;

FLASH_PARAM_PART_OBJ g_flash_param_part_obj;

static FLASH_PARAM_HEAD_OBJ flash_param_head = {.param_head = LIST_HEAD_INIT(flash_param_head.param_head)};


/* 定义扇区信息结构体 */
#define DEF_SECTOR_INFO(name)																				\
		char name_##buf[FLASH_PARAM_NAME_MAX_LEN];															\
		FLASH_PARAM_SECTOR_INFO_OBJ name = {.info_head = {.name_len = FLASH_PARAM_NAME_MAX_LEN},name_##buf};

/*************************************************************
** Descriptions:        链表初始化
*************************************************************/
static void list_init(void)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    uint32_t i = 0;
//    INIT_LIST_HEAD(&flash_param_head.param_head);
    while(1) {
        p_item = (P_FLASH_PARAM_OBJ)auto_reg_get_func_cont_by_index(AUTO_REG_FLASH_PARAM, i);
        if(p_item == NULL) {
            break;
        }
        INIT_LIST_HEAD(&p_item->list);
        list_add_tail(&p_item->list, &flash_param_head.param_head);
		i++;
		uint16_t cir = 0;
		uint16_t new_part_mark = true;
		for(;cir < g_flash_param_part_obj.part_tab_size;cir++){
			if(p_item->part_id == g_flash_param_part_obj.p_part_tab[cir].part_id){
				new_part_mark = false;
				break;
			}
		}
		if(new_part_mark == true){
			if(g_flash_param_part_obj.part_tab_size == FLASH_PARAM_PART_ID_MAX_NUM){
				DBG_E_BLOCK(50,"part id tab size limit, max size: %d",FLASH_PARAM_PART_ID_MAX_NUM);
                return;
			}
			g_flash_param_part_obj.p_part_tab[g_flash_param_part_obj.part_tab_size].part_id = p_item->part_id;
			g_flash_param_part_obj.part_tab_size++;
		}
	}
}

/*************************************************************
** Descriptions:        读取信息区域
*************************************************************/
static bool read_info(uint32_t part_id,uint32_t addr,P_FLASH_PARAM_SECTOR_INFO_OBJ p_info)
{
	uint16_t crc = 0;
	uint16_t name_len = p_info->info_head.name_len;
	flash_part_read(part_id,addr,(uint8_t*)p_info,sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ));
	crc = calc_chk_val(u_chk_crc16,((uint8_t*)&p_info->info_head) + 2,sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ) - 2);
	if(crc != p_info->info_head.head_crc){
		return false;
	}
	if(name_len < p_info->info_head.name_len){
		return false;
	}
	memset(p_info->p_name, 0, name_len);
	flash_part_read(part_id,addr + sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ),(uint8_t*)p_info->p_name,p_info->info_head.name_len);
	crc = calc_chk_val(u_chk_crc16,(uint8_t*)p_info->p_name,p_info->info_head.name_len);
	if(crc != p_info->info_head.name_crc){
		return false;
	}
	return true;
}

/*************************************************************
** Descriptions:        搜索参数
*************************************************************/
static P_FLASH_PARAM_OBJ search_param(uint32_t part_id, char *p_name, uint16_t name_len)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
		if(part_id != p_item->part_id){
			continue;
		}
        if(name_len != p_item->name_len){
			continue;
		}
		if(strcmp(p_name,p_item->p_name) != 0){
			continue;
		}
		return p_item;
    }
	return NULL;
}

/*************************************************************
** Descriptions:        填充参数信息区域
*************************************************************/
static void fill_param_info(P_FLASH_PARAM_OBJ p_item,P_FLASH_PARAM_SECTOR_INFO_HEAD_OBJ p_head_info,uint32_t save_index)
{
	p_head_info->data_area_size = CAL_FLASH_ALIGN_LEN(p_item->param_len + 2);
	p_head_info->data_len = p_item->param_len;
	p_head_info->info_area_size = CAL_FLASH_ALIGN_LEN(p_item->name_len + sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ));
	p_head_info->name_crc = calc_chk_val(u_chk_crc16,(uint8_t*)p_item->p_name,p_item->name_len);
	p_head_info->save_index = save_index;
	p_head_info->name_len = p_item->name_len;
	p_head_info->head_crc = calc_chk_val(u_chk_crc16,((uint8_t*)p_head_info) + 2,sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ) - 2);
}

/*************************************************************
** Descriptions:        读取数据
*************************************************************/
static FLASH_PARAM_STA_ID read_data(uint32_t part_id, uint32_t start_addr, uint32_t data_len, uint8_t *p_buf, uint16_t len)
{
	uint8_t data[300];
	int32_t remain_len;
	uint16_t crc16 = 0;
	uint16_t cal_crc16 = 0;
	uint16_t read_len;
	if(data_len <= 2){
		DBG_E("data len less than 2!!!");
		return FLASH_PARAM_LEN_LIMIT;
	}
	flash_part_read(part_id,start_addr,(uint8_t*)&crc16,2);
	start_addr += 2;
	remain_len = data_len;
	cal_crc16 = 0;
	while(remain_len > 0){
		read_len = X_FRAME_MIN(300,remain_len);
		flash_part_read(part_id,start_addr,data,read_len);
		cal_crc16 = calc_chk_init_val(u_chk_crc16,cal_crc16,data,read_len);
		remain_len -= read_len;
		start_addr += read_len;
		if(p_buf  == NULL){
			continue;
		}
		read_len = X_FRAME_MIN(read_len,len);
		memcpy(p_buf,data,read_len);
        p_buf += read_len;
		len = len - read_len;
	}
	if(cal_crc16 != crc16){
		DBG_E("param crc err!!!");
		return FLASH_PARAM_CRC_ERR;
	}
	return FLASH_PARAM_SUCCESS;
}


/*************************************************************
** Descriptions:        写入数据
*************************************************************/
static FLASH_PARAM_STA_ID write_data(uint32_t part_id, uint32_t start_addr, uint32_t data_len, uint8_t *p_buf)
{
	uint16_t cal_crc16 = 0;
	uint8_t data[CAL_FLASH_ALIGN_LEN(sizeof(cal_crc16))] = {0};
	uint16_t one_part_save_len = sizeof(data);
	uint16_t one_part_data_len = sizeof(cal_crc16);
	uint16_t one_part_add_len = one_part_save_len - one_part_data_len;
	uint16_t two_part_save_len = data_len - (one_part_save_len - one_part_data_len);
	
	if(data_len <= 2){
		DBG_E("data len less than 2!!!");
		return FLASH_PARAM_LEN_LIMIT;
	}
	cal_crc16 = calc_chk_init_val(u_chk_crc16,cal_crc16,p_buf,data_len);
	memcpy(data,&cal_crc16,one_part_data_len);
	memcpy(data + one_part_data_len,p_buf,one_part_add_len);
	flash_part_write(part_id,start_addr,data,one_part_save_len);
	flash_part_write(part_id,start_addr + one_part_save_len,p_buf + one_part_add_len,two_part_save_len);
	return FLASH_PARAM_SUCCESS;
}

/*************************************************************
** Descriptions:        写入信息区域
*************************************************************/
static void write_info_area(uint32_t part_id, uint32_t start_addr, P_FLASH_PARAM_SECTOR_INFO_HEAD_OBJ p_head_info,char *p_name)
{
	uint8_t data[CAL_FLASH_ALIGN_LEN(sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ))] = {0};
	uint16_t one_part_save_len = sizeof(data);
	uint16_t one_part_data_len = sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ);
	uint16_t one_part_add_len = one_part_save_len - one_part_data_len;
	uint16_t two_part_save_len = p_head_info->name_len - (one_part_save_len - one_part_data_len);
	
	memcpy(data,p_head_info,one_part_data_len);
	memcpy(data + one_part_data_len,p_name,one_part_add_len);
	flash_part_write(part_id,start_addr,data,one_part_save_len);
	flash_part_write(part_id,start_addr + one_part_save_len,((uint8_t*)p_name) + one_part_add_len,two_part_save_len);
}

/*************************************************************
** Descriptions:        搜索参数个数
*************************************************************/
static uint16_t search_data_num(FLASH_PARAM_TYPE type, uint32_t part_id, uint32_t start_addr,uint32_t stop_addr,uint32_t data_area_size, uint16_t data_len)
{
	uint16_t data_num = 0;
	while((start_addr + data_area_size) <= stop_addr){
		if(read_data(part_id,start_addr,data_len,NULL,0) != FLASH_PARAM_SUCCESS){
			break;
		}
		start_addr = start_addr + data_area_size;
		data_num++;
		if(type == FLASH_NORMAL_PARAM){
			break;
		}
	}
	return data_num;
}

/*************************************************************
** Descriptions:        搜索区域的参数
*************************************************************/
static void search_part_param(void)
{
	uint16_t cir = 0;
	uint16_t index = 0;
	uint32_t sector_size = 0;
	uint32_t part_id = 0;
	uint32_t part_size;
	uint32_t cur_sector_addr;
	uint32_t next_sector_addr;
	uint16_t data_num;
	P_FLASH_PARAM_SECTOR_OBJ p_sector;
	P_FLASH_PARAM_OBJ p_param;
	for(;cir < g_flash_param_part_obj.part_tab_size;cir++){
		part_id = g_flash_param_part_obj.p_part_tab[cir].part_id;
		if(flash_part_is_exit(part_id) == false){
			return;
		}
		part_size = flash_part_get_part_size(part_id);
		sector_size = flash_part_get_sector_size(part_id);
		if(sector_size == 0){
			continue;
		}
		cur_sector_addr = 0;
		next_sector_addr = cur_sector_addr;
		/* 遍历flash */
		while((cur_sector_addr + sector_size) <= part_size){
			DEF_SECTOR_INFO(sector_info)
			cur_sector_addr = next_sector_addr;
			next_sector_addr = cur_sector_addr + sector_size;
			if(read_info(part_id,cur_sector_addr,&sector_info) == false){
				continue;
			}
			p_param = search_param(part_id, sector_info.p_name,sector_info.info_head.name_len);
			if(NULL != p_param)
				data_num = search_data_num(p_param->type,part_id,cur_sector_addr + sector_info.info_head.info_area_size,cur_sector_addr + sector_size,sector_info.info_head.data_area_size,sector_info.info_head.data_len);
			else
				data_num = 0;
			if(p_param != NULL && data_num != 0){
				/* 保存参数 */
				for(index = 0;index < p_param->sec_tab_size;index++){
					p_sector = &p_param->p_sec_tab[index];
					if(p_sector->init == true){
						continue;
					}
					memcpy(&p_sector->info,&sector_info.info_head,sizeof(FLASH_PARAM_SECTOR_INFO_HEAD_OBJ));
					p_sector->sector_offset = cur_sector_addr;
					p_sector->data_num = data_num;
					p_sector->init = true;
					break;		/* 直接跳出,最后一次不自增 */
				}
				if(index != p_param->sec_tab_size){
					continue;
				}
			}
			flash_part_erase_len(part_id,cur_sector_addr,sector_size);
		}
	}
}

/*************************************************************
** Descriptions:        搜索未使用的扇区
*************************************************************/
static bool search_unuse_sector(uint32_t part_id, uint32_t part_size, uint32_t sector_size, uint32_t *p_sector_addr)
{
	uint32_t cur_sector_addr;
	uint32_t next_sector_addr;
	DEF_SECTOR_INFO(sector_info)
	cur_sector_addr = *p_sector_addr;
	next_sector_addr = cur_sector_addr;
	/* 遍历flash */
	while((next_sector_addr + sector_size) <= part_size){
		cur_sector_addr = next_sector_addr;
		next_sector_addr = cur_sector_addr + sector_size;
		if(read_info(part_id,cur_sector_addr,&sector_info) == true){
			continue;
		}
		*p_sector_addr = cur_sector_addr;
		return true;
	}
	return false;
}

/*************************************************************
** Descriptions:        分配参数扇区
*************************************************************/
static void assort_param_sector(void)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
	uint16_t index = 0;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
	uint16_t cir = 0;
	uint32_t unuse_sector_addr;
	uint32_t sector_size = 0;
	uint32_t part_id = 0;
	uint32_t part_size;
	for(;cir < g_flash_param_part_obj.part_tab_size;cir++){
		cur_list = NULL;
		tag_list_head = &flash_param_head.param_head;
		unuse_sector_addr = 0;
		part_id = g_flash_param_part_obj.p_part_tab[cir].part_id;
		uint8_t is_part_err = false;
		is_part_err = is_part_err; /* 防止编译警告 */
		if(flash_part_is_exit(part_id) == false){
			continue;
		}
		part_size = flash_part_get_part_size(part_id);
		sector_size = flash_part_get_sector_size(part_id);
		if(sector_size == 0){
			continue;
		}
		
		list_for_each(cur_list, tag_list_head) {
			p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
			if(p_item->part_id != g_flash_param_part_obj.p_part_tab[cir].part_id){
				continue;
			}
			for(index = 0;index < p_item->sec_tab_size;index++){
				if(p_item->p_sec_tab[index].init == true){
					continue;
				}
				if(search_unuse_sector(part_id,part_size,sector_size,&unuse_sector_addr) == false){
					is_part_err = true;
					break;
				}
				p_item->p_sec_tab[index].sector_offset = unuse_sector_addr;
				p_item->p_sec_tab[index].data_num = 0;
				p_item->p_sec_tab[index].init = true;
				memset(&p_item->p_sec_tab[index].info,0,sizeof(p_item->p_sec_tab[index].info));
				unuse_sector_addr += sector_size;
			}
		}
		if(is_part_err == true){
			DBG_E_BLOCK(10,"pard %d is too small, some param have no sector", part_id);
		}
	}
}

/*************************************************************
** Descriptions:        参数排序，越先存储的参数，放在越前面
*************************************************************/
static void param_sort(void)
{
	FLASH_PARAM_SECTOR_OBJ sector_cache;
	P_FLASH_PARAM_SECTOR_OBJ p_sort_sector;
	P_FLASH_PARAM_SECTOR_OBJ p_cur_sector;
	uint16_t cut_save_index;
	P_FLASH_PARAM_OBJ p_item = NULL;
	list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
	uint16_t cir = 0;
	uint16_t cir1 = 0;
	list_for_each(cur_list, tag_list_head) {
		p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
		p_item->exec.save_index = 0;
		p_item->exec.save_sector = 0;
		for(cir = 0;cir < p_item->sec_tab_size;cir++){
			p_sort_sector = &p_item->p_sec_tab[cir];
			if(p_sort_sector->data_num == 0){
				break;
			}
			p_item->exec.save_index = p_sort_sector->info.save_index;
			p_item->exec.save_sector = cir;
			for(cir1 = cir + 1;cir1 < p_item->sec_tab_size;cir1++){
				p_cur_sector = &p_item->p_sec_tab[cir1];
				if(p_cur_sector->data_num == 0){
					break;
				}
				cut_save_index = p_cur_sector->info.save_index - p_sort_sector->info.save_index;
				if(cut_save_index < 0x7FFF){
					continue;
				}
				memcpy(&sector_cache,p_sort_sector,sizeof(FLASH_PARAM_SECTOR_OBJ));
				memcpy(p_sort_sector,p_cur_sector,sizeof(FLASH_PARAM_SECTOR_OBJ));
				memcpy(p_cur_sector,&sector_cache,sizeof(FLASH_PARAM_SECTOR_OBJ));
			}
		}
	}
}

/*************************************************************
** Descriptions:        参数读取
*************************************************************/
static FLASH_PARAM_STA_ID param_read(P_FLASH_PARAM_OBJ p_item, uint8_t *p_buf, uint16_t len)
{
	uint32_t read_addr;
	P_FLASH_PARAM_SECTOR_OBJ p_sector = &p_item->p_sec_tab[p_item->exec.save_sector];
	if(p_sector->data_num == 0){
		DBG_E("param %s is no data",p_item->p_name);
		return FLASH_PARAM_NO_DATA;
	}
	read_addr = p_sector->sector_offset + p_sector->info.info_area_size + (p_sector->data_num - 1) * p_sector->info.data_area_size;
	return read_data(p_item->part_id, read_addr, p_sector->info.data_len,p_buf,len);
}

/*************************************************************
** Descriptions:        参数读取
*************************************************************/
static FLASH_PARAM_STA_ID param_read_history(P_FLASH_PARAM_OBJ p_item, uint16_t history_index, uint8_t *p_buf, uint16_t len)
{
	uint16_t cir;
	uint16_t save_sector = p_item->exec.save_sector;
	P_FLASH_PARAM_SECTOR_OBJ p_sector;
	uint32_t read_addr;
	uint16_t data_index;
	bool read_mark = false;
	uint16_t cur_index = history_index;
	cur_index = cur_index + 1;
	for(cir = 0;cir < p_item->sec_tab_size;cir++){
		p_sector = &p_item->p_sec_tab[save_sector];
		save_sector = (save_sector + 1) % p_item->sec_tab_size;
		if(p_sector->data_num == 0){
			continue;
		}
		if(cur_index > p_sector->data_num){
			cur_index -= p_sector->data_num;
		}else{
			data_index = p_sector->data_num - cur_index;
			read_mark = true;
			break;
		}
	}
	if(read_mark == false){
        DBG_I("history param %s is no data, index: %d",p_item->p_name,history_index);
		return FLASH_PARAM_NO_DATA;
	}
	read_addr = p_sector->sector_offset + p_sector->info.info_area_size + data_index * p_sector->info.data_area_size;
	return read_data(p_item->part_id, read_addr, p_sector->info.data_len,p_buf,len);
}

/*************************************************************
** Descriptions:        正常参数写入
*************************************************************/
static FLASH_PARAM_STA_ID param_write(P_FLASH_PARAM_OBJ p_item, uint8_t *p_buf, uint16_t len)
{
	uint32_t data_addr;
	uint32_t sector_size = flash_part_get_sector_size(p_item->part_id);
	P_FLASH_PARAM_SECTOR_OBJ p_sector = &p_item->p_sec_tab[p_item->exec.save_sector];
	if(p_sector->init == false){
		DBG_E("part %d is too small,the param %s have no sector!!!",p_item->part_id,p_item->p_name);
		return FLASH_PARAM_NO_SECTOR;
	}
	if(p_sector->data_num != 0 
		&& (p_item->type == FLASH_NORMAL_PARAM 
				|| ((p_sector->data_num + 1) * p_sector->info.data_area_size + p_sector->info.info_area_size) > sector_size 
				|| len != p_sector->info.data_len)){
		p_item->exec.save_index = p_sector->info.save_index + 1;
		p_item->exec.save_sector = (p_item->exec.save_sector + 1) % p_item->sec_tab_size;
		p_sector = &p_item->p_sec_tab[p_item->exec.save_sector];
		if(p_sector->init == false){
			DBG_E("part %d is too small,the param %s have no sector!!!",p_item->part_id,p_item->p_name);
			return FLASH_PARAM_NO_SECTOR;
		}
		p_sector->data_num = 1;
		flash_part_erase_len(p_item->part_id,p_sector->sector_offset,sector_size);
		fill_param_info(p_item,&p_sector->info,p_item->exec.save_index);
		write_info_area(p_item->part_id,p_sector->sector_offset,&p_sector->info,p_item->p_name);
	}else if(p_sector->data_num == 0){
		p_item->exec.save_index = 0;
		p_sector->data_num++;
		flash_part_erase_len(p_item->part_id,p_sector->sector_offset,sector_size);
		fill_param_info(p_item,&p_sector->info,p_item->exec.save_index);
		write_info_area(p_item->part_id,p_sector->sector_offset,&p_sector->info,p_item->p_name);
	}else{
		p_sector->data_num++;
	}
	data_addr =  p_sector->sector_offset + p_sector->info.info_area_size + p_sector->info.data_area_size * (p_sector->data_num - 1);
	return write_data(p_item->part_id, data_addr, len, p_buf);
}

/*************************************************************
** Descriptions:        保存所有参数
*************************************************************/
void real_flash_param_save_all(void)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
        if(p_item->p_param == NULL){
			continue;
		}
		param_write(p_item,p_item->p_param,p_item->param_len);
    }
}

/*************************************************************
** Descriptions:        恢复默认参数
*************************************************************/
void real_flash_param_rd_all(void)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
        if(p_item->p_param == NULL){
			continue;
		}
        if(p_item->p_def_param == NULL){
            continue;
        }
		param_write(p_item,(uint8_t*)p_item->p_def_param,p_item->param_len);
    }
}

/*************************************************************
** Descriptions:        读取所有参数
*************************************************************/
void real_flash_param_read_all(void)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
        if(p_item->p_param == NULL){
			continue;
		}
		param_read(p_item,p_item->p_param,p_item->param_len);
    }
}

/*************************************************************
** Descriptions:        保存区域的所有参数
*************************************************************/
void real_flash_param_id_save_all(uint32_t part_id)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
        if(p_item->p_param == NULL){
			continue;
		}
		if(p_item->part_id != part_id){
			continue;
		}
		param_write(p_item,p_item->p_param,p_item->param_len);
    }
}

/*************************************************************
** Descriptions:        恢复区域的所有参数
*************************************************************/
void real_flash_param_id_rd_all(uint32_t part_id)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
        if(p_item->p_param == NULL){
			continue;
		}
		if(p_item->part_id != part_id){
			continue;
		}
        if(p_item->p_def_param == NULL){
            continue;
        }
		param_write(p_item,(uint8_t*)p_item->p_def_param,p_item->param_len);
    }
}

/*************************************************************
** Descriptions:        读取区域的所有参数
*************************************************************/
void real_flash_param_id_read_all(uint32_t part_id)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
        if(p_item->p_param == NULL){
			continue;
		}
		if(p_item->part_id != part_id){
			continue;
		}
		param_read(p_item,p_item->p_param,p_item->param_len);
    }
}

/*************************************************************
** Descriptions:        保存参数
*************************************************************/
FLASH_PARAM_STA_ID real_flash_param_save(X_UID_TYPE param_name, uint8_t *p_buf, uint32_t len)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
		if(p_item->param_name != param_name){
			continue;
		}
		if(p_item->param_len != len){
			DBG_E("param %s len not equal to reg len!!!",p_item->p_name);
			return FLASH_PARAM_LEN_LIMIT;
		}
		return param_write(p_item,p_buf,len);
    }
	DBG_E("param %s is not exit!!!",p_item->p_name);
	return FLASH_PARAM_NO_PARAM;
}

/*************************************************************
** Descriptions:        恢复默认参数
*************************************************************/
FLASH_PARAM_STA_ID real_flash_param_rd(X_UID_TYPE param_name)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
		if(p_item->param_name != param_name){
			continue;
		}
        if(p_item->p_def_param == NULL){
            return FLASH_PARAM_NO_DEF_PARAM;
        }
		return param_write(p_item,(uint8_t*)p_item->p_def_param,p_item->param_len);
    }
	DBG_E("param %s is not exit!!!",p_item->p_name);
	return FLASH_PARAM_NO_PARAM;
}

/*************************************************************
** Descriptions:        读取参数
*************************************************************/
FLASH_PARAM_STA_ID real_flash_param_read(X_UID_TYPE param_name, uint8_t *p_buf, uint32_t len)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
		if(p_item->param_name != param_name){
			continue;
		}
		return param_read(p_item,p_buf,len);
    }
	DBG_E("param %s is not exit!!!",p_item->p_name);
	return FLASH_PARAM_NO_PARAM;
}

/*************************************************************
** Descriptions:        读取历史队列数据
*************************************************************/
FLASH_PARAM_STA_ID real_flash_queue_param_read_history(X_UID_TYPE param_name, uint16_t history_index, uint8_t *p_buf, uint32_t len)
{
	P_FLASH_PARAM_OBJ p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &flash_param_head.param_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, FLASH_PARAM_OBJ, list);
		if(p_item->param_name != param_name){
			continue;
		}
		return param_read_history(p_item,history_index,p_buf,len);
    }
	DBG_E("param %s is not exit!!!",p_item->p_name);
	return FLASH_PARAM_NO_PARAM;
	
}
	
/*************************************************************
** Descriptions:        参数初始化
** Input parameters:    p_frame_info：协议参数指针
*************************************************************/
static void flash_param_init(void)
{
	list_init();
	search_part_param();
	assort_param_sector();
	param_sort();
	real_flash_param_read_all();
}

REG_INIT_MOD_BASIC(flash_param_init);

/*************************************************************
** Descriptions:        恢复所有的flash 参数
*************************************************************/
int32_t rd_flash_param_all(hal_frame_info_t *p_frame_info)
{
    real_flash_param_rd_all();
    protocol_send_ack(p_frame_info, NULL, 0);
    return 0;
}



