/******************************************************************************
*																		      * 
*                                                                             * 
* File Name:   SPI_LCD.c                                                      *
* Autor: 		Johannes Holzer/Gregor Kirchner                               *
* Version: 	V1.00                                                             *
* Date: 		25/01/2017                                                    *
* Description: SPI_LCD_Ansteuerung						                      *
******************************************************************************/
#include "armv10_std.h"

/*________________________DEFINITIONEN__________________________*/
#define CS  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,12)))	// PB12	 Chip Select (Slave Select)
#define SCLK  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,13)))	// PB13	 Serial Clock
#define SDO  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,14)))	// PB14	 Serial Data Out (MOSI)
#define SDI  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,15)))	// PB15	 Serial Data In (MISO)

/*________________________FUNCTION-PROTOTYPES__________________________*/
void EADogm204_init(void);
void EADogm204_send(char ch);
void EADOGM204A_put_char(char t);
void EADogm204_on(void);

/******************************************************************************
*            U N T E R P R O G R A M M:    init_ports                         *
*                                                                             *
* Aufgabe:   Initialisiert          										  *
* Input:     keine                                                            *
* return:	  keine                                                           *
******************************************************************************/
void EADogm204_init()
{
	char temp;

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable clock for GPIOB
	temp = GPIOB->CRH; 					// aktuelle Konfiguration in temp Varibale sichern
	temp &= 0x0000FFFF; 				// Konfigurationbits für Leitungen PB15-PB12 löschen
	temp |= 0x80000000; // Leitung PB15 als Input Pull-Up/Pull-Down definieren
	temp |= 0x03330000; // Leitung PB14,PB13, PB12 als Output Push-pull definieren
	GPIOB->CRH = temp; // Konfiguration übernehmen

	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;	//Clock für SPI2 freigeben
	temp = SPI2->CR1;
	temp &= 0x00FC; 					//welche Bits muss ich 0 setzen?
	temp |= 0x00FC;						//Baudraute=f_clk/256, master configuration, [mit Register überprüfen] 
	SPI2->CR1 = temp;
}
/******************************************************************************
*            U N T E R P R O G R A M M:    EADogm204_send                     *
*                                                                             *
* Aufgabe:   Initialisiert          										  *
* Input:     keine                                                            *
* return:	  keine                                                           *
******************************************************************************/
void EADogm204_send(char ch)
{
	char TXE=0x0002;

	while(!(SPI2->SR & TXE)); //TXE-Buffer leer?
	SPI2->DR= (ch&0xFF);
}

/******************************************************************************
*            U N T E R P R O G R A M M:    EADogm204_on	                      *
*                                                                             *
* Aufgabe:   Initialisiert          										  *
* Input:     keine                                                            *
* return:	  keine                                                           *
******************************************************************************/
void EADogm204_on()
{
	
	char TXE=0x0002;

	while(!(SPI2->SR & TXE)); //TXE-Buffer leer?
	SPI2->DR= 0xF8;
	while(!(SPI2->SR & TXE)); //TXE-Buffer leer?
	SPI2->DR= 0xF0;
	while(!(SPI2->SR & TXE)); //TXE-Buffer leer?
	SPI2->DR= 0xFF;
}



/******************************************************************************
*            U N T E R P R O G R A M M:    EADOGM204-A_put_char               *
*                                                                             *
* Aufgabe:   Das Unterprogramm EADOGM204-A_put_char gibt das character c	  *
			 welches an das UP übergeben wird am Display aus. Dazu wird Char  *
			 in High- und Lownibble aufgeteilt und in ASCII-Code umbewandelt. *
			 Daraufhin wird überprüft ob der Tx-Buffer leer. Ist dies der 	  *
			 Fall, so wird das Character in das SPI_Data_Register geschrieben *
			 , welches den Inhalt daraufhin in den Tx-Buffer schreibt.        *
* Input:     Char c                                                           *
* return:	 keine                                                            *
******************************************************************************/

void EADOGM204A_put_char(char c)
{
	char nibbleh;	  	// Variable für die 4 höherwertigen Bits
	char nibblel;		// Variable für die 4 niederwertigen Bits
		
	nibbleh = c&0xf0; // um 4 nach rechts shiften um 4 höherwertigen Bits zu extrahieren
	nib2asc(&nibbleh);
	nibblel = (c<<4)&0xf0;
	nib2asc(&nibblel);

	while(!(SPI2->SR & 0x0002));
	SPI2->DR = 0x00FA;

	while(!(SPI2->SR & 0x0002));
	SPI2->DR = nibbleh;

	while(!(SPI2->SR & 0x0002));
	SPI2->DR = nibblel;
	
}
/************************************************************************/
/* 								MAIN function 							*/
/************************************************************************/
int main()
{
	char *t = "A";
	EADogm204_init();
	EADogm204_on();
	do{
//		EADogm204_send(0xAA);
		EADOGM204A_put_char(*t);
	}while(1);	
}
