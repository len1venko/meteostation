#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "len1venko";        
const char* password = "And2005le_"; 

const char* host = "script.google.com";
String Google_ID = "AKfycbwITO_0SvhIaLxTheA4xfsQviVggtTiWpLljHP7yy3WWkpX46bmQp4vHbmZCfsHBuY";

WiFiClientSecure client;  // Используем защищенный клиент

String urlencode(String str) {
    String encoded = "";
    char c;
    char buf[4];
    
    for (int i = 0; i < str.length(); i++) {
        c = str.charAt(i);
        if (isalnum(c)) {
            encoded += c;
        } else {
            sprintf(buf, "%%%02X", c); // Преобразуем в HEX-код
            encoded += buf;
        }
    }
    
    return encoded;
}


void setup() {
    Serial.begin(115200);
    
    WiFi.begin(ssid, password);
    Serial.println("Подключение к Wi-Fi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nWi-Fi подключен!");
    Serial.print("IP адрес: ");
    Serial.println(WiFi.localIP());

    client.setInsecure();  // Отключаем проверку сертификата
}

void loop() {
    sendData();
    delay(10000);  // Отправлять данные каждые 10 секунд
}

void sendData() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi не подключен!");
        return;
    }

    Serial.println("Отправляем данные...");

    String temperature = "28.70";  
    String humidity = "5.50";
    String pressure = "750.25";
    String altitude = "108.83";
    String gasvalue = "190";

    String url = "/macros/s/" + Google_ID + "/exec?"
                 "temperature=" + urlencode(temperature) +
                 "&humidity=" + urlencode(humidity) +
                 "&pressure=" + urlencode(pressure) +
                 "&altitude=" + urlencode(altitude) +
                 "&gasvalue=" + urlencode(gasvalue);

    Serial.println("URL запроса: " + url);

    HTTPClient https;
    https.begin(client, "https://" + String(host) + url);  
    https.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);  // Разрешаем редиректы

    int httpCode = https.GET();

    if (httpCode > 0) {
        Serial.printf("Код ответа сервера: %d\n", httpCode);
        String payload = https.getString();
        Serial.println("Ответ сервера: " + payload);
    } else {
        Serial.printf("Ошибка HTTP-запроса: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
}
