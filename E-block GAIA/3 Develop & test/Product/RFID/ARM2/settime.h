#ifndef __SETTIME_H__
#define __SETTIME_H__

extern void initTime();
extern int sec();
extern int minute();
extern int hour();
extern void setminute(int inmin);
extern void sethour(int inhour);
extern void setsec(int insec);

extern int day();
extern int month();
extern int year();
extern void setday(int inday);
extern void setmonth(int inmonth);
extern void setyear(int inyear);

#endif
