#define FOSC 8000000UL				// Enter correct clock frequency

/* Procedures */
void TWI_first_init(void);

/* Functions */
int TWI_write(unsigned char address, unsigned char *data, unsigned char bytes);
int TWI_read(unsigned char address, unsigned char *data, unsigned char bytes);


