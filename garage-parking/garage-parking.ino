#include <Ultrasonic.h>
#include <NewTone.h>
 
//Define os pinos para o trigger e echo
#define pino_trigger 12
#define pino_echo 11

#define tempo 200 
#define pino_buzzer 2 
int frequencia = 2000;

#define pino_led 9
#define pino_ldr A1
#define pino_pot A2

int last_atraso = 0;
int current_atraso = 0;
bool debug = true;
 
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup()
{
  pinMode(pino_buzzer, OUTPUT);
  pinMode(pino_led, OUTPUT);

  if (debug) {
    Serial.begin(9600);
  }
}
 
void loop()
{
  int ldr = analogRead(pino_ldr);
  int pot = analogRead(pino_pot);

  if (debug) {
    Serial.print(ldr);
    Serial.print(" - ");
    Serial.println(pot);
  }

  delay(100);
  digitalWrite(pino_led, LOW);

  if (ldr < pot) {
    
    process();
  }
}

void process() {
  //Le as informacoes do sensor, em cm e pol
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  int stage = getStage(cmMsec);
  int atraso = getAtraso(stage);

  if (atraso != last_atraso) {

    if (atraso < last_atraso) {
      current_atraso = 0;
    } else {
      int dif_atraso = last_atraso - current_atraso;
      current_atraso = atraso - dif_atraso;
    }
    
    last_atraso = atraso;
  }

  if (current_atraso <= 0) {
    sinalizar();
    current_atraso = last_atraso;
  } else {
    current_atraso--;
  }

  if (debug) {
    Serial.print("Process: ");
    Serial.print(stage);
    Serial.print(" - ");
    Serial.print(cmMsec);
    Serial.print(" - ");
    Serial.println(current_atraso);
  }
}

void sinalizar() {
  digitalWrite(pino_led, HIGH);
  NewTone(pino_buzzer, frequencia, tempo);
}

int getStage(int cm) {
  int stage3 = 200;
  int stage2 = 30;
  int stage1 = 20;
  int stage0 = 10;

  if (cm < stage0) {
    return 0;
  }

  if (cm < stage1) {
    return 1;
  }

  if (cm < stage2) {
    return 2;
  }

  if (cm < stage3) {
    return 3;
  }
  
  return 4;
}

int getAtraso(int stage) {
  int m[] = {1, 2, 8, 24, 30};
  return m[stage];
}

