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
void init_ports();


/******************************************************************************
*            U N T E R P R O G R A M M:    init_ports                         *
*                                                                             *
* Aufgabe:   Initialisiert          										  *
* Input:     keine                                                            *
* return:	  keine                                                           *
******************************************************************************/
void init_ports()
{
	int temp; // temp Varibale definieren

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable clock for GPIOB
	temp = GPIOB->CRH; // aktuelle Konfiguration in temp Varibale sichern
	temp = &= 0x0000FFFF; // Konfigurationbits für Leitungen PB15-PB8 löschen
	temp = |= 0x33330000; // Leitung PB15-PB8 als Push Pull Output definieren
	GPIOB->CRH = temp; // Konfiguration übernehmen
}

/************************************************************************/
/* 								MAIN function 							*/
/************************************************************************/
int main()
{
		
}