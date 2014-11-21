
//----------- include header file -------------
#include "LPC213x.h"
#include <stdio.h>
#include "type.h"
#include "RfidReader.h"
#include "settime.h"
#include "keypad.h"
#include "serial.h"
#include "spi.h"
#include "mmc.h"
#include "LCD.h"



// ------------- Constant Value -------------
#define COMPARE 0
#define CHANGEPASS 1
#define SETTIME 2
#define SAVE 3
#define DELETE 4
#define ADDPASSWORD 5
#define DELETEPASSWORD 6
#define CONFIG 10

#define MAX_PASSWORD_USER 5
#define MAX_PASSWORD 9
#define MAX_RFID_CARD 50
#define MAX_NUMBER_USER 50

#define MAX_BLOCK_NUM 512

// ------------- Golbal variable -------------
long timestate = 0;
int state;
int lMemTem = 0;
int lMemRFID = 0;
int lPasswordUser = 0;
int lPasswordUserTime = 0;
char menuold = '1';
char tempkey[MAX_PASSWORD];	// temp for store word from keypad
char password[MAX_PASSWORD] = "111"; // password
unsigned char temppic[504]; // temp for load picture from SD/MMC

//check message new program for sd/mmc
char ckmsg[] = "E-block Check 9"; // change this number for clear data in sd/mmc

typedef struct RFIDStorage
{
	int datarfid;
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int sec;
}RFIDStorage;

typedef struct PasswordStorage
{
	char passwordUser[MAX_PASSWORD_USER];
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int sec;
}PasswordStorage;

RFIDStorage memoryTem[MAX_RFID_CARD], memoryRFID[MAX_RFID_CARD];
PasswordStorage PassUserTime[MAX_NUMBER_USER], PassUser[MAX_NUMBER_USER];

								
//-------- function prototype ---------
int receive_keypad(int);
int check_password();
void change_password();
void loadpic_mmc(int);
void cleardata(); 

extern void delay_ms(long int ms); // use function from keypad.h/keypad.c

//this function clear temp of keypad(tempkey)
void cleardata(){
    int i;
	
    for(i = 0;i < MAX_PASSWORD;i++)
	{
		 tempkey[i] = '\0';
	}
}

