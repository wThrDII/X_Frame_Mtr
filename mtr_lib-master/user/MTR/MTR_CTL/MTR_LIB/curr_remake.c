#include "curr_remake.h"


#include "foc_curreconstruct.h"

static void FOC_PWM_CUR1S_Set_Point(MTR_1SCurrent_t *In,MATH_vec3 *pwm);
static void FOC_CUR1SRemake(MTR_1SCurrent_t *In,HAL_AdcData_t *pAdcData);


/*还需要添加电流转换实际电流*/
void CalRealVal(MTR_CTRL_typedef * ptr)
{
    #if SAMPLING_RESISTACE == Single_Shunt_1
    FOC_CUR1SRemake(&ptr->MTR_Cur1S,&ptr->HAL_AdcData_t);
    #elif SAMPLING_RESISTACE == Single_Shunt_3
	pAdcData->Iabc.value[0] = ptr->ADC_Value.Iadc_REG.value[0]-ptr->ADC_Value.Iadc_Offset_REG.value[0];
    pAdcData->Iabc.value[1] = ptr->ADC_Value.Iadc_REG.value[1]-ptr->ADC_Value.Iadc_Offset_REG.value[1];
    pAdcData->Iabc.value[2] = ptr->ADC_Value.Iadc_REG.value[2]-ptr->ADC_Value.Iadc_Offset_REG.value[2];
    #endif    
}



void MTR0_Adc_Get(MTR_CTRL_typedef * ptr)
{
    #if SAMPLING_RESISTACE == Single_Shunt_1
    ptr->ADC_Value.Iadc_REG.value[0]	  =  (int16_t)(MTR0_SE1 & 0xFFFF);
    ptr->ADC_Value.Iadc_REG.value[1]	  =  (int16_t)(MTR0_SE2 & 0xFFFF);	
    //FOC_CUR1SRemake(&ptr->MTR_Cur1S,&ptr->HAL_AdcData_t);
    #elif SAMPLING_RESISTACE == Single_Shunt_3
    ptr->ADC_Value.Iadc_REG.value[0]	  =  (int16_t)(MTR0_SE1 & 0xFFFF);
    ptr->ADC_Value.Iadc_REG.value[1]	  =  (int16_t)(MTR0_SE2 & 0xFFFF);	    
    ptr->ADC_Value.Iadc_REG.value[2]	  =  (int16_t)(MTR0_SE3 & 0xFFFF);	    
    #endif
}
void MTR1_Adc_Get(MTR_CTRL_typedef * ptr)
{
    #if SAMPLING_RESISTACE == Single_Shunt_1
    ptr->ADC_Value.Iadc_REG.value[0]	  =  (int16_t)(MTR1_SE1 & 0xFFFF);
    ptr->ADC_Value.Iadc_REG.value[1]	  =  (int16_t)(MTR1_SE2 & 0xFFFF);	
    //FOC_CUR1SRemake(&ptr->MTR_Cur1S,&ptr->HAL_AdcData_t);
    #elif SAMPLING_RESISTACE == Single_Shunt_3
    ptr->ADC_Value.Iadc_REG.value[0]	  =  (int16_t)(MTR1_SE1 & 0xFFFF);
    ptr->ADC_Value.Iadc_REG.value[1]	  =  (int16_t)(MTR1_SE2 & 0xFFFF);	    
    ptr->ADC_Value.Iadc_REG.value[2]	  =  (int16_t)(MTR1_SE3 & 0xFFFF);	
    #endif
}



void FOC_CUR1SRemake(MTR_1SCurrent_t *In,HAL_AdcData_t *pAdcData)
{
	int se1,se2;
	if(In->Last_Sample_Chang_Flag)
	{
		se1= pAdcData->Iadc_REG.value[1]-pAdcData->Iadc_Offset_REG.value[1];
		se2= pAdcData->Iadc_REG.value[0]-pAdcData->Iadc_Offset_REG.value[0];
	}else
	{
		se1=pAdcData->Iadc_REG.value[0]-pAdcData->Iadc_Offset_REG.value[0];
		se2=pAdcData->Iadc_REG.value[1]-pAdcData->Iadc_Offset_REG.value[1];
	}

	pAdcData->Iabc.value[In->Last_Sort_Index.value[0]] = se2;
	pAdcData->Iabc.value[In->Last_Sort_Index.value[1]] = se1-se2;
	pAdcData->Iabc.value[In->Last_Sort_Index.value[2]] = -se1;

}

