/****************************************Copyright(c)********************************************
**
**                            2015-2023, Guangzhou TX-Aviation Technology Co.Ltd.
**
**----------------------------------------File Info----------------------------------------------
** File name : bsp_hx711.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : Luzhi
** Created date :2015年7月14日10:09:19
** Version :V1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/
#include "bsp.h"
Weight_TypeDef	Weight;
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 		
************************************************************************************************/
void Hx711_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA clock enable */
	RCC_AHBPeriphClockCmd(RCC_HX711_PORT, ENABLE);		/* 打开GPIO时钟 */
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽输出模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = HX711_SCK_PIN;
	GPIO_Init(HX711_SCK_PORT, &GPIO_InitStructure);

	//GPIO_InitStructure.GPIO_Pin = HX711_SCK2_PIN;
	//GPIO_Init(HX711_SCK2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_Pin = HX711_DOUT_PIN;
	GPIO_Init(HX711_DOUT_PORT, &GPIO_InitStructure);
	//#ifdef DOUBLE_HX711
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	//GPIO_InitStructure.GPIO_Pin = HX711_DOUT2_PIN;
	//GPIO_Init(HX711_DOUT2_PORT, &GPIO_InitStructure);
	//#endif

	
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void Delay_Hx711_Us(void)
{
	u16 i;
	for(i=0;i<180;i++) {
		;
	}
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
u32 HX711_Read(void)	//增益128
{
	u32 count; 
	#if 1
	u8 i; 
	Delay_Hx711_Us();
  	HX711_SCK_0(); 
  	count=0; 
  	while(HX711_DOUT_READ()); 
  	for(i=0;i<24;i++) { 
	  	HX711_SCK_1(); 
		
	  	count=count<<1; 
		Delay_Hx711_Us();
		HX711_SCK_0(); 
		Delay_Hx711_Us();
	  	if(HX711_DOUT_READ()) {
			count++; 
	  	}
	} 
 	HX711_SCK_1(); 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	Delay_Hx711_Us();
	HX711_SCK_0();  
	#endif
	return(count);
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
u32 HX711_2_Read(void)	//增益128
{
	u32 count; 
	#if 1
	u8 i; 
	Delay_Hx711_Us();
  	HX711_SCK2_0(); 
  	count=0; 
  	while(HX711_DOUT2_READ()); 
  	for(i=0;i<24;i++) { 
	  	HX711_SCK2_1(); 
	  	count=count<<1; 
		Delay_Hx711_Us();
		HX711_SCK2_0(); 
		Delay_Hx711_Us();
	  	if(HX711_DOUT2_READ()) {
			count++; 
	  	}
	} 
 	HX711_SCK2_1(); 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	Delay_Hx711_Us();
	HX711_SCK2_0();  
	#endif
	return(count);
}

/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void Get_Weight_Ref(void)
{
	Weight.DataRef = HX711_Read();
	Weight.DataRef = HX711_Read();
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void HX711_Val_Init(void)
{
	#if 1
	Weight.GapValue = GAP_VALUE;
	Weight.DataRef = HX711_Read();
	Weight.DataRef = HX711_Read();
	Weight.DataSample2 = Weight.DataRef2;
	Weight.LastWeight = Weight.CurWeight;
	#endif
}

/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void Set_Weight_Gap(u32 gap)
{
	if(gap<100||gap>800) {
		return;
	}
	Weight.GapValue = gap;
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void Get_Weight(void)
{
	Weight.CurWeight = 0;
	#if 1
	Weight.DataSample = HX711_Read();
	//Weight.DataSample2 = HX711_2_Read();
	//Weight.DataSample2 = 0;
	//printf("重量=%d\r\n",Weight.DataSample);
	if((Weight.DataSample > Weight.DataRef) &&(Weight.DataSample2 > Weight.DataRef2)){
		Weight.DataSample -= Weight.DataRef;		//获取净重
		Weight.DataSample2 -= Weight.DataRef2;		//获取净重
		Weight.CurWeight = (u16)((float)(Weight.DataSample+Weight.DataSample2)/Weight.GapValue); 	//计算实物的实际重量
	} else if((Weight.DataSample > Weight.DataRef) &&(Weight.DataSample2 <= Weight.DataRef2)){
		Weight.DataSample -= Weight.DataRef;		//获取净重
		Weight.CurWeight = (u16)((float)Weight.DataSample/Weight.GapValue); 	//计算实物的实际重量
	} else if((Weight.DataSample <= Weight.DataRef) &&(Weight.DataSample2 > Weight.DataRef2)) {
		Weight.DataSample2 -= Weight.DataRef2;		//获取净重
		Weight.CurWeight = (u16)((float)Weight.DataSample2/Weight.GapValue); 	//计算实物的实际重量
	} else {
		Weight.CurWeight = 0;
	}
	#endif
}

