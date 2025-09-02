#include <Arduino.h>
#include <PS4Controller.h>

// Motores Locomoçao
#define IN1 12 // Motor esquerdo
#define IN2 13
#define IN3 14 // Motor direito
#define IN4 27

// Arma
#define WEAPON_PIN 25
#define WEAPON_PIN2 26

#define FAILSAFE_LED 2 // Pino do Led

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
  digitalWrite(WEAPON_PIN2, isWeaponActive ? LOW : HIGH);
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

  if (ly > deadzone && lx > deadzone)
  {
    Serial.println("Analogico Direita Cima");
    curveForwardRight();
  }
  else if (ly > deadzone && lx < -deadzone)
  {
    Serial.println("Analogico Esquerda Cima");
    curveForwardLeft();
  }
  else if (ly < -deadzone && lx > deadzone)
  {
    Serial.println("Analogico Direita Tras");
    curveBackwardRight();
  }
  else if (ly < -deadzone && lx < -deadzone)
  {
    Serial.println("Analogico Esquerda Tras");
    curveBackwardLeft();
  }
  else if (ly > deadzone)
  {
    Serial.println("Analogico Frente");
    moveForward();
  }
  else if (ly < -deadzone)
  {
    Serial.println("Analogico Tras");
    moveBackward();
  }
  else if (lx > deadzone)
  {
    Serial.println("Analogico Direita");
    turnRight();
  }
  else if (lx < -deadzone)
  {
    Serial.println("Analogico Esquerda");
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
      Serial.println("Bolinha!!!");
      isWeaponActive = !isWeaponActive;
      updateWeaponState();
      weaponTimer = millis();
    }
  }
}

void setup()
{

  Serial.begin(115200);

  // Pinos Dos LEDs
  pinMode(FAILSAFE_LED, OUTPUT);
  digitalWrite(FAILSAFE_LED, LOW); // Começa apagado

  // Pinos dos motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Pinos da arma
  pinMode(WEAPON_PIN, OUTPUT);
  pinMode(WEAPON_PIN2, OUTPUT);

  updateWeaponState();

  // Inicializar controle de PS4
  if (PS4.begin("18:1d:ea:f4:17:5c"))
  {
    Serial.println("Controle conectado!");
  }

  while (!PS4.begin("18:1d:ea:f4:17:5c"))
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
  // Verifica se o controle ainda está conectado
  if (PS4.isConnected())
  {
    digitalWrite(FAILSAFE_LED, HIGH); // Tudo OK
  }
  else
  {
    digitalWrite(FAILSAFE_LED, LOW); // FALHA: Controle desconectado
    stopRobot();                     // Para o robô como medida de segurança
    if (isWeaponActive)
    {
      isWeaponActive = false;

      // Arma completamente parada (ambos os lados do motor em LOW)
      digitalWrite(WEAPON_PIN, LOW);
      digitalWrite(WEAPON_PIN2, LOW);
    }
  }
}