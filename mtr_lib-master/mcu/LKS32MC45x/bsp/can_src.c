
#include "can_src.h"

#define CAN_PACK_VALID_DATA_LEN         (CAN_SRC_PACK_DAT_LEN-1)        //CAN帧中，有效数据长度
#define CAN_SRC_PACK_TAG                0                               //标识SRC类型的CAN包

#define CAN_PACK_WHOLE_FLG              1

static P_CAN_SRC_CACHE_OBJ get_empty_cache(P_CAN_SRC_OBJ p_obj);
static void can_src_rece_norm_pack(P_CAN_SRC_OBJ p_obj,P_CAN_SRC_CACHE_OBJ p_rece_list);
static void free_son_cache(P_CAN_SRC_OBJ p_obj, int type, P_CAN_SRC_CACHE_OBJ p_free_list);
static void can_src_free_timeout_pack(P_CAN_SRC_OBJ p_obj);

/*************************************************************
** Function name:       can_src_init
** Descriptions:        CAN对象初始化
** Input parameters:    
    @p_obj: CAN对象
** Returned value:      None
** Remarks:             None
*************************************************************/
void can_src_init(P_CAN_SRC_OBJ p_obj)
{
    P_CAN_SRC_CACHE_OBJ p_cur_list = &p_obj->exec.empt_list;
    uint16_t cir = 0;
    for(;cir < p_obj->r_cache_num;cir++){
        p_cur_list->p_head_list = p_obj->p_r_cache + cir;
        p_cur_list = p_cur_list->p_head_list;
    }
    p_cur_list->p_head_list = NULL;
    p_obj->exec.free_cache = p_obj->r_cache_num;
    p_obj->exec.norm_list.p_head_list = NULL;
}

/*************************************************************
** Function name:       can_src_task
** Descriptions:        CAN拼包任务
** Input parameters:    
    @p_obj: CAN对象
** Returned value:      None
** Remarks:             None
*************************************************************/
void can_src_task(P_CAN_SRC_OBJ p_obj)
{
    can_src_free_timeout_pack(p_obj);
}
        
/*************************************************************
** Function name:       can_src_rece_pack
** Descriptions:        CAN帧接收
** Input parameters:    
    @p_obj: CAN对象
    @pack_type: 
        CAN_ID_TYPE_STANDARD: CANID为标准帧
        CAN_ID_TYPE_EXTEND：CANID为扩展帧
    @p_pack: CAN数据
    @pack_len：CAN数据长度。不超过8字节。
** Returned value:      None
** Remarks:             None
*************************************************************/
void can_src_rece_pack(P_CAN_SRC_OBJ p_obj, uint32_t pack_type, uint32_t pack_id, uint8_t *p_pack, uint32_t pack_len)
{
    uint16_t cir = 0;
    /* 数据为标准帧时，直接返回 */
    if(pack_type == CAN_ID_TYPE_STANDARD){
        return;
    }

    /* 不接收非旧拼包类型的帧 */
    CAN_SRC_ID_OBJ *p_id = (CAN_SRC_ID_OBJ *)&pack_id;
    if(p_id->ver != CAN_SRC_PACK_TAG){
        return;
    }
    
    /* 申请空块 */
    P_CAN_SRC_CACHE_OBJ p_empty_list = get_empty_cache(p_obj);
    if(p_empty_list == NULL){
        return;
    }
    
    for(cir = 0; cir < pack_len; cir++){
        p_empty_list->pack.data[cir] = p_pack[cir];
    }
    p_empty_list->pack.len = pack_len;
    
    /* 有非对齐风险,但比较高效，因此结构体id必须保证地址4字节对齐 */
    p_empty_list->pack.id = *p_id;
    p_empty_list->rece_time = get_sys_ms();
    can_src_rece_norm_pack(p_obj,p_empty_list);
}

