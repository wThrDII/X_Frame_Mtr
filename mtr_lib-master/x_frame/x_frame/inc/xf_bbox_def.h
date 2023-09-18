/**
 * @brief 黑匣子功能宏定义
 * @file xf_bbox_def.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-7 9:58:2
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-7 9:58:2
 *   Author      : hzf
 *   Modification: Created file
 */


#ifdef GEN_BBOX_STRUCT_ENTITY
    #undef BBOX_START
    #undef BBOX_ITEM
    #undef BBOX_END
    #undef BBOX_RECORD_START
    #undef BBOX_RECORD_ITEM
    #undef BBOX_RECORD_END
    #include "x_frame.h"

    #pragma pack(1)
	#define BBOX_START(name,rec_cmd_set,rec_cmd_id,data_size)		typedef struct{\
                                                                        BBOX_HIDE_DATA_T _hide;
	#define BBOX_ITEM(type,var,var_name)					            typeof(var) var_name;
	#define BBOX_END(name)								            }x_##name##_bbox_t, *px_##name##_bbox_t;\
														            static x_##name##_bbox_t _##name##_data;

    #define BBOX_RECORD_START(name,rec_cmd_set,rec_cmd_id,data_size,head_cnt,tail_count)      typedef struct{\
                                                                                                BBOX_HIDE_DATA_T _hide;
    #define BBOX_RECORD_ITEM(type,var,var_name)                                                 typeof(var) var_name;
    #define BBOX_RECORD_END(name)                                                           }x_##name##_bbox_rec_t, *px_##name##_bbox_rec_t;\
                                                                                            static x_##name##_bbox_rec_t _##name##_rec_data;
	#pragma pack()
#endif

#ifdef GEN_BBOX_GET_ENTITY
    #undef BBOX_START
    #undef BBOX_ITEM
    #undef BBOX_END
    #undef BBOX_RECORD_START
    #undef BBOX_RECORD_ITEM
    #undef BBOX_RECORD_END
    
	#define BBOX_START(name,rec_cmd_set,rec_cmd_id,data_size)		void bbox_##name##_data_get(BBOX_HIDE_DATA_T *p_param, uint8_t *p_buff, uint32_t len){\
															            px_##name##_bbox_t p_bbox = &_##name##_data;\
															            memcpy(&p_bbox->_hide, p_param, sizeof(BBOX_HIDE_DATA_T));
	#define BBOX_ITEM(type,var,var_name)					            p_bbox->var_name = var;
	#define BBOX_END(name)								                memcpy(p_buff,p_bbox,X_FRAME_MIN(len,sizeof(x_##name##_bbox_t)));}

    #define BBOX_RECORD_START(name,rec_cmd_set,rec_cmd_id,data_size,head_cnt,tail_count)		void bbox_##name##_data_rec_get(BBOX_HIDE_DATA_T *p_param, uint8_t *p_buff, uint32_t len){\
															                                    px_##name##_bbox_rec_t p_bbox = &_##name##_rec_data;\
															                                    memcpy(&p_bbox->_hide, p_param, sizeof(BBOX_HIDE_DATA_T));
	#define BBOX_RECORD_ITEM(type,var,var_name)					                                p_bbox->var_name = var;
	#define BBOX_RECORD_END(name)								                                memcpy(p_buff,p_bbox,X_FRAME_MIN(len,sizeof(x_##name##_bbox_rec_t)));}
#endif

