#include<stm32f10x.h>
int samp,num,volt,cel,frh;
void delay(unsigned int cnt)
{
	while(cnt--);
}
void enable()
{
	GPIOC->ODR|=1<<14;
	delay(50);
	GPIOC->BRR=1<<14;
	delay(50);
}
void lcd(char rs,char dta)
{
	GPIOC->ODR=rs<<15;
	GPIOA->ODR=(dta & 0xf0)>>4;
	enable();
	GPIOA->ODR=(dta & 0x0f);
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
void adc()
{
	ADC1->CR2|=1<<0;
	while(!(ADC1->SR & 1<<1));
	samp=ADC1->DR;
	ADC1->SR=0<<1;
	lcd(0,0x80);
	string("samp :");
	split(samp);
	volt=((float)samp*(3.3/4095.0)*10.0);
	lcd(0,0xc0);
	string("volt :");
	split(volt);
  cel=(float)samp*(150.0/4095.0);
	lcd(0,0x90);
	string("celci:");
	split(cel);	
	frh=(cel*(9.0/5.0)+32.0);
	lcd(0,0xd0);
	string("frh  :");
	split(frh);	
	
}
int main()
{
	RCC->CR=0X00000083;
	RCC->CFGR=0X00000000;
	RCC->APB2ENR=1<<0|1<<2|1<<4|1<<9;
	ADC1->CR1=0;
	GPIOA->CRL=0X00002222;
	GPIOC->CRH=0X22000000;
	ADC1->SR=1<<4;
	ADC1->CR2=1<<0|1<<1|1<<2|1<<17|1<<18|1<<19|1<<22;
	ADC1->SMPR2=0;
	ADC1->SQR1=0;
	ADC1->SQR3=0X00000004;
	lcd(0,0x02);
	lcd(0,0x28);
	lcd(0,0x0e);
	
	while(1)
	{
	 adc();
	}
}