/*************************************************************
** Function name:       can_src_send_data
** Descriptions:        应用层流数据发送。只能V2协议
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t can_src_send_data(P_CAN_SRC_OBJ p_obj, uint8_t *p_data,uint32_t len)
{
    uint32_t i;
    uint32_t pack_total;
    CAN_SRC_ID_OBJ can_src_id;
    uint8_t dest;
    uint8_t * p_data_end = p_data + len;
    uint32_t send_len = 0;
    
    /*
     *目的地址直接解析V2协议得到。因此此驱动不能用于除V2之外的其他协议。
     *原有拼包方式，可能会用到CAN的过滤器功能，因此需要把目的地址填上。拼包本身并不需要目的地址。
     */
    if(len > 14)
        dest = p_data[13];
    else
        dest = 1;
    
    memset(&can_src_id, 0, sizeof(can_src_id));
    
    pack_total = len / CAN_PACK_VALID_DATA_LEN;
    pack_total += (0 == (len % CAN_PACK_VALID_DATA_LEN)) ? 0 : 1;

    can_src_id.ver = CAN_SRC_PACK_TAG;
    can_src_id.src = p_obj->dev_addr;
    can_src_id.dest = dest;
    can_src_id.total_pack = pack_total;

    for(i = 0; i < pack_total; i++)      //分包
    {
        uint32_t pack_len;
        uint8_t *p_real_buf;
        
        uint8_t pack_buf[CAN_SRC_PACK_DAT_LEN];

        can_src_id.curr_no = i + 1;
        p_real_buf = ((uint8_t*)&p_data[0]) + CAN_PACK_VALID_DATA_LEN * i;

        pack_len = p_data_end - p_real_buf;
        pack_len = X_FRAME_MIN(pack_len, CAN_PACK_VALID_DATA_LEN);
        
        pack_buf[0] = pack_len;
        memcpy(&pack_buf[1], p_real_buf, pack_len);
        if(!xh_can_write(p_obj->can_drv_id, CAN_ID_TYPE_EXTEND, *((uint32_t*)(&can_src_id)), pack_buf, pack_len + 1)){
            return pack_len;
        }
        
        send_len += pack_len;
    }
    
    return send_len;
}

/*************************************************************
** Function name:       can_src_read_data
** Descriptions:        应用层读取数据
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t can_src_read_data(P_CAN_SRC_OBJ p_obj, uint8_t *p_data,uint32_t len)
{
    P_CAN_SRC_CACHE_OBJ p_last_list = &p_obj->exec.norm_list;
    P_CAN_SRC_CACHE_OBJ p_cur_list;
    while (1){
        p_cur_list = p_last_list->p_head_list;
        if(p_cur_list == NULL){
            return 0;
        }
        if(p_cur_list->pack.id.whole != CAN_PACK_WHOLE_FLG){
            p_last_list = p_cur_list;
            continue;
        }
        p_last_list->p_head_list = p_cur_list->p_head_list;
        break;
    }
    uint16_t read_len = 0;
    P_CAN_SRC_CACHE_OBJ p_read_pack = p_cur_list;
    while(1){
        uint32_t pack_len;
        if(p_read_pack == NULL){
            break;
        }
        pack_len = p_read_pack->pack.data[0];
        if((read_len + pack_len) > len){
			memcpy(p_data,p_read_pack->pack.data,len);
			read_len += len;
            break;
        }
        memcpy(p_data,&(p_read_pack->pack.data[1]),pack_len);
        p_data += pack_len;
        read_len += pack_len;
        p_read_pack = p_read_pack->p_son_list;
    }
    free_son_cache(p_obj,0,p_cur_list);
    
    return read_len;
}

/*************************************************************
** Function name:       can_src_set_baudrate
** Descriptions:        设置波特率
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t can_src_set_baudrate(P_CAN_SRC_OBJ p_obj, uint32_t baudrate)
{
    return 0;
}

/*************************************************************
** Function name:       get_empty_cache
** Descriptions:        获取空缓存
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static P_CAN_SRC_CACHE_OBJ get_empty_cache(P_CAN_SRC_OBJ p_obj)
{
    P_CAN_SRC_CACHE_OBJ p_sec_empty_list = p_obj->exec.empt_list.p_head_list->p_head_list;
    P_CAN_SRC_CACHE_OBJ p_get_empty_list = p_sec_empty_list->p_head_list;
    if(p_get_empty_list == NULL || p_sec_empty_list == NULL){
        return NULL;
    }
    p_sec_empty_list->p_head_list = p_get_empty_list->p_head_list;
    p_get_empty_list->p_head_list = NULL;
    p_get_empty_list->p_son_list = NULL;
    p_get_empty_list->rece_time = 0;
    return p_get_empty_list;
}

/*************************************************************
** Function name:       can_src_rece_norm_pack
** Descriptions:        接收正常包
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_src_rece_norm_pack(P_CAN_SRC_OBJ p_obj,P_CAN_SRC_CACHE_OBJ p_rece_list)
{
    /* 查找需要放入的数据表头 */
    P_CAN_SRC_CACHE_OBJ p_rece_head_list = &p_obj->exec.norm_list;
    P_CAN_SRC_CACHE_OBJ p_cur_head_list = NULL;
    while (1){
        p_cur_head_list = p_rece_head_list->p_head_list;
        /* 遍历至缓存尾，直接填充 */
        if(p_cur_head_list == NULL){
            p_rece_head_list->p_head_list = p_rece_list;
            if(p_rece_list->pack.id.curr_no == p_rece_list->pack.id.total_pack){
                p_rece_list->pack.id.whole = CAN_PACK_WHOLE_FLG;
            }
            return;
        }
        /* 遍历是否具备相同源地址的pack */
        if(p_cur_head_list->pack.id.whole != CAN_PACK_WHOLE_FLG
            && p_cur_head_list->pack.id.src == p_rece_list->pack.id.src){
            break;
        }
        p_rece_head_list = p_cur_head_list;
    }
    
    /* 将数据放入子列表相应位置 */
    P_CAN_SRC_CACHE_OBJ p_cur_list = p_rece_head_list->p_head_list;
    while (1){
        if(p_cur_list->p_son_list == NULL){
            p_cur_list->p_son_list = p_rece_list;
            if(p_rece_list->pack.id.curr_no == p_rece_list->pack.id.total_pack){
                p_rece_head_list->p_head_list->pack.id.whole = CAN_PACK_WHOLE_FLG;
            }
            return;
        }
        p_cur_list = p_cur_list->p_son_list;
    }
}

