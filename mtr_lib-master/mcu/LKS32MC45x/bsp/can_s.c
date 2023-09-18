/********************** File Info ***************************
** File name:               can_s.c
** Latest modified date:    2021-11-21
** Latest version:          V1.0.0
** Description:             can总线 - 流解析
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2021-11-21
** Version:                 V1.0.0
** Descriptions:            can总线 - 流解析
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "can_s.h"
#include "xh_sys.h"

typedef enum
{
    CAN_S_FREE_LOCA_IRQ = 0, /* 中断释放 */
    CAN_S_FREE_LOCA_BAC,     /* 后台释放 */
} CAN_S_PACK_FREE_LOCA;

/*************************************************************
** Function name:       can_s_init
** Descriptions:        初始化
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void can_s_init(P_CAN_S_OBJ p_obj)
{
    P_CAN_S_CACHE_OBJ p_cur_list = &p_obj->exec.empt_list;
    uint16_t cir = 0;
    for (; cir < p_obj->r_cache_num; cir++)
    {
        p_cur_list->p_head_list = p_obj->p_r_cache + cir;
        p_cur_list = p_cur_list->p_head_list;
    }
    p_cur_list->p_head_list = NULL;
    p_obj->exec.free_cache = p_obj->r_cache_num;
    p_obj->exec.dev_list.p_head_list = NULL;
    p_obj->exec.hear_list.p_head_list = NULL;
    p_obj->exec.addr_list.p_head_list = NULL;
    p_obj->exec.norm_list.p_head_list = NULL;
    p_obj->exec.cast_list.p_head_list = NULL;
    p_obj->exec.cache_reset_time = get_sys_ms();
    p_obj->exec.reset_mark = false;
    /* 单设备模式时，直接进入连接态 */
    if (p_obj->single_module == true)
    {
        p_obj->exec.dev_sta = CAN_S_CONN_STA;
        p_obj->exec.dev_d_addr = 1;
    }
}

uint32_t can_s_set_baudrate(P_CAN_S_OBJ p_obj, uint32_t baudrate)
{
    return NULL;
}

static uint16_t get_dev_info(P_CAN_S_OBJ p_obj, uint16_t *p_d_addr, uint8_t *p_uniquely_id, uint8_t id_len)
{
    if (p_obj->exec.uniquely_id_mark == false)
    {
        p_obj->exec.uniquely_id_mark = true;
        uint8_t cpu_id[CPUID_BYTES];
        xh_sys_get_cpuid(p_obj->sys_drv_id, cpu_id, CPUID_BYTES);

        MD5_CTX md5c;
        uint8_t decrypt[16] = {0};
        MD5Init(&md5c); //初始化
        MD5Update(&md5c, (uint8_t *)cpu_id, CPUID_BYTES);
        MD5Final(&md5c, decrypt);

        memcpy(p_obj->exec.uniquely_id, decrypt, CAN_S_PACK_DATA_LEN);
    }
    memcpy(p_uniquely_id, p_obj->exec.uniquely_id, CAN_S_PACK_DATA_LEN);
    memcpy(p_d_addr, p_obj->exec.uniquely_id, sizeof(uint16_t));
    return CAN_S_PACK_DATA_LEN;
}

/*************************************************************
** Descriptions:        获取未使用缓存个数
*************************************************************/
uint32_t can_s_get_free_cache(P_CAN_S_OBJ p_obj)
{
    uint32_t free_cache = 0;
    P_CAN_S_CACHE_OBJ p_get_empty_list = p_obj->exec.empt_list.p_head_list;
    while (p_get_empty_list != NULL)
    {
        free_cache++;
        p_get_empty_list = p_get_empty_list->p_head_list;
    }
    return free_cache;
}

