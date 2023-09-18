/**
 * @brief 协议处理matrix层
 * @file protocol_matrix.c
 * @version 1.0
 * @author hzf
 * @date 2022-8-24 19:25:47
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-8-24 19:25:47
 *   Author      : hzf
 *   Modification: Created file
 */


#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "line_buf.h"
#include "protocol_ef_plus.h"
#include "protocol_matrix.h"

#define UNPACK_HEAD  0
#define UNPACK_BODY  1

static void matrix_id_and_index_calc(protocol_matrix_t *p_matrix, uint32_t read_index,uint32_t *p_id, uint32_t *p_index)
{
    uint32_t id_read = 0;
    uint16_t i = 0;

    if(p_matrix->rec_en == 0){
        return;
    }
    
    while(id_read < read_index){
        uint32_t len = *((uint32_t *)&p_matrix->id_buf_obj.pbuf[i * MATRIX_ID_TOTAL_LEN + MATRIX_ID_DATA_LEN]);
        uint32_t index = *((uint32_t *)&p_matrix->id_buf_obj.pbuf[i * MATRIX_ID_TOTAL_LEN + 2 * MATRIX_ID_DATA_LEN]);
        id_read += (len - index);
        if(id_read >= read_index){
            *p_id = *((uint32_t *)&p_matrix->id_buf_obj.pbuf[i * 3 * MATRIX_ID_DATA_LEN]);
            *p_index = len - (id_read - read_index) - 1;
            return;
        }
        i++;
    }
}

static void matrix_id_manage(protocol_matrix_t *p_matrix, uint32_t len, uint32_t id)
{
    uint16_t lb_rest_len = 0;
    uint32_t id_read = 0;
    uint16_t i = 0;

    if(p_matrix->rec_en == 0){
        return;
    }
    
    while(id_read < p_matrix->line_buf_obj.r_index){
        uint32_t len = *((uint32_t *)&p_matrix->id_buf_obj.pbuf[i * MATRIX_ID_TOTAL_LEN + MATRIX_ID_DATA_LEN]);
        uint32_t index = *((uint32_t *)&p_matrix->id_buf_obj.pbuf[i * MATRIX_ID_TOTAL_LEN + 2 * MATRIX_ID_DATA_LEN]);
        id_read += (len - index);
        if(id_read <= p_matrix->line_buf_obj.r_index){
            lb_move(&p_matrix->id_buf_obj, MATRIX_ID_TOTAL_LEN);
        }else{
            index = len - (id_read - p_matrix->line_buf_obj.r_index);
            memcpy(&p_matrix->id_buf_obj.pbuf[i * MATRIX_ID_TOTAL_LEN + 2 * MATRIX_ID_DATA_LEN], &index, sizeof(index));
            break;
        }
        i++;
    }

    if(p_matrix->init_flag == 0 || id != p_matrix->last_id){
        uint32_t index = 0;
        p_matrix->last_id = id;
        lb_rest_len = lb_get_rest_len(&p_matrix->id_buf_obj);
        if(lb_rest_len < MATRIX_ID_TOTAL_LEN){
            lb_reset(&p_matrix->line_buf_obj);
            lb_reset(&p_matrix->id_buf_obj);   
        }
        lb_push(&p_matrix->id_buf_obj, (uint8_t *)&id, sizeof(id));
        lb_push(&p_matrix->id_buf_obj, (uint8_t *)&len, sizeof(len));            
        lb_push(&p_matrix->id_buf_obj, (uint8_t *)&index, sizeof(index));
        p_matrix->init_flag = 1;
    }
}

static bool time_out_judge(protocol_matrix_t *p_matrix)
{
    if(p_matrix->pfn_get_time_ms){
        uint32_t cur_time_ms = p_matrix->pfn_get_time_ms();
        if((cur_time_ms - p_matrix->unpack_time_ms) >= p_matrix->timeout) {
            return true;
        }
    }
    
    return false;
}

