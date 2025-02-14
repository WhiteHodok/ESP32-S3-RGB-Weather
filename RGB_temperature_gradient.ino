#include <Adafruit_NeoPixel.h>

#define PIN            48    // Пин, к которому подключен NeoPixel
#define NUMPIXELS      1     // Количество NeoPixel

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int minTemp = -25;
int maxTemp = 25;

void setup() {
  Serial.begin(115200);
  pixels.begin(); // Инициализация NeoPixel
  Serial.println("Введите температуру в диапазоне от -25 до 25:");
}

void loop() {
  if (Serial.available() > 0) {
    int temp = Serial.parseInt();
    if (temp >= minTemp && temp <= maxTemp) {
      setPixelColor(temp);
      Serial.print("Температура: ");
      Serial.println(temp);
    } else {
      Serial.println("Ошибка: Введите число от -25 до 25.");
    }
    Serial.read();
  }
}

void setPixelColor(int temp) {
  if (temp == 0) {
    // При 0°C устанавливаем зелёный цвет
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Зелёный цвет (R=0, G=255, B=0)
  } else {
    // Для других температур используем градиент
    float ratio = (float)(temp - minTemp) / (maxTemp - minTemp);

    uint8_t red = map(ratio * 100, 0, 100, 0, 255);
    uint8_t green = map(ratio * 100, 0, 100, 255, 0);
    uint8_t blue = 255 - red;

    pixels.setPixelColor(0, pixels.Color(red, green, blue));
  }
  pixels.show();
}