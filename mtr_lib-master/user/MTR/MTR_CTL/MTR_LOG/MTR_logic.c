#include "MTR_logic.h"
#include "stdio.h"
#include "MTR_CTRL.h"
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                                 INIT                                                                                                                        
*********************************************************************************************************                                                                     
*/           
void STATE_INIT_in(void *P)
{
//	MTR_CTRL_typedef *p = MTR;
}
int STATE_INIT_exe(void *P)
{
    return 0;
}
void STATE_INIT_out(void *P)
{
    printf("MTR_init_out\r\n");
}
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                             STATE_PWR_OFF                                                                                                                      
*********************************************************************************************************                                                                     
*/
void STATE_BIAS_GET_in(void *P)
{
    printf("MTR_PWR_OFF_in\r\n");
}
int STATE_BIAS_GET_exe(void *P)
{
    printf("MTR_PWR_OFF_exe\r\n");
    return 0;
}
void STATE_BIAS_GET_out(void *P)
{
    printf("MTR_PWR_OFF_out\r\n");
}
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                                 STATE_PARAM_ID                                                                                                                        
*********************************************************************************************************                                                                     
*/           
void STATE_PARAM_ID_in(void *P)
{
    printf("MTR_init_in\n");
}
int STATE_PARAM_ID_exe(void *P)
{

    return STATE_BIAS_GET;
}
void STATE_PARAM_ID_out(void *P)
{

}
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                                 STATE_IDL                                                                                                                        
*********************************************************************************************************                                                                     
*/           
void STATE_IDLE_in(void *P)
{

}
int STATE_IDLE_exe(void *P)
{

    return STATE_BIAS_GET;
}
void STATE_IDLE_out(void *P)
{


}
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                                 STATE_RUN_SPEED                                                                                                                        
*********************************************************************************************************                                                                     
*/           
void STATE_RUN_SPEED_in(void *P)
{

}
int STATE_RUN_SPEED_exe(void *P)
{

    return STATE_BIAS_GET;
}
void STATE_RUN_SPEED_out(void *P)
{

}
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                                 STATE_RUN_POSITION                                                                                                                        
*********************************************************************************************************                                                                     
*/           
void STATE_RUN_POSITION_in(void *P)
{
    printf("MTR_init_in\n");
}
int STATE_RUN_POSITION_exe(void *P)
{
    return STATE_BIAS_GET;
}
void STATE_RUN_POSITION_out(void *P)
{
}
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                                 STATE_STOP                                                                                                                        
*********************************************************************************************************                                                                     
*/           
void STATE_STOP_in(void *P)
{
}
int STATE_STOP_exe(void *P)
{
    return STATE_BIAS_GET;
}
void STATE_STOP_out(void *P)
{
}
/*                                                                                                                                                                            
*********************************************************************************************************                                                                     
*                                                 STATE_ABNORMAL                                                                                                                        
*********************************************************************************************************                                                                     
*/           
void STATE_ABNORMAL_in(void *P)
{
}
int STATE_ABNORMAL_exe(void *P)
{
    return STATE_BIAS_GET;
}
void STATE_ABNORMAL_out(void *P)
{
	
}