//this function receive charecter from keypad and store it to string tempkey
int receive_keypad(int mode)
{
	char key = '$';
	int i = 0,initial;
	long sec_starts;
	sec_starts = (sec()+30)%60; // set time out 30 second

	// each function that call this fuction has individual receive style	
	// so it has been divide to many mode 
	if(mode == 1)// mode 1 is use for change password
	{
		cleardata(); // clear tempkey
		do
		{
			key = keypad(); // receivce charecter from keypad
		 	if(key != '$') // keypad sent $ if no key has been press so ignore it
			{	
				if(key == '*') // end receive when key * has been press
				{
			  		tempkey[i] = '\0';
					key = '*';
					printf("\n");
				}
				else if(key == '#') // delete data when key # has been press
				{
					if(i > 0)
					{
						lcd_gotoxy(10+(4*(i-1))+(5*(i-1)),3); // set position for lcd
						lcd_put_char(' '); // put charecter to lcd
			 			i--;
						printf("\b \b");
					}
				}
				else if(i == MAX_PASSWORD-1) // if reach last charecter then not do anything
				{

				}
				else // put charecter to string tempkey
				{
					printf("%c",key);
					lcd_gotoxy(10+(4*i)+(5*i),3); // set position for lcd
					lcd_put_char(key); // put charecter to lcd
					tempkey[i] = key;
					i++;
				}
			}
			if((sec()- sec_starts) == 0) // check time out
			{
				cleardata(); // clear tempkey
				return 0; // if receive fail then return 0
			}
		}while(key != '*'); // exit loop when key * has been press
	}
	else if(mode == 2) // mode 2 use for check password user to open door
	{
		i = 1; // start at second charecter since first charecter as been collect before
		lcd_gotoxy(8,4); // set position for lcd
		lcd_put_char('*'); // put charecter to lcd
		do
		{
		 	key = keypad(); // receivce charecter from keypad
	 		if(key != '$') // keypad sent $ if no key has been press so ignore it
			{
				if(i == (MAX_PASSWORD_USER-1)) // auto end receive when reach last charecter
				{
				  	tempkey[i] = '\0';
					printf("\n");
				}
				else if(key == '#') // delete data when key # has been press
				{
					if(i > 0)
					{
						lcd_gotoxy(8+(4*(i-1))+(5*(i-1)),4); // set position for lcd
						lcd_put_char(' '); // put charecter to lcd
			 			i--;
						printf("\b \b");
						if(i == 0) // exit and wait receive first charecter
						{
							tempkey[i] = '\0';
						 	return 0; // if receive fail then return 0
						}
					}
				}
				else if(key != '*')// if charecter is not * and # then put to string tempkey
				{
					printf("%c",key);
					lcd_gotoxy(8+(4*i)+(5*i),4); // set position for lcd
					lcd_put_char('*'); // put charecter to lcd
					tempkey[i] = key;
					i++;
				}
			}		
			if((sec()- sec_starts) == 0) // check time out
			{
				cleardata(); // clear tempkey
				return 0; // if receive fail then return 0
			}
		}while(i != (MAX_PASSWORD_USER-1)); // exit loop when reach last charecter
	}
 	else if(mode == 3) // mode 3 use for add and delete password user
	{
		cleardata(); // clear tempkey
		do
		{
		 	key = keypad();	// receivce charecter from keypad
	 		if(key != '$') // keypad sent $ if no key has been press so ignore it
			{
			
				if((key == '*')&&(i == MAX_PASSWORD_USER-1)) // end receive when key * has been press and reach last charecter
				{
			  		tempkey[i] = '\0';
					key = '*';
					printf("\n");
				}
				else if(key == '#') // delete data when key # has been press
				{
					if(i > 0)
					{
						lcd_gotoxy(24+(4*(i-1))+(5*(i-1)),3); // set position for lcd
						lcd_put_char(' '); // put charecter to lcd
			 			i--;
						printf("\b \b");
					}
				}
				else if(i == MAX_PASSWORD_USER-1) // if reach last charecter then not do anything
				{

				}
				else if(key != '*') // if charecter is not * and # then put to string tempkey
				{
					printf("%c",key);
					lcd_gotoxy(24+(4*i)+(5*i),3); // set position for lcd
					lcd_put_char(key); // put charecter to lcd
					tempkey[i] = key;
					i++;
				}
			}		
			if((sec()- sec_starts) == 0) // check time out
			{
				cleardata(); // clear tempkey
				return 0; // if receive fail then return 0
			}
		}while((key != '*')||(i != (MAX_PASSWORD_USER-1))); // exit loop when key * has been press and reach last charecter
	}
	else if(mode == 4) // mode 4 use for select menu (state config)
	{
		cleardata(); // clear tempkey
		printf(" ");
		initial = 1;
		do
		{
			key = keypad(); // receivce charecter from keypad
			if(initial == 1) // auto select one menu when open menu
			{
				key = menuold;
				initial = 0;
			}
			else if(tempkey[0] != '\0') // when exit from function, that fuction menu will be select
			{
			 	menuold = tempkey[0];
			}


		 	if(key != '$') // keypad sent $ if no key has been press so ignore it
			{
				
				if(key == '*') // end receive (confirm select) when key * has been press
				{
			  		tempkey[1] = '\0';
					key = '*';
					printf("\n");
				}
				else if(key == '#') // exit menu when key # has been press
				{
					tempkey[0] = '\0';
					menuold = '1';
					printf("\n");
					return 0; // if receive fail then return 0
				}
				else if(key == '1') // select menu 1 change password when key 1 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(403); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '2') // select menu 2 set date time when key 2 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(404); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '3') // select menu 3 save rfid card when key 3 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(405); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '4') // select menu 4 delete rfid card when key 4 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(406); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '5') // select menu 5 add password user when key 5 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(407); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '6') // select menu 6 delete password user when key 6 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(408); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '7') // select menu 7 show information of rfid card when key 7 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(409); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '8') // select menu 8 show information of password user when key 8 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(410); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '9') // select menu 9 show date time and card or password that use to open door when key 9 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(411); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
				else if(key == '0') // select menu 0 show date and time when key 0 has been press
				{
					printf("\b%c",key);
					tempkey[0] = key;
					loadpic_mmc(412); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
				}
			}
			if((sec()- sec_starts) == 0) // check time out
			{
				cleardata(); // clear tempkey
				menuold = '1';
				return 0; // if receive fail then return 0
			}
		}while((key != '*')&&(key != '#'));// exit loop key * or # has been press
	}
	else if(mode == 5)// mode 5 for password enter menu
	{
		cleardata(); // clear tempkey
		do
		{
			key = keypad();	// receivce charecter from keypad
		 	if(key != '$') // keypad sent $ if no key has been press so ignore it
			{
				
				if(key == '*') // end receive when key * has been press
				{
			  		tempkey[i] = '\0';
					key = '*';
					printf("\n");
				}
				else if(key == '#') // delete data when key # has been press
				{
					if(i > 0)
					{
						lcd_gotoxy(8+(2*(i-1))+(5*(i-1)),4); // set position for lcd
						lcd_put_char(' '); // put charecter to lcd
			 			i--;
						printf("\b \b");
					}
				}
				else if(i == MAX_PASSWORD-1) // if reach last charecter then not do anything
				{

				}
				else // put charecter to string tempkey
				{
					lcd_gotoxy(8+(2*i)+(5*i),4); // set position for lcd
					lcd_put_char('*'); // put charecter to lcd
					printf("%c",key);
					tempkey[i] = key;
					i++;
				}
			}
			if((sec()- sec_starts) == 0) // check time out
			{
				cleardata(); // clear tempkey
				return 0; // if receive fail then return 0
			}
		}while(key != '*'); // exit loop when key * has been press
	}
	else if(mode == 6) // mode 6 for enter day, month and year
	{
		cleardata(); // clear tempkey
		do
		{
		 	key = keypad();	// receivce charecter from keypad
	 		if(key != '$') // keypad sent $ if no key has been press so ignore it
			{
			
				if((key == '*')&&(i == 8)) // end receive when key * has been press and reach last charecter
				{
			  		tempkey[i] = '\0';
					key = '*';
					printf("\n");
				}
				else if(key == '#') // delete data when key # has been press
				{
					if(i > 0)
					{
						lcd_gotoxy(8+(4*(i-1))+(5*(i-1)),3); // set position for lcd
						lcd_put_char(' '); // put charecter to lcd
			 			i--;
						printf("\b \b");
					}
				}
				else if(i == 8) // if reach last charecter then not do anything
				{

				}
				else if(key != '*') // if charecter is not * and # then put to string tempkey
				{
					lcd_gotoxy(8+(4*i)+(5*i),3); // set position for lcd
					lcd_put_char(key); // put charecter to lcd
					printf("%c",key);
					tempkey[i] = key;
					i++;
				}
			}		
			if((sec()- sec_starts) == 0) // check time out
			{
				cleardata(); // clear tempkey
				return 0; // if receive fail then return 0
			}
		}while((key != '*')||(i != 8)); // exit loop when key * has been press and reach last charecter
	}
	else if(mode == 7) // mode 7 for enter hour and minute
	{
		cleardata(); // clear tempkey
		do
		{
		 	key = keypad();	// receivce charecter from keypad
	 		if(key != '$') // keypad sent $ if no key has been press so ignore it
			{
			
				if((key == '*')&&(i == 4)) // end receive when key * has been press and reach last charecter
				{
			  		tempkey[i] = '\0';
					key = '*';
					printf("\n");
				}
				else if(key == '#') // delete data when key # has been press
				{
					if(i > 0)
					{
						lcd_gotoxy(20+(4*(i-1))+(5*(i-1)),3); // set position for lcd
						lcd_put_char(' '); // put charecter to lcd
			 			i--;
						printf("\b \b");
					}
				}
				else if(i == 4) // if reach last charecter then not do anything
				{

				}
				else if(key != '*') // if charecter is not * and # then put to string tempkey
				{
					printf("%c",key);
					lcd_gotoxy(20+(4*i)+(5*i),3); // set position for lcd
					lcd_put_char(key); // put charecter to lcd
					tempkey[i] = key;
					i++;
				}
			}		
			if((sec()- sec_starts) == 0) // check time out
			{
				cleardata(); // clear tempkey
				return 0; // if receive fail then return 0
			}
		}while((key != '*')||(i != 4)); // exit loop when key * has been press and reach last charecter
	}

	return 1; // if receive complete then return 1
}

// this function use for check password both password user and password to enter menu
// if use for check password user select 1
// if use for check password to enter menu select 2
int check_password(int sel)
{
	int i,j,wrong_pass = 0;
	if(sel == 1) // enter mode check password user
	{
	    for(i = 0;i< MAX_NUMBER_USER;i++) // loop for check all password user
		{
			wrong_pass = 1;	
		   for(j = 0;j < MAX_PASSWORD_USER;j++) // loop compare password user with that password receive from user
		   {
			    if(tempkey[j] != PassUser[i].passwordUser[j]) // if has different set wrong_pass to 0
			    {
		 		   wrong_pass = 0;
				}
				
				// if both password user and password receive from user reach end then exit
			    if((tempkey[j] == '\0') && (PassUser[i].passwordUser[j] == '\0'))
			    {
					if(j == 0)// if no data in password receive from user then set wrong_pass to 0
						wrong_pass = 0;
			        j = MAX_PASSWORD_USER;
				}
		   }
		   if(wrong_pass) // if wrong_pass is 1 then password user is correct and return 1
		   		return 1;
		}
		return 0; // password user not correct and return 0
	}
	else if(sel == 2) // enter mode check password to enter menu
	{
 		for(i = 0;i < MAX_PASSWORD;i++)
		{
			// if has different between password and password receive from user then return 0
			if(tempkey[i] != password[i])
		 		return 0;

			// if both password and password receive from user reach end then exit
			if((tempkey[i] == '\0') && (password[i] == '\0'))
			{
			    i = MAX_PASSWORD;
			}
		}

		return 1; // password is correct and return 1
	}
	else
		return 0;
}