/*************************************************************
** Function name:       get_empty_cache
** Descriptions:        获取空缓存
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static P_CAN_S_CACHE_OBJ get_empty_cache(P_CAN_S_OBJ p_obj)
{
    P_CAN_S_CACHE_OBJ p_sec_empty_list = p_obj->exec.empt_list.p_head_list->p_head_list;
    P_CAN_S_CACHE_OBJ p_get_empty_list = p_sec_empty_list->p_head_list;
    if (p_get_empty_list == NULL || p_sec_empty_list == NULL)
    {
        return NULL;
    }
    p_sec_empty_list->p_head_list = p_get_empty_list->p_head_list;
    p_get_empty_list->p_head_list = NULL;
    p_get_empty_list->p_son_list = NULL;
    p_get_empty_list->rece_time = 0;
    return p_get_empty_list;
}

/*************************************************************
** Function name:       free_son_cache
** Descriptions:        释放缓存
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void free_son_cache(P_CAN_S_OBJ p_obj, CAN_S_PACK_FREE_LOCA type, P_CAN_S_CACHE_OBJ p_free_list)
{
    if (p_free_list == NULL)
    {
        return;
    }
    P_CAN_S_CACHE_OBJ p_emty_head_list;
    if (type == CAN_S_FREE_LOCA_IRQ)
    {
        p_emty_head_list = p_obj->exec.empt_list.p_head_list;
    }
    else
    {
        p_emty_head_list = &p_obj->exec.empt_list;
    }
    P_CAN_S_CACHE_OBJ p_head_free_list = p_free_list;
    P_CAN_S_CACHE_OBJ p_cur_free_list = p_free_list;
    while (p_cur_free_list->p_son_list != NULL)
    {
        p_cur_free_list->p_head_list = p_cur_free_list->p_son_list;
        p_cur_free_list->p_son_list = NULL;
        p_cur_free_list = p_cur_free_list->p_head_list;
    }
    p_cur_free_list->p_head_list = p_emty_head_list->p_head_list;
    p_emty_head_list->p_head_list = p_head_free_list;
}

/*************************************************************
** Function name:       free_head_cache
** Descriptions:        释放缓存
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void free_head_cache(P_CAN_S_OBJ p_obj,
                            CAN_S_PACK_FREE_LOCA type,
                            P_CAN_S_CACHE_OBJ p_start_list,
                            P_CAN_S_CACHE_OBJ p_end_list)
{
    P_CAN_S_CACHE_OBJ p_cur_list = p_start_list;
    if (p_start_list == NULL || p_end_list == NULL)
    {
        return;
    }
    while (p_cur_list != p_end_list)
    {
        p_cur_list = p_cur_list->p_head_list;
    }
    P_CAN_S_CACHE_OBJ p_emty_head_list;
    if (type == CAN_S_FREE_LOCA_IRQ)
    {
        p_emty_head_list = p_obj->exec.empt_list.p_head_list;
    }
    else
    {
        p_emty_head_list = &p_obj->exec.empt_list;
    }
    p_end_list->p_head_list = p_emty_head_list->p_head_list;
    p_emty_head_list->p_head_list = p_start_list;
}

/*************************************************************
** Function name:       can_s_rece_norm_pack
** Descriptions:        接收正常包
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_s_rece_norm_pack(P_CAN_S_OBJ p_obj, P_CAN_S_CACHE_OBJ p_rece_list)
{
    if (p_rece_list->pack.id.type != CAN_S_NORM_PACK)
    {
        return;
    }
    /* 查找需要放入的数据表头 */
    P_CAN_S_CACHE_OBJ p_rece_head_list = &p_obj->exec.norm_list;
    P_CAN_S_CACHE_OBJ p_cur_head_list = NULL;
    while (1)
    {
        p_cur_head_list = p_rece_head_list->p_head_list;
        /* 遍历至缓存尾，直接填充 */
        if (p_cur_head_list == NULL)
        {
            p_rece_head_list->p_head_list = p_rece_list;
            if (p_rece_list->pack.id.proc == CAN_S_PACK_FOOT)
            {
                p_rece_list->pack.id.proc = CAN_S_PACK_COMP;
            }
            return;
        }
        /* 遍历是否具备相同的pack, */
        if (p_cur_head_list->pack.id.proc != CAN_S_PACK_COMP &&
            p_cur_head_list->pack.id.d_addr == p_rece_list->pack.id.d_addr &&
            p_cur_head_list->pack.id.s_addr == p_rece_list->pack.id.s_addr &&
            p_cur_head_list->pack.id.type == p_rece_list->pack.id.type &&
            p_cur_head_list->pack.id.ver == p_rece_list->pack.id.ver)
        {
            break;
        }
        p_rece_head_list = p_cur_head_list;
    }
    /* 将数据放入子列表相应位置 */
    P_CAN_S_CACHE_OBJ p_cur_list = p_rece_head_list->p_head_list;
    while (1)
    {
        if (p_cur_list->p_son_list == NULL)
        {
            p_cur_list->p_son_list = p_rece_list;
            if (p_rece_list->pack.id.proc == CAN_S_PACK_FOOT)
            {
                p_rece_head_list->p_head_list->pack.id.proc = CAN_S_PACK_COMP;
            }
            return;
        }
        p_cur_list = p_cur_list->p_son_list;
    }
}

/*************************************************************
** Function name:       can_s_rece_other_pack
** Descriptions:        接收其它包
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_s_rece_other_pack(P_CAN_S_OBJ p_obj, P_CAN_S_CACHE_OBJ p_rece_list)
{
    if (p_rece_list->pack.id.type == CAN_S_NORM_PACK)
    {
        return;
    }
    P_CAN_S_CACHE_OBJ p_rece_head_list = NULL;
    switch (p_rece_list->pack.id.type)
    {
        case CAN_S_HEAR_PACK:
            p_rece_head_list = &p_obj->exec.hear_list;
            break;
        case CAN_S_CAST_PACK:
            p_rece_head_list = &p_obj->exec.cast_list;
            break;
        case CAN_S_ADDR_PACK:
            p_rece_head_list = &p_obj->exec.addr_list;
            break;
    }
    while (p_rece_head_list->p_head_list != NULL)
    {
        p_rece_head_list = p_rece_head_list->p_head_list;
    }
    p_rece_head_list->p_head_list = p_rece_list;
}

/*************************************************************
** Function name:       can_s_rece_pack
** Descriptions:        接收数据包
** Input parameters:    None
** Returned value:      存放成功
** Remarks:             None
*************************************************************/
void can_s_rece_pack(P_CAN_S_OBJ p_obj, uint32_t pack_type, uint32_t pack_id, uint8_t *p_pack, uint32_t pack_len)
{
    uint16_t cir = 0;
    /* 数据为标准帧时，直接返回 */
    if (pack_type == 0)
    {
        return;
    }
    if (p_obj->exec.reset_mark == true)
    {
        can_s_init(p_obj);
    }
    CAN_S_ID_OBJ *p_id = (CAN_S_ID_OBJ *)&pack_id;
    if (p_id->ver != CAN_S_VER)
    {
        return;
    }
    /* 申请空块 */
    P_CAN_S_CACHE_OBJ p_empty_list = get_empty_cache(p_obj);
    if (p_empty_list == NULL)
    {
        return;
    }
    for (cir = 0; cir < pack_len; cir++)
    {
        p_empty_list->pack.data[cir] = p_pack[cir];
    }
    p_empty_list->pack.len = pack_len;
    /* 有非对齐风险,但比较高效，因此结构体id必须保证地址4字节对齐 */
    *(uint32_t *)(&p_empty_list->pack.id) = pack_id;
    p_empty_list->rece_time = get_sys_ms();
    can_s_rece_norm_pack(p_obj, p_empty_list);
    can_s_rece_other_pack(p_obj, p_empty_list);
}

