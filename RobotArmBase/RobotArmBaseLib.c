/* ****************************************************************************
 *                           ____________________________
 *                           \| ROBOT ARM SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/         
 * ----------------------------------------------------------------------------
 * ------------------- [c]2010 - 2013 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * ****************************************************************************
 * File: RobotArmBaseLib.c
 * Version: 2.0
 * Target: Robotarm v3 - ATMEGA64 @16.000MHz
 * Author(s): Huy Nguyen 
 			  Hein Wielink
 * ****************************************************************************
 * Description:
 *
 * This is Robot Arm v3 version of the Library.
 *  
 * This is the Robotarm Library - it contains the following functions:
 * - Processor initialisation
 * - LED Control
 * - Beeper control
 * - A/D Convertor (Battery Voltage sensors, current measurement )
 * - Servo Control 
 * - Timing functions 
 *
 * PLEASE ALSO READ THE ROBOT ARM MANUAL! THERE YOU WILL FIND EXAMPLE
 * CODE AND SOME FURTHER EXPLANATIONS!
 *
 * In other parts of this library RobotArmUart.c, 
 * you can find UART communication.
 *
 * -----
 * Hint: You should better leave all this as it is if you just started with
 * C programming, but it is a very good idea to read the comments and review 
 * the code, it will help you to understand C programming for AVR better!
 * -----
 *
 * For the experienced users: 
 * This code works OK, but it is not optimal! There is a lot potential for 
 * tuning! 
 * Well, this leaves some tasks for you and this is what makes most 
 * fun: To improve the excisting!  
 *
 * Of course you are free to add new functions and improvements to this
 * library and make them available to the public on the Internet e.g. on
 * our Forum!
 * Please use the changelog at the end of this file to document your
 * changes! And add your name to any new function or modification you added! 
 * E.g. a "modified by <name> at <date>" is always a good idea to show 
 * other users where and WHAT you changed in the source code!
 *
 * It is a good idea to make your own includeable libraries instead of
 * changing this library - of course only if this is possible.
 *
 * Or create your own complete library with all specific functions you need.
 * This code is GPL'd - s. license at the end of this file!
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RobotArmBaseLib.h"

int BeepSound;
volatile uint16_t delay_timer;
volatile uint8_t ms_timer;
volatile stopwatches_t stopwatches;
volatile uint8_t feeler_timer;

// Can be used to detect which board revision is used
uint8_t robot_arm_v3 = 0; 

/*****************************************************************************/
//Timer Interrupts:

//Timer2 (100uS Timer) 
ISR(TIMER2_COMP_vect)
{
	delay_timer++;

	if(ms_timer++ >= 10) { // 10 * 100탎 = 1ms
  	// 16bit Stopwatches:
  	if(stopwatches.watches & STOPWATCH1)
  		stopwatches.watch1++;
  	if(stopwatches.watches & STOPWATCH2)
  		stopwatches.watch2++;
  	if(stopwatches.watches & STOPWATCH3)
  		stopwatches.watch3++;
  	if(stopwatches.watches & STOPWATCH4)
  		stopwatches.watch4++;
  	if(stopwatches.watches & STOPWATCH5)
  		stopwatches.watch5++;
  	if(stopwatches.watches & STOPWATCH6)
  		stopwatches.watch6++;
  	if(stopwatches.watches & STOPWATCH7)
  		stopwatches.watch7++;
  	if(stopwatches.watches & STOPWATCH8)
  		stopwatches.watch8++;
  	ms_timer=0;
	}
}

/*****************************************************************************/
/* Delay with the help of timer2.
 * mSleep(1) delays for *about* 1ms! Not exaclty, as we do not use assembly routines
 * anywhere in this library!
 * sleep has 100탎 resolution but can only delay for max. 255 * 100탎. 
 *
 * This is a blocking routine, which means that the processor
 * will loop in this routine and (except for interrupts) the
 * normal program flow is stopped!
 * Thus you should use the Stopwatch functions wherever you can!
 *
 * Example:
 *		mSleep(1); // delay 1 * 1ms = 1ms 
 *		mSleep(10); // delay 10 * 1ms = 10ms 
 *		mSleep(100); // delay 100 * 1ms = 100ms 
 *		// The maximum delay is:
 *		mSleep(65535); // delay 65535 * 1ms = 65535ms = 1 min. 5s   
 */  