#ifdef GEN_BBOX_UPLOAD_ENTITY
    #undef BBOX_START
    #undef BBOX_ITEM
    #undef BBOX_END
    #undef BBOX_RECORD_START
    #undef BBOX_RECORD_ITEM
    #undef BBOX_RECORD_END

    #define BBOX_START(name,rec_cmd_set,rec_cmd_id,data_size)       bool bbox_##name##_upload(bool rec_cfg, uint8_t link_id, uint8_t dest_addr){\
                                                                        hal_frame_info_t frame_info = {0}; frame_info.link_id = link_id; frame_info.dest = dest_addr; \
                                                                        if(rec_cfg){\
                                                                        extern bool rec_cfg_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *));	\
                                                                        return rec_cfg_##name(&frame_info,x_rec_custom_send);}\
                                                                        else{EXT_REC(name);return (0 == rec_dat_##name(&frame_info, x_rec_custom_send)) ? (true) : (false);} }
                                                                    
    #define BBOX_ITEM(type,var,var_name)
    #define BBOX_END(name)

    #define BBOX_RECORD_START(name,rec_cmd_set,rec_cmd_id,data_size,head_cnt,tail_count)       bool bbox_##name##_rec_upload(bool rec_cfg, uint8_t link_id, uint8_t dest_addr){\
                                                                                                hal_frame_info_t frame_info = {0}; frame_info.link_id = link_id; frame_info.dest = dest_addr; \
                                                                                                if(rec_cfg){\
                                                                                                extern bool rec_cfg_##name##rec(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *));	\
                                                                                                return rec_cfg_##name##rec(&frame_info,x_rec_custom_send);}\
                                                                                                else{EXT_REC(name##rec);return (0 == rec_dat_##name##rec(&frame_info, x_rec_custom_send)) ? (true) : (false);} }
                                                                    
    #define BBOX_RECORD_ITEM(type,var,var_name)
    #define BBOX_RECORD_END(name)
#endif


#ifdef GEN_BBOX_SET_ENTITY
    #undef BBOX_START
    #undef BBOX_ITEM
    #undef BBOX_END
    #undef BBOX_RECORD_START
    #undef BBOX_RECORD_ITEM
    #undef BBOX_RECORD_END

	#define BBOX_START(name,rec_cmd_set,rec_cmd_id,data_size)		void bbox_##name##_data_set(uint8_t *p_buff, uint32_t len){
	#define BBOX_ITEM(type,var,var_name)					
	#define BBOX_END(name)								            memcpy(&_##name##_data, p_buff, X_FRAME_MIN(len, sizeof(x_##name##_bbox_t)));}

    #define BBOX_RECORD_START(name,rec_cmd_set,rec_cmd_id,data_size,head_cnt,tail_count)		void bbox_##name##_data_rec_set(uint8_t *p_buff, uint32_t len){
	#define BBOX_RECORD_ITEM(type,var,var_name)					
	#define BBOX_RECORD_END(name)								                                    memcpy(&_##name##_rec_data, p_buff, X_FRAME_MIN(len, sizeof(x_##name##_bbox_rec_t)));}
#endif

#ifdef GEN_BBOX_INIT_ENTITY
    #undef BBOX_START
    #undef BBOX_ITEM
    #undef BBOX_END
    #undef BBOX_RECORD_START
    #undef BBOX_RECORD_ITEM
    #undef BBOX_RECORD_END

    ///> 实际的data_size要再加上BBOX_HIDE_DATA_T还有一个crc
	#define BBOX_START(name,rec_cmd_set,rec_cmd_id,data_size)		static x_##name##_bbox_t _##name##_buf_arr[BBOX_MAX_READ_DATA_CNT + 1];\
                                                                    static uint8_t _##name##_flash_buff[data_size + sizeof(BBOX_HIDE_DATA_T) + 4];\
                                                                    static const BBOX_FLASH_CFG_T  _bbox_cfg_##name= {\
                                                                                           .pfn_data_get = bbox_##name##_data_get,\
                                                                                           .pfn_data_set = bbox_##name##_data_set,\
                                                                                           .pfn_data_upload = bbox_##name##_upload,\
                                                                                           .p_flash_buf = _##name##_flash_buff,\
                                                                                           .p_buf = (uint8_t *)&_##name##_buf_arr[0],\
                                                                                           .buf_size = sizeof(x_##name##_bbox_t),\
                                                                                           .dat_size = data_size + sizeof(BBOX_HIDE_DATA_T) + 4,\
                                                                                           .head_cnt = BBOX_MAX_READ_DATA_CNT,\
                                                                                           .tail_cnt = 1,\
                                                                                           .type = BBOX_CTRL_NORMAL};\
                                                                    AUTO_REG_ITEM(bbox_cfg_##name,AUTO_REG_BBOX,(void*)&_bbox_cfg_##name)
                                                            
	#define BBOX_ITEM(type,var,var_name)					
	#define BBOX_END(name)	

    #define BBOX_RECORD_START(name,rec_cmd_set,rec_cmd_id,data_size,head_count,tail_count)		static x_##name##_bbox_rec_t _##name##_rec_buf_arr[head_count + tail_count];\
                                                                                                static uint8_t _##name##_rec_flash_buff[data_size + sizeof(BBOX_HIDE_DATA_T) + 4];\
                                                                                                static const BBOX_FLASH_CFG_T  _bbox_rec_cfg_##name= {\
                                                                                                bbox_##name##_data_rec_get,bbox_##name##_data_rec_set,bbox_##name##_rec_upload,_##name##_rec_flash_buff,\
                                                                                                                       .pfn_data_get = bbox_##name##_data_rec_get,\
                                                                                                                       .pfn_data_set = bbox_##name##_data_rec_set,\
                                                                                                                       .pfn_data_upload = bbox_##name##_rec_upload,\
                                                                                                                       .p_flash_buf = _##name##_rec_flash_buff,\
                                                                                                                       .p_buf = (uint8_t *)&_##name##_rec_buf_arr[0],\
                                                                                                                       .buf_size = sizeof(x_##name##_bbox_rec_t),\
                                                                                                                       .dat_size = data_size + sizeof(BBOX_HIDE_DATA_T) + 4,\
                                                                                                                       .head_cnt = head_count,\
                                                                                                                       .tail_cnt = tail_count,\
                                                                                                                       .type = BBOX_CTRL_RECORD};\
                                                                                                AUTO_REG_ITEM(bbox_rec_cfg_##name,AUTO_REG_BBOX,(void*)&_bbox_rec_cfg_##name)
                                                            
	#define BBOX_RECORD_ITEM(type,var,var_name)					
	#define BBOX_RECORD_END(name)		
#endif

#if defined GEN_BBOX_REC_DAT_ENTITY || defined GEN_BBOX_REC_CFG_ENTITY || defined GEN_BBOX_REC_INIT_ENTITY
    #undef BBOX_START
    #undef BBOX_ITEM
    #undef BBOX_END
    #undef BBOX_RECORD_START
    #undef BBOX_RECORD_ITEM
    #undef BBOX_RECORD_END

    #ifdef GEN_BBOX_REC_CFG_ENTITY
	    #define GEN_REC_CFG_ENTITY
    #endif
    
    #ifdef GEN_BBOX_REC_DAT_ENTITY
	    #define GEN_REC_DAT_ENTITY
    #endif
    
    #ifdef GEN_BBOX_REC_INIT_ENTITY
	    #define GEN_REC_INIT_ENTITY
    #endif

    #include "rec_def.h"
	#define BBOX_START(name,rec_cmd_set,rec_cmd_id,data_size)		REC_START(name,FUNC_CMDSET_REC,CMD_ID_BBOX_NORMAL_UPLOAD)\
                                                                    px_##name##_bbox_t p_bbox = &_##name##_data;\
                                                                    (void)p_bbox;\
                                                                    REC_ITEM(REC_TYPE_UINT8,_##name##_data._hide.type);\
                                                                    REC_ITEM(REC_TYPE_UINT32,_##name##_data._hide.index);\
                                                                    REC_ITEM(REC_TYPE_UINT16,_##name##_data._hide.time.year);\
                                                                    REC_ITEM(REC_TYPE_UINT8,_##name##_data._hide.time.month);\
                                                                    REC_ITEM(REC_TYPE_UINT8,_##name##_data._hide.time.day);\
                                                                    REC_ITEM(REC_TYPE_UINT8,_##name##_data._hide.time.hour);\
                                                                    REC_ITEM(REC_TYPE_UINT8,_##name##_data._hide.time.min);\
                                                                    REC_ITEM(REC_TYPE_UINT8,_##name##_data._hide.time.sec);\
                                                                    REC_ITEM(REC_TYPE_UINT8,_##name##_data._hide.time.zone);\
                                                                    REC_ITEM(REC_TYPE_UINT16,_##name##_data._hide.event_name);\
                                                                    REC_ITEM(REC_TYPE_UINT16,_##name##_data._hide.event_type);\
                                                                    REC_ITEM(REC_TYPE_UINT32,_##name##_data._hide.bbox_ver);
	#define BBOX_ITEM(type,var,var_name)						    REC_ITEM(type,p_bbox->var_name)
	#define BBOX_END(name)								            REC_END()

    #define BBOX_RECORD_START(name,rec_cmd_set,rec_cmd_id,data_size,head_cnt,tail_count)		REC_START(name##rec,FUNC_CMDSET_REC,CMD_ID_BBOX_RECORD_UPLOAD)\
                                                                                            px_##name##_bbox_rec_t p_bbox = &_##name##_rec_data;\
                                                                                            (void)p_bbox;\
                                                                                            REC_ITEM(REC_TYPE_UINT8,_##name##_rec_data._hide.type);\
                                                                                            REC_ITEM(REC_TYPE_UINT32,_##name##_rec_data._hide.index);\
                                                                                            REC_ITEM(REC_TYPE_UINT16,_##name##_rec_data._hide.time.year);\
                                                                                            REC_ITEM(REC_TYPE_UINT8,_##name##_rec_data._hide.time.month);\
                                                                                            REC_ITEM(REC_TYPE_UINT8,_##name##_rec_data._hide.time.day);\
                                                                                            REC_ITEM(REC_TYPE_UINT8,_##name##_rec_data._hide.time.hour);\
                                                                                            REC_ITEM(REC_TYPE_UINT8,_##name##_rec_data._hide.time.min);\
                                                                                            REC_ITEM(REC_TYPE_UINT8,_##name##_rec_data._hide.time.sec);\
                                                                                            REC_ITEM(REC_TYPE_UINT8,_##name##_rec_data._hide.time.zone);\
                                                                                            REC_ITEM(REC_TYPE_UINT16,_##name##_rec_data._hide.event_name);\
                                                                                            REC_ITEM(REC_TYPE_UINT16,_##name##_rec_data._hide.event_type);\
                                                                                            REC_ITEM(REC_TYPE_UINT32,_##name##_rec_data._hide.bbox_ver);
	#define BBOX_RECORD_ITEM(type,var,var_name)						                        REC_ITEM(type,p_bbox->var_name)
	#define BBOX_RECORD_END(name)								                            REC_END()
    
    #ifdef GEN_BBOX_REC_CFG_ENTITY
	    #undef GEN_REC_CFG_ENTITY
    #endif

    #ifdef GEN_BBOX_REC_DAT_ENTITY
	    #undef GEN_REC_DAT_ENTITY
    #endif
    
    #ifdef GEN_BBOX_REC_INIT_ENTITY
	    #undef GEN_REC_INIT_ENTITY
    #endif
    
#endif


