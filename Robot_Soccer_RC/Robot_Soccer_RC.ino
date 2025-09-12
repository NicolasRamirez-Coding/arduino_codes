#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include <Bluepad32.h>

Servo s1;
Servo s2;

// Pines de control de los motores

int PWMI = 13;
int PWMD = 14;

int MIA = 33;
int MIR = 32;
int MDA = 27;
int MDR = 12;

int v = 0;
int x = 0;

int ly = 0;
int rx = 0;

int va = 0;
int vr = 0;
int iz = 0;
int dr = 0;

int Va = 0;
int Vr = 0;
int Iz = 0;
int Dr = 0;

int mode = 0;
int limit;

#define LED_ESTRATEGIA 23
#define LED_MODO 4

Adafruit_NeoPixel pixelEstrategia(1, LED_ESTRATEGIA, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pixelModo(1, LED_MODO, NEO_RGB + NEO_KHZ800);


ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
      }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }
}

void processGamepad(ControllerPtr ctl) {

ly = ctl->axisY(), DEC;
rx = ctl->axisRX(), DEC;
x = ctl->buttons(), DEC;
mode = ctl->buttons() == 0x0020 ? 1 : 0; 


  if (x == 0x0008) {
    
    pixelModo.setPixelColor(0, pixelModo.Color(255, 0, 0));                // rojo
    pixelModo.show();
    pixelEstrategia.setPixelColor(0, pixelEstrategia.Color(255, 0, 0));                // rojo
    pixelEstrategia.show();
    s1.write(90);
    s2.write(4);
    delay(20);
  }

  if(mode == 1){
    pixelModo.setPixelColor(0, pixelModo.Color(255, 0, 0));                // Agresive Mode
    pixelModo.show();
    pixelEstrategia.setPixelColor(0, pixelEstrategia.Color(255, 0, 0));                
    pixelEstrategia.show();

    ctl->setColorLED(255, 0, 0);

    limit = 255;
  }
  else{
    pixelModo.setPixelColor(0, pixelModo.Color(0, 0, 255));                // Passive Mode
    pixelModo.show();
    pixelEstrategia.setPixelColor(0, pixelEstrategia.Color(0, 0, 255));                
    pixelEstrategia.show();

    ctl->setColorLED(0, 0, 255);

    limit = 100;
  }

}


void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      }
      else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

void setup() {
  // Configuración de los pines de los motores
pinMode (MIA,OUTPUT );
pinMode (MIR,OUTPUT );
pinMode (MDA,OUTPUT );
pinMode (MDR,OUTPUT );
pinMode (PWMI,OUTPUT );
pinMode (PWMD,OUTPUT );

ESP32PWM::allocateTimer(0);
ESP32PWM::allocateTimer(1);
ESP32PWM::allocateTimer(2);
ESP32PWM::allocateTimer(3);


  s1.setPeriodHertz(50);  // Standard 50hz servo
  s1.attach(21, 500, 2400);

  s2.setPeriodHertz(50);  // Standard 50hz servo
  s2.attach(17, 500, 2400);

Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);

    // Inicialización de Neopixels
  pixelEstrategia.begin(); pixelModo.begin();
  pixelEstrategia.setBrightness(100); pixelModo.setBrightness(100);
  pixelEstrategia.show(); pixelModo.show();

  // Animación al iniciar: morado por 3 segundos
  pixelEstrategia.setPixelColor(0, pixelEstrategia.Color(128, 0, 128));
  pixelModo.setPixelColor(0, pixelModo.Color(128, 0, 128));
  pixelEstrategia.show();
  pixelModo.show();
  delay(3000);
  pixelEstrategia.clear();
  pixelModo.clear();
  pixelEstrategia.show();
  pixelModo.show();

}


void loop() {


  // put your main code here, to run repeatedly:

digitalWrite(PWMI,HIGH);
digitalWrite(PWMD,HIGH);

  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();

va = constrain(ly, -511, -20); 
vr = constrain(ly, 20, 512); 
iz = constrain(rx, -511, -20); 
dr = constrain(rx, 20, 512); 

Va = map(va, -511, -20, limit, 0); 
Vr = map(vr, 20, 512, 0, limit); 
Iz = map(iz, -511, -20, (limit - 20), 0); 
Dr = map(dr, 20, 512, 0, (limit - 20));

if (ly>-20 && ly<20 && rx>-20 && rx<20){
Va=0;
Vr=0;
Iz=0;
Dr=0;}

if(Va<2 && Vr<2 && Iz<2 && Dr<2){
analogWrite(MIA,0); 
analogWrite(MIR,0);
analogWrite(MDA,0); 
analogWrite(MDR,0);}



if(Va>2){
analogWrite(MIA,Va); 
analogWrite(MIR,v);
analogWrite(MDA,Va); 
analogWrite(MDR,v);
}

if(Vr>2){
analogWrite(MIA,v); 
analogWrite(MIR,Vr);
analogWrite(MDA,v); 
analogWrite(MDR,Vr);}

if(Iz>2){
analogWrite(MIA,v); 
analogWrite(MIR,Iz);
analogWrite(MDA,Iz); 
analogWrite(MDR,v);}

if(Dr>2){
analogWrite(MIA,Dr); 
analogWrite(MIR,v);
analogWrite(MDA,v); 
analogWrite(MDR,Dr);}

Serial.print(Va  );
Serial.println(Iz );


}