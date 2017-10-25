#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BT(3, 2);
const int smokeSensor = A0;
const int flameSensor = A2;
const int tempSensor = A4;
const int sensorMin = 0;
const int sensorMax = 1023;
const int buzzer = 9;
const int servoPin = 10;
const int barreraComida = A5;
float nextTemp = 0;
int contador = 0;
int flag = 0;
int flameReading;
int smokeReading;
float tempReading;
Servo servo;
void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  MsTimer2::set(1000, getTemperature);
  MsTimer2::start();
  servo.attach(servoPin);
  interrupts();
  BT.begin(9600);
  Serial.begin(9600);
  

}
void getTemperature() {
  Serial.println("Cuento");
  String paquete;
  contador++;
  if (contador % 5 == 0) {
    Serial.println("TEMPERATURA");
    nextTemp = analogRead(tempSensor) * (5.0 / 1023.0) * 100;
  }
  if (contador % 3 == 0) {
    Serial.println("comida");
    if (analogRead(barreraComida) < 950) {
      servo.write(50);
    } else {
      servo.write(-50);
    }
  }
  if (contador == 15) {
    contador = 0;
  }
  flag = 1;

  paquete = tempReading;
  if (smokeReading < 120) {
    paquete.concat("-No detectado");
  } else {
    paquete.concat("-Detectado");
    }
  if (flameReading > 500) {
      paquete.concat("-No detectado");
  } else {
    paquete.concat("-Detectado");
  }
  paquete.concat("-Alto");
  paquete.concat("-Bajo");
  Serial.println(paquete);
  BT.println(paquete);
  paquete = "";
  
}


void loop() {
  flameReading = analogRead(flameSensor);
  smokeReading = analogRead(smokeSensor);
  tempReading = analogRead(tempSensor) * (5.0 / 1023.0) * 100;
  Serial.print("Humo: ");
  Serial.print(smokeReading);
  Serial.println();
  Serial.print("Flama: ");
  Serial.print(flameReading);
  Serial.println();
  Serial.println();
  Serial.print("Temperatura diferencia: ");
  Serial.print(tempReading - nextTemp);
  Serial.println();
  if (flameReading <= 500 && smokeReading >= 120 && tempReading - nextTemp >= 5 ) {
    Serial.println("FUEGOOOO");
    digitalWrite(LED_BUILTIN, HIGH);
    //tone(buzzer, 1000);
  }
  delay(1000);


}
