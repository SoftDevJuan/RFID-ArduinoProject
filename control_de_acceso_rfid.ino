#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9           // configuramos el pin 9 para reset del mfrc522
#define SS_PIN          8         // configuramos el pin 53 para el selector de chip para el caso del Arduino Mega 
#define ledVerde        4           // led verde en el pin 4
#define ledRojo         5           // led rojo en el pin 5
#define buzzerPin       6           // pin para el buzzer


MFRC522 mfrc522(SS_PIN, RST_PIN);   //
MFRC522::MIFARE_Key key;

Servo servo1;

byte tag1[4] = {0xE3, 0x7A, 0x32, 0x95};
byte tag2[4] = {0x63, 0xA1, 0xC6, 0x91};
byte tag3[7] = {0x04, 0x3E, 0x33, 0x9A, 0x7E, 0x15, 0x90};
byte tag4[4] = {0x08, 0x8C, 0x00, 0xB1};

byte tagActual[4];

void setup() {
  Serial.begin(9600); // velocidad del puerto serie 
   while (!Serial);    // comprobar si ya ha iniciado 
   SPI.begin();        // inicia la comunicacion SPI
   mfrc522.PCD_Init(); //inicia la comunicacion con el modulo RFID
   servo1.attach(3);      // asigna el pin para el Servo 
   pinMode(ledVerde,OUTPUT);  // configuram como salida 
   pinMode(ledRojo,OUTPUT);
   pinMode(buzzerPin, OUTPUT); 
}

void loop() {
   if (!mfrc522.PICC_IsNewCardPresent())  // comprobar si esta presente la tarjeta 
        return;

    // Selecciona una tarjeta 
    if (!mfrc522.PICC_ReadCardSerial())       // leer la tarjeta 
        return;

    Serial.print(F("Card UID: "));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);   // obtiene los valores leídos de la tarjeta 
    Serial.println(); // Agrega un salto de línea para mayor claridad
    
    servo1.write(3);        // coloca el servo en la posición cero 
    digitalWrite(ledVerde, LOW);       // apaga led 
    digitalWrite(ledRojo, LOW);  
}

void dump_byte_array(byte *buffer, byte bufferSize) {      // realiza la lectura del codigo de la tarjeta y comprueba 
    for (byte i = 0; i < bufferSize; i++) {         // extrae valores del código 
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
      tagActual[i] = buffer[i];
    }
    Serial.println(); // Agrega un salto de línea para mayor claridad
    
    if (compararArray(tagActual, tag1, sizeof(tag1))) {              // comprobamos el acceso para el primer usuario 
        Serial.println(" Acceso Permitido para tarjeta blanca");
        digitalWrite(ledVerde, HIGH);
        tone(buzzerPin, 1000);  // Enciende el buzzer a una frecuencia de 1000 Hz
        delay(3000);
        noTone(buzzerPin); 
        servo1.write(90);
        delay(1000);
    } else if (compararArray(tagActual, tag2, sizeof(tag2))) {       // comprobamos el acceso para el segundo usuario 
        Serial.println(" Acceso Permitido para el llaver azul");
        digitalWrite(ledVerde, HIGH);
        tone(buzzerPin, 5000);  // Enciende el buzzer a una frecuencia de 1000 Hz
        delay(3000);
        noTone(buzzerPin); 
        servo1.write(90);
        delay(1000);
    } else if (compararArray(tagActual, tag3, sizeof(tag3))) {       // comprobamos el acceso para el tercer usuario 
        Serial.println(" Acceso Permitido para la tarjeta del tren de Juan Carlos");
        digitalWrite(ledVerde, HIGH);
        tone(buzzerPin, 500);  // Enciende el buzzer a una frecuencia de 1000 Hz
        delay(3000);
        noTone(buzzerPin); 
        servo1.write(90);
        delay(1000);
    }else if (compararArray(tagActual, tag4, sizeof(tag4))) {       // comprobamos el acceso para el tercer usuario 
        Serial.println(" Acceso Permitido para la tarjeta del tren de Juan Carlos");
        digitalWrite(ledVerde, HIGH);
        tone(buzzerPin, 1000);  // Enciende el buzzer a una frecuencia de 1000 Hz
        delay(1000);
        noTone(buzzerPin); 
        servo1.write(90);
        delay(1000);
    } else {
        Serial.println(" Desconocido");            // si el código no está registrado denegar acceso 
        digitalWrite(ledRojo, HIGH);
        delay(1000);
    }       
}

boolean compararArray(byte array1[], byte array2[], int size) {  // comprobará código leído con el registrado previamente
    for (int i = 0; i < size; i++) {
        if (array1[i] != array2[i]) return false;
    }
    return true;
}