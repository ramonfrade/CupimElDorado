#include <Arduino.h>
#include <PS4Controller.h>

// Motores Ponte H
#define IN1 2 // Motor esquerdo
#define IN2 3
#define IN3 4 // Motor direito
#define IN4 5

// Arma
#define ARMA_PIN 6 // Motor 1 da arma
#define ARMA_PIN2 7
#define ARMA_PIN3 8 // Motor 2 da arma
#define ARMA_PIN4 9

bool arma_ativa = false;

// Funçoes de movimento do robo

void parar()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void frente()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void tras()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void esquerda()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void direita()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Funçoes de movimentos na diagonal

void curvaFrenteDireita()
{
  digitalWrite(IN1, HIGH); // Motor esquerdo para frente
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); // Motor direito parado
  digitalWrite(IN4, LOW);
}

void curvaFrenteEsquerda()
{
  digitalWrite(IN1, LOW); // Motor esquerdo parado
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); // Motor direito para frente
  digitalWrite(IN4, LOW);
}

void curvaTrasDireita()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); // Motor esquerdo para trás
  digitalWrite(IN3, LOW);  // Motor direito parado
  digitalWrite(IN4, LOW);
}

void curvaTrasEsquerda()
{
  digitalWrite(IN1, LOW); // Motor esquerdo parado
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // Motor direito para trás
}

// Funçao da arma

void ativarArma(bool ativar)
{
  digitalWrite(ARMA_PIN, ativar ? HIGH : LOW);
  digitalWrite(ARMA_PIN2, ativar ? HIGH : LOW);
  digitalWrite(ARMA_PIN3, ativar ? HIGH : LOW);
  digitalWrite(ARMA_PIN4, ativar ? HIGH : LOW);
}

/*

void onPS4data()  // USAR ESSA FUNÇAO PARA MOVIMENTAR COM DPAD
{
  // =======================================================
  // ========= LÓGICA DE MOVIMENTO USANDO O D-PAD ==========
  // =======================================================

  // Checa primeiro as diagonais (quando dois botões são pressionados ao mesmo tempo)
  if (PS4.Up() && PS4.Right())
  {
    curvaFrenteDireita();
  }
  else if (PS4.Up() && PS4.Left())
  {
    curvaFrenteEsquerda();
  }
  else if (PS4.Down() && PS4.Right())
  {
    curvaTrasDireita();
  }
  else if (PS4.Down() && PS4.Left())
  {
    curvaTrasEsquerda();
  }

  // Se não for uma diagonal, checa as direções retas
  else if (PS4.Up())
  {
    frente();
  }
  else if (PS4.Down())
  {
    tras();
  }
  else if (PS4.Right())
  {
    direita();
  }
  else if (PS4.Left())
  {
    esquerda();
  }

  // Se nenhum botão de direção do D-pad estiver pressionado, para o robô
  else
  {
    parar();
  }

  if (PS4.Circle())
  {
    arma_ativa = !arma_ativa;
    ativarArma(arma_ativa);
    delay(300);
  }
}

*/

// Controle PS4

void onPS4data() // USAR ESSA FUNCAO PARA MOVIMENTAR COM ANALOGICO
{
  // Logica de movimento dentro de onPS4data()
  int lx = PS4.LStickX();
  int ly = PS4.LStickY();
  int deadzone = 30; // Zona morta do analógico

  if (ly < -deadzone && lx > deadzone)
  {
    curvaFrenteDireita();
  }
  else if (ly < -deadzone && lx < -deadzone)
  {
    curvaFrenteEsquerda();
  }
  else if (ly < -deadzone)
  {
    frente();
  }
  else if (ly > deadzone)
  {
    tras();
  }
  else if (lx > deadzone)
  {
    direita();
  }
  else if (lx < -deadzone)
  {
    esquerda();
  }
  else
  {
    parar();
  }

  if (PS4.Circle())
  {
    arma_ativa = !arma_ativa;
    ativarArma(arma_ativa);
    delay(300);
  }
}

void setup()
{
  Serial.begin(115200);

  // Pinos dos motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Pinos da arma
  pinMode(ARMA_PIN, OUTPUT);
  pinMode(ARMA_PIN2, OUTPUT);
  pinMode(ARMA_PIN3, OUTPUT);
  pinMode(ARMA_PIN4, OUTPUT);
  ativarArma(false);

  // Inicializar controle de PS4
  if (!PS4.begin("colocar codigo MAC aqui ex: 00:1A:7D:DA:71:13 "))
  {
    Serial.println("Erro ao iniciar Bluetooth do PS4");
    while (1)
      ;
  }

  PS4.setLed(0, 0, 255); // Define a cor para azul
  PS4.attach(onPS4data);
  Serial.println("Esperando controle PS4...");
}

void loop()
{
  // Tudo sendo controlado por callback
}