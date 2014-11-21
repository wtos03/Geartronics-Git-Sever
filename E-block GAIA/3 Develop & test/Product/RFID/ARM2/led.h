#include "LPC213x.h"

void led0(int on){
     int pinled0 = 24;
     IODIR1 |= (1<<pinled0);
	 if(on == 0){
		IOCLR1 = (1<<pinled0);
	 }
	 else{
	    IOSET1 = (1<<pinled0);
	 }
}

void led1(int on){
     int pinled1 = 25;
     IODIR1 |= (1<<pinled1);
     if(on == 0){
	    IOCLR1 = (1<<pinled1);
	 }
	 else{
	    IOSET1 = (1<<pinled1);
	 }
}

void led2(int on){
     int pinled2 = 26;
     IODIR1 |= (1<<pinled2);
     if(on == 0){
	     IOCLR1 = (1<<pinled2);
	 }
	 else{
	     IOSET1 = (1<<pinled2);
	 }
}

void led3(int on){
     int pinled3 = 27;
     IODIR1 |= (1<<pinled3);
     if(on == 0){
	    IOCLR1 = (1<<pinled3);
	 }
	 else{
	    IOSET1 = (1<<pinled3);
	 }
}

void led4(int on){
     int pinled4 = 28;
     IODIR1 |= (1<<pinled4);
     if(on == 0){
	    IOCLR1 = (1<<pinled4);
	 }
	 else{
	    IOSET1 = (1<<pinled4);
	 }
}

void led5(int on){
     int pinled5 = 29;
     IODIR1 |= (1<<pinled5);
     if(on == 0){
	    IOCLR1 = (1<<pinled5);
	 }
	 else{
	    IOSET1 = (1<<pinled5);
	 }
}

void led6(int on){
     int pinled5 = 30;
     IODIR1 |= (1<<pinled5);
     if(on == 0){
	    IOCLR1 = (1<<pinled5);
	 }
	 else{
	    IOSET1 = (1<<pinled5);
	 }
}