/*************************************************************
** Function name:       can_s_read_data
** Descriptions:        读取数据
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t can_s_read_data(P_CAN_S_OBJ p_obj, uint8_t *p_data, uint32_t len)
{
    P_CAN_S_CACHE_OBJ p_last_list = &p_obj->exec.norm_list;
    P_CAN_S_CACHE_OBJ p_cur_list;
    while (1)
    {
        p_cur_list = p_last_list->p_head_list;
        if (p_cur_list == NULL)
        {
            return 0;
        }
        if (p_cur_list->pack.id.proc != CAN_S_PACK_COMP)
        {
            p_last_list = p_cur_list;
            continue;
        }
        p_last_list->p_head_list = p_cur_list->p_head_list;
        break;
    }
    uint16_t read_len = 0;
    P_CAN_S_CACHE_OBJ p_read_pack = p_cur_list;
    while (1)
    {
        if (p_read_pack == NULL)
        {
            break;
        }
        if ((read_len + p_read_pack->pack.len) > len)
        {
			memcpy(p_data,p_read_pack->pack.data,len);
			read_len += len;
            break;
        }
        memcpy(p_data, p_read_pack->pack.data, p_read_pack->pack.len);
        p_data += p_read_pack->pack.len;
        read_len += p_read_pack->pack.len;
        p_read_pack = p_read_pack->p_son_list;
    }
    free_son_cache(p_obj, CAN_S_FREE_LOCA_BAC, p_cur_list);
    return read_len;
}

/*************************************************************
** Function name:       can_s_send_data
** Descriptions:        发送数据
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t can_s_send_data(P_CAN_S_OBJ p_obj, uint8_t *p_data, uint32_t len)
{
    uint16_t send_len = 0;
    uint16_t remain_len = len;
    /* 设备未上线的时候不允许发送数据 */
    if (p_obj->exec.dev_sta != CAN_S_CONN_STA)
    {
        return 0;
    }
    CAN_S_PACK_OBJ pack;
    pack.id.reserved = 0;
    pack.id.s_addr = p_obj->dev_s_addr;
    pack.id.d_addr = p_obj->exec.dev_d_addr;
    pack.id.type = CAN_S_NORM_PACK;
    pack.id.ver = CAN_S_VER;
    uint8_t head_mark = true;
    while (send_len != len)
    {
        if (remain_len > 8)
        {
            if (head_mark == true)
            {
                head_mark = false;
                pack.id.proc = CAN_S_PACK_HEAD;
            }
            else
            {
                pack.id.proc = CAN_S_PACK_BODY;
            }
            pack.len = 8;
        }
        else
        {
            pack.id.proc = CAN_S_PACK_FOOT;
            pack.len = remain_len;
        }
        memcpy(pack.data, p_data, pack.len);
        p_data += pack.len;
        remain_len -= pack.len;
        if (xh_can_write(p_obj->can_drv_id, 1, *((uint32_t *)(&pack.id)), pack.data, pack.len) == false)
        {
            return send_len;
        }
        send_len += pack.len;
    }
    return send_len;
}

/*************************************************************
** Function name:       can_s_free_timeout_pack
** Descriptions:        释放超时数据包
** Input parameters:    None
** Returned value:      存放成功
** Remarks:             None
*************************************************************/
static void can_s_free_timeout_pack(P_CAN_S_OBJ p_obj)
{
    while (1)
    {
        P_CAN_S_CACHE_OBJ p_free_list = p_obj->exec.norm_list.p_head_list;
        if (p_free_list == NULL)
        {
            break;
        }
        if (check_time_out(p_free_list->rece_time, p_obj->r_timeout) == false)
        {
            break;
        }
        p_obj->exec.norm_list.p_head_list = p_free_list->p_head_list;
        free_son_cache(p_obj, CAN_S_FREE_LOCA_BAC, p_free_list);
    }
}

