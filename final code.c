#include<htc.h>
#include<pic.h>
#include<stdio.h>
#include<math.h>

#define _XTAL_FREQ 4000000

void serial_init(void);
void sendsms();
void sendt();
void sendv();
void sendc();
void sendvib();
void InitADC(void);
unsigned int GetADCvalue(unsigned char channel);
unsigned char array[5];
void buzz(void);
char str6[]="Transformer overheating above 50C";
char str7[]="Transformer voltage above 3KV";
char str8[]="Transformer vibrating";
char str9[]="over current flowing";
unsigned int i=0;

unsigned int GetADCvalue(unsigned char channel)
{
	ADCON0&=0xC7;
    ADCON0|=(channel);
    ADCON0|=(1<<ADON);
    GO_nDONE=1;
    while(GO_nDONE);
	return((ADRESH<<8)+ADRESL); 
}

void InitADC()
{                            
    TRISA0=1;
	TRISA1=1;
	ANSEL=0xF7;
	ADCON0=0xC1;		
	ADCON1=0x80;  
	ADIF=0;
}

void serial_init(void)
{
	RCSTA=0x80;  
	SPBRG=25;   
	TXSTA=0x06;   
	TXEN=1;
}

void sendsms(void)
{
	char str1[] = "AT";
	char str2[] = "AT+CMGF=1";
	char str4[] = "AT+CMGS=";
	char str5[] = "9725221231";
	while(str1[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str1[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 10;
	TXREG = 13;
	i=0;	
	__delay_ms(1000);
	while(str2[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str2[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 10;
	TXREG = 13;
	i=0;
	__delay_ms(1000);
	while(str4[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str4[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 34;
	i=0;
	while(str5[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str5[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 34;
	TXREG = 10;
	TXREG = 13;
	i=0;
	__delay_ms(2000);
}
void sendt(void)
{
	while(str6[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str6[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 10;
	TXREG = 13;
	i=0;
	__delay_ms(1000);
	TXREG = 10;
	TXREG = 13;
	TXREG = 0x1A;
}
void sendc(void)
{
	while(str9[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str9[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 10;
	TXREG = 13;
	i=0;
	__delay_ms(1000);
	TXREG = 10;
	TXREG = 13;
	TXREG = 0x1A;
}
void sendv(void)
{
	while(str7[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str7[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 10;
	TXREG = 13;
	i=0;
	__delay_ms(1000);
	TXREG = 10;
	TXREG = 13;
	TXREG = 0x1A;
}
void sendvib(void)
{
	while(str8[i]!='\0')
	{
		while(TRMT==0);  
		TXREG=str8[i];
		__delay_ms(10);
		i++;
	}
	TXREG = 10;
	TXREG = 13;
	i=0;
	__delay_ms(1000);
	TXREG = 10;
	TXREG = 13;
	TXREG = 0x1A;
}
void buzz(void)
{
	TRISD0 = 0;
	RD0 = 1;
	__delay_ms(2000);
	RD0 = 0;
}

void main()
{	
	  TRISD6 = 1;
	  TRISC6 = 0;
	  float a,b,c;
	  int temp,i;
	  unsigned int temp1;
	  InitADC();
	  serial_init();	
	  while(1)
	  {
			a = GetADCvalue(0x00);//an0
			a *=0.496;
			if(a>70)
			{
				sendsms();
				TRISA5 = 0;
				RA5 = 1;
				buzz();
				sendt();
				RA5 = 0;
			} 
			if(RD6 == 1)
			{
				sendsms();
				TRISE0 = 0;
				RE0 = 1;
				buzz();
				sendvib();
				RE0 = 0;
				RD6 = 0;
			}
			b = GetADCvalue(0xc5);//an1
			b *=4.96;
			if(b>3000)
			{
				sendsms();
				TRISE1 = 0;
				RE1 = 1; 
				sendv();
				buzz();
				RE1 = 0;
			}
			c = GetADCvalue(0xC9);//an2
			c *= 4.96;
			if(c>2650)
			{
				sendsms();
				TRISE2 = 0;
				RE2 = 1;
				sendc();
				buzz();
				RE2 = 0;
			}
		}
}