// this function use for change password of enter menu
void change_password()
{
	int i;
	if(tempkey[0] == '\0') // if no data then exit
 	{
   		return;
	}
	for(i = 0;i < MAX_PASSWORD;i++) // set new password 
	{
		 password[i] = tempkey[i];
	}
}

// this function use for change state to Compare
void setStateCompare(void){
	printf("\n*****************\n");
	printf("* State Compare *\n");
	printf("*****************\n");
    state = COMPARE;
	loadpic_mmc(401); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
}

// this function use for change state to Config
void setStateConfig(void){
	printf("\n****************\n");
	printf("* State Config *\n");
	printf("****************\n");
	printf("[1] Change password\n");
	printf("[2] Set time\n");
	printf("[3] Save RFID card\n");
	printf("[4] Delete RFID card\n");
	printf("[5] Add password user\n");
	printf("[6] Delete password user\n");
	printf("[7] Show RFID card save time\n");
	printf("[8] Show password user save time\n");
	printf("[9] Show Stamp time\n");
	printf("[0] Show time\n");
	printf("Select Menu :");
    state = CONFIG; 
}

// this function use for change state to Change password
void setStateChangepass(void){
	printf("*************************\n");
	printf("* State Change Password *\n");
	printf("*************************\n");
	printf("Enter New Password :");
    state = CHANGEPASS;
}

// this function use for change state to Set time
void setStateSettime(void){
	printf("******************\n");
	printf("* State Set Time *\n");
	printf("******************\n");
	printf("Enter Time (ddmmyyyy) :");
    state = SETTIME;
}

// this function use for change state to Save
void setStateSave(void){
	printf("**************\n");
	printf("* State Save *\n");
	printf("**************\n");
	printf("Please Stamp Card in 1 minute\n");
	loadpic_mmc(418); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
    state = SAVE; 
}

// this function use for change state to Delete
void setStateDelete(void){
	printf("****************\n");
	printf("* State Delete *\n");
	printf("****************\n");
	printf("Please Stamp Card in 1 minute\n");
	loadpic_mmc(418); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
    state = DELETE; 
}

// this function use for change state to Add password
void setStateAddPassword(void){
	printf("**********************\n");
	printf("* State Add Password *\n");
	printf("**********************\n");
	printf("Enter Password User :");
	loadpic_mmc(419); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
    state = ADDPASSWORD; 
}

// this function use for change state to Delete password
void setStateDeletePassword(void){
	printf("*************************\n");
	printf("* State Delete Password *\n");
	printf("*************************\n");
	printf("Enter Password User :");
	loadpic_mmc(419); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
    state = DELETEPASSWORD; 
}

// this function use for check ID of RFID card
int CheckRFIDCard(int rfid)
{
	 int i;
	 for(i=0;i<MAX_RFID_CARD;i++) // loop for check all card
	 {
	     if( memoryRFID[i].datarfid == rfid) // if ID of RFID card that stamp and ID of RFID card in system
		 									 // is match then return 1
		 {
		     return 1;
		 }
	 }
	 return 0; // not found match ID of card then return 0
	
}

// this function use for collect ID of RFID card, date and time that open door
void inputMemTem(int datarfid)
{
      // collect data
      memoryTem[lMemTem].datarfid = datarfid;
	  memoryTem[lMemTem].day = day();
	  memoryTem[lMemTem].month = month();
	  memoryTem[lMemTem].year = year();
	  memoryTem[lMemTem].hour = hour();
	  memoryTem[lMemTem].minute = minute();
	  memoryTem[lMemTem].sec = sec();
	  lMemTem++; // increase number of card that use to open door
	  if( lMemTem == MAX_RFID_CARD) // if number of card reach to maximun then reset number of card to 0
	  {
		  lMemTem = 0;
	  }
}

// this function use for add ID of RFID card and add time to system
int inputMemRFID(int inrfid)
{
      int i;
      if( lMemRFID == MAX_RFID_CARD) // if number of card is full then return 0
	  {
		  return 0;
	  }
	  for(i=0;i<MAX_RFID_CARD;i++) // if card already save then return 0
	  {
	     if( memoryRFID[i].datarfid == inrfid)
		 {
		     return 0;
		 }
	  }

	  // add card to system
      memoryRFID[lMemRFID].datarfid = inrfid;
	  memoryRFID[lMemRFID].day = day();
	  memoryRFID[lMemRFID].month = month();
	  memoryRFID[lMemRFID].year = year();
	  memoryRFID[lMemRFID].hour = hour();
	  memoryRFID[lMemRFID].minute = minute();
	  memoryRFID[lMemRFID].sec = sec();
	  lMemRFID++; // increase number of card in system
	  return 1; // return 1 when add card is done
}

// this function use for delete ID of RFID card and card information to system
int deleteMemRFID(int inrfid)
{
	 int i;
	 for(i=0;i<MAX_RFID_CARD;i++)
	 {
	     if( memoryRFID[i].datarfid == inrfid) // if ID of card that stamp and ID of card in system is match then delete 
		 {
			 memoryRFID[i] = memoryRFID[--lMemRFID]; // copy data of last card to data of match card
			 										 // and decrease number of card in system
			 memoryRFID[lMemRFID].datarfid = 0; // remove data of last card
			 return 1; // return 1 when done
		 }
	 }
	 return 0; // not found ID of card then return 0
}

// this function use for add password user and add time to system
int addPasswordUser()
{
	int i;
	if(lPasswordUser == MAX_NUMBER_USER) // if number of password user is full then return 0
	{
		return 0;
	}
	else if(check_password(1) == 1) // if password user already save then return 0
	{
		return 0;
	}
	else // add password user to system
	{
		for(i = 0;i < MAX_PASSWORD_USER;i++)
		{
			PassUser[lPasswordUser].passwordUser[i] = tempkey[i];
		}
		PassUser[lPasswordUser].day = day();
		PassUser[lPasswordUser].month = month();
		PassUser[lPasswordUser].year = year();
		PassUser[lPasswordUser].hour = hour();
		PassUser[lPasswordUser].minute = minute();
		PassUser[lPasswordUser].sec = sec();
		lPasswordUser++; // increase number of password user in system 
  	}
	return 1; // add passord user complete then return 1
}