/*************************************************************
** Function name:       cmp_uniquely_id
** Descriptions:        比较唯一id
** Input parameters:    None
** Returned value:      0   相等
**                      1   p_id_1 > p_id_2,
**                      -1  p_id_1 < p_id_2
** Remarks:             None
*************************************************************/
static int8_t cmp_uniquely_id(uint8_t *p_id_1, uint8_t *p_id_2)
{
    uint16_t cir = 0;
    for (; cir < CAN_S_PACK_DATA_LEN; cir++)
    {
        if (p_id_1[cir] > p_id_2[cir])
        {
            return 1;
        }
        else if (p_id_1[cir] < p_id_2[cir])
        {
            return -1;
        }
    }
    return 0;
}

/*************************************************************
** Function name:       search_s_addr_dev_head
** Descriptions:        搜索静态地址指针头
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static P_CAN_S_CACHE_OBJ search_s_addr_dev_head(P_CAN_S_OBJ p_obj, uint8_t dev_s_addr)
{
    P_CAN_S_CACHE_OBJ p_cur_list = p_obj->exec.dev_list.p_head_list;
    while (1)
    {
        if (p_cur_list == NULL)
        {
            return NULL;
        }
        if (p_cur_list->pack.id.s_addr == dev_s_addr)
        {
            return p_cur_list;
        }
        p_cur_list = p_cur_list->p_head_list;
    }
}

/*************************************************************
** Function name:       search_addr_dev
** Descriptions:        搜索指定地址设备
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static P_CAN_S_CACHE_OBJ search_addr_dev(P_CAN_S_OBJ p_obj, uint8_t dev_s_addr, uint8_t dev_d_addr)
{
    P_CAN_S_CACHE_OBJ p_cur_list = search_s_addr_dev_head(p_obj, dev_s_addr);
    while (1)
    {
        if (p_cur_list == NULL)
        {
            return NULL;
        }
        if (p_cur_list->pack.id.d_addr == dev_d_addr)
        {
            return p_cur_list;
        }
        p_cur_list = p_cur_list->p_son_list;
    }
}

/*************************************************************
** Function name:       search_id_dev
** Descriptions:        搜索id设备
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static P_CAN_S_CACHE_OBJ search_id_dev(P_CAN_S_OBJ p_obj, uint8_t s_addr, uint8_t *p_uniquely_id)
{
    P_CAN_S_CACHE_OBJ p_head_list = p_obj->exec.dev_list.p_head_list;
    P_CAN_S_CACHE_OBJ p_son_list;
    while (1)
    {
        if (p_head_list == NULL)
        {
            return NULL;
        }
        p_son_list = p_head_list;
        while (1)
        {
            if (p_son_list == NULL)
            {
                break;
            }
            if (cmp_uniquely_id(p_son_list->pack.data, p_uniquely_id) == 0 && s_addr == p_son_list->pack.id.s_addr)
            {
                return p_son_list;
            }
            p_son_list = p_son_list->p_son_list;
        }
        p_head_list = p_head_list->p_head_list;
    }
}

/*************************************************************
** Function name:       insert_dev
** Descriptions:        插入设备
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static bool insert_dev(P_CAN_S_OBJ p_obj, P_CAN_S_CACHE_OBJ p_dev)
{
    P_CAN_S_CACHE_OBJ p_cur_list = &p_obj->exec.dev_list;
    while (1)
    {
        if (p_cur_list->p_head_list == NULL)
        {
            p_cur_list->p_head_list = p_dev;
            return true;
        }
        if (p_cur_list->p_head_list->pack.id.s_addr == p_dev->pack.id.s_addr)
        {
            if (p_cur_list->p_head_list->pack.id.d_addr > p_dev->pack.id.d_addr)
            {
                p_dev->p_son_list = p_cur_list->p_head_list;
                p_dev->p_head_list = p_cur_list->p_head_list->p_head_list;
                p_cur_list->p_head_list = p_dev;
                return true;
            }
            p_cur_list = p_cur_list->p_head_list;
            break;
        }
        p_cur_list = p_cur_list->p_head_list;
    }
    while (1)
    {
        if (p_cur_list->pack.id.d_addr == p_dev->pack.id.d_addr)
        {
            /* 地址冲突，且接收的地址较大时直接退出,地址小的优先级高 */
            if (cmp_uniquely_id(p_cur_list->pack.data, p_dev->pack.data) == -1)
            {
                return false;
            }
            memcpy(&p_cur_list->pack, &p_dev->pack, sizeof(CAN_S_PACK_OBJ));
            p_cur_list->rece_time = p_dev->rece_time;
            return false;
        }
        if (p_cur_list->p_son_list == NULL || p_cur_list->p_son_list->pack.id.d_addr > p_dev->pack.id.d_addr)
        {
            p_dev->p_son_list = p_cur_list->p_son_list;
            p_cur_list->p_son_list = p_dev;
            return true;
        }
        p_cur_list = p_cur_list->p_son_list;
    }
}

