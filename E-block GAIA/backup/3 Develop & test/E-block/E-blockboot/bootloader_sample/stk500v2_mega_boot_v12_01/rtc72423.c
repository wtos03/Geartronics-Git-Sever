/*************************************************************************
**  STK500 Protocol v2 Bootloader/monitor for ATmega128,AT90CAN128
**  clock functions for MB-128-XXX modules
**  (c) Milosz Klosowicz, MikloBit
**
**  Released under GNU GENERAL PUBLIC LICENSE
**  See gpl.txt
**
**  contact: 	support@miklobit.com
**  homepage:	www.miklobit.com
**
**************************************************************************/

struct date
{
    uint8_t year10 ;
    uint8_t year ;
    uint8_t month10 ;
    uint8_t month ;
    uint8_t day10 ;
    uint8_t day ;
    uint8_t week ;
};

struct time
{
    uint8_t hour10 ;
    uint8_t hour ;
    uint8_t min10 ;
    uint8_t min ;
    uint8_t sec10 ;
    uint8_t sec ;
};



uint8_t *rtc = 0x6000 ;  // clock address base with default GAL config


/* set date and time */

void set_clock( struct date *d, struct time *t )
{
  rtc[15]  = 7 ;  /* init clock */
  if( d ) 
  {
      rtc[12] = d->week ;
      rtc[11] = d->year10 ;
      rtc[10] = d->year ;
      rtc[9] = d->month10 ;
      rtc[8] = d->month ;
      rtc[7] = d->day10 ;
      rtc[6] = d->day ;
  }
  if( t )
  {
      rtc[5] = t->hour10 ;
      rtc[4] = t->hour ;
      rtc[3] = t->min10 ;
      rtc[2] = t->min ;
      rtc[1] = t->sec10 ;
      rtc[0] = t->sec ;
  };
  rtc[13] = 0 ; /* start clock */
  rtc[15] = 4 ;
}

/* read time HH:MM:SS */

struct time *get_time( struct time *t )
{
 t->hour10 = rtc[5] & 0x0f ;
 t->hour = rtc[4] & 0x0f ;
 t->min10 = rtc[3] & 0x0f ;
 t->min = rtc[2] & 0x0f ;
 t->sec10 = rtc[1] & 0x0f ;
 t->sec = rtc[0] & 0x0f ;
 return( t ) ;
}

/* odczytanie daty YY:MM:DD */

struct date *get_date( struct date *d )
{
 d->week =  rtc[12] & 0x0f ;
 d->year10 = rtc[11] & 0x0f ;
 d->year = rtc[10] & 0x0f ;
 d->month10 = rtc[9] & 0x0f ;
 d->month = rtc[8] & 0x0f ;
 d->day10 = rtc[7] & 0x0f ;
 d->day = rtc[6] & 0x0f ;
 return( d );
}


/* convert time HH:MM:SS  to string */

uint8_t *time2str(struct time *t ,  uint8_t *str )
{
  str[0] = ( t->hour10 + '0');
  str[1] = ( t->hour + '0');
  str[2] = (':');
  str[3] = ( t->min10 + '0');
  str[4] = ( t->min + '0');
  str[5] = (':');
  str[6] = (t->sec10 + '0');
  str[7] = (t->sec + '0');
  str[8] = 0 ;
  return str;
}


/* convert date YY:MM:DD  to string  */

uint8_t *date2str(struct date *d ,  uint8_t *str )
{

  str[0] = ( d->year10 + '0');
  str[1] = ( d->year + '0');
  str[2] = ('/');
  str[3] = ( d->month10 + '0');
  str[4] = ( d->month + '0');
  str[5] = ('/');
  str[6] = (d->day10 + '0');
  str[7] = (d->day + '0');
  str[8] = 0 ;
  return str;
}

