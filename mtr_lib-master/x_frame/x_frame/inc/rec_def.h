#ifdef GEN_REC_CFG_ENTITY
    #undef REC_START
    #undef REC_ITEM
    #undef REC_ITEM_RENAME
    #undef REC_END
	
	#undef REC_CACHE_START
    #undef REC_CACHE_ITEM
    #undef REC_CACHE_ITEM_RENAME
    #undef REC_CACHE_END
    
    #undef REC_CAP_START
    #undef REC_CAP_ITEM
    #undef REC_CAP_ITEM_RENAME
    #undef REC_CAP_END

    #ifndef __REC_CFG_FUNC__
    #define __REC_CFG_FUNC__
        static void __rec_cfg_item_add(uint8_t *flag,                   \
                                                uint8_t *p_buf,         \
                                                uint8_t index,          \
                                                uint8_t type,           \
                                                uint8_t var_size,       \
                                                uint16_t var_buf_len,   \
                                                const char* p_name,     \
                                                uint16_t *wrt_len,      \
                                                uint8_t *p_num,         \
                                                uint32_t *p_var_cur_total)
        {
            if((var_size + (*p_var_cur_total)) > var_buf_len){
                return;
            }

            (*p_var_cur_total) += var_size;

            if((*flag) || index < (*p_num)){
                return;
            }
            
            rec_var_item_t item = {index, type, 0, strlen(p_name)};
            if((*wrt_len) + sizeof(rec_var_item_t) + item.rec_var_name_len <= MAX_RECORDER_PACK_LEN){
                memcpy(p_buf + (*wrt_len), &item, sizeof(rec_var_item_t));
                memcpy(p_buf + (*wrt_len) + sizeof(rec_var_item_t), p_name, item.rec_var_name_len);
                (*wrt_len) += sizeof(rec_var_item_t) + item.rec_var_name_len;
                (*p_num)++;
            }else{
                (*flag) = 1;
            }
        }
    #endif
    
    #define REC_START(name,rec_cmd_set,rec_cmd_id)      bool rec_cfg_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *))                           \
                                                { uint8_t _rec_buf[MAX_RECORDER_PACK_LEN]={0};              \
                                                  uint16_t rec_w_index = sizeof(rec_cfg_item_t);          \
                                                  rec_cfg_item_t* p_cfg_item=(rec_cfg_item_t*)(_rec_buf);   \
                                                  static uint8_t i = 0;                                            \
                                                  uint8_t last_i = i;                                       \
                                                  uint8_t flag = 0;                                         \
                                                  const char* pack_name = #name;                            \
                                                  uint8_t max_var_item = 0;                                 \
                                                  uint32_t rec_pack_len = 4;                                \
                                                  p_cfg_item->pack_cmd_set = rec_cmd_set;                   \
                                                  p_cfg_item->pack_cmd_id  = rec_cmd_id;                    \
                                                  p_frame_info->src     = HOST_ADDR;                        \
                                                  p_frame_info->cmd_func= REC_CMDSET_REC;                   \
                                                  p_frame_info->cmd_id  = REC_CMDID_REC_CFG;                \
                                                  p_cfg_item->pack_name_len = strlen(pack_name);             \
                                                  memcpy(p_cfg_item->pack_name,pack_name,strlen(pack_name));\
                                                  rec_w_index+=strlen(pack_name);
                                                
                                                
     #define REC_ITEM(type,var)                   __rec_cfg_item_add(&flag,                     \
                                                                     _rec_buf,                  \
                                                                     max_var_item++,            \
                                                                     type,                      \
                                                                     sizeof(var),               \
                                                                     MAX_RECORDER_PACK_LEN,     \
                                                                     get_var_name(#var),        \
                                                                     &rec_w_index,              \
                                                                     &i,                        \
                                                                     &rec_pack_len);
                                                  
     #define REC_ITEM_RENAME(type,var,var_name)       __rec_cfg_item_add(&flag,                 \
                                                                     _rec_buf,                  \
                                                                     max_var_item++,            \
                                                                     type,                      \
                                                                     sizeof(var),               \
                                                                     MAX_RECORDER_PACK_LEN,     \
                                                                     #var_name,                 \
                                                                     &rec_w_index,              \
                                                                     &i,                        \
                                                                     &rec_pack_len);                                          
                                    
    #define REC_END()                             p_cfg_item->pack_item_num = max_var_item;                                            \
                                                  p_frame_info->pdata    =_rec_buf;                                                 \
                                                  p_frame_info->data_len = rec_w_index;                                             \
                                                  if(0 != rec_protocol_send(p_frame_info)){i = last_i;};if(i >= max_var_item){i = 0; return true;}else{return false;}}
												  
												  
												  
	#define REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)     \
													static uint32_t rec_pack_cache_##name##_len=4;\
													static uint8_t flg_pack_##name##_init=0;\
													bool rec_cfg_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *)) \
                                                { uint8_t _rec_buf[MAX_RECORDER_PACK_LEN]={0};              \
                                                  uint16_t rec_w_index = sizeof(rec_cfg_item_t);          \
                                                  rec_cfg_item_t* p_cfg_item=(rec_cfg_item_t*)(_rec_buf);   \
                                                  static uint8_t i = 0;                                            \
                                                  uint8_t flag = 0;                                         \
                                                  uint32_t cur_cache_size = cache_size;						\
                                                  uint32_t rec_pack_len = 4;								\
                                                  const char* pack_name = #name;                            \
                                                  uint8_t max_var_item = 0;                                 \
                                                  p_cfg_item->pack_cmd_set = rec_cmd_set;                   \
                                                  p_cfg_item->pack_cmd_id  = rec_cmd_id;                    \
                                                  p_frame_info->src     = HOST_ADDR;                        \
                                                  p_frame_info->cmd_func= REC_CMDSET_REC;                   \
                                                  p_frame_info->cmd_id  = REC_CMDID_REC_CFG;                \
                                                  p_cfg_item->pack_name_len = strlen(pack_name);             \
                                                  memcpy(p_cfg_item->pack_name,pack_name,strlen(pack_name));\
                                                  rec_w_index+=strlen(pack_name);
                                                

     #define REC_CACHE_ITEM(name,type,var)        __rec_cfg_item_add(&flag,                     \
                                                                     _rec_buf,                  \
                                                                     max_var_item++,            \
                                                                     type,                      \
                                                                     sizeof(var),               \
                                                                     cur_cache_size,            \
                                                                     get_var_name(#var),        \
                                                                     &rec_w_index,              \
                                                                     &i,                        \
                                                                     &rec_pack_len);
												  
     #define REC_CACHE_ITEM_RENAME(name,type,var,var_name)        __rec_cfg_item_add(&flag,                 \
                                                                                     _rec_buf,              \
                                                                                     max_var_item++,        \
                                                                                     type,                  \
                                                                                     sizeof(var),           \
                                                                                     cur_cache_size,        \
                                                                                     #var_name,             \
                                                                                     &rec_w_index,          \
                                                                                     &i,                    \
                                                                                     &rec_pack_len);                                         
                                                

    #define REC_CACHE_END(name)            		  p_cfg_item->pack_item_num = max_var_item;                                            \
												  if(flg_pack_##name##_init==0){flg_pack_##name##_init = 1;rec_pack_cache_##name##_len=rec_pack_len;}      \
                                                  p_frame_info->pdata    =_rec_buf;                                                 \
                                                  p_frame_info->data_len = rec_w_index;                                             \
                                                  rec_protocol_send(p_frame_info);if(i >= max_var_item){i = 0; return true;}else{return false;}}

    #define REC_CAP_START(name,rec_cmd_set,rec_cmd_id,cache_size)     \
													static uint32_t rec_pack_cache_##name##_len=0;\
													static uint8_t flg_pack_##name##_init=0;\
													static uint8_t flg_full_##name=1;\
													static int32_t (*rec_cap_send##name)(hal_frame_info_t *) = NULL;   \
													bool rec_cfg_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *))                          			 \
                                                { uint8_t _rec_buf[MAX_RECORDER_PACK_LEN]={0};              \
                                                  uint16_t rec_w_index = sizeof(rec_cfg_item_t);          \
                                                  rec_cfg_item_t* p_cfg_item=(rec_cfg_item_t*)(_rec_buf);   \
                                                  static uint8_t i = 0;                                            \
                                                  uint8_t flag = 0;                                         \
                                                  const char* pack_name = #name;                            \
                                                  uint8_t max_var_item = 0;                                 \
                                                  uint32_t rec_pack_len = 0;								\
                                                  uint32_t cur_cache_size = cache_size;						\
                                                  p_cfg_item->pack_cmd_set = rec_cmd_set;                   \
                                                  p_cfg_item->pack_cmd_id  = rec_cmd_id;                    \
                                                  p_frame_info->src     = HOST_ADDR;                        \
                                                  p_frame_info->dest    = PC_ADDR;                          \
                                                  p_frame_info->cmd_func= REC_CMDSET_REC;                   \
                                                  p_frame_info->cmd_id  = REC_CMDID_REC_CAP_CFG;                \
                                                  p_cfg_item->pack_name_len = strlen(pack_name);             \
                                                  memcpy(p_cfg_item->pack_name,pack_name,strlen(pack_name));\
                                                  rec_w_index+=strlen(pack_name);

    #define REC_CAP_ITEM(name,type,var)           __rec_cfg_item_add(&flag,                 \
                                                                     _rec_buf,              \
                                                                     max_var_item++,        \
                                                                     type,                  \
                                                                     sizeof(var),           \
                                                                     cur_cache_size,        \
                                                                     get_var_name(#var),    \
                                                                     &rec_w_index,          \
                                                                     &i,                    \
                                                                     &rec_pack_len);
    
    #define REC_CAP_ITEM_RENAME(name,type,var,var_name)           __rec_cfg_item_add(&flag,             \
                                                                                     _rec_buf,          \
                                                                                     max_var_item++,    \
                                                                                     type,              \
                                                                                     sizeof(var),       \
                                                                                     cur_cache_size,    \
                                                                                     #var_name,         \
                                                                                     &rec_w_index,      \
                                                                                     &i,                \
                                                                                     &rec_pack_len);

    #define REC_CAP_END(name)                     p_cfg_item->pack_item_num = max_var_item;                                            \
                                                  if(flg_pack_##name##_init==0){flg_pack_##name##_init=1;rec_pack_cache_##name##_len=rec_pack_len;rec_cap_send##name=rec_protocol_send;}     \
                                                  p_frame_info->pdata    =_rec_buf;                                                 \
                                                  p_frame_info->data_len = rec_w_index;                                             \
                                                  rec_protocol_send(p_frame_info);if(i >= max_var_item){i = 0; return true;}else{return false;}}
