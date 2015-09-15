#ifndef _BSP_HX711_H
#define _BSP_HX711_H




#define RCC_HX711_PORT RCC_AHBPeriph_GPIOA 		/* GPIO端口时钟 */

#define HX711_SCK_PORT	GPIOA
#define HX711_SCK_PIN	GPIO_Pin_11

#define HX711_SCK_1()	HX711_SCK_PORT->BSRR = HX711_SCK_PIN
#define HX711_SCK_0()	HX711_SCK_PORT->BRR = HX711_SCK_PIN

#define HX711_DOUT_PORT	GPIOA
#define HX711_DOUT_PIN	GPIO_Pin_12
#define HX711_DOUT_READ()  ((HX711_DOUT_PORT->IDR & HX711_DOUT_PIN) != 0)	/* 读HX711_DOUT_PIN口线状态 */


#define HX711_SCK2_PORT	GPIOA
#define HX711_SCK2_PIN	GPIO_Pin_5

#define HX711_SCK2_1()	HX711_SCK2_PORT->BSRR = HX711_SCK2_PIN
#define HX711_SCK2_0()	HX711_SCK2_PORT->BRR = HX711_SCK2_PIN

#define HX711_DOUT2_PORT	GPIOA		
#define HX711_DOUT2_PIN		GPIO_Pin_6	
#define HX711_DOUT2_READ()  ((HX711_DOUT2_PORT->IDR & HX711_DOUT2_PIN) != 0)	/* 读HX711_DOUT_PIN口线状态 */



//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数


//传感器最大测量值，单位是g
#define AlarmValue 3000			

#define GAP_VALUE 219


typedef enum
{
	WEIGHT_STATUS_NORMAL,
	WEIGHT_STATUS_OVER_WEIGHT
}WeightStatus_Enum;	

typedef struct {
  WeightStatus_Enum	Status;
  u16 	WeightFullScale;	//设定的最大量程值
  u16	LastWeight;
  u16	CurWeight;
  u16	CurWeightMax;
  u32   GapValue;
  u32 	DataRef;		//基准重量，皮重
  u32 	DataSample;		//当前测量重量
  u32 	DataRef2;		//基准重量，皮重
  u32 	DataSample2;		//当前测量重量
}Weight_TypeDef;

extern Weight_TypeDef	Weight;
void Hx711_GPIO_Config(void);
u32 HX711_Read(void);
u32 HX711_2_Read(void);
void Get_Weight(void);
void Get_Weight_Ref(void);
void HX711_Val_Init(void);
void Set_Weight_Gap(u32 gap);


#endif
