#include<stm32f10x.h>
int samp,cel,frh,volt;
void delay(unsigned int cnt)
{
	while(cnt--);
}
void enable()
{
	GPIOC->ODR|=1<<14;
	delay(2000);
	GPIOC->BRR=1<<14;
	delay(2000);
}
void lcd(char rs,char dta)
{
	GPIOC->ODR=rs<<15;
	GPIOA->ODR=(dta & 0xf0)>>4;
	enable();
	GPIOA->ODR=(dta& 0x0f);
	enable();
}
void string(char *ptr)
{
	while(*ptr)
	{
		lcd(1,*ptr++);
	}
}
void split(int num)
{
	lcd(1,(num/1000)+48);
	lcd(1,(num%1000/100)+48);
	lcd(1,(num%1000%100/10)+48);
	lcd(1,(num%1000%100%10)+48);
}
void ADC1_2_IRQHandler()
{
	ADC1->CR2|=1<<0;
	while(!(ADC1->SR &1<<1));
	samp=ADC1->DR;
	ADC1->SR=0<<1;
	lcd(0,0x80);
	string("samp :");
	split(samp);
	cel=samp*(150.0/1230.0);
	lcd(0,0xc0);
	string("cel :");
	split(cel);
	frh=(cel*9.0/5.0)+32.0;
	lcd(0,0x90);
	string("frh :");
	split(frh);
	volt=(float)samp*(3.3/1230.0)*10.0;
	lcd(0,0xd0);
	string("volt :");
	split(volt);
}
int main()
{
	RCC->CR=0X00000083;
	RCC->CFGR=0;
	RCC->APB2ENR=1<<0|1<<2|1<<4|1<<9;
	GPIOA->CRL=0X00002222;
	GPIOC->CRH=0X22000000;
	ADC1->CR2=1<<0|1<<1|1<<2|1<<17|1<<18|1<<19|1<<22;
	ADC1->SR=1<<4;
	ADC1->CR1=1<<5;//adc1_2 interrupt enable
	ADC1->SMPR2=0;
	ADC1->SQR1=0;
	ADC1->SQR3=0X000000004;
	lcd(0,0x02);
	lcd(0,0x28);
	lcd(0,0x0e);
	ADC1->CR2 |= 1<<0;//pgno 243//This bit is set and cleared by software. If this bit holds a value of zero and a 1 is written to it 
                              //then it wakes up the ADC from Power Down state.
	NVIC_EnableIRQ(ADC1_2_IRQn);//interrupt 
	
	while(1)
	{
		
	}
}