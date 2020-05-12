

//Led ON/OF para saber que nuestro Arduino está prendido
#define status_led 13

//Se define el periodo para mostrar la temperatura
#define periodo 3

//Libreria para usar las interrupciones 

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


float sensor = 0;       //declaramos las variables que vamos a usar
float temperatura;
int contador = 0;
float promedio = 0;
float sumador = 0;


void setup() {
  // put your setup code here, to run once:
  
  //Se define el led indicador como salida y se activa
  pinMode(status_led, OUTPUT);
  digitalWrite(status_led, HIGH);
  
  //Se inicializa el puerto de comunicacion serie
  Serial.begin(9600);

  //Indicamos el uso de referencia interna de 1.1 V
  analogReference(INTERNAL);

  

  //Se inicializa el LCD
  lcd.init(); 
lcd.backlight();
  
  
  

}


void loop() {
  if(contador <= periodo){            
    temperatura = (analogRead(A0)*1.1*100)/1023;              //leemos el valor analogico 
    sumador = temperatura + sumador;
    promedio = sumador/periodo; 
    contador += 1;
    
    
  }
   if(contador >= periodo){                                 //imprimimos lo deseado en la LCD
      lcd.setCursor(3, 0);
      lcd.print("Sensor LM35");   
      lcd.setCursor(5, 1);
      lcd.print(promedio, 1);
      lcd.print("C");
       contador = sumador = promedio = 0;
      delay(3000);
    }// put your main code here, to run repeatedly:
  }


  
