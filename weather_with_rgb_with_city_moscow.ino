#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN            48    // Пин, к которому подключен NeoPixel
#define NUMPIXELS      1     // Количество NeoPixel

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "ESP32_Connect";
const char* password = "ESP32S30";

int minTemp = -25;
int maxTemp = 25;

void setup() {
  Serial.begin(115200);
  pixels.begin(); // Инициализация NeoPixel

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
    delay(1000);
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { // Проверка статуса подключения
  
    HTTPClient http;
  
    http.begin("https://wttr.in/Moscow?format=%t"); // Указываем URL
    int httpCode = http.GET(); // Отправляем запрос
    
    if (httpCode > 0) { // Проверяем код ответа
  
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);

        int temperatureEnd = payload.indexOf("°C");  
 
        // Извлекаем температуру 
        String temperatureStr = payload.substring(0, temperatureEnd);    
        int temperature = temperatureStr.toInt();

        Serial.print("Температура: ");
        Serial.println(temperature);

        if (temperature >= minTemp && temperature <= maxTemp) {
          setPixelColor(temperature);
        } else {
          Serial.println("Температура вне диапазона для изменения цвета NeoPixel.");
        }
    }
    else {
      Serial.println("Error on HTTP request");
    }
      
    http.end(); // Освобождаем ресурсы
  }
  delay(60000); // Задержка перед следующим запросом (1 минута)
}

void setPixelColor(int temp) {
  float ratio = (float)(temp - minTemp) / (maxTemp - minTemp);

  uint8_t red = map(ratio * 100, 0, 100, 0, 255);
  uint8_t green = map(ratio * 100, 0, 100, 255, 0);
  uint8_t blue = 255 - red;

  pixels.setPixelColor(0, pixels.Color(red, green, blue));
  pixels.show();
}