// this function use for collect password user, date and time that open door
void addPasswordUserTime()
{
	int i;
	// collect data
 	for(i = 0;i < MAX_PASSWORD_USER;i++)
	{
		PassUserTime[lPasswordUserTime].passwordUser[i] = tempkey[i];
	}
	PassUserTime[lPasswordUserTime].day = day();
	PassUserTime[lPasswordUserTime].month = month();
	PassUserTime[lPasswordUserTime].year = year();
	PassUserTime[lPasswordUserTime].hour = hour();
	PassUserTime[lPasswordUserTime].minute = minute();
	PassUserTime[lPasswordUserTime].sec = sec();
	lPasswordUserTime++; // increase number of password user that use to open door
	if( lPasswordUserTime == MAX_NUMBER_USER)  // if number of password user reach to maximun then reset number of card to 0
	{
		lPasswordUserTime = 0;
	}	
}

// this function use for delete password user and password user information to system
int deletePasswordUser()
{
	int i,j,check_correct = 0;
  	if(check_password(1) == 1) // if has password user same as password that receive from user then delete
  	{
 		for(i = 0;i < MAX_NUMBER_USER;i++)
		{
			check_correct = 1;
			for(j = 0;j < MAX_PASSWORD_USER;j++)
			{
			 	// if has different between password user and password that receive from user
			 	// then set check_correct to 0 for skip delete section
				if(tempkey[j] != PassUser[i].passwordUser[j])
				{
					check_correct = 0;
				}
			}
			if(check_correct == 1) // if check_correct is 1 then delete current password user
			{
				lPasswordUser--; // decrease number of password user
				for(j = 0;j < MAX_PASSWORD_USER;j++)
				{
					// copy data of last password user to data of current password user
			 		PassUser[i] = PassUser[lPasswordUser];
				}
				PassUser[lPasswordUser].passwordUser[0] = '\0'; // clear data of last password user
			}
		}
	}
  	else // if not has password user same as password that receive from user then return 0
  	{
   		return 0;
  	}
	return 1; // if delete complete then return 1
}

// this function use for open door							 
void opendoor()
{
    //IODIR0 |= (1<<10);
	//IOSET0 = (1<<10);
	
	loadpic_mmc(413); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
	delay_ms(500);
	loadpic_mmc(414); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd

	delay_ms(5000);
	
	loadpic_mmc(413); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
	delay_ms(1000);
	loadpic_mmc(401); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
	//IOCLR0 = (1<<10);
}

// this function use for load data from sd/mmc
int loaddata_mmc()
{
	/*
	   block 0 for check and password
	   block 1 to 50 for password user
	   block 51 to 100 for RFID Card data
	   block 101 to 150 for RFID stamp time
	   block 151 to 200 fot password user time
	   block 201 for global variable

	   block 401 for pic state compare
	   block 402 for pic enter password to state config
	   block 403 for menu 1 change password
	   block 404 for menu 2 set time
	   block 405 for menu 3 save rfid card
	   block 406 for menu 4 delete rfid card
	   block 407 for menu 5 add password user
	   block 408 for menu 6 delete password user
	   block 409 for menu 7 RFID save time
	   block 410 for menu 8	Password User save time
	   block 411 for menu 9	Stamp time
	   block 412 for menu 0	Show time

	   block 413 for pic close door
	   block 414 for pic open door
	   block 415 for pic check fail
	   block 416 for pic store complete
	   block 417 for pic store fail
	   block 418 for pic wait card
	   block 419 for pic wait pass
	   block 420 for pic delete complete
	   block 421 for pic delete fail
	   block 422 for pic Enter day month year time
	   block 423 for pic Enter Time
	 */
	int i = 0,j = 0;
	int temp[MMC_DATA_SIZE/4];

	
	// load block 0
	mmc_read_block_int(0,temp); // load data to temp

	// check message for new program if different then load fail
	for(i = 0;i < sizeof(ckmsg);i++)
	{
	 	if(temp[i] != ckmsg[i])
			return 0;
	}
	printf("\n");
	for(j = 0;j < i;j++)
	{
	 	printf("%c",temp[j]);
	}
	printf("\n");

	// copy password from temp
	for(j = 0;j < MAX_PASSWORD;j++)
	{
	 	password[j] = temp[i];
		i++; 
	}

	// load block 1-50
	for(i = 0;i < MAX_NUMBER_USER;i++)
	{
		mmc_read_block_int(1+i,temp); // load data to temp
		
		// if temp has data then copy password user and save time to password user storage
		if(temp[0] != '\0')
		{
	 		for(j = 0;j < MAX_PASSWORD_USER;j++)
			{
			 	PassUser[i].passwordUser[j] = temp[j];
			}
			PassUser[i].day = temp[MAX_PASSWORD_USER];
			PassUser[i].month = temp[MAX_PASSWORD_USER+1];
			PassUser[i].year = temp[MAX_PASSWORD_USER+2];
			PassUser[i].hour = temp[MAX_PASSWORD_USER+3];
			PassUser[i].minute = temp[MAX_PASSWORD_USER+4];
			PassUser[i].sec = temp[MAX_PASSWORD_USER+5];
		}
	}
	
	// load block 51-100
	for(i = 0;i < MAX_RFID_CARD;i++)
	{
		mmc_read_block_int(51+i,temp); // load data to temp
		
		// if temp has data then copy ID of RFID card and save time to RFID card storage
		if(temp[0] != 0)
		{
			memoryRFID[i].datarfid = temp[0];
			memoryRFID[i].day = temp[1];
			memoryRFID[i].month = temp[2];
			memoryRFID[i].year = temp[3];
			memoryRFID[i].hour = temp[4];
			memoryRFID[i].minute = temp[5];
			memoryRFID[i].sec = temp[6];
		}
	}
	
	// load block 101-150
	for(i = 0;i < MAX_RFID_CARD;i++)
	{
		mmc_read_block_int(101+i,temp); // load data to temp
		
		// if temp has data then copy to RFID card open door time storage
		if(temp[0] != 0)
		{
			memoryTem[i].datarfid = temp[0];
			memoryTem[i].day = temp[1];
			memoryTem[i].month = temp[2];
			memoryTem[i].year = temp[3];
			memoryTem[i].hour = temp[4];
			memoryTem[i].minute = temp[5];
			memoryTem[i].sec = temp[6];
		}
	} 

	// load block 151-200
	for(i = 0;i < MAX_NUMBER_USER;i++)
	{
		mmc_read_block_int(151+i,temp); // load data to temp

		// if temp has data then copy to password user open door time storage
		if(temp[0] != '\0')
		{
	 		for(j = 0;j < MAX_PASSWORD_USER;j++)
			{
			 	PassUserTime[i].passwordUser[j] = temp[j];
			}
			PassUserTime[i].day = temp[MAX_PASSWORD_USER];
			PassUserTime[i].month = temp[MAX_PASSWORD_USER+1];
			PassUserTime[i].year = temp[MAX_PASSWORD_USER+2];
			PassUserTime[i].hour = temp[MAX_PASSWORD_USER+3];
			PassUserTime[i].minute = temp[MAX_PASSWORD_USER+4];
			PassUserTime[i].sec = temp[MAX_PASSWORD_USER+5];
		}
	}
	
	// load block 201
	mmc_read_block_int(201,temp); // load data to temp
	
	// copy data from temp to global variable
	lMemTem = temp[0];
	lMemRFID = temp[1];
	lPasswordUser = temp[2]; 
	lPasswordUserTime = temp[3];

	return 1; // if load complete then return 1
}

