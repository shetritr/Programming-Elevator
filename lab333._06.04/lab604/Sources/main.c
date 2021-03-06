/*

  * main implementation: use this 'C' sample to create your own application
 *
 */


//#include "derivative.h" /* include peripheral declarations */
# include "TFC.h"
#define MUDULO_REGISTER  0x2EE0
unsigned int LightIntensity = MUDULO_REGISTER/2;  // Global variable
void Motor_Dir_Speed (int Direction, int Speed);
int MinSpeed=0x10e0;//36% Duty
int Speed;//Global Speed
int SWpeed;//Global Speed
int CurrntLevel=0;//Global Level
int main(void){
	ClockSetupTPM();
	InitGPIO();
	InitPIT();
	MotorConfig();
	wait(); // sleep
	return 0;
}

void Motor_Dir_Speed (int Direction, int Speed){
	int i;
	//Direction
	GPIOC_PDOR = PORT_LOC(Direction+4);  // PTD7 is Input
	//speed
	TPM2_C1V = 0x2EE0;//boost
	for(i=100000 ; i>0 ; i--); //delay
	if(Speed==0){
		TPM2_C1V = 0;}
	else{
		speed=speed%10;
		SWpeed=MinSpeed +Speed*768;
		TPM2_C1V = MinSpeed +Speed*768;
	}
}
//-----------------------------------------------------------------
//  PORTD - ISR = Interrupt Service Routine
//-----------------------------------------------------------------
void PORTD_IRQHandler(void){
	volatile unsigned int i;
	while(!(GPIOD_PDIR & ((SW_POS1)|(SW_POS2))) );// wait of release the button
	// check that the interrupt was for switch
	if (PORTD_ISFR & SW_POS1) {
		Speed=GPIOB_PDIR;
	}
	if(PORTD_ISFR & SW_POS2){
		if(CurrntLevel<GPIOB_PDIR){
			Motor_Dir_Speed (1, Speed);
		}else{
			Motor_Dir_Speed (2, Speed);
		}
		
	}
	//Debounce or using PFE field
	for(i=10000 ; i>0 ; i--); //delay, button debounce
	
	PORTD_ISFR |= 0x00000080;  // clear interrupt flag bit of PTD7
}
//-----------------------------------------------------------------
// PIT - ISR = Interrupt Service Routine
//-----------------------------------------------------------------
void PIT_IRQHandler(){
	
	
}
