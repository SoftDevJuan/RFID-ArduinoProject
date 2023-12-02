#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialEsp(2, 3);

#define RST_PIN     9
#define SS_PIN      8
#define ledVerde    4
#define ledRojo     5
#define buzzerPin   6

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo servo;

//----------------------------------------

void setup() {
  Serial.begin(9600);
  SerialEsp.begin(9600); // Inicializa la comunicación con el ESP
  while (!Serial); // Espera hasta que el puerto serie esté disponible
  SPI.begin();
  mfrc522.PCD_Init();
  servo.attach(10);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  
}

//---------------------------------------------------

void loop() {
  servo.write(0);
  
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  tone(buzzerPin, 2000);  // Enciende el buzzer a una frecuencia de 2000 Hz
  delay(500);
  noTone(buzzerPin);

  // Imprimir directamente el ID del RFID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
  }
  Serial.println();

  // Crear el JSON con el UID y enviarlo al ESP
  String jsonString = "{\"rfid\":\"";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    jsonString += String(mfrc522.uid.uidByte[i], HEX);
  }
  jsonString += "\"}";

  // Imprimir el JSON
  Serial.println(jsonString);

  // Añadir un breve retardo después de imprimir el JSON
  delay(50);


  // Esperar la respuesta del ESP
  unsigned long startTime = millis();
  while (!Serial.available() && millis() - startTime < 5000);

   while (Serial.available() > 0) {
    String response = Serial.readStringUntil('\n');

    // Verificar si la cadena contiene "Permitido" o "Denegado"
    if (response.indexOf("Permitido") != -1) {
      Serial.println(response);
      digitalWrite(ledVerde, HIGH);
      tone(buzzerPin, 3000);  // Enciende el buzzer a una frecuencia de 1000 Hz
      delay(3000);
        noTone(buzzerPin); 
        servo.write(90);
        delay(1000);
        digitalWrite(ledVerde, LOW);
        
      
    } else if (response.indexOf("Denegado") != -1) {
      Serial.println(response);
      digitalWrite(ledRojo, HIGH);
      tone(buzzerPin, 1000);  // Enciende el buzzer a una frecuencia de 1000 Hz
      delay(3000);
       noTone(buzzerPin);
       digitalWrite(ledRojo, LOW);
      
    } else {
      Serial.println("Invalid");
    }
  }
    
}