/*************************************************************
** Function name:       free_son_cache
** Descriptions:        释放缓存
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void free_son_cache(P_CAN_SRC_OBJ p_obj, int type, P_CAN_SRC_CACHE_OBJ p_free_list)
{
    if(p_free_list == NULL){
        return;
    }
    (void)type;
    P_CAN_SRC_CACHE_OBJ p_emty_head_list;
#if 0
    if(type == CAN_S_FREE_LOCA_IRQ){
        p_emty_head_list = p_obj->exec.empt_list.p_head_list;
    }else{
        p_emty_head_list = &p_obj->exec.empt_list;
    }
#else
    p_emty_head_list = &p_obj->exec.empt_list;
#endif
    P_CAN_SRC_CACHE_OBJ p_head_free_list = p_free_list;
    P_CAN_SRC_CACHE_OBJ p_cur_free_list = p_free_list;
    while (p_cur_free_list->p_son_list != NULL){
        p_cur_free_list->p_head_list = p_cur_free_list->p_son_list;
        p_cur_free_list->p_son_list = NULL;
        p_cur_free_list = p_cur_free_list->p_head_list;
    }
    p_cur_free_list->p_head_list = p_emty_head_list->p_head_list;
    p_emty_head_list->p_head_list = p_head_free_list;
}

/*************************************************************
** Function name:       can_src_free_timeout_pack
** Descriptions:        释放超时数据包
** Input parameters:    None
** Returned value:      存放成功
** Remarks:             None
*************************************************************/
static void can_src_free_timeout_pack(P_CAN_SRC_OBJ p_obj)
{
    while (1){
        P_CAN_SRC_CACHE_OBJ p_free_list = p_obj->exec.norm_list.p_head_list;
        if(p_free_list == NULL){
            break;
        }
        if(check_time_out(p_free_list->rece_time,p_obj->r_timeout) == false){
            break;
        }
        p_obj->exec.norm_list.p_head_list = p_free_list->p_head_list;
        free_son_cache(p_obj,0,p_free_list);
    }
}
