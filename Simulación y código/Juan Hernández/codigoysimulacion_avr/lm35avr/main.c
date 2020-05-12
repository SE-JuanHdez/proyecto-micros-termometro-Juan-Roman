/*
 * lm35avr.c
 *
 * Created: 26/04/2020 02:38:47 p. m.
 * Author : Juan Manuel Hdez
 */ 

#define F_CPU 16000000UL	//FRECUENCIA DEL RELOJ CON QUE TRABAJA EL MICROCONTROLADOR
#include <avr/io.h>			//ARCHIVO DE DEFINICIONES DE REGISTROS Y BITS
#include <util/delay.h>		//EN CASO DE USAR LA FUNCION DELAY
#include <stdlib.h>			//biblioteca que contiene la función itoa
#define	LCD_DPRT  PORTD		//puerto de datos
#define	LCD_DDDR  DDRD		//datos ddr
#define	LCD_DPIN  PIND		//datos de los pines
#define	LCD_CPRT  PORTB		//comandos PORT
#define	LCD_CDDR  DDRB		//comandos DDR
#define	LCD_CPIN  PINB		//comandos PIN
#define	LCD_RS  0			//LCD RS
#define	LCD_RW  1			//LCD RW
#define	LCD_EN  2			//LCD EN

char Valor;

void lcdCommand( unsigned char cmnd )	//función de comando
{
	LCD_DPRT = cmnd;					//envia cmnd al puerto de datos
	LCD_CPRT &= ~ (1<<LCD_RS);			//RS = 0 para comando
	LCD_CPRT &= ~ (1<<LCD_RW);			//RW = 0 para escribir
	LCD_CPRT |= (1<<LCD_EN);			//EN = 1 para pulso de H a L
	_delay_us(1);
	LCD_CPRT &= ~ (1<<LCD_EN);			//EN = 1 para pulso de H a L
	_delay_us(100);
}
void lcdData( unsigned char data )		//función de datos
{
	LCD_DPRT = data;					//enviar los datos al puerto de datos
	LCD_CPRT |= (1<<LCD_RS);			//RS = 1 para datos
	LCD_CPRT &= ~ (1<<LCD_RW);			//RW = 0 para escribir
	LCD_CPRT |= (1<<LCD_EN);			//EN = 1 para pulso de H a L
	_delay_us(1);
	LCD_CPRT &= ~ (1<<LCD_EN);			//EN = 1 para pulso de H a L
	_delay_us(100);
}
void lcd_init()							//función para iniciar la LCD
{
	LCD_DDDR = 0xFF;
	LCD_CDDR = 0xFF;
	
	LCD_CPRT &=~(1<<LCD_EN);			//LCD_EN =0
	_delay_us(2000);					//esperar para inicar
	lcdCommand(0x38);					//iniciar LCD con 2 lineas, matriz de 5x7
	lcdCommand(0x0E);					//display encendido y cursor encendido
	lcdCommand(0x01);					//limpiar la lCD
	_delay_us(2000);					//esperar
	lcdCommand(0x06);					//cambiar el cursor
}
void lcd_gotoxy(unsigned char x, unsigned char y)	//función para el posicionamiento del cursor con base en la tabla del libro
{
	unsigned char firstCharAdr[]={0x80,0xC0,0x94,0xD4};//table 12-4
	lcdCommand(firstCharAdr[y-1] + x - 1);
	_delay_us(100);
}
void lcd_print( char * str )			//función para imprimir en la lCD
{
	unsigned char i = 0 ;
	while(str[i]!=0)					//mientras no termine la cadena
	{
		lcdData(str[i]);
		i++ ;
	}
}


int main(void)
{
	char numcad[5];
	DDRC = 0;		//puerto C como entrada para el ADC
    UBRR0=51;		//PARA CONFIGURAR LA VELOCIDAD
    ADMUX=0xc0;		//Vref a 1.1V, ADC0, justificado
    ADCSRA=0X87;	//Habilitar el ADC y prescalador ck/128
	lcd_init();		//inicializar la LCD
	
    while (1) 
    {
		ADCSRA|=(1<<ADSC);					//inciar conversión
		while((ADCSRA&(1<<ADIF))==0);		//esperar para el final de la conversión
		ADCSRA |= (1<<ADIF);				//limpiar la bandera ADIF
		Valor=(ADCL|(ADCH<<8))*10/93;		//valor = valor del ADC/ 9.3 
		itoa(Valor,numcad,10);				//convertimos valor en una cadena de texto para imprimirla por la LCD
		lcd_gotoxy(1,1);					//posición del cursos
		lcd_print("sensor lm35");			//imprimimos sensor lm35
		lcd_gotoxy(1,2);					//posición del cursor
		lcd_print(numcad);					//imprimimos la temperatura
		
    }
}