/*************************************************************
** Function name:       search_unused_d_addr
** Descriptions:        搜索未使用的动态地址
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static uint16_t search_unused_d_addr(P_CAN_S_OBJ p_obj, uint8_t s_addr)
{
    uint16_t unused_d_addr = 0;
    P_CAN_S_CACHE_OBJ p_cur_list = search_s_addr_dev_head(p_obj, s_addr);
    while (1)
    {
        unused_d_addr++;
        if (unused_d_addr == p_obj->exec.dev_d_addr)
        {
            continue;
        }
        if (p_cur_list == NULL)
        {
            break;
        }
        if (p_cur_list->pack.id.d_addr == unused_d_addr)
        {
            p_cur_list = p_cur_list->p_son_list;
            continue;
        }
        break;
    }
    return unused_d_addr;
}

/*************************************************************
** Function name:       can_s_mast_time_updata
** Descriptions:        主机时间更新
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_s_mast_time_updata(P_CAN_S_OBJ p_obj, P_CAN_S_CACHE_OBJ p_pack)
{
    if (p_obj->exec.dev_sta != CAN_S_CONN_STA)
    {
        return;
    }
    if (p_pack->pack.id.s_addr != p_obj->dev_s_addr)
    {
        return;
    }
    if (p_pack->pack.id.d_addr > p_obj->exec.dev_d_addr)
    {
        return;
    }
    if (p_obj->exec.conn.host_mark == true && p_obj->exec.conn.wait_mark == false)
    {
        p_obj->exec.conn.wait_mark = true;
        p_obj->exec.conn.mast_wait_time = get_sys_ms();
    }
    p_obj->exec.conn.mast_conn_time = get_sys_ms();
    if (check_time_out(p_obj->exec.conn.mast_wait_time, CAN_S_MAST_TIME) == true)
    {
        p_obj->exec.conn.host_mark = false;
        p_obj->exec.conn.wait_mark = false;
    }
}

/*************************************************************
** Function name:       can_s_hear_handle
** Descriptions:        心跳处理
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_s_hear_handle(P_CAN_S_OBJ p_obj)
{
    P_CAN_S_CACHE_OBJ p_start_list = NULL;
    P_CAN_S_CACHE_OBJ p_end_list = NULL;
    P_CAN_S_CACHE_OBJ p_mid_list = NULL;

    /* 防止进入中断的时候, 指针为空的时候异常清空，导致缓存丢失 */
    if (p_obj->exec.hear_list.p_head_list == NULL)
    {
        return;
    }
    P_CAN_S_CACHE_OBJ p_cur_list = p_obj->exec.hear_list.p_head_list;
    p_obj->exec.hear_list.p_head_list = NULL;

    P_CAN_S_CACHE_OBJ p_next_list = p_cur_list;
    while (1)
    {
        p_cur_list = p_next_list;
        if (p_cur_list == NULL)
        {
            break;
        }
        p_next_list = p_cur_list->p_head_list;
        /* 本机地址冲突 */
        if (p_obj->exec.dev_sta == CAN_S_CONN_STA && p_cur_list->pack.id.s_addr == p_obj->dev_s_addr &&
            p_cur_list->pack.id.d_addr == p_obj->exec.dev_d_addr)
        {
            /* 主机id大于设备id，清空设备 */
            if (cmp_uniquely_id(p_obj->exec.uniquely_id, p_cur_list->pack.data) == -1)
            {
                /* 记录释放的缓存 */
                if (p_start_list == NULL)
                {
                    p_start_list = p_cur_list;
                    p_mid_list = p_cur_list;
                }
                else
                {
                    p_mid_list->p_head_list = p_cur_list;
                    p_mid_list = p_mid_list->p_head_list;
                }
                p_end_list = p_cur_list;
                continue;
            }
            p_obj->exec.conn.conflict_mark = true;
            p_obj->exec.conflict_timestamp = get_sys_ms();
        }
        /* 更新主机时间 */
        can_s_mast_time_updata(p_obj, p_cur_list);
        /* 查找已存在的设备，并更新设备列表 */
        P_CAN_S_CACHE_OBJ p_exit_dev = search_addr_dev(p_obj, p_cur_list->pack.id.s_addr, p_cur_list->pack.id.d_addr);
        if (p_exit_dev != NULL)
        {
            memcpy(&p_exit_dev->pack, &p_cur_list->pack, sizeof(p_exit_dev->pack));
            p_exit_dev->rece_time = p_cur_list->rece_time;
            /* 记录释放的缓存 */
            if (p_start_list == NULL)
            {
                p_start_list = p_cur_list;
                p_mid_list = p_cur_list;
            }
            else
            {
                p_mid_list->p_head_list = p_cur_list;
                p_mid_list = p_mid_list->p_head_list;
            }
            p_end_list = p_cur_list;
            continue;
        }
        p_cur_list->p_head_list = NULL;
        p_cur_list->p_son_list = NULL;
        if (insert_dev(p_obj, p_cur_list) == false)
        {
            /* 记录释放的缓存 */
            if (p_start_list == NULL)
            {
                p_start_list = p_cur_list;
                p_mid_list = p_cur_list;
            }
            else
            {
                p_mid_list->p_head_list = p_cur_list;
                p_mid_list = p_mid_list->p_head_list;
            }
            p_end_list = p_cur_list;
        }
    }
    free_head_cache(p_obj, CAN_S_FREE_LOCA_BAC, p_start_list, p_end_list);
}