void sleep(uint8_t time)
{
	for (delay_timer = 0; delay_timer < time;);
}


void mSleep(uint16_t time)
{
	while (time--) sleep(10);
}


/*****************************************************************************/
// 4 blue Status LEDs (SL1 - SL4) for Robot Arm v3:

/** 
 * Set the 4 Status LEDs that are connected to the Microcontroller
 * 
 * Example:
 *  setLEDs(0b1111); // All LEDs on
 *  setLEDs(0b0011); // LED1 + 2 on, LED3 + 4 off
 */
void setLEDs(uint8_t leds)
{
	PORTG &= ~0xF; 
	PORTG |= (leds & 0xf);	
}	

/** 
 * Set ONLY LED1, don't change anything for the other LEDs.
 */
void setLED1(uint8_t led)
{
	PORTG &= ~SLED1; 
	PORTG |= (led > 0) ? SLED1 : 0;	
}

/** 
 * Set ONLY LED2, don't change anything for the other LEDs.
 */
void setLED2(uint8_t led)
{
	PORTG &= ~SLED2; 
	PORTG |= (led > 0) ? SLED2 : 0;	
}	

/** 
 * Set ONLY LED3, don't change anything for the other LEDs.
 */
void setLED3(uint8_t led)
{
	PORTG &= ~SLED3; 
	PORTG |= (led > 0) ? SLED3 : 0;	
}	

/** 
 * Set ONLY LED4, don't change anything for the other LEDs.
 */
void setLED4(uint8_t led)
{
	PORTG &= ~SLED4; 
	PORTG |= (led > 0) ? SLED4 : 0;	
}	


/*****************************************************************************/
// Duo LED red+green for old Robot Arm PCBs:

// Set Power LED on red
/*
 * Example:
 *
 *			PowerLEDred();
 *			// Power led = RED
 */

void PowerLEDred(void){
PORTG |= LED_RED;
PORTG &= ~LED_GREEN;	
}

// Set Power LED on green
void PowerLEDgreen(void){	
PORTG &= ~LED_RED;
PORTG |= LED_GREEN;		
}

// Set Power LED on orange
void PowerLEDorange(void){	
PORTG |= LED_RED;
PORTG |= LED_GREEN;	
}

// Set Power LED off
void PowerLEDoff(void){
PORTG &= ~LED_RED;
PORTG &= ~LED_GREEN;	
}



/*****************************************************************************/
// Set Beep sound on
void setBeepsound(void)
{
  if(!robot_arm_v3) {
	//Clear OC0 on compare match, set OC0 at BOTTOM,
	TCCR0 =   (1 << WGM00) | (1 << WGM01) 
			| (0 << COM00) | (1 << COM01)   
			| (0 << CS02)  | (1 << CS01) | (0 << CS00);
	}
	else
	{
		//Clear OC0 on compare match, set OC0 at BOTTOM,
  	TCCR0 =   (0 << WGM00) | (1 << WGM01) 
			| (1 << COM00) | (0 << COM01)   
			| (1 << CS02)  | (0 << CS01) | (1 << CS00);
	}
}

/*****************************************************************************/
// Set Beep sound off
void clearBeepsound(void)
{	
	//Normal port operation, OC0 disconnected.
	TCCR0 =   (1 << WGM00) | (1 << WGM01) 
			| (0 << COM00) | (0 << COM01)   
			| (0 << CS02)  | (1 << CS01) | (0 << CS00);
}


/*****************************************************************************/
// Change Beep Sound
/* Example:

 *		While(true)
 *		{
 *			setBeepsound();
 *			changeBeepsound(150);
 *			mSleep(1000);
 *			changeBeepsound(200);
 *			mSleep(1000);
 *			changeBeepsound(255);
 *			mSleep(1000);
 *		}
 */		
	
void changeBeepsound(int PWMvalue)
{
	OCR0=PWMvalue;
}


/*****************************************************************************/
/************************** Extern Keyboard **********************************/
/*****************************************************************************/

