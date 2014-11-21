#include "LPC213x.h"
#include "settime.h"

// initial real time clock 
void initTime(){
	PREINT = 914; // set RTC prescaler
	PREFRAC = 17280;
	CCR = 0x00000001; // start the RTC
}

// send second
int sec(){
    return SEC;
}

// send minute
int minute(){
	return MIN;
}

// send hour
int hour(){
    return HOUR;
}

// send day
int day(){
	return DOM;
}

// send month 
int month(){
  	return MONTH;
}

// send year
int year(){
 	return YEAR;
}

// receive new minute and change it
void setminute(int inmin){
    MIN = inmin; 
}

// receive new hour and change it
void sethour(int inhour){
    HOUR = inhour;
}

// receive new second and change it
void setsec(int insec){
    SEC = insec;
}

// receive new day and change it
void setday(int inday){
 	DOM = inday;
}

// receive new month and change it
void setmonth(int inmonth){
 	MONTH = inmonth;
}

// receive new year and change it
void setyear(int inyear){
	YEAR = inyear;
}