void MCPWM0_Set_PWM (MATH_vec3 *pwm)
{
    MCPWM0_PWMA_OUT(-pwm->value[0],pwm->value[0]);
    MCPWM0_PWMB_OUT(-pwm->value[1],pwm->value[1]);
    MCPWM0_PWMC_OUT(-pwm->value[2],pwm->value[2]);
}
void MCPWM1_Set_PWM (MATH_vec3 *pwm)
{
    MCPWM1_PWMA_OUT(-pwm->value[0],pwm->value[0]);
    MCPWM1_PWMB_OUT(-pwm->value[1],pwm->value[1]);
    MCPWM1_PWMC_OUT(-pwm->value[2],pwm->value[2]);
}

void MCPWM0_Out(struct _MTR_CTRL_ *ptr)
{
    #if SAMPLING_RESISTACE == Single_Shunt_1
    FOC_PWM_CUR1S_Set_Point(ptr->MTR_Cur1S,P_MTR->Uabc_pwm);
    MCPWM0_PRT = 0xDEAD;
    MCPWM0_TMR0 = ptr->MTR_Cur1S.Sample_Point.value[0];
    MCPWM0_TMR1 = ptr->MTR_Cur1S.Sample_Point.value[1];
    MCPWM0_PRT = 0x00;

    MCPWM0_TH00 =  -ptr->MTR_Cur1S.Pwm_Out.value[0] + ptr->MTR_Cur1S.Pwm_Offset.value[0]; 
    MCPWM0_TH01 =   ptr->MTR_Cur1S.Pwm_Out.value[0] + ptr->MTR_Cur1S.Pwm_Offset.value[0];

    MCPWM0_TH10 =  -ptr->MTR_Cur1S.Pwm_Out.value[1] + ptr->MTR_Cur1S.Pwm_Offset.value[1]; 
    MCPWM0_TH11 =   ptr->MTR_Cur1S.Pwm_Out.value[1] + ptr->MTR_Cur1S.Pwm_Offset.value[1];

    MCPWM0_TH20 =  -ptr->MTR_Cur1S.Pwm_Out.value[2] + ptr->MTR_Cur1S.Pwm_Offset.value[2]; 
    MCPWM0_TH21 =   ptr->MTR_Cur1S.Pwm_Out.value[2] + ptr->MTR_Cur1S.Pwm_Offset.value[2];	
	//MTR_PWM_OUT_OFFSET				(&(In->Pwm_Out),   &(In->Pwm_Offset),    MtrNum);
    #elif SAMPLING_RESISTACE == Single_Shunt_3
    MCPWM0_Set_PWM(&P_MTR->Uabc_pwm);
    #endif
}