int scan_keyboard(void){

	int i,mask,x;
	if(robot_arm_v3) // connector pinout is different for v3
	{
		DDRA = 0x0F;
		PORTA = 0x01;
		sleep(15);
		x = PINA;
		mask = 0x80;
		for(i=0;i<4;i++){
			if((x & mask) == mask)
				return i + 13;
		  mask >>= 1;
		}
		
		PORTA = 0x02;
		sleep(15);
		x = PINA;
		mask = 0x80;
		for(i=0;i<4;i++){
			if((x & mask) == mask)
		        return i + 9;
		  mask >>= 1;
		}

		PORTA = 0x04;
		sleep(15);
		x = PINA;
		mask = 0x80;
		for(i=0;i<4;i++){
			if((x & mask) == mask)
		        return i + 5;
		  mask >>= 1;
		}

		PORTA = 0x08;
		sleep(15);
		x = PINA;
		mask = 0x80;
		for(i=0;i<4;i++){
			if((x & mask) == mask)
		        return i + 1;
		  mask >>= 1;
		}
	}
	else
	{
	  DDRA = 0xF0;
    PORTA = 0x10;
  	sleep(15);	
  	mask = 1;
  	x = PINA;
  
  	for(i=0;i<4;i++){
  		if((x & mask) == mask)
              return i + 1;
          mask <<= 1;
  	}
  	
  	PORTA = 0x20;
  	sleep(15);	
  	mask = 1;
  	x = PINA;
  
  	for(i=0;i<4;i++){
  		if((x & mask) == mask)
              return i + 5;
          mask <<= 1;
  	}
  	
  	PORTA = 0x40;
  	sleep(15);	
  	mask = 1;
  	x = PINA;
  
  	for(i=0;i<4;i++){
  		if((x & mask) == mask)
              return i + 9;
          mask <<= 1;
  	}
  	
  	PORTA = 0x80;
  	sleep(15);	
  	mask = 1;
  	x = PINA;
  
  	for(i=0;i<4;i++){
  		if((x & mask) == mask)
              return i + 13;
          mask <<= 1;
  	}
  }
	
	return 0;
}


/*****************************************************************************/
/************************** Servo Control   **********************************/
/*****************************************************************************/


//Measuring Current 
/**
 * Read ADC channel (10 bit -> result is an integer from 0 to 1023).
 *
 * This is a blocking function, which means it waits until the conversion
 * is complete. There is a more complicated alternative that frequently 
 * checks all channels (s. below).
 *
 * This function returns 0 if the ADC is buisy! This has been done to
 * prevents problems when the automatical function is used.
 *
 */
