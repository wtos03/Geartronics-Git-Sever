
/*
  

 Module 					:   BDC Motor control
 Description 				:   Module Description
 Original written for 		:   ATMEGA32
 CPU port/tested			:   ATMEGA32
 Hardware use		:   IC Chip
 Port use			:   Port name / Function
 Memory	FLASH	:   XXX
		RAM		:   XXX
		EEPROM	:   XXX
 Document			:   Document describe algorithm
 Written by 			:   Wichai Tossamartvorakul / wichai@geartronics.net
 Date				:  4/5/2006
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
 */
 
 #include "DC_motor.h"
 
 void main ();
 
 
void main (void)
{
   uint16_t i;
   init_motor();
   while (1)
   {
   
    motor_control (1, MOTOR_FORWARD);
    for (i = 0 ; i < 60000 ;i++);  
	motor_control (2, MOTOR_BACKWARD);
    for (i = 0 ; i < 60000 ;i++);  
   
   }
 
 
}