void MCPWM1_Out(struct _MTR_CTRL_ *ptr)
{
	#if SAMPLING_RESISTACE == Single_Shunt_1
	FOC_PWM_CUR1S_Set_Point				(ptr->MTR_Cur1S,P_MTR->Uabc_pwm);
    MCPWM1_PRT = 0xDEAD;
    MCPWM1_TMR0 = ptr->MTR_Cur1S.Sample_Point.value[0];
    MCPWM1_TMR1 = ptr->MTR_Cur1S.Sample_Point.value[1];
    MCPWM1_PRT = 0x00;

    MCPWM1_TH00 =  -ptr->MTR_Cur1S.Pwm_Out.value[0] + ptr->MTR_Cur1S.Pwm_Offset.value[0]; 
    MCPWM1_TH01 =   ptr->MTR_Cur1S.Pwm_Out.value[0] + ptr->MTR_Cur1S.Pwm_Offset.value[0];

    MCPWM1_TH10 =  -ptr->MTR_Cur1S.Pwm_Out.value[1] + ptr->MTR_Cur1S.Pwm_Offset.value[1]; 
    MCPWM1_TH11 =   ptr->MTR_Cur1S.Pwm_Out.value[1] + ptr->MTR_Cur1S.Pwm_Offset.value[1];

    MCPWM1_TH20 =  -ptr->MTR_Cur1S.Pwm_Out.value[2] + ptr->MTR_Cur1S.Pwm_Offset.value[2]; 
    MCPWM1_TH21 =   ptr->MTR_Cur1S.Pwm_Out.value[2] + ptr->MTR_Cur1S.Pwm_Offset.value[2];	
	//MTR_PWM_OUT_OFFSET				(&(In->Pwm_Out),   &(In->Pwm_Offset),    MtrNum);
    #elif SAMPLING_RESISTACE == Single_Shunt_3
    MCPWM1_Set_PWM(&P_MTR->Uabc_pwm);
    #endif
}

