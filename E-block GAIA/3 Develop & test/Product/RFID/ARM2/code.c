#include "LPC213x.H" 	                               
#include <stdio.h>
#include <keypad.h>

//define
#define MAX_PASSWORD 9

//word from keypad
char tempkey[MAX_PASSWORD];

// password
char password[MAX_PASSWORD];

//function
int receive_keypad();
int check_password();
void change_password();


int main()
{
	return 0;
}

int receive_keypad()
{
	char key = '$';
	int i = 0;
	int minute_starts;
	minute_starts = minute();
	do
	{
		key = keypad();
	 	if(key != '$')
		{
			if((key == '*')||(i == MAX_PASSWORD-1))
			{
			  	tempkey[i] = '\0';
				key = '*';
			}
			else if(key == '#')
			{
			 	i--;
			}
			else
			{
				tempkey[i] = key;
			}
			i++;
		}
		if((minute()-minute_starts) == 1)
			return 0;
	}
 	while(key != '*');

	return 1;
}


int check_password()
{
	int i;
 	for(i = 0;i < MAX_PASSWORD;i++)
	{
		if(tempkey[i] != password[i])	
	 		return 0; 
	}
	return 1;
}

void change_password()
{
	int i;
 	for(i = 0;i < MAX_PASSWORD;i++)
	{
		 password[i] = tempkey[i];
	}
}


