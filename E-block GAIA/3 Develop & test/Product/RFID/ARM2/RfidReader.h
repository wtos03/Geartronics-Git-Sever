#ifndef __RFID_READER_H__
#define __RFID_READER_H__

extern void irq_EINT2(void)__irq;
extern void irq_EINT3(void)__irq;
extern void init_EINT2(void);
extern void init_EINT3(void);
extern void init_RFID(void);
extern int checkcorrectdata(void);
extern int dataRFID(void);
extern int readRFID(void);

#endif