void FOC_PWM_CUR1S_Set_Point(MTR_1SCurrent_t *In,MATH_vec3 *pwm)
{
#define SAMPLING_WINDOW 	500				//CNT   5us/75us*Perider
#define PERIOD 				PWM_PERIOD
#define MAX_SAMPLING_PWM 	PERIOD-SAMPLING_WINDOW-1
        uint8_t max_index = 0;
        uint8_t mid_index = 1;
        uint8_t min_index = 2;        
		In->Pwm_Out = *pwm;
        int tmp;
        if(In->Pwm_Out.value[max_index] < In->Pwm_Out.value[mid_index]){
                tmp=mid_index;
                mid_index = max_index;
                max_index = tmp;
        }
        if(In->Pwm_Out.value[max_index] < In->Pwm_Out.value[min_index]){
                tmp=min_index;
                min_index = max_index;
                max_index = tmp;
        }

        if(In->Pwm_Out.value[mid_index] < In->Pwm_Out.value[min_index]){
                tmp=mid_index;
                mid_index = min_index;
                min_index = tmp;
        }

        if(max_index==0){
                In->Svpwm_Sector = (min_index==2)?0:5;
        }
        if(max_index==1){
                In->Svpwm_Sector = (min_index==2)?1:2;
        }
        if(max_index==2){
                In->Svpwm_Sector = (min_index==0)?3:4;
        }

        In->Last_Sort_Index = In->Pwm_Sort_Index;

        In->Pwm_Sort_Index.value[0] = max_index;
        In->Pwm_Sort_Index.value[1] = mid_index;
        In->Pwm_Sort_Index.value[2] = min_index;

        In->Pwm_Offset.value[0] = 0;
        In->Pwm_Offset.value[1] = 0;
        In->Pwm_Offset.value[2] = 0;
		In->Last_Sample_Chang_Flag = In->Sample_Chang_Flag;
        In->Sample_Chang_Flag = 0;


		
        int Movable = (PERIOD-1) - In->Pwm_Out.value[max_index];
		
        int maxtmp = In->Pwm_Out.value[max_index] - In->Pwm_Out.value[mid_index];
        int mintmp = In->Pwm_Out.value[mid_index] - In->Pwm_Out.value[min_index];

        In->Sample_Point.value[1] = In->Pwm_Out.value[max_index];
        In->Sample_Point.value[0] = In->Pwm_Out.value[mid_index];

        if(maxtmp<=SAMPLING_WINDOW||mintmp<=SAMPLING_WINDOW){
                if(In->Pwm_Out.value[max_index]>MAX_SAMPLING_PWM && In->Pwm_Out.value[mid_index]>MAX_SAMPLING_PWM){//max mid 都大于最大区，均无法移动补窗口，强制改值，在左右移动
                        In->Pwm_Out.value[max_index] = MAX_SAMPLING_PWM;
                        In->Pwm_Out.value[mid_index] = MAX_SAMPLING_PWM-1;
                        maxtmp = In->Pwm_Out.value[max_index] - In->Pwm_Out.value[mid_index];
                        mintmp = In->Pwm_Out.value[mid_index] - In->Pwm_Out.value[min_index];
                        In->Sample_Point.value[1] = In->Pwm_Out.value[max_index];
                        In->Sample_Point.value[0] = In->Pwm_Out.value[mid_index];
                }
                if(In->Pwm_Out.value[min_index]<SAMPLING_WINDOW && In->Pwm_Out.value[mid_index]<SAMPLING_WINDOW){//mid min 太小了，采样窗口无法左右移动补，强制改值，扩大，无需左右移动了
                        In->Pwm_Out.value[mid_index] = SAMPLING_WINDOW;
                        In->Pwm_Out.value[min_index] = SAMPLING_WINDOW-1;
                        maxtmp = In->Pwm_Out.value[max_index] - In->Pwm_Out.value[mid_index];
                        mintmp = In->Pwm_Out.value[mid_index] - In->Pwm_Out.value[min_index];
                        In->Sample_Point.value[1] = In->Pwm_Out.value[max_index];
                        In->Sample_Point.value[0] = In->Pwm_Out.value[mid_index];
                }

                if(Movable<SAMPLING_WINDOW)//max最大，左右移动无法补足，当max达到无法移动的地段时，采取中间pwm移动
                {

                        if(In->Svpwm_Sector%2==0)
                        {
                                if(maxtmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[mid_index]=-(SAMPLING_WINDOW-maxtmp);
                                        In->Sample_Point.value[0] = In->Pwm_Out.value[mid_index]+In->Pwm_Offset.value[mid_index];
                                }
                                if(mintmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[mid_index]=-(SAMPLING_WINDOW-mintmp);
                                        In->Sample_Point.value[0] = -In->Pwm_Out.value[min_index]+In->Pwm_Offset.value[min_index];
                                }
                        }else
                        {
                                if(maxtmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[max_index]=-(SAMPLING_WINDOW-maxtmp);
                                        In->Sample_Point.value[1] = -In->Pwm_Out.value[mid_index]+In->Pwm_Offset.value[mid_index];
                                        int mid_sample = In->Pwm_Out.value[mid_index]+In->Pwm_Offset.value[mid_index];
                                        int max_sample = In->Pwm_Out.value[max_index]+In->Pwm_Offset.value[max_index];
                                        In->Sample_Point.value[0] = mid_sample<max_sample?mid_sample:max_sample;
                                }
                                if(mintmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[min_index]=-(SAMPLING_WINDOW-mintmp);
                                        In->Sample_Point.value[0] = In->Pwm_Out.value[mid_index]+In->Pwm_Offset.value[mid_index];
                                }
                        }
                }
                else 
                if(maxtmp<=SAMPLING_WINDOW&&mintmp<=SAMPLING_WINDOW)//低调制比，固定max右移动，min左移动，这个状态噪音会比较大
                {
                        In->Pwm_Offset.value[max_index]=(SAMPLING_WINDOW-maxtmp);
                        In->Pwm_Offset.value[min_index]=-(SAMPLING_WINDOW-mintmp);
                        In->Sample_Point.value[1] = In->Pwm_Out.value[max_index]+In->Pwm_Offset.value[max_index];
                        In->Sample_Point.value[0] = In->Pwm_Out.value[mid_index]+In->Pwm_Offset.value[mid_index];
                }
                else										//中间调制比，max，mid，min根据情况进行移动，平滑，噪音低
                {
                        if(In->Svpwm_Sector%2==0)
                        {
                                if(maxtmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[max_index]=(SAMPLING_WINDOW-maxtmp);
                                        In->Sample_Point.value[1] = In->Pwm_Out.value[max_index]+In->Pwm_Offset.value[max_index];
                                }
                                if(mintmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[mid_index]=-(SAMPLING_WINDOW-mintmp);
                                        In->Sample_Point.value[0] = -In->Pwm_Out.value[min_index]+In->Pwm_Offset.value[min_index];
                                }
                        }else
                        {
                                if(maxtmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[mid_index]=(SAMPLING_WINDOW-maxtmp);
                                        In->Sample_Point.value[1] = -In->Pwm_Out.value[mid_index]+In->Pwm_Offset.value[mid_index];
                                }
                                if(mintmp<=SAMPLING_WINDOW){
                                        In->Pwm_Offset.value[min_index]=-(SAMPLING_WINDOW-mintmp);
                                        In->Sample_Point.value[0] = In->Pwm_Out.value[mid_index]+In->Pwm_Offset.value[mid_index];
                                }
                        }
                }
        }

        In->Sample_Chang_Flag=0;

        if(In->Sample_Point.value[1]<In->Sample_Point.value[0])
        {
                int c=In->Sample_Point.value[1];
                In->Sample_Point.value[1] = In->Sample_Point.value[0];
                In->Sample_Point.value[0] = c;
                In->Sample_Chang_Flag=1;
        }
				/*******************根据需要修改PWM寄存器*********************************/
				// if(MtrNum == MTR0)
				// {
				// 		MCPWM0_PRT = 0xDEAD;
				// 		MCPWM0_TMR0 = In->Sample_Point.value[0];
				// 		MCPWM0_TMR1 = In->Sample_Point.value[1];
				// 		MCPWM0_PRT = 0x00;
				// }
				// else if(MtrNum == MTR1)
				// {
				// 		MCPWM1_PRT = 0xDEAD;
				// 		MCPWM1_TMR0 = In->Sample_Point.value[0];
				// 		MCPWM1_TMR1 = In->Sample_Point.value[1];
				// 		MCPWM1_PRT = 0x00;				
				// }
}
// void MTR_PWM_OUT_OFFSET(MATH_vec3 *out,MATH_vec3 *offset,uint8_t MtrNum)
// {	
// 	if(MtrNum == MTR0)
// 	{
// 			MCPWM0_TH00 =  -out->value[0] + offset->value[0]; 
// 			MCPWM0_TH01 =   out->value[0] + offset->value[0];
			
