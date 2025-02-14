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
