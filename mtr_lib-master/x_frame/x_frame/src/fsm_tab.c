#include "x_frame.h"



static const fsm_item_obj_t* find_index_by_state(fsm_obj_t* p_fsm,uint8_t state)
{
	uint8_t i = 0;
	for(i=0;i<p_fsm->fsm_state_num;i++)
	{
		if(p_fsm->p_fsm_item_tab[i].fsm_state_id == state)
		{
			return &p_fsm->p_fsm_item_tab[i];
		}
	}
	return NULL;
}

static uint8_t  check_state_is_ok(fsm_obj_t* p_fsm ,uint8_t state)
{
	uint8_t i = 0;
	for(i=0;i<p_fsm->fsm_state_num;i++)
	{
		if(p_fsm->p_fsm_item_tab[i].fsm_state_id == state)
		{
			return 1;
		}
	}
	return 0;
}

//static void show_fsm_info(fsm_obj_t* p_fsm)
//{
//	DBG_I("p_fsm:%s item:%d cur_state:%d ",p_fsm->fsm_name,p_fsm->fsm_state_num,p_fsm->cur_state);
//}

void real_fsm_fun(fsm_obj_t* p_fsm)
{
	const fsm_item_obj_t* p_cur_fsm_item;
	const fsm_item_obj_t* p_last_fsm_item;
	
	//show_fsm_info(p_fsm);
	if(p_fsm->fsm_is_init==0)
	{
		uint8_t i = 0;
		p_fsm->fsm_is_init = 1;
		
		p_fsm->last_state = p_fsm->fsm_init_state;
		p_fsm->cur_state  = p_fsm->fsm_init_state;
		p_fsm->next_state = p_fsm->fsm_init_state;
		
		p_fsm->state_is_change = 1;		
		
		
		for(i=0;i<p_fsm->fsm_state_num;i++)
		{
			if(p_fsm->p_fsm_item_tab[i].fsm_state_id == UNCHANGE_STATE)
			{
				DBG_E("fsm_name:[%s]-[%d] count:%d  STATE_ERR ",p_fsm->fsm_name,p_fsm->fsm_id,i);
			}
		}
		
	}
	
	if(p_fsm->fsm_is_init ==0 ) {return ;}
	
	if(p_fsm->state_is_change>0)
	{
		p_fsm->state_is_change = 0;
		p_fsm->last_state = p_fsm->cur_state;
		p_fsm->cur_state  = p_fsm->next_state;
		
		p_last_fsm_item = find_index_by_state(p_fsm,p_fsm->last_state);
		if( (p_last_fsm_item!=NULL) && (p_last_fsm_item->fsm_out_func!=NULL))
		{
			p_last_fsm_item->fsm_out_func();
		}

		p_cur_fsm_item = find_index_by_state(p_fsm,p_fsm->cur_state);
		if( (p_cur_fsm_item!=NULL) && (p_cur_fsm_item->fsm_in_func!=NULL))
		{
			p_cur_fsm_item->fsm_in_func();
		}

        if(0 == p_fsm->state_is_change)
            p_fsm->action_event = 0;
	}
	
    if(0 == p_fsm->state_is_change)
    {
        p_cur_fsm_item = find_index_by_state(p_fsm,p_fsm->cur_state);
        if( (p_cur_fsm_item!=NULL) && (p_cur_fsm_item->fsm_exe_func!=NULL))
        {
            p_cur_fsm_item->fsm_exe_func();
        }
    }	
}

void real_fsm_emit_event(fsm_obj_t* p_fsm, uint32_t event, uint8_t *pbuf, uint32_t len)
{
	
	const fsm_item_obj_t* p_cur_fsm_item;
	uint8_t next_state;
	p_cur_fsm_item = find_index_by_state(p_fsm,p_fsm->cur_state);
	if( (p_cur_fsm_item!=NULL) && (p_cur_fsm_item->fsm_cond_func!=NULL))
	{
		next_state = p_cur_fsm_item->fsm_cond_func(event,pbuf,len);
		if(check_state_is_ok(p_fsm,next_state))
		{
			p_fsm->next_state = next_state;
			p_fsm->state_is_change  = 1;
			p_fsm->action_event = event;
		}
	}
}

#if  0   //demo code
//==================================================================
		
#define DEMO_ID  1		
		
#define EV_DEMO1  1
#define EV_DEMO2  2
		
static void demo_init_in				(void);
static void demo_init_exe				(void);
static uint32_t demo_init_chk_func		(uint32_t event, uint8_t *pbuf, uint32_t len);
static void demo_init_in2				(void);
static void demo_init_exe2				(void);
static uint32_t demo_init_chk_func2		(uint32_t event, uint8_t *pbuf, uint32_t len);
				
		
REG_FSM_TAB(DEMO_ID,0,1,
	{1,demo_init_in ,	demo_init_exe,	NULL,	demo_init_chk_func},		
	{2,demo_init_in2,	demo_init_exe2,	NULL,	demo_init_chk_func2},	
);
	
		
static uint32_t in1_snamp = 0;		
static void demo_init_in				(void)
{
	DBG_I("demo_init_in");
	in1_snamp = get_sys_ms();
}

static void demo_init_exe				(void)
{
	DBG_I("demo_init_exe");
	if( (get_sys_ms() - in1_snamp) > 3000)
	{
		EMIT_FSM(DEMO_ID,EV_DEMO1,NULL,0);
	}
}

static uint32_t demo_init_chk_func		(uint32_t event, uint8_t *pbuf, uint32_t len)
{
	DBG_I("demo_init_chk_func @@@@@@@@@@");
	if(event == EV_DEMO1) {return 2;}
	return 0;
}

static uint32_t in1_snamp2 = 0;		
static void demo_init_in2				(void)
{
	DBG_I("demo_init_in2 -------");
	in1_snamp2 = get_sys_ms();
}

static void demo_init_exe2				(void)
{
	DBG_I("demo_init_exe2 -------");
	if( (get_sys_ms() - in1_snamp2) > 3000)
	{
		EMIT_FSM(DEMO_ID,EV_DEMO2,NULL,0);
	}
}

static uint32_t demo_init_chk_func2		(uint32_t event, uint8_t *pbuf, uint32_t len)
{
	DBG_I("demo_init_chk_func @@@@@@@@@@");
	if(event == EV_DEMO2) {return 1;}
	return 0;
}
		


	
void demo_fsm_task(void)
{
	RUN_FSM(DEMO_ID) ;
	DBG_I("demo_fsm_task");
}	
REG_TASK(0,500,demo_fsm_task);
		

#endif 