// this function use for save data to sd/mmc 
// number = 0 is for check message and password
// number = 1 is for password user
// number = 2 is for RFID Card data
// number = 3 is for RFID card open door time
// number = 4 is for password user open door time  
int savedata_mmc(int number)
{
	/*
	   block 0 for check and password ===> Number 0
	   block 1 to 50 for password user ===> Number 1
	   block 51 to 100 for RFID Card data ===> Number 2
	   block 101 to 150 for RFID stamp time	===> Number 3
	   block 151 to 200 fot password user time ===>Number 4
	   block 201 for global variable

	   block 401 for pic state compare
	   block 402 for pic enter password to state config
	   block 403 for menu 1 change password
	   block 404 for menu 2 set time
	   block 405 for menu 3 save rfid card
	   block 406 for menu 4 delete rfid card
	   block 407 for menu 5 add password user
	   block 408 for menu 6 delete password user
	   block 409 for menu 7 RFID save time
	   block 410 for menu 8	Password User save time
	   block 411 for menu 9	Stamp time
	   block 412 for menu 0	Show time

	   block 413 for pic close door
	   block 414 for pic open door
	   block 415 for pic check fail
	   block 416 for pic store complete
	   block 417 for pic store fail
	   block 418 for pic wait card
	   block 419 for pic wait pass
	   block 420 for pic delete complete
	   block 421 for pic delete fail
	   block 422 for pic Enter day month year time
	   block 423 for pic Enter Time
	 */
 	int i = 0,j = 0;
	int temp[MMC_DATA_SIZE/4];
	
	if(number == 0) // number = 0 is for check message and password
	{
		// add check message for new program to temp
		for(i = 0;i < sizeof(ckmsg);i++)
		{
		 	temp[i] = ckmsg[i];
		}

		// add password to temp
		for(j = 0;j < MAX_PASSWORD;j++)
		{
	 		temp[i] = password[j];
			i++; 
		}

		// write data from temp to mmc 
		if(mmc_write_block_int(0,temp) != 0)
		{
		 	return 0; // write fail return 0
		}
		return 1;	
	}
	else if(number == 1) // number = 1 is for password user
	{
	  	for(i = 0;i < MAX_NUMBER_USER;i++)
		{
			// if password user has data then add password user and save time to temp
			if(PassUser[i].passwordUser[0] != '\0')
			{
				for(j = 0;j < MAX_PASSWORD_USER;j++)
				{
				 	temp[j] = PassUser[i].passwordUser[j];
				}
				temp[MAX_PASSWORD_USER] = PassUser[i].day;
				temp[MAX_PASSWORD_USER+1] = PassUser[i].month;
				temp[MAX_PASSWORD_USER+2] = PassUser[i].year;
				temp[MAX_PASSWORD_USER+3] = PassUser[i].hour;
				temp[MAX_PASSWORD_USER+4] = PassUser[i].minute;
				temp[MAX_PASSWORD_USER+5] = PassUser[i].sec;

				// write data from temp to mmc
				if(mmc_write_block_int(1+i,temp) != 0)
				{
			 		return 0; // write fail return 0
				}
			}
			// if password user not has data then add no data to temp
			else
			{
				temp[0] = '\0';

				// write data from temp to mmc
				if(mmc_write_block_int(1+i,temp) != 0)
				{
			 		return 0; // write fail return 0
				}
			}
		}

		// add global variable to temp
		temp[0] = lMemTem;
		temp[1] = lMemRFID;
		temp[2] = lPasswordUser;
		temp[3] = lPasswordUserTime;

		// write data from temp to mmc
		if(mmc_write_block_int(201,temp) != 0)
		{
			return 0; // write fail return 0
		}

		return 1; // write complete return 1
	}
	else if(number == 2) // number = 2 is for RFID Card data
	{
		for(i = 0;i < MAX_RFID_CARD;i++)
		{
			// if RFID card has data then add ID of RFID card and save time to temp
			if(memoryRFID[i].datarfid != 0)
			{
				temp[0] = memoryRFID[i].datarfid;
				temp[1] = memoryRFID[i].day;
				temp[2] = memoryRFID[i].month;
				temp[3] = memoryRFID[i].year;
				temp[4] = memoryRFID[i].hour;
				temp[5] = memoryRFID[i].minute;
				temp[6] = memoryRFID[i].sec;

				// write data from temp to mmc
				if(mmc_write_block_int(51+i,temp) != 0)
				{
					return 0; // write fail return 0
				}
			}
			// if RFID card not has data then add zero data to temp
			else
			{
				temp[0] = 0;

				// write data from temp to mmc
				if(mmc_write_block_int(51+i,temp) != 0)
				{
					return 0; // write fail return 0
				}
			}
		}

		// add global variable to temp
		temp[0] = lMemTem;
		temp[1] = lMemRFID;
		temp[2] = lPasswordUser;
		temp[3] = lPasswordUserTime;

		// write data from temp to mmc
		if(mmc_write_block_int(201,temp) != 0)
		{
			return 0; // write fail return 0
		}

	  	return 1; // write complete return 1
	}
	else if(number == 3) // number = 3 is for RFID card open door time
	{
		for(i = 0;i < MAX_RFID_CARD;i++)
		{
			// if rfid card open door time has data then add rfid card open door time data to temp
			if(memoryTem[i].datarfid != 0)
			{
				temp[0] = memoryTem[i].datarfid;
				temp[1] = memoryTem[i].day;
				temp[2] = memoryTem[i].month;
				temp[3] = memoryTem[i].year;
				temp[4] = memoryTem[i].hour;
				temp[5] = memoryTem[i].minute;
				temp[6] = memoryTem[i].sec;
			
				// write data from temp to mmc
				if(mmc_write_block_int(101+i,temp) != 0)
				{
					return 0; // write fail return 0
				}
			}
			// if rfid card open door time not has data then add zero data to temp
			else
			{
				temp[0] = 0;

				// write data from temp to mmc
				if(mmc_write_block_int(101+i,temp) != 0)
				{
					return 0; // write fail return 0
				}
			}
		}

		// add global variable to temp
		temp[0] = lMemTem;
		temp[1] = lMemRFID;
		temp[2] = lPasswordUser;
		temp[3] = lPasswordUserTime;

		// write data from temp to mmc
		if(mmc_write_block_int(201,temp) != 0)
		{
			return 0; // write fail return 0
		}

	  	return 1; // write complete return 1
	}
	else if(number == 4) // number = 4 is for password user open door time
	{
	   	for(i = 0;i < MAX_NUMBER_USER;i++)
		{
			// if password user open door time has data then add password user open door time data to temp
			if(PassUserTime[i].passwordUser[0] != '\0')
			{
				for(j = 0;j < MAX_PASSWORD_USER;j++)
				{
				 	temp[j] = PassUserTime[i].passwordUser[j];
				}
				temp[MAX_PASSWORD_USER] = PassUserTime[i].day;
				temp[MAX_PASSWORD_USER+1] = PassUserTime[i].month;
				temp[MAX_PASSWORD_USER+2] = PassUserTime[i].year;
				temp[MAX_PASSWORD_USER+3] = PassUserTime[i].hour;
				temp[MAX_PASSWORD_USER+4] = PassUserTime[i].minute;
				temp[MAX_PASSWORD_USER+5] = PassUserTime[i].sec;

				// write data from temp to mmc
				if(mmc_write_block_int(151+i,temp) != 0)
				{
			 		return 0; // write fail return 0
				}
			}
			// if password user open door time not has data then add no data to temp
			else
			{
				temp[0] = '\0';

				// write data from temp to mmc
				if(mmc_write_block_int(151+i,temp) != 0)
				{
			 		return 0; // write fail return 0
				}
			}
		}
		
		// add global variable to temp
		temp[0] = lMemTem;
		temp[1] = lMemRFID;
		temp[2] = lPasswordUser;
		temp[3] = lPasswordUserTime;

		// write data from temp to mmc
		if(mmc_write_block_int(201,temp) != 0)
		{
			return 0; // write fail return 0
		}

		return 1; // write complete return 1
	}

	return 0; // write fail return 0
}