// 			MCPWM0_TH10 =  -out->value[1] + offset->value[1]; 
// 			MCPWM0_TH11 =   out->value[1] + offset->value[1];
			
// 			MCPWM0_TH20 =  -out->value[2] + offset->value[2]; 
// 			MCPWM0_TH21 =   out->value[2] + offset->value[2];	
// //			MCPWM0_TH00 =  -test_cnt; 
// //			MCPWM0_TH01 =   test_cnt;
// //			
// //			MCPWM0_TH10 =  -test_cnt1; 
// //			MCPWM0_TH11 =   test_cnt1;
// //			
// //			MCPWM0_TH20 =  -test_cnt2; 
// //			MCPWM0_TH21 =   test_cnt2;	
// 	}
// 	else 	if(MtrNum == MTR1)
// 	{
// 			MCPWM1_TH00 =  -out->value[0] + offset->value[0]; 
// 			MCPWM1_TH01 =   out->value[0] + offset->value[0];
			
// 			MCPWM1_TH10 =  -out->value[1] + offset->value[1]; 
// 			MCPWM1_TH11 =   out->value[1] + offset->value[1];
			
// 			MCPWM1_TH20 =  -out->value[2] + offset->value[2]; 
// 			MCPWM1_TH21 =   out->value[2] + offset->value[2];	
// 	}
	
// }


// void MCPWM0_Out(MTR_CTRL_typedef * ptr)
// {
//     MCPWM0_Set_Sample_Point_And_Out(&ptr->MTR_Cur1S,&ptr->PWM_OUT);
// }
// void MCPWM1_Out(MTR_CTRL_typedef * ptr)
// {
//     MCPWM1_Set_Sample_Point_And_Out(&ptr->MTR_Cur1S,&ptr->PWM_OUT);
// }
