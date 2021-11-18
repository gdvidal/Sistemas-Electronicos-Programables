/*
 * lab_8.c
 *
 * Created: 22-10-2019 21:54:44
 * Author : Guillermo
 */ 

#define F_CPU	16000000UL

#include <avr/io.h>
 #include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#include "USART/USART_implement_me.h"
#include "SPI/SPI_implement_me.h"
#include "PetitFatFilesystem/PetitFS.h"

#include "display/ST7735_commands.h"
#include "display/graphic_shapes.h"
#include "display/testroutines.h"


/* A simple helper function to stop in case of some error */
void errorHalt(char* msg) {
	USART_Transmit_String("Error: ");
	USART_Transmit_String(msg);
	USART_Transmit_String("\n");

	while(1);
}

void read_files_sd()
{
	
	FATFS fs;     		/* File system structure */
	uint8_t buf[32];	/* Buffer for card access */
	UINT nr;
	
	// Initialise SD and file system.
	USART_Transmit_String("Trying to mount the SD card's file system: ");
	//if (pf_mount(&fs)) errorHalt("pf_mount");
	while (pf_mount(&fs));
	USART_Transmit_String("success.\n");
	
	// #1
	// Open read test file.
	USART_Transmit_String("Trying to open a text file for reading: ");
	if (pf_open("SEPtest.txt")) errorHalt("pf_open");
	USART_Transmit_String("success.\n");
	
	// Print test file to the serial interface.
	USART_Transmit_String("Here is the file content:\n");
	USART_Transmit_String("\n-----------------------------------------------\n");
	while (1) {
		if (pf_read(buf, sizeof(buf), &nr)) errorHalt("pf_read");
		if (nr == 0) break;
		for(uint8_t i=0; i<nr; i++)
		{
			USART_Transmit_char(buf[i]);
		}
	}
	
	USART_Transmit_String("\n-----------------------------------------------\n\n\n");
	
	// #2
	// Open write test file.
	USART_Transmit_String("Trying to open a text file for writing: ");
	if (pf_open("writeMe.txt")) errorHalt("pf_open");
	USART_Transmit_String("success.\n");
	
	// Set read/write pointer to beginning of file.
	USART_Transmit_String("Going to beginning of file: ");
	if (pf_lseek(0)) errorHalt("pf_lseek");
	USART_Transmit_String("success.\n");
	
	// Write data and finish the transaction.
	USART_Transmit_String("Writing some data to the file: ");
	char message[] = "If this message does not appear in your text file, you failed to read the PFF documentation. Read about the use of pf_write() there!";
	if (pf_write(message, strlen(message), &nr)) errorHalt("pf_write while writing the file content");
	if (pf_write(0, 0, &nr)) errorHalt("pf_write while closing the file.");
	USART_Transmit_String("success.\n");
	
	
	//#3
	// Open write test file.
	USART_Transmit_String("Trying to open another text file for writing more: ");
	if (pf_open("writeMe2.txt")) errorHalt("pf_open");
	USART_Transmit_String("success.\n");
	
	// Set read/write pointer to beginning of file.
	USART_Transmit_String("Going to beginning of file: ");
	if (pf_lseek(0)) errorHalt("pf_lseek");
	USART_Transmit_String("success.\n");
	
	
	USART_Transmit_String("Please type a short message and press enter. ");
	char msj[40];
	USART_Receive_String(msj, 40);
	
	USART_Transmit_String(msj);
	
	USART_Transmit_String("Writing message to the file: ");
	if (pf_write(msj, strlen(msj), &nr)) errorHalt("pf_write while writing the file content");
	if (pf_write(0, 0, &nr)) errorHalt("pf_write while closing the file.");
	USART_Transmit_String("success.\n");
	USART_Transmit_String("\n\n<---- The SD card test is done! ---->\n\n");
}


void Mostrar_Img(char* img){
	
	FATFS fs;     		/* File system structure */
	UINT nr;			/* Used for various file access functions. */
	
	//to mount sd card: slaves
	PORTB &= ~(1<<2); //card cs
	PORTB |= (1<<1); //tft 
	PORTB |= (1<<0); //dc

	USART_Transmit_String("Trying to mount the SD card's file system: ");
	while (pf_mount(&fs));
	USART_Transmit_String("success.\n");
	PORTD |= (1<<3); 
	
	
	
	if (pf_open(img)){
		errorHalt("error pf open.");
	}
	if (!(pf_open(img))){
		
		char off[1]; //buffer to read elements of offset
		char* offset = (char*)malloc(4 * sizeof(char)); //asignar 4 byte en memoria para el offset
		
		//read offset of the image (lee en byte) los 4 correspondientes al offset)
		//archivos del header
		//2 byte type: caracteres BM
		//4 byte tamaño total del archivo
		//4 byte: reservado 
		// = 10 byte -13 -> offset
		for(uint8_t j=13; j>9; j--){
			pf_lseek(j); //move pointer 
			pf_read(off, 1, &nr);
			strcpy(offset, off);
		}
		
		pf_lseek(*offset); //locate pixels array to read data
		free(offset); //free memory space asociated to offset
		
		char colors[480];  // buffer, 3x160
		
		for(uint8_t pixel_y=0; pixel_y<TFT_HEIGHT; pixel_y++)
		{
			// slave on to read image from card SD
			PORTB |= (1<<1); //tft
			PORTB &= ~(1<<2); //card cs
			
			if (pf_read(colors, sizeof(colors), &nr)) errorHalt("pf_read");
			for(uint8_t i=0; i<TFT_WIDTH; i++)
			{
				//secuency of colors in maps of bits
				uint8_t red=colors[3*i+2];       //2
				uint8_t green=colors[3*i+1];     //1
				uint8_t blue=colors[3*i];        //0
				uint8_t pixel_x=TFT_WIDTH-i;
				
				//to show image
				PORTB |= (1<<2); //card sd
				PORTB &= ~(1<<1); //tft
				drawPixelRGB(pixel_x, pixel_y, red, green, blue);
			}
		}
	}
	PORTD &= ~(1<<3);
}


int main(void)
{	
	// Initialises the serial communication interface
	struct USART_configuration config_57600_8N1 = {57600, 8, 0, 1};
	USART_Init(config_57600_8N1);
	
	// Hardware initialisation
	USART_Transmit_String("Initialising hardware... ");
	SPI_Master_Init();
	ST7735_init();
	USART_Transmit_String("success.\r\n");
	
	//show txt files
	USART_Transmit_String("Bienvenido a la prueba de lectura de tus archivos txt.\r\n");
	read_files_sd();
	
	//show bmp
	USART_Transmit_String("Iniciando tu animal espiritual\r\n");
	Mostrar_Img("cat.bmp");
	USART_Transmit_String("La imagen se ha cargado exitosamente en tu TFT!.\r\n");
	
}