// this function use for clear data of sd/mmc
int cleardata_mmc()
{
 	int i = 0,j = 0;
	int temp[MMC_DATA_SIZE/4];
	
	// add no data to temp
	for(i = 0;i < MAX_NUMBER_USER;i++)
	{
		for(j = 0;j < MAX_PASSWORD_USER;j++)
		{
		 	temp[j] = '\0';
		}

		// write data from temp to mmc
		if(mmc_write_block_int(1+i,temp) != 0)
		{
	 		return 0; // write fail return 0
		}
	}

	// add zero data to temp
	for(i = 0;i < MAX_RFID_CARD;i++)
	{
		temp[0] = 0;
		temp[1] = 0;
		temp[2] = 0;
		temp[3] = 0;
		temp[4] = 0;
		temp[5] = 0;
		temp[6] = 0;
	 	
		// write data from temp to mmc
	 	if(mmc_write_block_int(51+i,temp) != 0)
		{
			return 0; // write fail return 0
		}
	}

	// add zero data to temp
	for(i = 0;i < MAX_RFID_CARD;i++)
	{
		temp[0] = 0;
		temp[1] = 0;
		temp[2] = 0;
		temp[3] = 0;
		temp[4] = 0;
		temp[5] = 0;
		temp[6] = 0;
		
		// write data from temp to mmc	
		if(mmc_write_block_int(101+i,temp) != 0)
		{
			return 0; // write fail return 0
		}
	}

	// add no data to temp
	for(i = 0;i < MAX_NUMBER_USER;i++)
	{
		for(j = 0;j < MAX_PASSWORD_USER;j++)
		{
		 	temp[j] = '\0';
		}

		// write data from temp to mmc
		if(mmc_write_block_int(151+i,temp) != 0)
		{
	 		return 0; // write fail return 0
		}
	}

	// add zero data to temp
	temp[1] = 0;
	temp[2] = 0;
	temp[3] = 0;
	temp[4] = 0;

	// write data from temp to mmc
	if(mmc_write_block_int(201,temp) != 0)
	{
		return 0; // write fail return 0
	}

	return 1; // write complete return 1
}

// this function use for load picture from sd/mmc
void loadpic_mmc(int blocknum)
{
	int i =0;
	char temp[MMC_DATA_SIZE] = { '\0' };

	// load data to temp
 	mmc_read_block_char(blocknum,temp);
	
	// copy from temp to temp picture
	for(i = 0;i < 504;i++)
	{
		temppic[i] = temp[i];
	}
}

// this function use for set date and time
void settime(void)
{
	int minutenew = 0,hournew = 0,daynew = 0,monthnew = 0,yearnew = 0,daymod = 0;

 	//------ date zone ------

	loadpic_mmc(422); // load picture from sd/mmc
	lcd_fill_picture(temppic); // show picture on lcd
	receive_keypad(6); // receive data from keypad

	// check if all tempkey has data
	if((tempkey[0] != '\0') && (tempkey[1] != '\0') && (tempkey[2] != '\0') && (tempkey[3] != '\0')
	&&(tempkey[4] != '\0') && (tempkey[5] != '\0') && (tempkey[6] != '\0') && (tempkey[7] != '\0'))
	{
		// convert tempkey to year
		yearnew = (((tempkey[4]-48)*1000)+((tempkey[5]-48)*100)+((tempkey[6]-48)*10)+(tempkey[7]-48));

		// convert tempkey to month
	    monthnew = (((tempkey[2]-48)*10)+(tempkey[3]-48))%13;
		if(monthnew == 0)
			monthnew++;
	
		// set day of month
		if(monthnew == 1)
		{
			daymod = 32;
		}
		else if(monthnew == 2)
		{
		 	if((yearnew%4) == 0)
			{
				daymod = 30;
			}
			else
			{
			 	daymod = 29;
			}				
		}
		else if(monthnew == 3)
		{
		 	daymod = 32;
		}
		else if(monthnew == 4)
		{
		 	daymod = 31;
		}
		else if(monthnew == 5)
		{
		 	daymod = 32;
		}
		else if(monthnew == 6)
		{
		 	daymod = 31;
		}
		else if(monthnew == 7)
		{
		 	daymod = 32;
		}
		else if(monthnew == 8)
		{
		 	daymod = 32;
		}
		else if(monthnew == 9)
		{
		 	daymod = 31;
		}
		else if(monthnew == 10)
		{
		 	daymod = 32;
		}
		else if(monthnew == 11)
		{
		 	daymod = 31;
		}
		else if(monthnew == 12)
		{
		 	daymod = 32;
		}
		
		// convert tempkey to day				
	    daynew = (((tempkey[0]-48)*10)+(tempkey[1]-48))%daymod;
		if(daynew == 0)
			daynew++;
	 }

	 setyear(yearnew); // set year = new year
	 setmonth(monthnew); // set month = new month
	 setday(daynew); // set day = new day

	 //------ time zone ------

	 printf("Enter time (hhmm) :");
	 loadpic_mmc(423); // load picture from sd/mmc
	 lcd_fill_picture(temppic); // show picture on lcd
	 receive_keypad(7); // receive data from keypad

	 // check if all tempkey has data
     if((tempkey[0] != '\0') && (tempkey[1] != '\0') && (tempkey[2] != '\0') && (tempkey[3] != '\0'))
	 {
			minutenew = (((tempkey[2]-48)*10)+(tempkey[3]-48))%60; // convert tempkey to minute
			hournew = (((tempkey[0]-48)*10)+(tempkey[1]-48))%24; // convert tempkey to hour
	 }

	 setminute(minutenew); // set minute = new minute
	 sethour(hournew); // set hour = new hour
	 setsec(0);	// set sec = 0

	 printf("\n%d/%d/%d\n%d:%d:%d\n",day(),month(),year(),hour(),minute(),sec());
}