uint16_t readADC (int channel)
{
	if (ADCSRA & (1<<ADSC)) return 0;  // check if ADC is buisy...
	
	ADMUX = (1<<REFS0) | (0<<REFS1) | (channel<<MUX0);// AVCC
	//	ADMUX = (1<<REFS0) | (1<<REFS1) | (channel<<MUX0); // INT
	ADCSRA = (0<<ADIE) | (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
	
	//ADMUX&=0xE0;	// Reset Channel  
//	ADMUX|=channel;	// Set Channel 
	mSleep(2);			//waits to become stable after changing the channel
	
	ADCSRA |= (1<<ADSC); // start Analog to Digital Conversion 
	while (ADCSRA & (1<<ADSC)); 

  ///while((ADCSRA & 0x10)==0);	// Wait for the AD conversion to complete
	ADCSRA = 0; //set the bit to clear ADIF flag 
	
	return ADC;
}


void init_adc(void)
{
	// Initialize ADC:
	ADMUX = 0; //external reference 
	ADCSRA = (0<<ADIE) | (0<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0) | (1<<ADIF);
	SFIOR = 0;
}



/**
 * This function starts an ADC conversion - it does not return the
 * read value! You need to poll if the conversion is complete somewhere
 * else and then read it from the ADC result register.
 * (s. task_ADC function below)
 */
void startADC(uint8_t channel)
{
	ADMUX = (1<<REFS0) | (0<<REFS1) | (channel<<MUX0);
	ADCSRA = (0<<ADIE) | (1<<ADSC) | (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0) | (1<<ADIF);
}

// -----------------------

uint16_t Current_1; 
uint16_t Current_2; 
uint16_t Current_3; 
uint16_t Current_4; 
uint16_t Current_5; 
uint16_t Current_6; 

uint16_t adcUBat;
uint16_t adcExt;

uint8_t current_adc_channel = 255; // 255 = inital run

/**
 * This functions checks all ADC channels sequentially in the background!
 * It can save a lot of time, if the ADC channels are checked like this, because
 * each A/D conversion takes some time. With this function you don't need to
 * wait until the A/D conversion is finished and you can do other things in the 
 * meanwhile.
 * While you use this function, you can NOT use readADC anymore!
 * (but you may use one function and after this is finished use the other) 
 *
 * Instead or readADC you can use the 8 global variables you see above to
 * get the current ADC values!
 * 
 * You MUST call this function frequently to make this work!  
 * See sampleADCs_and_sleep function! 
 */
void task_ADC(void)
{
	if(!(ADCSRA & (1<<ADSC))) {
	//	ADCSRA |= (1<<ADIF);
		switch(current_adc_channel) {
		 	case 255: startADC(ADC_CURRENT_1); break; // initial
			case 0: Current_1 = ADC; startADC(ADC_CURRENT_2); break;
			case 1: Current_2 = ADC; startADC(ADC_CURRENT_3); break;
			case 2: Current_3 = ADC; startADC(ADC_CURRENT_4); break;
			case 3: Current_4 = ADC; startADC(ADC_CURRENT_5); break;
			case 4: Current_5 = ADC; startADC(ADC_CURRENT_6); break;
			case 5: Current_6 = ADC; startADC(ADC_UBAT); break;
			case 6: adcUBat = ADC; startADC(ADC_EXT_ADC); break;
			case 7: adcExt = ADC; startADC(ADC_CURRENT_1); break;
		}
		if(current_adc_channel >= 7)
			current_adc_channel = 0;
		else
			current_adc_channel++;
	}
}

/**
 * Same as task_ADC, but with simple averaging filter for current and last sample. 
 */ 
void task_ADC_average(void)
{
	if(!(ADCSRA & (1<<ADSC))) {
	//	ADCSRA |= (1<<ADIF);
		switch(current_adc_channel) {
		  case 255: startADC(ADC_CURRENT_1); break; // initial
			case 0: Current_1 = (Current_1 + ADC)/2; startADC(ADC_CURRENT_2); break;
			case 1: Current_2 = (Current_2 + ADC)/2; startADC(ADC_CURRENT_3); break;
			case 2: Current_3 = (Current_3 + ADC)/2; startADC(ADC_CURRENT_4); break;
			case 3: Current_4 = (Current_4 + ADC)/2; startADC(ADC_CURRENT_5); break;
			case 4: Current_5 = (Current_5 + ADC)/2; startADC(ADC_CURRENT_6); break;
			case 5: Current_6 = (Current_6 + ADC)/2; startADC(ADC_UBAT); break;
			case 6: adcUBat = (adcUBat + ADC)/2; startADC(ADC_EXT_ADC); break;
			case 7: adcExt = (adcExt + ADC)/2; startADC(ADC_CURRENT_1 ); break;
		}
		if(current_adc_channel >= 7)
			current_adc_channel = 0;
		else
			current_adc_channel++;
	}
}

/**
 * Only one specific channel is updated frequently in this function. 
 * Otherwise it behaves similar to task_ADC.  
 */ 
void task_ADC_channel(uint8_t channel)
{
	if(!(ADCSRA & (1<<ADSC))) {
	//	ADCSRA |= (1<<ADIF);
		switch(current_adc_channel) {
		 	case 255: startADC(channel); break; // initial
			case 0: Current_1 = ADC; startADC(ADC_CURRENT_1); break;
			case 1: Current_2 = ADC; startADC(ADC_CURRENT_2); break;
			case 2: Current_3 = ADC; startADC(ADC_CURRENT_3); break;
			case 3: Current_4 = ADC; startADC(ADC_CURRENT_4); break;
			case 4: Current_5 = ADC; startADC(ADC_CURRENT_5); break;
			case 5: Current_6 = ADC; startADC(ADC_CURRENT_6); break;
			case 6: adcUBat = ADC; startADC(ADC_UBAT); break;
			case 7: adcExt = ADC; startADC(ADC_EXT_ADC); break;
		}
		current_adc_channel = channel;
	}
}


/**
 * This functions updates all ADC values while "sleeping" a speficied time.
 *  
 */ 
void sampleADCs_and_sleep(uint16_t ms)
{
    setStopwatch8(0);
    startStopwatch8();
    while(getStopwatch8() < ms)
    {
    	task_ADC();
    }
    stopStopwatch8();
}

/**
 * Same as sampleADCs_and_sleep but with task_ADC_average.
 *  
 */ 
void sampleADCs_average_and_sleep(uint16_t ms)
{
    setStopwatch8(0);
    startStopwatch8();
    while(getStopwatch8() < ms)
    {
    	task_ADC_average();
    }
    stopStopwatch8();
}


// Calculate current in mA for Robot Arm v3  (does NOT work like that for older models)
uint16_t calc_current(uint16_t adc_value_ext_ref)
{
	return (uint16_t)(((uint32_t)adc_value_ext_ref * 50 * 1000) / (1024 * 16));
}


/*****************************************************************************/
// Internal EEPROM:

// Write to EEPROM adress. 
// Warning: Bytes 0 + 1 are reserved for the Bootloader (autostart)
void writeINTEE(uint8_t adr, uint8_t data)
{
	eeprom_busy_wait();
	EEAR = adr;
	EEDR = data;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}

// Read from internal EEPROM
uint8_t readINTEE(uint8_t adr)
{
	eeprom_busy_wait();	
	EEAR = adr;
	EECR |= (1<<EERE);
	return EEDR;
	return 0;
}


// Declare storage areas for EEPROM
//	VALUE EEMEM ee_DEFAULT;
//uint16_t EEMEM ee_Start_Position[7];


// Write default values from servos, to the EEPROM.
// Warning: Bytes 0 + 1 are reserved for the Bootloader (autostart)
void write_Values_EE (void){

// Write Values to specific locations:
  writeINTEE(2, Start_Position[1] & 0xFF);
  writeINTEE(3, (Start_Position[1] >> 8) & 0xFF);
  writeINTEE(4, Start_Position[2] & 0xFF);
  writeINTEE(5, (Start_Position[2] >> 8) & 0xFF);
  writeINTEE(6, Start_Position[3] & 0xFF);
  writeINTEE(7, (Start_Position[3] >> 8) & 0xFF);
  writeINTEE(8, Start_Position[4] & 0xFF);
  writeINTEE(9, (Start_Position[4] >> 8) & 0xFF);
  writeINTEE(10, Start_Position[5] & 0xFF);
  writeINTEE(11, (Start_Position[5] >> 8) & 0xFF);
  writeINTEE(12, Start_Position[6] & 0xFF);
  writeINTEE(13, (Start_Position[6] >> 8) & 0xFF);
  

	// To write EEPROM from SRAM
//	eeprom_write_block((const void *)&Start_Position,(void *)&ee_Start_Position, sizeof(Start_Position));
}

// Read default values from servos, from the EEPROM 
void Read_Values_EE (void){

	// To read EEPROM back to SRAM
//	eeprom_read_block((void *)&Start_Position, (const void*)&ee_Start_Position, sizeof(Start_Position));

  Start_Position[1] = readINTEE(2) | (readINTEE(3) << 8);
  Start_Position[2] = readINTEE(4) | (readINTEE(5) << 8);
  Start_Position[3] = readINTEE(6) | (readINTEE(7) << 8);
	Start_Position[4] = readINTEE(8) | (readINTEE(9) << 8);
  Start_Position[5] = readINTEE(10) | (readINTEE(11) << 8);
  Start_Position[6] = readINTEE(12) | (readINTEE(13) << 8);
	
	if(Start_Position[1]  == -1)  Start_Position[1] = 1500;  
	if(Start_Position[2]  == -1)  Start_Position[2] = 1500;  
	if(Start_Position[3]  == -1)  Start_Position[3] = 1500;  
	if(Start_Position[4]  == -1)  Start_Position[4] = 1500;  
	if(Start_Position[5]  == -1)  Start_Position[5] = 1500;  
	if(Start_Position[6]  == -1)  Start_Position[6] = 1500;  

	Start_position(); 	
}


/*****************************************************************************/
// Power off servo's 
void Power_Servos (void)
{
  if(robot_arm_v3) 
  {
    PORTG |= SERVO_POWER_v3;
  }
  else
  {
    PORTG &=~SERVO_POWER;
  }
}

/*****************************************************************************/
// Power off servo's 

void Power_Off_Servos (void)
{
  if(robot_arm_v3) 
  {
    PORTG &=~ SERVO_POWER_v3;
    
  }
  else
  {
    PORTG |= SERVO_POWER;
  }
}


/*****************************************************************************/
// Set servo motors in normal position
void Start_position(void)
{
	Pos_Servo_1=Start_Position [1];
	mSleep(100);
	Pos_Servo_2=Start_Position [2];
	mSleep(100);
	Pos_Servo_3=Start_Position [3];
	mSleep(50);
	Pos_Servo_4=Start_Position [4];
	mSleep(50);
	Pos_Servo_5=Start_Position [5];
	mSleep(50);
	Pos_Servo_6=Start_Position [6];
	mSleep(50);
}

/*****************************************************************************/
// Default (uncalibrated) start position
void Default_Start_position(void)
{
	Pos_Servo_1=1000;
	mSleep(100);
	Pos_Servo_2=1000;
	mSleep(100);
	Pos_Servo_3=1000;
	mSleep(50);
	Pos_Servo_4=1000;
	mSleep(50);
	Pos_Servo_5=1000;
	mSleep(50);
	Pos_Servo_6=1000;
	mSleep(50);
}

/*****************************************************************************/
// Set all Servo PWM values to 0
void Servo_PWM_Zero(void)
{
	Pos_Servo_1=0;
	Pos_Servo_2=0;
	Pos_Servo_3=0;
	Pos_Servo_4=0;
	Pos_Servo_5=0;
	Pos_Servo_6=0;
}

/*****************************************************************************/
// This Routine performs Startup sequence of the servos with small 
// delay in between each servo power on event. This reduces the peak
// current during startup.
void Servo_Power_And_Start(void)
{
	Servo_PWM_Zero();
	Power_Servos();
	mSleep(250); 
	Pos_Servo_1=Start_Position[1];
	mSleep(100);
	Pos_Servo_2=Start_Position[2];
	mSleep(100);
	Pos_Servo_3=Start_Position[3];
	mSleep(50);
	Pos_Servo_4=Start_Position[4];
	mSleep(50);
	Pos_Servo_5=Start_Position[5];
	mSleep(50);
	Pos_Servo_6=Start_Position[6];
	mSleep(50);
}

// Same as Servo_Power_And_Start() but does not use the values from the EEPROM 
// but default values. 
void Servo_Power_And_Start_Default(void)
{
	Servo_PWM_Zero();
	Power_Servos();
	mSleep(250); 
	Pos_Servo_1=1600;
	mSleep(100);
	Pos_Servo_2=1500;
	mSleep(100);
	Pos_Servo_3=1500;
	mSleep(50);
	Pos_Servo_4=1500;
	mSleep(50);
	Pos_Servo_5=1500;
	mSleep(50);
	Pos_Servo_6=1500;
	mSleep(50);
}        

/*****************************************************************************/
// Move Servo's 

void Move (uint8_t Servo, uint16_t Value) 
{
	switch (Servo) 
	{
  	case 1:	Pos_Servo_1 = Start_Position[1] + Value; break; 
  	case 2:	Pos_Servo_2 = Start_Position[2] + Value; break; 
  	case 3:	Pos_Servo_3 = Start_Position[3] + Value; break; 
  	case 4:	Pos_Servo_4 = Start_Position[4] + Value; break; 
  	case 5:	Pos_Servo_5 = Start_Position[5] + Value; break; 
  	case 6:	Pos_Servo_6 = Start_Position[6] + Value; break; 
	}
}


/*****************************************************************************/
/* Move 
* Example:
*
* 	s_Move(6, 500,2);
*		1 - Servo 6
*		2 - (Startpostion + 500) = 2ms (right)
*		3 - (speed = 2) 
*/
void s_Move (uint8_t Servo, int16_t D_Value, uint16_t Speed) 
{
	int16_t Actual_position=2700;  
	uint8_t move_servo1_back = 0;
	switch (Servo) 
	{
	case 1:	Actual_position = Pos_Servo_1 - Start_Position[1]; break; 
	case 2:	Actual_position = Pos_Servo_2 - Start_Position[2]; break; 
	case 3:	Actual_position = Pos_Servo_3 - Start_Position[3]; break; 
	case 4:	Actual_position = Pos_Servo_4 - Start_Position[4]; break; 
	case 5:	Actual_position = Pos_Servo_5 - Start_Position[5]; break; 
	case 6:	Actual_position = Pos_Servo_6 - Start_Position[6]; break; 
	}
	move_servo1_back = 0;
	if (Actual_position > D_Value )
	{ 
    if(Servo == 1 && (Actual_position - D_Value) >= 40)   // Gripper Servo? 
    {                                              // Difference > 40 ?        
        move_servo1_back = 1;
    }
		while (Actual_position > D_Value)
		{
			Actual_position--; 
			Move(Servo, Actual_position);
			
    	//	mSleep(Speed);
    		// Instead of mSleep, perform ADC readings while waiting:
    		setStopwatch8(0);
    		startStopwatch8();
    		while(getStopwatch8() < Speed)
    		{
    			task_ADC();
    		}
    		stopStopwatch8();
		}
    if(move_servo1_back)     // Gripper Servo? 
    {                  // Move it backwards a little to reduce current consumption 
                       // with closed gripper     
                       
        while (Actual_position < (D_Value+50))
    		{
    			Actual_position++; 
          if(Actual_position < 500 && Actual_position > -500)
            Move(Servo, Actual_position);
                     /*
    			writeString_P("SERVO1-POS:"); 
          writeInteger(Actual_position, DEC);
          writeString_P("\n");    */
        	//	mSleep(Speed);
        		// Instead of mSleep, perform ADC readings while waiting:
        		setStopwatch8(0);
        		startStopwatch8();
        		while(getStopwatch8() < Speed+2)
        		{
        			task_ADC();
        		}
        		stopStopwatch8();
    		}
        	
    }
		return; 
	}
  
  move_servo1_back = 0;
	if (Actual_position < D_Value )
	{ 
    if(Servo == 1 && (D_Value - Actual_position) >= 30)   // Gripper Servo? 
    {                                              // Difference > 30 ?      
        move_servo1_back = 1;
    }
		while (Actual_position < D_Value)
		{
			Actual_position++; 
			Move(Servo, Actual_position);
			
			//	mSleep(Speed);
    		// Instead of mSleep, perform ADC readings while waiting:
    		setStopwatch8(0);
    		startStopwatch8();
    		while(getStopwatch8() < Speed)
    		{
    			task_ADC();
    		}
    		stopStopwatch8();
		}
    if(move_servo1_back)     // Gripper Servo? 
    {                  // Move it backwards a little to reduce current consumption 
        while (Actual_position > (D_Value-30))
    		{
    			Actual_position--; 
          if(Actual_position < 500 && Actual_position > -500)
            Move(Servo, Actual_position);
          
        	//	mSleep(Speed);
        		// Instead of mSleep, perform ADC readings while waiting:
        		setStopwatch8(0);
        		startStopwatch8();
        		while(getStopwatch8() < Speed)
        		{
        			task_ADC();
        		}
        		stopStopwatch8();
    		}
    }
	}	
}


/*****************************************************************************/
// This can be used to check for revision of the Robotarm Board.
// There are some pinout changes, this is already taken care of in the lib.
// Result is written to "robot_arm_v3" variable, it is true if it is the
// new revision. 
void check_board(void)
{
  DDRC &= ~BOARD_ID;
  PORTC |= BOARD_ID;
  nop();
  if(PINC & BOARD_ID)
  {
    robot_arm_v3 = false;
  }
  else
  {
    robot_arm_v3 = true;
  }
}



/*****************************************************************************/
// Total init for Robotarm 
/*
 * Initialise the Robot Controller - ALWAYS CALL THIS FIRST!
 * The Processor will not work correctly otherwise.
 * (If you don't implement your own init routine!)
 *
 * Example:
 *
 *			int16_t main(void)
 *			{
 *				initRobotBase();
 *
 *				// ... your application code
 *
 *				while(true);
 *				return 0;
 *			}
 *
 */
void initRobotBase(void)
{
	cli();			// Disable global interrupts
	
	check_board();

	/****************************************************************************/
	// Setup port directions and initial values.
	// THIS IS THE MOST IMPORTANT STEP!
	if(robot_arm_v3)
	{
    portInit_v3();
	}
	else
	{
	   portInit();
	   writeString_P("\n\n#############################################################\n");
	   writeString_P("ERROR: This program is intended for the new Robot ARM v3 PCB!\n");
		 writeString_P("For older revisions you need the old example programs!\n");
		 writeString_P("#############################################################\n\n");
		 while(true)
     {
       delay_ms(200);
       PowerLEDred();
       delay_ms(100);
       PowerLEDoff();
     }
     	// Note: 
      // The software is in principle compatible with the older PCBs. 
     	// Current sensing is different, crystal oscillator is slightly different, 
      // the pinout is also a bit different. 
     	// There are also more LEDs on the v3 PCB.
     	// But you may change this library to operate with the older PCBs as well!
	}

	
	Power_Off_Servos();
	/*****************************************************************************/
	// setADC
	// AVCC with external capacitor at AREF pin
	ADMUX = (1<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
	// ADC Enable, ADC Start Conversion, Auto Trigger Enable, Division Factor 8
	
	
	//ADCSRA = (0<<ADEN)|(0<<ADSC)|(1<<ADATE)|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
	ADCSRA = (0<<ADEN)|(0<<ADSC)|(1<<ADATE)|(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0); 
	// Free Running mode
	ADCSRB  = (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
	//ADCSRA |= (1<<ADIF);
	

	/*****************************************************************************/
	// Initialize Timer 0 -  PWM Beeper
	TCCR0 =   (1 << WGM00) | (1 << WGM01) 
			| (0 << COM00) | (0 << COM01)   
			| (0 << CS02)  | (1 << CS01) | (0 << CS00);
			
	//TIMSK = (0 << OCIE0)|(0<<TOIE0) ;
	OCR0  = 255;

	/*****************************************************************************/
	// Initialize Timer 2 -  100탎 cycle for Delays and Stopwatches:
	TCCR2 =   (0 << WGM20) | (1 << WGM21) 	| (0 << COM20) | (0 << COM21) 
			   | (0 << CS22)  | (1 << CS21) | (0 << CS20);	   
	OCR2  = 204;
	

	/*****************************************************************************/
	// **** Timer/Counter 1 initialization (timer for servo's) ****
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off

	TCCR1A=0xA8;

	// Clock source: System Clock
	// Clock value: 20000 kHz
	//
	//    16.000.000 / 2*8*20000 = 50 Hz. 
	//	  (Datasheet page 130) 
	//
	//
	// Mode: Ph. & fr. cor. PWM top=ICR1
	TCCR1B=0x12;
	ICR1 = 20000;

	TCNT1H=0x00;
	TCNT1L=0x00;

	// OC1A output: Non-Inv.
	// OC1B output: Non-Inv.
	// OC1C output: Non-Inv.

	/*****************************************************************************/
	// *** Timer/Counter 3 initialization (timer for servo's)***

	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR3A=0xA8;

	// Clock source: System Clock
	// Clock value: 20000 kHz
	// Mode: Ph. & fr. cor. PWM top=ICR3
	
	TCCR3B=0x12;
	ICR3 = 20000;
	TCNT3H=0x00;
	TCNT3L=0x00;

	// OC3A output: Non-Inv.
	// OC3B output: Non-Inv.
	// OC3C output: Non-Inv.

	/*****************************************************************************/
	// UART:
	UBRR1H = UBRR_BAUD_LOW >> 8;	// Setup UART: Baudrate is Low Speed
	UBRR1L = (uint8_t) UBRR_BAUD_LOW;
	UCSR1A = 0x00;
  UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
  UCSR1B = (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1);
	

	
	
	/*****************************************************************************/
	//Enable Compare output Timer 2 
	TIMSK |= (1 << OCIE2);

	sei();
	
  
  	
	Read_Values_EE ();		//Read default values for the servos from the EEprom 
							//And store in SRAM	

  
	PowerLEDred();		//Power Led red
	mSleep(1000);		//wait for 1 sec. 
	PowerLEDgreen();	//Power Led green (OK) 
}


/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * - v. 1.0 (initial release) 27.05.2010 by Huy Nguyen 
 *											Hein Wielink
 * - v. 2.0  30.04.2013 by AREXX
 *
* ****************************************************************************
 * Bugs, feedback, questions and modifications can be posted on the AREXX Forum
 * on http://www.arexx.com/forum/ !
 * Of course you can also write us an e-mail to: info@arexx.nl
 * AREXX Engineering may publish updates from time to time on AREXX.com!
 * ****************************************************************************
 * - LICENSE -
 * GNU GPL v2 (http://www.gnu.org/licenses/gpl.txt, a local copy can be found
 * on the Robotarm CD!)
 * This program is free software. You can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 * ****************************************************************************
 */

/*****************************************************************************/
// EOF



