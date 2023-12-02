#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Galaxy34";
const char* password = "11223344";
const char* serverAddress = "192.168.104.224";
const int serverPort = 4000;

void setup() {
  Serial.begin(9600);
  connectToWiFi();
}
//-----------------------------------------
void loop() {
   while (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');

    // Verificar si la cadena contiene "{"
    if (receivedData.indexOf("{") != -1) {
      sendPostRequest(receivedData);
      // Salir del bucle después de procesar la cadena con "{"
    }
  }
}

//---------------------------------------------------
void connectToWiFi() {
  //Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    //Serial.println("Connecting to WiFi...");
  }

  //Serial.println("Connected to WiFi");
}

//-----------------------------------------------------------
void sendPostRequest(String data) {
  //Serial.println("Sending POST request...");

  WiFiClient client;
  HTTPClient http;

  // Construir la URL del servidor
  String url = "http://" + String(serverAddress) + ":" + String(serverPort) + "/api/users";

  http.begin(client, url);

  // Configurar el encabezado del contenido
  http.addHeader("Content-Type", "application/json");

  // Enviar la solicitud POST con los datos recibidos del Arduino
  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    //Serial.print("POST request successful. Response code: ");
    //Serial.println(httpResponseCode);

    // Leer y analizar la respuesta JSON
    DynamicJsonDocument jsonDocument(1024); // Ajusta el tamaño según tus necesidades
    DeserializationError error = deserializeJson(jsonDocument, http.getString());

    if (error) {
      //Serial.print("JSON parsing failed. Error: ");
      //Serial.println(error.c_str());
    } else {
      // Extraer el valor de "AccesoRespuesta"
      const char* accesoRespuesta = jsonDocument[0]["AccesoRespuesta"];
      //Serial.print("Acceso Respuesta: ");
      Serial.println(accesoRespuesta);
      
    }
  } else {
    //Serial.print("POST request failed. Response code: ");
    //Serial.println(httpResponseCode);
  }

  http.end();
}