#endif
                                                  

#ifdef GEN_REC_DAT_ENTITY
    #undef REC_START
    #undef REC_ITEM
    #undef REC_ITEM_RENAME
    #undef REC_END
	
	#undef REC_CACHE_START
    #undef REC_CACHE_ITEM
    #undef REC_CACHE_ITEM_RENAME
    #undef REC_CACHE_END
    
    #undef REC_CAP_START
    #undef REC_CAP_ITEM
    #undef REC_CAP_ITEM_RENAME
    #undef REC_CAP_END

    #define REC_START(name,rec_cmd_set,rec_cmd_id)      int32_t rec_dat_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *))                           \
                                                { uint8_t _rec_buf[MAX_RECORDER_PACK_LEN + 4]={0};      \
                                                  uint16_t rec_w_index = sizeof(uint32_t);          \
                                                  uint32_t cur_ms = get_sys_ms();                   \
                                                  p_frame_info->src     = HOST_ADDR;                \
                                                  p_frame_info->cmd_func= rec_cmd_set;                  \
                                                  p_frame_info->cmd_id  = rec_cmd_id;                   \
                                                  memcpy(_rec_buf,&cur_ms,sizeof(uint32_t));
                                                
                                                
                                                  
     #define REC_ITEM(type,var)                   if(rec_w_index+sizeof(var)<=MAX_RECORDER_PACK_LEN){\
                                                    memcpy(&_rec_buf[rec_w_index],(uint8_t*)&var,sizeof(var));rec_w_index+=sizeof(var);}
                                                  
     #define REC_ITEM_RENAME(type,var,var_name)   REC_ITEM(type,var)    
     
     #define REC_END()                            p_frame_info->pdata    =_rec_buf;\
                                                  p_frame_info->data_len = rec_w_index;\
                                                  return rec_protocol_send(p_frame_info);}

												  
	 #define REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)    \
												void rec_cache_dat_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *))   \
                                                {   static uint8_t  rec_cache_dat[cache_size];\
													static uint32_t cur_cnt= 0;          \
													uint32_t cur_cache_size = cache_size;\
													uint32_t cur_ms= get_sys_ms(); \
													if(flg_pack_##name##_init==0){return;}\
													p_frame_info->cmd_func= rec_cmd_set;                  \
													p_frame_info->cmd_id  = rec_cmd_id;    \
													if( (cur_cnt+rec_pack_cache_##name##_len) > (cur_cache_size )){\
													p_frame_info->pdata    = rec_cache_dat;					\
													p_frame_info->data_len = cur_cnt;					\
													cur_cnt = 0;\
													p_frame_info->src     = HOST_ADDR;                \
													rec_protocol_send(p_frame_info);}                  	\
													memcpy(&rec_cache_dat[cur_cnt],&cur_ms,sizeof(uint32_t)); \
													cur_cnt+=4;
													
                                              
     #define REC_CACHE_ITEM(name,type,var)           if(cur_cnt+sizeof(var)<=cur_cache_size){\
	 													memcpy(&rec_cache_dat[cur_cnt],(uint8_t*)&var,sizeof(var));cur_cnt+=sizeof(var);}      
													
     #define REC_CACHE_ITEM_RENAME(name,type,var,var_name)                REC_CACHE_ITEM(name,type,var)                                   
                                    
     #define REC_CACHE_END(name)        			}				
												
    #define REC_CAP_START(name,rec_cmd_set,rec_cmd_id,cache_size)    \
                                                    static uint8_t  rec_cap_buf_##name[cache_size+8]={0};					\
                                                    static rec_cap_node_t  rec_init_##name##_node={{NULL,NULL},NULL,{0,cache_size,0}};	\
													void rec_cap_dat_##name(void)          		                   \
                                                    { \
													if(flg_pack_##name##_init==0){return;}\
													if(rec_init_##name##_node.cap_rec_priv_dat.is_ready==0){return;}\
													if( (rec_init_##name##_node.cap_rec_priv_dat.current_size + rec_pack_cache_##name##_len) <= (rec_init_##name##_node.cap_rec_priv_dat.total_size  )){

													
                                              
     #define REC_CAP_ITEM(name,type,var)           memcpy(&rec_cap_buf_##name[rec_init_##name##_node.cap_rec_priv_dat.current_size],(uint8_t*)&var,sizeof(var));      \
                                                   rec_init_##name##_node.cap_rec_priv_dat.current_size+=sizeof(var);     
													
     #define REC_CAP_ITEM_RENAME(name,type,var,var_name)      REC_CAP_ITEM(name,type,var)                                       
                                    
     #define REC_CAP_END(name)        			    }else{flg_full_##name=1;}		}
#endif

#ifdef GEN_REC_INIT_ENTITY
    #undef REC_START
    #undef REC_ITEM
    #undef REC_ITEM_RENAME
    #undef REC_END
	
	#undef REC_CACHE_START
    #undef REC_CACHE_ITEM
    #undef REC_CACHE_ITEM_RENAME
    #undef REC_CACHE_END
    
    #undef REC_CAP_START
    #undef REC_CAP_ITEM
    #undef REC_CAP_ITEM_RENAME
    #undef REC_CAP_END
    
    #define REC_START(name,cmd_set,cmd_id)      static rec_node_t  rec_init_##name##_node={0};                   \
												extern void rec_init_##name(void);								 \
												REG_INIT_MOD_BASIC(rec_init_##name)									\
                                                void rec_init_##name(void)                                \
                                                {                                                         \
                                                    INIT_LIST_HEAD(&rec_init_##name##_node.rec_init_node);\
                                                    rec_init_##name##_node.rec_cfg_func = rec_cfg_##name;\
                                                    add_rec_node(&rec_init_##name##_node.rec_init_node);   \
                                                
                                                                                           
     #define REC_ITEM(type,var)                 
     #define REC_ITEM_RENAME(type,var,var_name)
     #define REC_END()    }

	 #define REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)       static rec_node_t  rec_init_##name##_node={0};                   \
																			extern void rec_init_##name(void);								 \
																			REG_INIT_MOD_BASIC(rec_init_##name)									\
																			void rec_init_##name(void)                                \
																			{                                                         \
																				INIT_LIST_HEAD(&rec_init_##name##_node.rec_init_node);\
																				rec_init_##name##_node.rec_cfg_func = rec_cfg_##name;\
																				add_rec_node(&rec_init_##name##_node.rec_init_node);   \
																			
                                                                                           
     #define REC_CACHE_ITEM(name,type,var)   
     #define REC_CACHE_ITEM_RENAME(name,type,var,var_name)
     #define REC_CACHE_END(name)    }

    #define REC_CAP_START(name,rec_cmd_set,rec_cmd_id,cache_size)					\
            extern void rec_init_##name(void);								 \
			REG_INIT_MOD_BASIC(rec_init_##name)									\
			void rec_init_##name(void)								\
			{											\
				INIT_LIST_HEAD(&rec_init_##name##_node.rec_init_node);				\
				rec_init_##name##_node.rec_cfg_func = rec_cfg_##name;				\
				add_rec_node(&rec_init_##name##_node.rec_init_node);   				\
			}

	 #define REC_CAP_ITEM(name,type,var)
     #define REC_CAP_ITEM_RENAME(name,type,var,var_name)
	 #define REC_CAP_END(name)
#endif                                                
                                                  
#ifdef GEN_REC_CAP_ACTION_ENTITY
    #undef REC_START
    #undef REC_ITEM
    #undef REC_ITEM_RENAME
    #undef REC_END
	
	#undef REC_CACHE_START
    #undef REC_CACHE_ITEM
    #undef REC_CACHE_ITEM_RENAME
    #undef REC_CACHE_END
    
    #undef REC_CAP_START
    #undef REC_CAP_ITEM
    #undef REC_CAP_ITEM_RENAME
    #undef REC_CAP_END
    
		
		
    #define REC_START(name,cmd_set,cmd_id)        
    #define REC_ITEM(type,var)
    #define REC_ITEM_RENAME(type,var,var_name)
    #define REC_END()
	
    #define REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)         
    #define REC_CACHE_ITEM(name,type,var)
    #define REC_CACHE_ITEM_RENAME(name,type,var,var_name)
    #define REC_CACHE_END(name)
		
		
    #define REC_CAP_START(name,rec_cmd_set,rec_cmd_id,cache_size)      \
															uint32_t clear_rec_cap_flg(void)\
															{\
																if(flg_full_##name>0)\
																{\
																	rec_init_##name##_node.cap_rec_priv_dat.current_size = 0;\
																	rec_init_##name##_node.cap_rec_priv_dat.is_ready = 1;\
																	flg_full_##name = 0;\
																	return 1;\
																}\
																else\
																{\
																	return 0;\
																}\
															}\
															int32_t start_rec_cap(hal_frame_info_t *p_frame) \
															{\
																	uint8_t  is_ok = 0;\
																	clear_rec_cap_flg();\
																	protocol_send_ack(p_frame,&is_ok,sizeof(uint8_t));\
																	return 0;\
															}\
												            REG_CMDSET_ACTION(REC_CMDSET_REC,REC_CMDID_START_CAP,start_rec_cap);

    #define REC_CAP_ITEM(name,type,var)          
    #define REC_CAP_ITEM_RENAME(name,type,var,var_name)
    #define REC_CAP_END(name) 
#endif



#ifdef GEN_REC_CAP_PUSH_ENTITY
    #undef REC_START
    #undef REC_ITEM
    #undef REC_ITEM_RENAME
    #undef REC_END
	
	#undef REC_CACHE_START
    #undef REC_CACHE_ITEM
    #undef REC_CACHE_ITEM_RENAME
    #undef REC_CACHE_END
    
    #undef REC_CAP_START
    #undef REC_CAP_ITEM
    #undef REC_CAP_ITEM_RENAME
    #undef REC_CAP_END
	
    #define REC_START(name,cmd_set,cmd_id)        
    #define REC_ITEM(type,var)
    #define REC_ITEM_RENAME(type,var,var_name)
    #define REC_END()
	
    #define REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)         
    #define REC_CACHE_ITEM(name,type,var)
    #define REC_CACHE_ITEM_RENAME(name,type,var,var_name)
    #define REC_CACHE_END(name)
		
		
    #define REC_CAP_START(name,rec_cmd_set,rec_cmd_id,cache_size)      \
															void rec_cap_push_task(void) \
															{\
															        if(flg_pack_##name##_init==0){return;}\
																	hal_frame_info_t frame_info={0};\
																	frame_info.cmd_func = REC_CMDSET_REC;\
																	frame_info.cmd_id = REC_CMDID_PUSH_CAP_BUF_STA;\
																	frame_info.src = HOST_ADDR;\
																	frame_info.pdata = (uint8_t *)&rec_init_##name##_node.cap_rec_priv_dat;\
																	frame_info.data_len = sizeof(cap_rec_priv_dat_t);\
																	rec_cap_send##name(&frame_info);\
															}\
														    REG_TASK(ID_TASK,2000,rec_cap_push_task);

    #define REC_CAP_ITEM(name,type,var)  
    #define REC_CAP_ITEM_RENAME(name,type,var,var_name)
    #define REC_CAP_END(name) 
#endif



#ifdef GEN_REC_CAP_TRANS_CFG_ENTITY
	#undef REC_START
    #undef REC_ITEM
    #undef REC_ITEM_RENAME
    #undef REC_END
	
	#undef REC_CACHE_START
    #undef REC_CACHE_ITEM
    #undef REC_CACHE_ITEM_RENAME
    #undef REC_CACHE_END
    
    #undef REC_CAP_START
    #undef REC_CAP_ITEM
    #undef REC_CAP_ITEM_RENAME
    #undef REC_CAP_END
			
	#define REC_START(name,cmd_set,cmd_id)        
    #define REC_ITEM(type,var)
    #define REC_ITEM_RENAME(type,var,var_name)
    #define REC_END()
	
	#define REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)         
    #define REC_CACHE_ITEM(name,type,var)
    #define REC_CACHE_ITEM_RENAME(name,type,var,var_name)
    #define REC_CACHE_END(name)
		
		
    #define REC_CAP_START(name,rec_cmd_set,rec_cmd_id,cache_size)      \
					uint8_t  trans_dat[REC_CAP_TRANS_DAT_LEN+2] = {0};\
					cap_rec_cfg_dat_t cap_rec_cfg_dat = {0};\
					int32_t cap_trans_cfg_action(hal_frame_info_t *p_frame) \
					{\
							cap_rec_cfg_dat.cap_pack_cmd_set 					= rec_cmd_set;\
							cap_rec_cfg_dat.cap_pack_cmd_id  					= rec_cmd_id;\
							cap_rec_cfg_dat.max_trans_bytes_per_block = REC_CAP_TRANS_DAT_LEN;\
							cap_rec_cfg_dat.total_blocks              = rec_init_##name##_node.cap_rec_priv_dat.total_size / cap_rec_cfg_dat.max_trans_bytes_per_block;\
							if(rec_init_##name##_node.cap_rec_priv_dat.total_size % cap_rec_cfg_dat.max_trans_bytes_per_block) {cap_rec_cfg_dat.total_blocks++;}\
							protocol_send_ack(p_frame,(uint8_t*)&cap_rec_cfg_dat,sizeof(cap_rec_cfg_dat_t));\
							return 0;\
					}\
				    REG_CMDSET_ACTION(REC_CMDSET_REC,REC_CMDID_GET_CAP_BUF_CFG,cap_trans_cfg_action);

    #define REC_CAP_ITEM(name,type,var)
    #define REC_CAP_ITEM_RENAME(name,type,var,var_name)
    #define REC_CAP_END(name) 
#endif




#ifdef GEN_REC_CAP_TRANS_DAT_ENTITY
    #undef REC_START
    #undef REC_ITEM
    #undef REC_ITEM_RENAME
    #undef REC_END
	
	#undef REC_CACHE_START
    #undef REC_CACHE_ITEM
    #undef REC_CACHE_ITEM_RENAME
    #undef REC_CACHE_END
    
    #undef REC_CAP_START
    #undef REC_CAP_ITEM
    #undef REC_CAP_ITEM_RENAME
    #undef REC_CAP_END
    
    #define REC_START(name,cmd_set,cmd_id)        
    #define REC_ITEM(type,var)
    #define REC_ITEM_RENAME(type,var,var_name)
    #define REC_END()
	
    #define REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)         
    #define REC_CACHE_ITEM(name,type,var)
    #define REC_CACHE_ITEM_RENAME(name,type,var,var_name)
    #define REC_CACHE_END(name)
		
		
    #define REC_CAP_START(name,rec_cmd_set,rec_cmd_id,cache_size)      \
					int32_t cap_trans_dat_action(hal_frame_info_t *p_frame) \
					{\
							uint16_t need_trans_block_num = 0;\
							uint32_t trans_dat_len = 0;\
							if(p_frame->data_len>=sizeof(uint16_t))\
							{\
								memcpy(&need_trans_block_num,p_frame->pdata,sizeof(uint16_t));\
								if(need_trans_block_num<(cap_rec_cfg_dat.total_blocks-1))\
								{\
									trans_dat_len = REC_CAP_TRANS_DAT_LEN;\
								}\
								else if(need_trans_block_num==(cap_rec_cfg_dat.total_blocks-1))\
								{\
									 trans_dat_len = rec_init_##name##_node.cap_rec_priv_dat.total_size - (need_trans_block_num*REC_CAP_TRANS_DAT_LEN);\
								}\
								memcpy(&trans_dat[0],&need_trans_block_num,sizeof(uint16_t));\
								memcpy(&trans_dat[2],&rec_cap_buf_##name[need_trans_block_num*REC_CAP_TRANS_DAT_LEN],trans_dat_len);\
								protocol_send_ack(p_frame,&trans_dat[0],trans_dat_len+2);\
							}\
							return 0;\
					}\
				    REG_CMDSET_ACTION(REC_CMDSET_REC,REC_CMDID_GET_CAP_BUF_DAT,cap_trans_dat_action);

    #define REC_CAP_ITEM(name,type,var)     
    #define REC_CAP_ITEM_RENAME(name,type,var,var_name)
    #define REC_CAP_END(name) 
#endif