// main program
void main(void)
{    
	char ch;
	int value;
	int j;

	init_serial0(); // initial serial port 0
	init_keypad(); // initial keypad
    
	initTime(); // initial date and time
    init_RFID(); // initial RFID Reader
	SPI_Init(); // initial SPI1
	lcd_initial(); // initial LCD                							
  	lcd_clear_screen(); // clear LCD screen
	if( mmc_init() != 0 ) // initial sd/mmc fail
	{
		printf("\nMMC init fail");
	}
	else // initial sd/mmc success
	{
		// load data from mmc
		// if this a new program then load will fail and will clear data and save new check message to sd/mmc	
	 	if(loaddata_mmc() == 0) // load fail
	 	{ 
			printf("Load fail\n");
			cleardata_mmc(); // clear data in sd/mmc
			if (savedata_mmc(0) == 0) // save fail
			{
		  		printf("Save fail\n");
			}
			else // save success
			{
		  		printf("Save complete");
			}
		}
		// if this a old program then program will load data from sd/mmc
		else // load success
		{
		  	printf("Load complete");
		}
	}
	
	// change state to compare to ready for use
	setStateCompare();

	printf("\n%d/%d/%d\n%d:%d:%d\n",day(),month(),year(),hour(),minute(),sec());

	// infinity	loop 
	while(1)
	{
	   	value = readRFID(); // get ID of RFID card
	   	ch = keypad(); // get pressing key of keypad
	   	
		// if card has been stamp value will not be 0
		// so in this if will be state that require stamp card to operate
		if(value != 0) 
	   	{
			// when state is compare, program will check ID of RFID card (value) to open door
	     	if(state == COMPARE)
		  	{
				// if has ID of RFID card in system then door will open and save door open time
				if(CheckRFIDCard(value) == 1) // check ID of RFID card (value) success (has ID of RFID card in system)
				{
			    	inputMemTem(value); // save door open time by RFID card
					savedata_mmc(3); // save door open time to sd/mmc
				 	printf("Door Open\n");
				 	opendoor(); // open door
				 	printf("Door Close\n"); 
			  	}
			  	else //check ID of RFID card (value) fail (not has ID of RFID card in system)
			  	{
			    	printf("Wrong Card\n");
					loadpic_mmc(415); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
					delay_ms(1000);
					loadpic_mmc(401); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
			  	}
		  	}
			// when state is save, program will save ID of RFID card (value) to system
	      	else if(state == SAVE)
		  	{
				// if save ID of RFID card (value) success then program will save current ID of RFID card to sd/mmc too
				if(inputMemRFID(value) == 1) // save ID of RFID card (value) success
				{
				 	savedata_mmc(2); // save current ID of RFID card to sd/mmc
				 	printf("ID card = %d\n",value);
				 	loadpic_mmc(416); // load picture from sd/mmc
				 	lcd_fill_picture(temppic); // show picture on lcd
				 	printf("Save RFID card OK!\n");
				 	delay_ms(1000);
				}
				else // save ID of RFID card (value) fail
				{
				    loadpic_mmc(417); // load picture from sd/mmc
				 	lcd_fill_picture(temppic); // show picture on lcd
					printf("This card already has been save\n");
					delay_ms(1000);
				}
				setStateConfig(); // back to menu
		  	}
			// when state is delete, program will delete ID of RFID card (value) from system
		  	else if( state == DELETE)
		  	{
				// if delete ID of RFID card (value) success then program will save current ID of RFID card to sd/mmc too
				if(deleteMemRFID(value) == 1) // delete ID of RFID card (value) success
				{
				 	savedata_mmc(2); // save current ID of RFID card to sd/mmc
				 	loadpic_mmc(420); // load picture from sd/mmc
				 	lcd_fill_picture(temppic); // show picture on lcd
				 	printf("Delete RFID card OK!\n");
				 	delay_ms(1000);
				}
				else // delete ID of RFID card (value) fail
				{
				  	loadpic_mmc(421); // load picture from sd/mmc
				 	lcd_fill_picture(temppic); // show picture on lcd
					printf("Not found\n");
					delay_ms(1000);
				}
				setStateConfig(); // back to menu
		  	}
	   	}
		// when state is change password, program will receive and change password to enter menu
		// and save new password to sd/mmc
	   	else if (state == CHANGEPASS)
		{
			loadpic_mmc(419); // load picture from sd/mmc
			lcd_fill_picture(temppic); // show picture on lcd
			receive_keypad(1); // receive data from keypad
			change_password(); // change password
			savedata_mmc(0); // save new password to sd/mmc
			loadpic_mmc(416); // load picture from sd/mmc
			lcd_fill_picture(temppic); // show picture on lcd
			delay_ms(1000);
			setStateConfig(); // back to menu
		}
		// when state is set time, program will receive and change date and time
		else if(state == SETTIME)
		{
		     settime(); // receive and change date and time
			 loadpic_mmc(416); // load picture from sd/mmc
			 lcd_fill_picture(temppic); // show picture on lcd
			 delay_ms(1000);
			 setStateConfig(); // back to menu
		}
		// when state is add password, program will receive and add password user and save current password user to sd/mmc
		else if(state == ADDPASSWORD)
		{
			 if(receive_keypad(3) != 0) // receive data from keypad success
			 {
			 	if(addPasswordUser() == 1) // add password user success
				{
			 		savedata_mmc(1); // save current password user to sd/mmc
					loadpic_mmc(416); // load picture from sd/mmc
			 		lcd_fill_picture(temppic); // show picture on lcd
			 		printf("Save Password User OK!\n");
					delay_ms(1000);
				}
				else // add password user fail
				{
				 	loadpic_mmc(417); // load picture from sd/mmc
			 		lcd_fill_picture(temppic); // show picture on lcd
					printf("This user already has been save\n");
					delay_ms(1000);
				}
			 }
			 else // receive data from keypad fail
			 {
			  	printf("\nSave Password User Fail!\n");
			 }
			 setStateConfig(); // back to menu
      	}
		// when state is delete password, program will receive and delete password user
		// and save current password user to sd/mmc
		else if(state == DELETEPASSWORD)
		{
			 if(receive_keypad(3) != 0) // receive data from keypad success
			 {
			 	if(deletePasswordUser() == 1) // delete password user success
				{
			 		savedata_mmc(1); // save current password user to sd/mmc
					loadpic_mmc(420); // load picture from sd/mmc
			 		lcd_fill_picture(temppic); // show picture on lcd
			 		printf("Delete Password User OK!\n");
					delay_ms(1000);
				}
				else // delete password user fail
				{
					loadpic_mmc(421); // load picture from sd/mmc
			 		lcd_fill_picture(temppic); // show picture on lcd
				 	printf("Not Found\n");
					delay_ms(1000);
				}
			 }
			 else // receive data from keypad fail
			 {
			  	printf("\nDelete Password User Fail!\n");
			 }
			 setStateConfig(); // back to menu
      	}
		// program will check password user to open door
		else if((ch != '$')&&(ch != '*')&&(ch != '#')&&(state == COMPARE))
	   	{
	   		cleardata(); // clear tempkey
		 	tempkey[0] = ch;
			printf("%c",ch);
			if(receive_keypad(2) == 1) // receive data from keypad success
			{
				printf("\n");
				if(check_password(1) == 1) // check password user success (has password user in system)
			    {
					addPasswordUserTime(); // save door open time by password user
					savedata_mmc(4);
					printf("Door Open\n");
					opendoor();
					printf("Door Close\n"); 
			    }
			    else // check password user fail (not has password user in system)
 		    	{
					printf("Wrong Password User\n");
					delay_ms(1000);
					loadpic_mmc(415); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
					delay_ms(1000);
					loadpic_mmc(401); // load picture from sd/mmc
					lcd_fill_picture(temppic); // show picture on lcd
 		    	}
			}
	   	}
		// this is menu section can go by two way
		// 1) key * has been press in state compare
		// 2) back from other state
		else if((ch == '*' && state != SAVE && state !=DELETE)||(state == CONFIG))
		{
			// if back from other state then auto enter password
			if(state == CONFIG)
			{
				cleardata(); // clear tempkey
			 	for(j = 0;j < MAX_PASSWORD-1;j++) // auto enter password
					tempkey[j] = password[j];
			}
			// if come by key * then program will receive and check password
			else
			{
				printf("Enter Password :");
				loadpic_mmc(402); // load picture from sd/mmc
				lcd_fill_picture(temppic); // show picture on lcd
		   		receive_keypad(5);  // receive data from keypad
			}
		
		   	if(check_password(2) == 1) // check password success (password right)
			{
				// if state is not a config then change state to config
				if(state != CONFIG)
				{
		   			setStateConfig();
				}
		       
			   	if(receive_keypad(4) == 1) // receive menu success
			   	{
			   		if(tempkey[0] == '1' && tempkey[1] =='\0') // if select menu 1 then go to state change password
				   	{
						setStateChangepass(); // go to state change password
					}
					else if(tempkey[0] == '2' && tempkey[1] =='\0') // if select menu 2 then go to state set time
					{
						setStateSettime(); // go to state set time
					}
					else if(tempkey[0] == '3' && tempkey[1] =='\0') // if select menu 3 then go to state save
					{	
						setStateSave(); // go to state save
						timestate = (sec()+59)%60; // set time out
					}
					else if(tempkey[0] == '4' && tempkey[1] =='\0') // if select menu 4 then go to state delete
					{
						setStateDelete(); // go to state delete
						timestate = (sec()+59)%60; // set time out
					}
					else if(tempkey[0] == '5' && tempkey[1] =='\0')	// if select menu 5 then go to state add password
					{
						setStateAddPassword(); // go to state add password							 
					}		
					else if(tempkey[0] == '6' && tempkey[1] =='\0')	// if select menu 6 then go to state delete password
					{
						setStateDeletePassword(); // go to state delete password
					}
					else if(tempkey[0] == '7' && tempkey[1] =='\0')	// if select menu 7 then show ID of RFID card
																	// and save time
					{
						for(j = 0;j < MAX_RFID_CARD;j++) // show all ID of RFID card and save time
						{
						 	if(memoryRFID[j].datarfid != 0)
								printf("%d save at %d/%d/%d %d:%d:%d\n",memoryRFID[j].datarfid,memoryRFID[j].day
									,memoryRFID[j].month,memoryRFID[j].year,memoryRFID[j].hour
									,memoryRFID[j].minute,memoryRFID[j].sec);
						}
						
						setStateConfig(); // back to menu
					}
					else if(tempkey[0] == '8' && tempkey[1] == '\0') // if select menu 8 then show password user
																	 // and save time
					{		
						for(j = 0;j < MAX_NUMBER_USER;j++) // show all password user and save time
						{
							if(PassUser[j].passwordUser[0] != '\0')
								printf("%s save at %d/%d/%d %d:%d:%d\n",PassUser[j].passwordUser,PassUser[j].day
									,PassUser[j].month,PassUser[j].year,PassUser[j].hour
									,PassUser[j].minute,PassUser[j].sec);
						}
						setStateConfig(); // back to menu
					}
					else if(tempkey[0] == '9' && tempkey[1] =='\0')	// if select menu 9 then show door open time
																	// by RFID card and password user 
					{
						for(j = 0;j < MAX_RFID_CARD;j++) // show door open time by RFID card 
						{
							if(memoryTem[j].datarfid != 0)
								printf("%d stamp at %d/%d/%d %d:%d:%d\n",memoryTem[j].datarfid,memoryTem[j].day
									,memoryTem[j].month,memoryTem[j].year,memoryTem[j].hour
									,memoryTem[j].minute,memoryTem[j].sec);
						}
		
						for(j = 0;j < MAX_NUMBER_USER;j++) // show door open time by password user
						{
							if(PassUserTime[j].passwordUser[0] != '\0')
								printf("%s use at %d/%d/%d %d:%d:%d\n",PassUserTime[j].passwordUser,PassUserTime[j].day
									,PassUserTime[j].month,PassUserTime[j].year,PassUserTime[j].hour
									,PassUserTime[j].minute,PassUserTime[j].sec);
						}
					
						setStateConfig(); // back to menu
					}
					else if(tempkey[0] == '0' && tempkey[1] =='\0') // if select menu 0 then show date and time
					{
						printf("\n%d/%d/%d\n%d:%d:%d\n",day(),month(),year(),hour(),minute(),sec());
						setStateConfig(); // back to menu
					}
					else
					{
						setStateConfig(); // back to menu
					}
				}
			   	else // receive menu fail
				{
				  	setStateCompare(); // go to state compare	
				}
			}
			else  // check password fail (password wrong)
			{
				printf("Wrong Password\n");
				setStateCompare(); // go to state compare	
			}
		}
		// check time out of state save and delete
		else if(state == SAVE || state == DELETE)
		{
			// if time out then go back to menu
			if(sec()- timestate == 0)
			{
				setStateConfig(); // back to menu
			}
		}		
	}
}