/*************************************************************
** Function name:       can_s_cast_handle
** Descriptions:        广播处理
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_s_cast_handle(P_CAN_S_OBJ p_obj)
{
    /* 防止进入中断的时候, 指针为空的时候异常清空，导致缓存丢失 */
    if (p_obj->exec.cast_list.p_head_list == NULL)
    {
        return;
    }
    P_CAN_S_CACHE_OBJ p_start_list = p_obj->exec.cast_list.p_head_list;
    p_obj->exec.cast_list.p_head_list = NULL;
    P_CAN_S_CACHE_OBJ p_cur_list = p_start_list;
    P_CAN_S_CACHE_OBJ p_end_list = NULL;
    P_CAN_S_CACHE_OBJ p_next_list = p_cur_list;
    while (1)
    {
        p_cur_list = p_next_list;
        if (p_cur_list == NULL)
        {
            break;
        }
        p_next_list = p_cur_list->p_head_list;
        p_end_list = p_cur_list;
        if (p_obj->dev_s_addr != p_cur_list->pack.id.s_addr)
        {
            continue;
        }
        if (p_obj->exec.dev_sta == CAN_S_CAST_STA)
        {
            if (cmp_uniquely_id(p_obj->exec.uniquely_id, p_cur_list->pack.data) == -1)
            {
                continue;
            }
            p_obj->exec.cast.start_time = get_sys_ms();
        }
        else if (p_obj->exec.dev_sta == CAN_S_CONN_STA)
        {
            if (p_obj->exec.conn.host_mark == false)
            {
                continue;
            }
            P_CAN_S_CACHE_OBJ p_dev_list = search_id_dev(p_obj, p_cur_list->pack.id.s_addr, p_cur_list->pack.data);
            CAN_S_PACK_OBJ pack;
            pack.id.reserved = 0;
            pack.len = CAN_S_PACK_DATA_LEN;
            pack.id.proc = CAN_S_PACK_FOOT;
            pack.id.s_addr = p_obj->dev_s_addr;
            pack.id.type = CAN_S_ADDR_PACK;
            pack.id.ver = CAN_S_VER;
            memcpy(pack.data, p_cur_list->pack.data, CAN_S_PACK_DATA_LEN);
            if (p_dev_list != NULL && (p_dev_list->pack.id.s_addr != p_obj->dev_s_addr ||
                                       p_dev_list->pack.id.d_addr != p_obj->exec.dev_d_addr))
            {
                pack.id.d_addr = p_dev_list->pack.id.d_addr;
            }
            else
            {
                pack.id.d_addr = search_unused_d_addr(p_obj, pack.id.s_addr);
            }
            xh_can_write(p_obj->can_drv_id, 1, *((uint32_t *)(&pack.id)), pack.data, pack.len);
        }
    }
    free_head_cache(p_obj, CAN_S_FREE_LOCA_BAC, p_start_list, p_end_list);
}

/*************************************************************
** Function name:       can_s_addr_handle
** Descriptions:        地址分配处理
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_s_addr_handle(P_CAN_S_OBJ p_obj)
{
    /* 防止进入中断的时候, 指针为空的时候异常清空，导致缓存丢失 */
    if (p_obj->exec.addr_list.p_head_list == NULL)
    {
        return;
    }
    P_CAN_S_CACHE_OBJ p_start_list = p_obj->exec.addr_list.p_head_list;
    p_obj->exec.addr_list.p_head_list = NULL;
    P_CAN_S_CACHE_OBJ p_cur_list = p_start_list;
    P_CAN_S_CACHE_OBJ p_next_list = p_cur_list;
    P_CAN_S_CACHE_OBJ p_end_list = NULL;
    while (1)
    {
        p_cur_list = p_next_list;
        if (p_cur_list == NULL)
        {
            break;
        }
        p_next_list = p_cur_list->p_head_list;
        p_end_list = p_cur_list;
        if (p_obj->exec.dev_sta != CAN_S_CAST_STA)
        {
            continue;
        }
        if (p_obj->dev_s_addr != p_cur_list->pack.id.s_addr)
        {
            continue;
        }
        if (cmp_uniquely_id(p_obj->exec.uniquely_id, p_cur_list->pack.data) != 0)
        {
            continue;
        }
        p_obj->exec.cast.set_addr_mark = true;
        p_obj->exec.dev_d_addr = p_cur_list->pack.id.d_addr;
    }
    free_head_cache(p_obj, CAN_S_FREE_LOCA_BAC, p_start_list, p_end_list);
}

