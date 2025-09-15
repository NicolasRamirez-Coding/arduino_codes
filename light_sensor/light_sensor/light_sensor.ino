#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>


#include <ThreeWire.h>
#include <RtcDS1302.h>

// Variables

int i;
String state;

// Dimensiones de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Dirección I2C del SSD1306 (comúnmente 0x3C)
#define OLED_ADDR 0x3C

// Pines

#define luz A1
int brillo;

ThreeWire myWire(1, 10, 0);
RtcDS1302<ThreeWire> Rtc(myWire);


// Crear objeto de pantalla
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servo-Motor
Servo alas;

String dosDigitos(int numero) {
  return (numero < 10) ? "0" + String(numero) : String(numero);
}

  String FechaActual() {
    RtcDateTime dt = Rtc.GetDateTime();

    String fechaHora = String(dt.Year()) + "/" +
                      dosDigitos(dt.Month()) + "/" +
                      dosDigitos(dt.Day()) + " " +
                      dosDigitos(dt.Hour()) + ":" +
                      dosDigitos(dt.Minute()) + ":" +
                      dosDigitos(dt.Second());

    return fechaHora;
  }


void setup() {
  pinMode(luz, INPUT);  
  alas.attach(9);


  // Inicializar pantalla
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("No se pudo encontrar la pantalla OLED"));
    for (;;); // Detener ejecución
  }

  Rtc.Begin();

  if (Rtc.GetIsWriteProtected()) {
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
  }
  //RtcDateTime setTime(2025, 9, 15, 16, 15, 0); // Año, mes, día, hora, minuto, segundo
  //Rtc.SetDateTime(setTime);
  
}

void loop() {
  brillo = map(analogRead(luz), 0, 1023, 0, 100);
  while(brillo >= 70){
    brillo = map(analogRead(luz), 0, 1023, 0, 100);
    display.clearDisplay();
    display.setTextSize(0.5); // Puedes cambiar el tamaño: 1, 2, 3...
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0); // Posición del texto en la pantalla
    display.println("Mariposa Chafa");
    display.display(); // Enviar a la pantalla


    display.setTextSize(0.5); // Puedes cambiar el tamaño: 1, 2, 3...
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10); // Posición del texto en la pantalla
    display.println(brillo);
    display.display(); // Enviar a la pantalla

    display.setTextSize(0.5);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println("ON");
    display.display();

    display.setTextSize(0.5); // Puedes cambiar el tamaño: 1, 2, 3...
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 30); // Posición del texto en la pantalla
    display.println(FechaActual());
    display.display(); // Enviar a la pantalla

  delay(1000);



    alas.write(10);
    delay(100);
    alas.write(75);
    delay(100);
  }
  alas.write(15);
    
  display.clearDisplay();
  display.setTextSize(0.5); // Puedes cambiar el tamaño: 1, 2, 3...
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0); // Posición del texto en la pantalla
  display.println("Mariposa Chafa");
  display.display(); // Enviar a la pantalla


  display.setTextSize(0.5); // Puedes cambiar el tamaño: 1, 2, 3...
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10); // Posición del texto en la pantalla
  display.println(brillo);
  display.display(); // Enviar a la pantalla

  display.setTextSize(0.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("OFF");
  display.display();

  display.setTextSize(0.5); // Puedes cambiar el tamaño: 1, 2, 3...
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 30); // Posición del texto en la pantalla
  display.println(FechaActual());
  display.display(); // Enviar a la pantalla

  delay(1000);

  delay(100);

}


/*

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


*/