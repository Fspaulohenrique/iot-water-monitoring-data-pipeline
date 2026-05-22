/*
   IoT Water Monitoring System
   - Flow sensor data collection
   - Real-time processing (L/min, m³, cost)
   - Pump control logic
   - Bluetooth interface (RemoteXY)
*/

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLUETOOTH

#include <BluetoothSerial.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "PIO-ENG-TCC"

#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 470 bytes
  { 255,2,0,16,0,207,1,19,0,0,0,0,202,1,106,200,2,1,0,8,
  0,129,6,12,98,9,64,16,84,67,67,45,32,69,110,103,46,67,111,109,
  112,117,116,97,195,167,195,163,111,0,129,9,36,89,8,64,16,65,116,105,
  118,97,195,167,195,163,111,32,68,111,115,32,77,111,116,111,114,101,115,32,
  0,2,7,86,89,18,1,12,24,31,31,76,105,103,97,100,97,0,68,101,
  115,108,105,103,97,100,97,0,2,8,137,89,18,1,12,24,16,31,76,105,
  103,97,100,97,0,68,101,115,108,105,103,97,100,97,0,129,7,68,86,12,
  64,16,73,82,82,73,71,65,195,135,195,131,79,32,240,159,146,166,0,129,
  24,120,59,12,64,16,67,72,85,86,65,32,240,159,146,167,0,129,252,48,
  148,10,64,31,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,
  95,95,95,95,95,95,95,95,95,95,95,95,95,95,0,131,32,171,40,14,
  3,8,16,31,80,97,103,101,0,6,11,0,129,18,7,68,12,64,16,76,
  105,116,114,111,115,32,112,47,32,77,105,110,0,67,19,22,68,15,77,16,
  26,3,67,19,65,68,15,77,16,26,3,67,19,102,68,15,77,16,26,3,
  67,18,142,68,15,77,16,26,3,129,14,49,80,12,64,16,84,111,116,97,
  108,32,101,109,32,76,105,116,114,111,115,0,129,9,92,90,8,64,16,84,
  111,116,97,108,32,101,109,32,77,101,116,114,111,115,32,67,117,98,105,99,
  111,115,0,129,45,130,12,10,64,16,82,36,0,129,255,114,140,10,64,16,
  95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,
  95,95,95,95,95,95,95,95,95,95,95,0,129,1,75,140,10,64,16,95,
  95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,
  95,95,95,95,95,95,95,95,95,95,0,129,248,36,140,10,64,16,95,95,
  95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,
  95,95,95,95,95,95,95,95,95,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t bomba_03; // =1 if switch ON and =0 if OFF
  uint8_t bomba_04; // =1 if switch ON and =0 if OFF

    // output variables
  float value_01;
  float value_02;
  float value_03;
  float value_04;

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

// Configurações do sensor de fluxo de água
const int flowSensorPin = 15; // Pino digital para o sensor de fluxo
volatile int pulseCount = 0;
unsigned long previousMillis = 0;
const float calibrationFactor = 7.5;
const float tarifaPorMetroCubico = 15.14;

float fluxoLitrosPorMinuto = 0.0;
float totalLitros = 0.0;
float totalMetrosCubicos = 0.0;
float valorTotal = 0.0;

// Definição das pinagens das boias e bombas
int boia1Pin = 13;
int boia2Pin = 12;
int boia3Pin = 14;
int boia4Pin = 27;

int IN1 = 26; 
int IN2 = 25;
int IN3 = 33; 
int IN4 = 32;
int IN5 = 23; 
int IN6 = 22;
int IN7 = 19;
int IN8 = 18;

void setup() 
{
  RemoteXY_Init(); 
  Serial.begin(9600);
  pinMode(flowSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), contarPulso, RISING);
  
  // Configuração dos pinos das boias como entradas
  pinMode(boia1Pin, INPUT);
  pinMode(boia2Pin, INPUT);
  pinMode(boia3Pin, INPUT);
  pinMode(boia4Pin, INPUT); 
  
  // Configuração dos pinos das bombas como saídas
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
}

void loop() 
{ 
  RemoteXY_Handler();

  unsigned long currentMillis = millis();

  // Calcular fluxo de água a cada segundo (1000 ms)
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

    // Calcular fluxo em litros por minuto
    fluxoLitrosPorMinuto = (pulseCount / calibrationFactor);
    totalLitros += (fluxoLitrosPorMinuto / 60.0);
    totalMetrosCubicos = totalLitros / 1000.0;
    valorTotal = totalMetrosCubicos * tarifaPorMetroCubico;

    // Atribuir os valores na interface RemoteXY
    RemoteXY.value_01 = fluxoLitrosPorMinuto;
    RemoteXY.value_02 = totalLitros;
    RemoteXY.value_03 = totalMetrosCubicos;
    RemoteXY.value_04 = valorTotal;

    pulseCount = 0;
  }

  // Controle das boias e bombas
  int boia1 = digitalRead(boia1Pin);
  int boia2 = digitalRead(boia2Pin);
  int boia3 = digitalRead(boia3Pin);
  int boia4 = digitalRead(boia4Pin);

  if (boia1 == LOW && boia2 == HIGH) { 
    digitalWrite(IN1, HIGH);  
    digitalWrite(IN2, LOW); 
  } else { 
    digitalWrite(IN1, LOW);  
    digitalWrite(IN2, LOW);
  }

  if (boia3 == LOW && boia4 == LOW) { 
    digitalWrite(IN3, HIGH);  
    digitalWrite(IN4, LOW); 
  } else { 
    digitalWrite(IN3, LOW);  
    digitalWrite(IN4, LOW);
  }

  // Controle da Irrigação
  if (RemoteXY.bomba_03 == 1) {
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, LOW);
  } else {
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, LOW);
  }

  // Controle da SABESP
  if (RemoteXY.bomba_04 == 1) {
    digitalWrite(IN7, HIGH);
    digitalWrite(IN8, LOW);
  } else {
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, LOW);
  }
}

void contarPulso() {
  pulseCount++;
}

