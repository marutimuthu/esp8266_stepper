
/*****************************************************
   Date: 30 Jan 2022
   Written by: Maruti M.
   Property off: Open
 * ***************************************************/
#include <ESP8266WiFi.h>
#include <WS2812FX.h>

#define LED_COUNT 1
#define LED_PIN D1

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define LIMIT_SWITCH D5
int switchState = 0;         // variable for reading the switch status

const char* ssid = "ALITER";
const char* password = "Aliter@256";

int Step = 0; //GPIO0---D3 of Nodemcu--Step of stepper motor driver
int Dir  = 2; //GPIO2---D4 of Nodemcu--Direction of stepper motor driver

bool homing_done = false;

WiFiServer server(80);

void setup() {
  // Initialise RGB
  ws2812fx.init();
  ws2812fx.setBrightness(100);
  ws2812fx.setSpeed(200);
  ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
  ws2812fx.start();

  Serial.begin(115200);
  delay(10);

  pinMode(LIMIT_SWITCH, INPUT_PULLUP); //Step pin as output

  pinMode(Step, OUTPUT); //Step pin as output
  pinMode(Dir,  OUTPUT); //Direcction pin as output
  digitalWrite(Step, LOW); // Currently no stepper motor movement
  digitalWrite(Dir, LOW);
  delay(100);
  move(true, 2000, 1);
  delay(2000);
}

void loop() {
  // Start RGB
  ws2812fx.service();
  move(false, 1800, 1);
  delay(5000);
  move(true, 1800, 1);
  delay(5000);
}

void move(bool dir, int steps, int stepInterval)
{
  // dir -> true:forward, false:reversep
  // steps -> (200 steps for one rotation)
  // stepInterval -> Speed Setting // 800ms ideal

  digitalWrite(Dir, dir); //Rotate stepper motor in anti clock wise direction
  for (int i = 1; i <= steps; i++) {
    digitalWrite(Step, HIGH);
    delay(stepInterval);
    digitalWrite(Step, LOW);
    delay(stepInterval);
    if (digitalRead(LIMIT_SWITCH) == 0 && dir == true) {
      return;
    }
  }
}
