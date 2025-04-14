
#include <TimerOne.h>  

// Definição dos pinos para os mosfet
const int MOSFET_A1 = 2;  // MOSFET A/1
const int MOSFET_A2 = 3;  // MOSFET A/2
const int MOSFET_B1 = 4;  // MOSFET B/1
const int MOSFET_B2 = 5;  // MOSFET B/2
const int MOSFET_C1 = 6;  // MOSFET C/1
const int MOSFET_C2 = 7;  // MOSFET C/2

// Definição dos pinos para os sensores hall
const int HALL_A = 8;  // sensor hall A
const int HALL_B = 9;  // sensor hall B
const int HALL_C = 10; // sensor hall C

const int potenciometro = A0;  // pino do potenciômetro
const int freioPin = 11;       // pino D11 para o freio
bool freioAtivado = false;

void setup() {
  // configuração dos pinos como saidas
  pinMode(MOSFET_A1, OUTPUT);
  pinMode(MOSFET_A2, OUTPUT);
  pinMode(MOSFET_B1, OUTPUT);
  pinMode(MOSFET_B2, OUTPUT);
  pinMode(MOSFET_C1, OUTPUT);
  pinMode(MOSFET_C2, OUTPUT);

  // Configuração dos sensores hall como entradas
  pinMode(HALL_A, INPUT);
  pinMode(HALL_B, INPUT);
  pinMode(HALL_C, INPUT);

  // Configuração do pino de freio como entrada
  pinMode(freioPin, INPUT);

  // Inicializa todos os mosfet como LOW (desligado)
  definirEstado(LOW, LOW, LOW, LOW, LOW, LOW);
}

void loop() {
  // Verifica o estado do freio
  if (digitalRead(freioPin) == HIGH) {
    freioMotor();  // Ativa o freio se o pino D11 estiver em high
  } else {
    freioAtivado = false;  // Desativa o freio se o pino D11 estiver em low

    // ler o valor do potenciometro
    int valorPotenciometro = analogRead(potenciometro);

    // Verifica se o potenciômetro está em zero
    if (valorPotenciometro == 0) {
      pararMotor();  // para o motor
    } else {
      int tempoComutacao = map(valorPotenciometro, 0, 1023, 5000, 500); // De 5 ms a 0.5 ms

      // ler os sensores hall
      int hallA = digitalRead(HALL_A);
      int hallB = digitalRead(HALL_B);
      int hallC = digitalRead(HALL_C);

      // Determinar o estado dos mosfet com base nos sensores hall
      comutaMotor(hallA, hallB, hallC);
    }
  }
}

// Função para definir o estado dos mosfet
void definirEstado(int a1, int a2, int b1, int b2, int c1, int c2) {
  digitalWrite(MOSFET_A1, a1);
  digitalWrite(MOSFET_A2, a2);
  digitalWrite(MOSFET_B1, b1);
  digitalWrite(MOSFET_B2, b2);
  digitalWrite(MOSFET_C1, c1);
  digitalWrite(MOSFET_C2, c2);
}

// Função para comutar o motor com base nos sensores hall
void comutaMotor(int hallA, int hallB, int hallC) {
  if (!freioAtivado) {  // só comuta se o freio não estiver ativado
    if (hallA == HIGH && hallB == LOW && hallC == LOW) {
      definirEstado(HIGH, LOW, LOW, HIGH, LOW, LOW);  // A1, B2 ativos
    } else if (hallA == HIGH && hallB == LOW && hallC == HIGH) {
      definirEstado(HIGH, LOW, LOW, LOW, LOW, HIGH);  // A1, C2 ativos
    } else if (hallA == LOW && hallB == HIGH && hallC == HIGH) {
      definirEstado(LOW, LOW, HIGH, LOW, LOW, HIGH);  // B1, C2 ativos
    } else if (hallA == LOW && hallB == HIGH && hallC == LOW) {
      definirEstado(LOW, HIGH, HIGH, LOW, LOW, LOW);  // B1, A2 ativos
    } else if (hallA == LOW && hallB == LOW && hallC == HIGH) {
      definirEstado(LOW, HIGH, LOW, LOW, HIGH, LOW);  // C1, A2 ativos
    } else if (hallA == LOW && hallB == LOW && hallC == LOW) {
      definirEstado(LOW, LOW, LOW, HIGH, HIGH, LOW);  // C1, B2 ativos
    }
  }
}

// Função que parar o motor
void pararMotor() {
  definirEstado(LOW, LOW, LOW, LOW, LOW, LOW);  // desliga todos os mosfet
}

// Função de freio do motor
void freioMotor() {
  if (!freioAtivado) {
    freioAtivado = true;
    definirEstado(LOW, LOW, LOW, LOW, LOW, LOW);  // desliga todos os mosfet
  }
}