/*************************************************************
** Function name:       can_s_dev_sta_task
** Descriptions:        设备状态任务
** Input parameters:    None
** Returned value:      存放成功
** Remarks:             None
*************************************************************/
static void can_s_dev_sta_task(P_CAN_S_OBJ p_obj)
{
    switch (p_obj->exec.dev_sta)
    {
        case CAN_S_IDLE_STA:
            p_obj->exec.conn.host_mark = false;
            p_obj->exec.cast.send_time = get_sys_ms() - CAN_S_CAST_INTE_TIME;
            p_obj->exec.cast.start_time = get_sys_ms();
            p_obj->exec.cast.set_addr_mark = false;
            p_obj->exec.dev_sta = CAN_S_CAST_STA;
        case CAN_S_CAST_STA:
            if (p_obj->single_module == true)
            {
                p_obj->exec.dev_d_addr = 1;
                p_obj->exec.dev_sta = CAN_S_CONN_STA;
                break;
            }
            if (check_time_out(p_obj->exec.cast.send_time, CAN_S_CAST_INTE_TIME) == true)
            {
                CAN_S_PACK_OBJ pack;
                uint16_t d_addr = 0;
                get_dev_info(p_obj, &d_addr, p_obj->exec.uniquely_id, CAN_S_PACK_DATA_LEN);
                memcpy(pack.data, p_obj->exec.uniquely_id, CAN_S_PACK_DATA_LEN);
                pack.len = CAN_S_PACK_DATA_LEN;
                pack.id.reserved = 0;
                pack.id.d_addr = d_addr;
                pack.id.proc = CAN_S_PACK_FOOT;
                pack.id.s_addr = p_obj->dev_s_addr;
                pack.id.type = CAN_S_CAST_PACK;
                pack.id.ver = CAN_S_VER;
                xh_can_write(p_obj->can_drv_id, 1, *((uint32_t *)(&pack.id)), pack.data, pack.len);
                p_obj->exec.cast.send_time = get_sys_ms();
            }
            if (check_time_out(p_obj->exec.cast.start_time, CAN_S_CAST_FINI_TIME) == true)
            {
                p_obj->exec.dev_d_addr = 1;
                p_obj->exec.dev_sta = CAN_S_CONN_STA;
            }
            if (p_obj->exec.cast.set_addr_mark == true)
            {
                p_obj->exec.dev_sta = CAN_S_CONN_STA;
            }
            if (p_obj->exec.dev_sta != CAN_S_CONN_STA)
            {
                break;
            }
            p_obj->exec.conn.hear_send_time = get_sys_ms();
            if (p_obj->exec.dev_d_addr == 1 && p_obj->exec.cast.set_addr_mark == false)
            {
                p_obj->exec.conn.host_mark = true;
            }
            else
            {
                p_obj->exec.conn.host_mark = false;
                p_obj->exec.conn.mast_conn_time = get_sys_ms();
            }
            p_obj->exec.conn.conflict_mark = false;
        case CAN_S_CONN_STA:
            /* 单设备模式时不发送心跳,不做状态跳转 */
            if (p_obj->single_module == true)
            {
                p_obj->exec.conn.conflict_mark = false;
                break;
            }
            if (p_obj->exec.conn.conflict_mark == true)
            {
                p_obj->exec.dev_sta = CAN_S_IDLE_STA;
                break;
            }
            if (get_sys_ms() > p_obj->exec.conn.hear_send_time)
            {
                CAN_S_PACK_OBJ pack;
                memcpy(pack.data, p_obj->exec.uniquely_id, CAN_S_PACK_DATA_LEN);
                pack.len = CAN_S_PACK_DATA_LEN;
                pack.id.reserved = 0;
                pack.id.d_addr = p_obj->exec.dev_d_addr;
                pack.id.proc = CAN_S_PACK_FOOT;
                pack.id.s_addr = p_obj->dev_s_addr;
                pack.id.type = CAN_S_HEAR_PACK;
                pack.id.ver = CAN_S_VER;
                xh_can_write(p_obj->can_drv_id, 1, *((uint32_t *)(&pack.id)), pack.data, pack.len);
                p_obj->exec.conn.hear_send_time = get_sys_ms() + CAN_S_CAST_INTE_TIME;
            }
            if (check_time_out(p_obj->exec.conn.mast_conn_time, CAN_S_MAST_TIME) == true)
            {
                p_obj->exec.conn.host_mark = true;
            }
            break;
        default:
            break;
    }
}

/*************************************************************
** Function name:       can_s_info_updata
** Descriptions:        信息刷新
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void can_s_info_updata(P_CAN_S_OBJ p_obj)
{
    uint16_t d_addr;
    get_dev_info(p_obj, &d_addr, p_obj->exec.uniquely_id, CAN_S_PACK_DATA_LEN);
}

/*************************************************************
** Function name:       can_s_task
** Descriptions:        任务
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t can_s_get_d_addr(P_CAN_S_OBJ p_obj)
{
    if (p_obj->exec.dev_sta != CAN_S_CONN_STA)
    {
        return 0;
    }
    return p_obj->exec.dev_d_addr;
}

/*************************************************************
** Function name:       can_s_task
** Descriptions:        任务
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
bool can_s_get_host_mark(P_CAN_S_OBJ p_obj)
{
    if (p_obj->exec.dev_sta != CAN_S_CONN_STA)
    {
        return false;
    }
    return p_obj->exec.conn.host_mark;
}

/*************************************************************
** Descriptions:        移除设备
*************************************************************/
static void can_s_rm_dev(P_CAN_S_OBJ p_obj)
{
    P_CAN_S_CACHE_OBJ p_head_list = &p_obj->exec.dev_list;
    P_CAN_S_CACHE_OBJ p_son_list = NULL;
    P_CAN_S_CACHE_OBJ p_cur_list = NULL;
    P_CAN_S_CACHE_OBJ p_rm_list = NULL;
    while (p_head_list->p_head_list != NULL)
    {
        if (p_rm_list != NULL)
        {
            free_head_cache(p_obj, CAN_S_FREE_LOCA_BAC, p_rm_list, p_rm_list);
        }
        p_rm_list = NULL;
        if (p_head_list->p_head_list == NULL)
        {
            break;
        }
        if (p_son_list == NULL)
        {
            p_cur_list = p_head_list->p_head_list;
            if (check_time_out(p_cur_list->rece_time, CAN_S_DEV_RM_TIME) == true)
            {
                if (p_cur_list->p_son_list != NULL)
                {
                    p_rm_list = p_cur_list;
                    p_cur_list->p_son_list->p_head_list = p_cur_list->p_head_list;
                    p_head_list->p_head_list = p_cur_list->p_son_list;
                    continue;
                }
                p_head_list->p_head_list = p_cur_list->p_head_list;
                free_head_cache(p_obj, CAN_S_FREE_LOCA_BAC, p_cur_list, p_cur_list);
                continue;
            }
            p_son_list = p_cur_list;
        }
        if (p_son_list->p_son_list == NULL)
        {
            p_son_list = NULL;
            p_head_list = p_head_list->p_head_list;
            continue;
        }
        p_cur_list = p_son_list->p_son_list;
        if (check_time_out(p_cur_list->rece_time, CAN_S_DEV_RM_TIME) == true)
        {
            p_rm_list = p_cur_list;
            p_son_list->p_son_list = p_cur_list->p_son_list;
            continue;
        }
        p_son_list = p_son_list->p_son_list;
    }
}

