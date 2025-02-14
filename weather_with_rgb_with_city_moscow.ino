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
  if (temp == 0) {
    // При 0°C устанавливаем зелёный цвет
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Зелёный цвет (R=0, G=255, B=0)
  } else if (temp > 0 && temp <= 13) {
    // От 0°C до 13°C: плавный переход от зелёного к жёлтому
    float ratio = (float)(temp - 0) / (13 - 0); // Нормализация в диапазоне 0-13
    uint8_t red = map(ratio * 100, 0, 100, 0, 255); // Зелёный -> Жёлтый (R увеличивается)
    uint8_t green = 255; // Зелёный остаётся максимальным
    uint8_t blue = 0; // Синий отсутствует
    pixels.setPixelColor(0, pixels.Color(red, green, blue));
  } else if (temp > 13 && temp <= 25) {
    // От 13°C до 25°C: плавный переход от жёлтого к красному
    float ratio = (float)(temp - 13) / (25 - 13); // Нормализация в диапазоне 13-25
    uint8_t red = 255; // Красный остаётся максимальным
    uint8_t green = map(ratio * 100, 0, 100, 255, 0); // Зелёный уменьшается
    uint8_t blue = 0; // Синий отсутствует
    pixels.setPixelColor(0, pixels.Color(red, green, blue));
  } else if (temp < 0 && temp >= -13) {
    // От -13°C до 0°C: плавный переход от голубого к зелёному
    float ratio = (float)(temp - (-13)) / (0 - (-13)); // Нормализация в диапазоне -13-0
    uint8_t red = 0; // Красный отсутствует
    uint8_t green = map(ratio * 100, 0, 100, 255, 255); // Зелёный остаётся максимальным
    uint8_t blue = map(ratio * 100, 0, 100, 255, 0); // Синий уменьшается
    pixels.setPixelColor(0, pixels.Color(red, green, blue));
  } else if (temp < -13 && temp >= -25) {
    // От -25°C до -13°C: плавный переход от синего к голубому
    float ratio = (float)(temp - (-25)) / (-13 - (-25)); // Нормализация в диапазоне -25--13
    uint8_t red = 0; // Красный отсутствует
    uint8_t green = map(ratio * 100, 0, 100, 0, 255); // Зелёный увеличивается
    uint8_t blue = 255; // Синий остаётся максимальным
    pixels.setPixelColor(0, pixels.Color(red, green, blue));
  }
  pixels.show();
}
