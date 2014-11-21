#include "LPC213x.h"
#include "RfidReader.h"

int countBitRFID;
int start;
int datacomplete;
int datanow;

// this function will be call to receive data 1 when has interrupt 2
void irq_EINT2 (void)__irq
{
	start = 1; // start receive then set start to 1
    datanow |= (1<<countBitRFID); // add 1 to temp data
	countBitRFID = countBitRFID + 1; // increase bit count
	
	if(countBitRFID == 26) // receive all data
	{
	   	datacomplete = datanow; // copy temp data to datacomplete for ready to use
	   	datanow = 0; // clear temp
	   	start = 0; // end receive then set start to 0
	   	countBitRFID = 0; // clear bit count
	}

	EXTINT = (1 << 2); // clear interrupt bit
	VICVectAddr  = 0; // end interrupt
}

// this function will be call to receive data 0 when has interrupt 3
void irq_EINT3 (void)__irq
{
	start = 1; // start receive then set start to 1
    datanow |= (0<<countBitRFID); // add 0 to temp data
	countBitRFID = countBitRFID + 1; // increase bit count

	if(countBitRFID == 26) // receive all data
	{
	  	datacomplete = datanow;	// copy temp data to datacomplete for ready to use
	  	datanow = 0; // clear temp
	  	start = 0; // end receive then set start to 0
	  	countBitRFID = 0; // clear bit count
	}
	
	EXTINT = (1 << 3); // clear interrupt bit
	VICVectAddr  = 0; // end interrupt
}

// initial external interrupt 2
void init_EINT2(void)
{
 	PINSEL0  |= (3<<14); // set p 0.7 EINT 2
	EXTMODE  |= (1<<2);	// set EINT 2 detect edge
	EXTPOLAR  = (0<<2);	// falling edge interrupt
	VICVectCntl0 = (1<<5) | 16;	// set priority
	VICVectAddr0 = (unsigned) irq_EINT2; // interrupt sevice routine
	VICIntEnable |= (1 << 16); // enable interrupt

}

// initial external interrupt 3
void init_EINT3(void)
{
 	PINSEL1   |= (2<<28); // set p 0.30 EINT 3
	EXTMODE   |= (1<<3); // set EINT 3 detect edge
	EXTPOLAR  |= (0<<3); // falling edge interrupt
	VICVectCntl1 = (1<<5) | 17; // set priority
	VICVectAddr1 = (unsigned) irq_EINT3; // interrupt sevice routine
	VICIntEnable |= (1 << 17); // enable interrupt

}

// initial value of golbal variable, interrupt 2, interrupt 3
void init_RFID(void){

    countBitRFID = 0; // clear bit count
    start = 0; // clear start receive
    datacomplete = 0; // clear datacomplete
    datanow = 0; // clear temp data
	init_EINT2(); // initial interrupt 2
	init_EINT3(); // initial interrupt 3 
}

// check data by use even parity and odd parity
int checkcorrectdata(void)
{
	int vceven,checkeven,i,checkood;
	checkeven = 0;
	checkood  = 0;
	
	
	for(i=0;i<32;i++)
	{
	   // bit 0 - 12 use even parity
	   if(i < 13)
	   {
		   vceven = (1<<i)& datacomplete; // each check will use only 1 bit
		   if(vceven != 0) // this bit is not 0
		   {
              checkeven = checkeven ^ 1; // even count          
           }
	   }

	   // bit 13 - 26 use odd parity
 	   if(i >= 13)
	   {
           vceven = (1<<i)& datacomplete; // each check will use only 1 bit
           if(vceven != 0) // this bit is not 0
		   {
              checkood = checkood ^ 1; // odd count                  
           }      
       }
	}

	if(checkood == 1 && checkeven == 0) // pass check then return 1
	{
       return 1; 
    }       
    return 0; // check fail then return 0
}

// this function use to get only data of RFID card
int dataRFID(void){
     return (datacomplete >> 1)&(0xFEFFFFFF); // ignore odd and even bit
}

// this function use when want to check and get data from RFID reader 
int readRFID(void)
{
     if (start == 1) // program is receiving
	 {
	     while(1) // wait unti receive all data
		 {
		    if(start == 0) // program get all data and stop receive
			{
			     if(checkcorrectdata() == 1) // check data success then return data
				 {
					 return dataRFID();	
				 }
				 else // check data fail then return 0
				 {
				     return 0; 
				 }
			}
		 }
	 }
	 datacomplete = 0; // clear data
	 return 0; // if not have card stamp then return 0
}