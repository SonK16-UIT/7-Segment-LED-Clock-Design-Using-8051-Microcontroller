#include <reg51.h>
#include <stdio.h>
#include <stdlib.h>
//Set pin to control digit
sbit P2_0=P2^0;
sbit P2_1=P2^1;
sbit P2_2=P2^2;
sbit P2_3=P2^3;
sbit P2_4=P2^4;
sbit P2_5=P2^5;
//assign button
sbit P0_0=P0^0;
sbit P0_1=P0^1;
sbit P0_2=P0^2;
sbit P0_3=P0^3;
sbit P0_4=P0^4;
sbit P0_5=P0^5;
//R5:R4(hour) R3:R2(minute) R1:R0(second)
int R0=0;
int R1=0;
int R2=0;
int R3=0;
int R4=0;
int R5=0;
int count[] = {0, 0, 0};
unsigned char code digit[] = {
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
};

void delay(int time) {
unsigned int i,j;
for (i = 0; i < time;i++)
	{
	for (j = 0; j < 250; j++);
	}
}
void buzzer()
	{
		P0_5=1;
		delay(2);
		P0_5=0;
	}
void Check_bomb()
	{
		if(R5>=0 && R4>=0 && R3>=0 && R2 >= 3 && R1 >= 0 && R0 >= 0 )
			{
				if ( R3>=0 && R2 >= 3 )
				{
				while(1)
					{
						buzzer();
					}
				}
			}
	}
void display()
	{
		int R6;
		for(R6=0;R6<86;R6++)
		{
		P1=digit[R0]; P2_0 = 0; delay(1); P2=0xFF;
		P1=digit[R1]; P2_1 = 0; delay(1); P2=0xFF;
		P1=digit[R2]; P2_2 = 0; delay(1); P2=0xFF;
		P1=digit[R3]; P2_3 = 0; delay(1); P2=0xFF;
		P1=digit[R4]; P2_4 = 0; delay(1); P2=0xFF;
		P1=digit[R5]; P2_5 = 0; delay(1); P2=0xFF;
		}
	}
void Increase_hour()
{
	buzzer();
	if(R5!=2)
					{
						R4++;
						Check_bomb();
						if(R4==10)
						{
							display();
							R4=0;
							R5++;
							if(R5==3)
							{
								display();
							}
						}
					}
	 else{
				R4++;
		    Check_bomb();
				if(R4==4)
				{
					display();
					R4=0;
					R5=0;
					R5++;
					if(R5==3)
					{
						display();
					}
			 	 }
			  }
}
void Increase_min()
{
	Check_bomb();
	R2++;
	if(R2==10)
		{
			display();
			R2=0;
			R3++;
			if(R3==6)
			{
			  display();
				R3=0;
				Increase_hour();
			}
		}
}
void Increase_sec()
{
	Check_bomb();
	R0++;
	if(R0==10)
	{
		display();
		R0=0;
		R1++;
		if(R1==6) 
			{ 
			 display();
		   R1=0; 
			 Increase_min(); 
			}
	}
}
void Signal_Receive(int a)
	{
		if(a==1) {Increase_hour(); return; }
		if(a==2) {Increase_min(); return; }
		if(a==3) {count[0]=1; return; }
		if(a==4) {count[1]=1; return; }
		if(a==5) {count[2]=1; return; }
		return;
	}
unsigned int myRandom() {
    static unsigned long seed = 0;
    seed = (seed * 1103515245 + 12345) & 0xFFFFFFFF;
    return seed;
}
int main() {
	  int Random_sig[5];
	  int A; 
    int randomNumbers[5] = {1, 2, 3, 4, 5};
    int usedIndices[5] = {0, 0, 0, 0, 0}; 
    int index; 
    int i; 
    for (i = 0; i < 5; i++) {
        do {
            index = myRandom() % 5;
        } while (usedIndices[index]); 

        usedIndices[index] = 1;
        Random_sig[i] = randomNumbers[index]; 
    }
	P2=0xFF;
	P0=0x00;
	while(1)
	{
		if(count[0]==1 && count[1]==1 && count[2]==1)
			{
				while(1)
					{
						display();
					}
			}
		display();
		//signal_1
		if(P0_0==0)
			{
				delay(1);
				Signal_Receive(Random_sig[0]);
			}
		//signal_2
		if(P0_1==0) 
			{
				delay(1);
				Signal_Receive(Random_sig[1]);
			}
		//signal_3
		if(P0_2==0) 
			{
				delay(1);
				Signal_Receive(Random_sig[2]);
			}
		//signal_4
		if(P0_3==0) 
			{
				delay(1);
				Signal_Receive(Random_sig[3]);
			}
		//signal_5
		if(P0_4==0) 
			{
				delay(1);
				Signal_Receive(Random_sig[4]);
			}
		//run clock linear
		 Increase_sec();
	}
}