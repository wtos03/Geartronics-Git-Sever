#include "LPC213x.h"

void led0(int on){
     IODIR0 |= (1<<0);
	 if(on == 0){
		IOPIN0 = 0;
	 }
	 else{
	    IOPIN0 = 1;
	 }
}

void led1(int on){
     if(on == 0){
	    IOPIN0 = (0<<1);
	 }
	 else{
	    IOPIN0 = (1<<1);
	 }
}

void led2(int on){
     if(on == 0){
	    IOPIN0 = (0<<2);
	 }
	 else{
	    IOPIN0 = (1<<2);
	 }
}

void led3(int on){
     if(on == 0){
	    IOPIN0 = (0<<3);
	 }
	 else{
	    IOPIN0 = (1<<3);
	 }
}

void led4(int on){
     if(on == 0){
	    IOPIN0 = (0<<4);
	 }
	 else{
	    IOPIN0 = (1<<4);
	 }
}