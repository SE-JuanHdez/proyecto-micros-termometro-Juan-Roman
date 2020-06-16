#include <LiquidCrystal_I2C.h>    // importa libreria LiquidCrystal
LiquidCrystal_I2C lcd(0x27,16,2);
/////////////////////////////////////////////////////////////////////
int SENSOR;   // variable almacena valor leido de entrada analogica A0
float TEMPERATURA;  // valor de temperatura en grados centigrados
float SUMA;   // valor de la suma de las 5 lecturas de temperatura
int ledVerde=4;//Verde
int ledRojo=5;//Rojo
int buzzer=A2;//Bocina
/////////////////////////////////////////////////////////////////////
void setup() {
                     // Las entradas analógicas no requieren inicialización
                    
  
  Serial.begin(9600);//Se inicializa el puerto de comunicacion serie
  lcd.init(); //Se inicializa el LCD
  lcd.backlight();
  pinMode(ledVerde,OUTPUT);
  pinMode(ledRojo,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {
  SUMA = 0;         // valor inicial de SUMA en cero
  {      
  SENSOR = analogRead(A0);      // lectura de entrada analogica A0
  TEMPERATURA = ((SENSOR * 5000.0) / 1023) / 10;// lectura de entrada analogica A0
  SUMA = TEMPERATURA + SUMA;      // suma de cada lectura de temperatura
  delay(100);         // demora de medio seg. entre lecturas
  }
  ///////////
if (SUMA < 38){
    digitalWrite(ledVerde,HIGH);
    digitalWrite(ledRojo,LOW);
    digitalWrite(buzzer,LOW);
    
  }
  if (SUMA >= 38){
    digitalWrite(ledVerde,LOW);
    digitalWrite(ledRojo,HIGH);
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    delay(1000);
   
  }
  ////////////////
  lcd.setCursor(0, 0);        // cursor en primer fila y primer columna
  lcd.print("Temperatura: ");        // escribe Temp:
  lcd.setCursor(0, 1); 
  lcd.print(SUMA, 1);     // escribe valor promedio de 5 lecturas con
  lcd.print(" C");        // imprime C
}
