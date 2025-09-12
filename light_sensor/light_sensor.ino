#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Dimensiones de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Dirección I2C del SSD1306 (comúnmente 0x3C)
#define OLED_ADDR 0x3C

// Pines

#define luz A1
int brillo;
// Crear objeto de pantalla
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  pinMode(luz, INPUT);
  Serial.begin(9600);


  // Inicializar pantalla
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("No se pudo encontrar la pantalla OLED"));
    for (;;); // Detener ejecución
  }
  
}

void loop() {
  brillo = map(analogRead(luz), 0, 1023, 0, 100);

  display.clearDisplay();
  display.setTextSize(1); // Puedes cambiar el tamaño: 1, 2, 3...
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0); // Posición del texto en la pantalla
  display.println("Mariposa Chafa");
  display.display(); // Enviar a la pantalla


  display.setTextSize(1); // Puedes cambiar el tamaño: 1, 2, 3...
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10); // Posición del texto en la pantalla
  display.println(brillo);
  display.display(); // Enviar a la pantalla

  Serial.println(brillo);

  delay(2000);
}