/*************************************************************
** Function name:       can_s_cache_monitor
** Descriptions:        任务
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define RESET_NUM 20
#define RESET_TIME (10 * 1000)
void can_s_cache_monitor(P_CAN_S_OBJ p_obj)
{
    uint32_t free_cache = can_s_get_free_cache(p_obj);
    uint32_t dev_total = can_s_get_dev_total(p_obj);
    if ((free_cache + dev_total + RESET_NUM >= p_obj->r_cache_num) && (free_cache + dev_total <= p_obj->r_cache_num))
    {
        p_obj->exec.cache_reset_time = get_sys_ms();
        return;
    }
    if (check_time_out(p_obj->exec.cache_reset_time, RESET_TIME) == true)
    {
        p_obj->exec.reset_mark = true;
    }
}

/*************************************************************
** Function name:       can_s_task
** Descriptions:        任务
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void can_s_task(P_CAN_S_OBJ p_obj)
{
    if (p_obj->exec.reset_mark == true)
    {
        return;
    }
    can_s_info_updata(p_obj);
    can_s_free_timeout_pack(p_obj);
    can_s_dev_sta_task(p_obj);
    can_s_hear_handle(p_obj);
    can_s_cast_handle(p_obj);
    can_s_addr_handle(p_obj);
    can_s_rm_dev(p_obj);
    can_s_cache_monitor(p_obj);
}

/*************************************************************
** Function name:       can_s_get_dev_num
** Descriptions:        获取设备个数
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t can_s_get_dev_num(P_CAN_S_OBJ p_obj, uint32_t dev_s_addr)
{
    uint16_t dev_num = 0;
    P_CAN_S_CACHE_OBJ p_cur_list = search_s_addr_dev_head(p_obj, dev_s_addr);

    while (p_cur_list != NULL)
    {
        if (check_time_out(p_cur_list->rece_time, CAN_S_MAST_TIME) == false)
        {
            dev_num++;
        }
        p_cur_list = p_cur_list->p_son_list;
    }
	// 如果获取是本机地址相同的加上本机模块
	if(dev_s_addr == HOST_ADDR){
		dev_num++;
	}
    return dev_num;
}

/*************************************************************
** Function name:       can_s_get_dev_num
** Descriptions:        获取设备个数
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
bool can_s_get_dev_online(P_CAN_S_OBJ p_obj, uint32_t dev_s_addr, uint32_t dev_d_addr)
{
    P_CAN_S_CACHE_OBJ p_cur_list = search_s_addr_dev_head(p_obj, dev_s_addr);

    while (p_cur_list != NULL)
    {
        if (check_time_out(p_cur_list->rece_time, CAN_S_MAST_TIME) == false)
        {
            if (p_cur_list->pack.id.d_addr == dev_d_addr)
            {
                return true;
            }
        }
        p_cur_list = p_cur_list->p_son_list;
    }
    return false;
}

/*************************************************************
** Descriptions:        获取设备总数
*************************************************************/
uint32_t can_s_get_dev_total(P_CAN_S_OBJ p_obj)
{
    uint32_t total_num = 0;
    P_CAN_S_CACHE_OBJ p_head_list = p_obj->exec.dev_list.p_head_list;
    P_CAN_S_CACHE_OBJ p_son_list = NULL;
    while (1)
    {
        if (p_head_list == NULL)
        {
            break;
        }
        total_num++;
        p_son_list = p_head_list;
        while (p_son_list->p_son_list != NULL)
        {
            total_num++;
            p_son_list = p_son_list->p_son_list;
        }
        p_head_list = p_head_list->p_head_list;
    }
    return total_num;
}

/*************************************************************
** Function name:       can_s_set_dev_s_addr
** Descriptions:        设置设备静态地址
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void can_s_set_dev_s_addr(P_CAN_S_OBJ p_obj, uint32_t s_addr)
{
    p_obj->dev_s_addr = (uint8_t)s_addr;
}
