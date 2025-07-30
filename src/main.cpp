#include <Arduino.h>
#include <PS4Controller.h>

// Motores Ponte H
#define IN1 2 // Motor esquerdo
#define IN2 3
#define IN3 4 // Motor direito
#define IN4 5

// Arma
#define WEAPON_PIN 6 // Motor 1 da arma
#define WEAPON_PIN2 7
#define WEAPON_PIN3 8 // Motor 2 da arma
#define WEAPON_PIN4 9

bool isWeaponActive = false;
unsigned long weaponTimer = 0;

// Funçoes de movimento do robo

void stopRobot()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Funçoes de movimentos na diagonal

void curveForwardRight()
{
  digitalWrite(IN1, HIGH); // Motor esquerdo para frente
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); // Motor direito parado
  digitalWrite(IN4, LOW);
}

void curveForwardLeft()
{
  digitalWrite(IN1, LOW); // Motor esquerdo parado
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); // Motor direito para frente
  digitalWrite(IN4, LOW);
}

void curveBackwardRight()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); // Motor esquerdo para trás
  digitalWrite(IN3, LOW);  // Motor direito parado
  digitalWrite(IN4, LOW);
}

void curveBackwardLeft()
{
  digitalWrite(IN1, LOW); // Motor esquerdo parado
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // Motor direito para trás
}

// Funçao da arma

void updateWeaponState()
{
  digitalWrite(WEAPON_PIN, isWeaponActive ? HIGH : LOW);
  digitalWrite(WEAPON_PIN2, isWeaponActive ? HIGH : LOW);
  digitalWrite(WEAPON_PIN3, isWeaponActive ? HIGH : LOW);
  digitalWrite(WEAPON_PIN4, isWeaponActive ? HIGH : LOW);
}

/*

void onPS4data()  // USAR ESSA FUNÇAO PARA MOVIMENTAR COM DPAD
{
  // =======================================================
  // ========= LÓGICA DE MOVIMENTO USANDO O D-PAD ==========
  // =======================================================

  // Checa primeiro as diagonais (quando dois botões são pressionados ao mesmo tempo)
  if (PS4.moveForward() && PS4.turnRight())
  {
    curveForwardRight();
  }
  else if (PS4.moveForward() && PS4.turnLeft())
  {
    curveForwardLeft();
  }
  else if (PS4.moveBackward() && PS4.turnRight())
  {
    curveBackwardRight();
  }
  else if (PS4.moveBackward() && PS4.turnLeft())
  {
    curveBackwardLeft();
  }

  // Se não for uma diagonal, checa as direções retas
  else if (PS4.moveForward())
  {
    moveForward();
  }
  else if (PS4.moveBackward())
  {
    moveBackward();
  }
  else if (PS4.turnRight())
  {
    turnRight();
  }
  else if (PS4.turnLeft())
  {
    turnLeft();
  }

  // Se nenhum botão de direção do D-pad estiver pressionado, para o robô
  else
  {
    stopRobot();
  }

  if (PS4.Circle())
  {
    unsigned int newTimer = millis();
    if (newTimer - weaponTimer > 300)
    {
    isWeaponActive = !isWeaponActive;
    updateWeaponState();
    weaponTimer = millis();
    }
  }
}

*/

// Controle PS4

void onPS4data() // USAR ESSA FUNCAO PARA MOVIMENTAR COM ANALOGICO
{
  // Logica de movimento dentro de onPS4data()
  int lx = PS4.LStickX();
  int ly = PS4.LStickY();
  int deadzone = 60; // Zona morta do analógico

  if (ly < -deadzone && lx > deadzone)
  {
    curveForwardRight();
  }
  else if (ly < -deadzone && lx < -deadzone)
  {
    curveForwardLeft();
  }
  else if (ly > deadzone && lx > deadzone)
  {
    curveBackwardRight();
  }
  else if (ly > deadzone && lx < -deadzone)
  {
    curveBackwardLeft();
  }
  else if (ly < -deadzone)
  {
    moveForward();
  }
  else if (ly > deadzone)
  {
    moveBackward();
  }
  else if (lx > deadzone)
  {
    turnRight();
  }
  else if (lx < -deadzone)
  {
    turnLeft();
  }
  else
  {
    stopRobot();
  }

  if (PS4.Circle())
  {
    unsigned int newTimer = millis();
    if (newTimer - weaponTimer > 300)
    {
    isWeaponActive = !isWeaponActive;
    updateWeaponState();
    weaponTimer = millis();
    }
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
  pinMode(WEAPON_PIN, OUTPUT);
  pinMode(WEAPON_PIN2, OUTPUT);
  pinMode(WEAPON_PIN3, OUTPUT);
  pinMode(WEAPON_PIN4, OUTPUT);
  updateWeaponState();

  // Inicializar controle de PS4
  while (!PS4.begin("colocar codigo MAC aqui ex: 00:1A:7D:DA:71:13 "))
  {
    Serial.println("Erro ao iniciar Bluetooth do PS4");
    delay(3000);
  }

  PS4.setLed(0, 0, 255); // Define a cor para azul
  PS4.attach(onPS4data);
  Serial.println("Esperando controle PS4...");
}

void loop()
{
  // Tudo sendo controlado por callback
}