int32_t protocol_decode(protocol_matrix_t *p_matrix, uint8_t *p_data, uint32_t len, uint32_t id, hal_frame_info_t *p_frame_info)
{
    if(NULL == p_matrix || NULL == p_data || 0 == len || NULL == p_frame_info || NULL == p_matrix->protocol){
        return -1;
    }
    
    protocol_obj_t *p_obj = (protocol_obj_t *)p_matrix->protocol;
    uint32_t real_len = 0;
    uint16_t lb_rest_len = 0;
    uint16_t lb_have_len = 0;
    int32_t  ret = -1;
    uint8_t *p_buf = NULL;
    uint32_t data_index = 0;
    
    if(id == p_matrix->last_id){
        data_index = p_matrix->input_read_index;
    }
    
retry:
    matrix_id_manage(p_matrix, len, id);    
    lb_move(&p_matrix->line_buf_obj, p_matrix->line_buf_obj.r_index);
    lb_rest_len = lb_get_rest_len(&p_matrix->line_buf_obj);
    real_len = (lb_rest_len < (len - data_index)) ? lb_rest_len : (len - data_index);
    lb_push(&p_matrix->line_buf_obj, p_data + data_index, real_len);
    
    data_index += real_len;

    while(1){
        lb_have_len = lb_get_data_len(&p_matrix->line_buf_obj);
        switch(p_matrix->unpack_step){
            case UNPACK_HEAD:
                if(lb_have_len >= p_obj->get_head_len(NULL, 0)) {
                    p_buf = lb_get_r_ptr(&p_matrix->line_buf_obj);
                    if(p_obj->check_header(p_buf, lb_have_len)) {
                        p_matrix->unpack_step = UNPACK_BODY;
                        matrix_id_and_index_calc(p_matrix, p_matrix->line_buf_obj.r_index + 1, &p_matrix->start_id, &p_matrix->start_index);
                        if(p_matrix->pfn_get_time_ms){
                            p_matrix->unpack_time_ms = p_matrix->pfn_get_time_ms();
                        }                        
                    } else {
                        p_matrix->line_buf_obj.r_index++;
                    }
                } else {
                    goto end;
                }
            break;
            case UNPACK_BODY:
                p_buf = lb_get_r_ptr(&p_matrix->line_buf_obj);
                if(lb_have_len >= p_obj->get_pack_len(p_buf, lb_have_len)) {
                    p_matrix->unpack_step = UNPACK_HEAD;
                    if(p_obj->check_data(p_buf, lb_have_len)) {
                        p_obj->unpack_data(p_buf, p_frame_info);

                        p_matrix->line_buf_obj.r_index += p_obj->get_pack_len(p_buf, lb_have_len);
                        ret = 0;
                        p_frame_info->start_id = p_matrix->start_id;
                        p_frame_info->start_index = p_matrix->start_index;
                        matrix_id_and_index_calc(p_matrix, p_matrix->line_buf_obj.r_index, &p_frame_info->end_id, &p_frame_info->end_index);
                        ///> 避免应用上处理hal_frame_info_t里的data数据越界时影响matrix->line_buf_obj的数据，下次处理时重新从原始数据拷贝到linebuf内
                        matrix_id_manage(p_matrix, len, id);
                        data_index = data_index - (p_matrix->line_buf_obj.w_index - p_matrix->line_buf_obj.r_index);
                        lb_reset(&p_matrix->line_buf_obj);
                        goto end;
                    } else {
                        p_matrix->line_buf_obj.r_index++;
                    }
                } else {
                    if(lb_have_len >= p_matrix->line_buf_obj.size || time_out_judge(p_matrix)) {
                        p_matrix->unpack_step = UNPACK_HEAD;
                        p_matrix->line_buf_obj.r_index++;
                    } else {
                        goto end;
                    }
                }
            break;
            default:
                while(1);
        }
    }
    
end:
    if(ret < 0 && data_index < len){
        goto retry;
    }
    p_matrix->input_read_index = data_index;
    p_matrix->last_id = id;
    return ret;
}

uint16_t protocol_encode(protocol_matrix_t *p_matrix, hal_frame_info_t *p_frame_info, uint8_t *p_buff)
{
    if(NULL == p_matrix || NULL == p_buff || NULL == p_buff || NULL == p_matrix->protocol){
        return 0;
    }
    
    protocol_obj_t *p_obj = (protocol_obj_t *)p_matrix->protocol;
    return p_obj->pack_data(p_buff, p_frame_info);
}


