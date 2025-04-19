#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>  
#include "DHT.h"

#define DHTPIN 2   
DHT dht(DHTPIN, DHT22);
#define GAS_SENSOR_PIN A0

const char* ssid = "len1venko";        
const char* password = "And2005le_"; 

String temperature;
String humidity;
String pressure;
String altitude;
String gasvalue;

Adafruit_BMP280 bmp;  // Инициализация датчика
AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);
    dht.begin();

    Serial.println(F("BMP280 Forced Mode Test."));
    
    if (!bmp.begin(0x76)) {  // Адрес BMP280
        Serial.println("Не удалось найти BMP280, проверьте соединения!");
        while (1);
    }

    bmp.setSampling(Adafruit_BMP280::MODE_FORCED,  
                    Adafruit_BMP280::SAMPLING_X2,  
                    Adafruit_BMP280::SAMPLING_X16, 
                    Adafruit_BMP280::FILTER_X16,  
                    Adafruit_BMP280::STANDBY_MS_500);
  // Инициализация SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Ошибка монтирования SPIFFS");
    return;
  } else {
    Serial.println("SPIFFS успешно смонтирован");
  }

  // Перечисление файлов в SPIFFS
  Dir dir = SPIFFS.openDir("/");
  if (!dir.next()) {
    Serial.println("Нет файлов в SPIFFS");
    return;
  }

  Serial.println("Список файлов в SPIFFS:");
  do {
    Serial.print("Файл: ");
    Serial.println(dir.fileName());
  } while (dir.next());

 // Подключаемся к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Подключаемся к Wi-Fi...");
  }
  Serial.println("Подключение к Wi-Fi успешно!");
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());

  // Обработчик для загрузки главной страницы main.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/main.html", "text/html; charset=UTF-8");
  });
  
  // Обработчик для загрузки index.html
  server.on("/index", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html; charset=UTF-8");
  });

  // Обработчик для загрузки index.html
  server.on("/calendar", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/calendar.html", "text/html; charset=UTF-8");
  });

  server.on("/highstock.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/highstock.js", "application/javascript");
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/1.ico", "image/x-icon");
  });

  server.on("/pac.css", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/pac.css", "text/css");
  });

  // API для получения данных с датчика
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", temperature.c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", humidity.c_str());
  });

 server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", pressure.c_str());
  });

  server.on("/altitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", altitude.c_str());
  });

  server.on("/gasvalue", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", gasvalue.c_str());
    });
  
    
  server.begin();
  Serial.println("HTTP сервер запущен");
}

void sendDataToGoogleSheet() {
  WiFiClient client;
  const char* host = "script.google.com";
  const int httpsPort = 443;

  String url = "/macros/s/AKfycbz5DH-UVC3OJGBq_cwbqnHYcQ8yQrNXM3-5Eae46Lg5RiIN2RJkpU4L8D49dAnMRME5/exec";
  
  String jsonData = "{";
  jsonData += "\"temperature\":\"" + temperature +" °C" + "\",";
  jsonData += "\"humidity\":\"" + humidity +" %" + "\",";
  jsonData += "\"pressure\":\"" + pressure +" hPa" + "\",";
  jsonData += "\"altitude\":\"" + altitude +" m" + "\",";
  jsonData += "\"gasValue\":\"" + gasvalue +" ppm" + "\",";
  jsonData += "\"gasState\":\"" + String(gasvalue.toInt() > 400 ? "Небезпечний рівень!" : "Все в нормі!") + "\"";
  jsonData += "}";

  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();

  if (!httpsClient.connect(host, httpsPort)) {
    Serial.println("❌ Ошибка подключения к Google Script");
    return;
  }

  httpsClient.print(String("POST ") + url + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Content-Type: application/json\r\n" +
                    "Content-Length: " + jsonData.length() + "\r\n\r\n" +
                    jsonData);

  Serial.println("✅ Данные отправлены в Google Таблицу");
}


unsigned long lastSendTime = 0;

void loop() {
  bmp.takeForcedMeasurement();

  // Считываем данные с датчиков
  temperature = String(dht.readTemperature(), 2);
  humidity = String(dht.readHumidity(), 2);
  pressure = String(bmp.readPressure() / 133.322, 2);
  altitude = String(bmp.readAltitude(1013.25) * 10, 2);
  gasvalue = String(analogRead(GAS_SENSOR_PIN));

    // Каждые 10 секунд отправляем данные
  if (millis() - lastSendTime > 10000) {
    sendDataToGoogleSheet();
    lastSendTime = millis();
  }
  
  delay(2000);  // Обновляем данные раз в 2 секунды

}
