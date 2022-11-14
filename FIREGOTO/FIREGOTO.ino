/*
 *  FireGoTo - an Arduino Motorized Telescope Project for Dobsonian Mounts
 *  https://firegoto.com.br
    Copyright (C) 2021  Rangel Perez Sardinha / Marcos Lorensini originally created by Reginaldo Nazar

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

/*
  ATENÇÃO - Antes de fazer upload para a sua placa Arduino, você deve fazer as configurações iniciais do seu sistema

  A configuração de passo deve ser feita da seguinte forma:

  Quantidade de passos do motor de passo: 200
  Tamanho da polia da caixa de redução (quantidade de dentes): 185
  Tamanho da polia do eixo sem fim: 60
  Tamanho da polia do motor: 20
  Sendo assim, a relação do sistema é: 60 / 20 = 3. Ou seja, relação de 3 pra 1 (3:1)
  Redução: 32 (esse valor pode variar, sendo, 4, 8, 16, 32, 64)

  Cálculo final: (185 * (60 / 20) * 32 * 200) = 3552000
  Sendo assim, o valor calculado acima deve ser inserido na variavel dMaxPassoAltCalculo.
  Levando em consideração que as caixas de redução são iguais, o mesmo valor deve ser inserido na variavel dMaxPassoAzCalculo.

  Se o seu sistema possui LCD, definir "true" na variavel possuiLCD, senão "false"
*/
double dMaxPassoAltCalculo = 11840000;
double dMaxPassoAzCalculo = 11840000;
double dMaxSpeedAltCalculo = dMaxPassoAltCalculo;
double dMaxSpeedAzCalculo = dMaxPassoAzCalculo;
int dMinTimerCalculo = 150;
int dReducaoCalculo = 64;
bool possuiLCD = true; 
bool tmcFlag = true; 
// if true, using TMC2209; if false, using DVR8825

// Código adaptado por L.H.Bonani 
#include <AccelStepper.h>
#include <Arduino.h>
#include <math.h>
#include <Time.h>
#include <TimeLib.h>
#include <DueTimer.h>
#include <DueFlashStorage.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

//DEBUG
int flagDebug = 0;

//2. Pinos Joystick
#define yPin     A0   // eixo y (VT na placa)
#define xPin     A1   // eixo x (HR na placa)
#define kPin     A2   // botão (SL na placa)
/*
             Configuração Física Joystick
                   Y- 
                   o (512,0)
- GND |            |
- +5V |  (0,512)|
- VRx | X- o---------------o X+ (Eixo X)
- VRy |            |      (1023,512)
- SW  |            |
           (512,1023) o 
                   Y+
                (Eixo Y) 

**/
String sDriver;

//Menu e joystick
int tCount1;
bool refresh;//lcd clear On/Off
//leerJoystick
int joyRead;
int joyPos; 
int lastJoyPos;
long lastDebounceTime = 0; 
long debounceDelay = 70;                 //user define
//Control Joystick
bool PQCP;
bool editMode;
//sistema de menus
int mNivel1;  
int mNivel11; // para subníveis de configuração
int mNivel2;  
int mState;
int sizeMS = 8;
int sizeSD = 3;
int micropassosRA[8];
int indexRA = 0;
String mpRA;
int micropassosDEC[8];
int indexDEC = 0;
String mpDEC;
int vReducao;
int indexRDVR = 0;
String mpRDVR;
int redDriver[4] = {8, 16, 32, 64};
int lengthRDVR = 4;
int velocidadeMotor[3];
int indexVMot = 0;
String mpVMot;
int sentidoMotorRA;
int sentidoMotorDEC;
String dirRA;
String dirDEC;

bool configPassosRA = false;
bool configPassosDEC = false;
bool configReducaoDriver = false;
bool configVelocidadeMotores = false;
bool configSentidoRA = false;
bool configSentidoDEC = false;
bool configDriver = false;
bool runRADECMotors = false;
String dvr = "DVR8825";
String tmc = "TMC2209";
String rRADECMot = "Off";
int estadoSetup;
long nextTimeStopRA;
long nextTimeStopDEC;
long nextTimeRunRA;
long nextTimeRunDEC;

//editmode
byte n[19];
int lastN;
int lcdX;
int jBut;
//int lcdY;
bool exiT;
bool moveRADEC;
int thisHour;
bool timeFlag = false;
// Variáveis de estado (incluído por L. H. Bonani)
bool focStatus = false;
bool parkStatus = false; // Ricardo
bool nivelarStatus = false; // Ricardo
long lastTrueFocStatus = 0;
long lastTrueParkStatus = 0; // Ricardo
long lastTrueNivelarStatus = 0; // Ricardo
bool manualMoveRADEC = false;
bool configuracao = false;
bool XhighRes = false;
bool YhighRes = false;

int accelRateLevel = 0; // (nível de aceleração valor / comando LX200 --> 0 / :RS# (máxima aceleração); 1 / :RM#; 2 / :RC#; 3 / :RG# (mínima aceleração)

//  Variáveis para a identificação dos movimentos manuais dos motores (incluído por L. H. Bonani)
bool mRA_leste = false;
bool mRA_oeste = false;
bool mDEC_norte = false;
bool mDEC_sul = false;
int motorSpeedX = 0;
int motorSpeedY = 0;
int motorSpeedFoc = 0;
int subMenuConfig = 0;


int auxComm = 0;

//Criacao dos motores
#define MotorALT_Direcao 22
#define MotorALT_Passo 24
#define MotorALT_Sleep 26
#define MotorALT_Reset 28
#define MotorALT_M2 30
#define MotorALT_M1 32
#define MotorALT_M0 34
#define MotorALT_Ativa 36
#define MotorAZ_Direcao 38
#define MotorAZ_Passo 40
#define MotorAZ_Sleep 42
#define MotorAZ_Reset 44
#define MotorAZ_M2 46
#define MotorAZ_M1 48
#define MotorAZ_M0 50
#define MotorAZ_Ativa 52

// Variáveis para o controle do motor do focalizador usando portas do terceiro driver (Rômulo Almeida)
#define MotorFoc_Direcao 2 // 
#define MotorFoc_Passo 3 // 
#define MotorFoc_Enable 4 // 

LiquidCrystal_I2C lcd(0x27, 20, 4);
 

AccelStepper AltMotor(AccelStepper::DRIVER, MotorALT_Passo, MotorALT_Direcao);
AccelStepper AzMotor(AccelStepper::DRIVER, MotorAZ_Passo, MotorAZ_Direcao);
AccelStepper focMotor(AccelStepper::DRIVER, MotorFoc_Passo, MotorFoc_Direcao); // Motor do focalizador (incluído por L. H. Bonani)

int accel = 1;


//LEDs
#define LedB 53
#define LedR 49
#define LedG 51
int ledStateR = LOW;
int ledStateB = LOW;
int ledStateG = LOW;
long lastInputCheckTime = 0;

// Para movimento manual (incluído por L.H.Bonani, 2021.10.29)
int raLess0 = 0;
int raGreater360 = 0;

/*valores maximo para o passo (Valor ideal 1286400)*/
double dMaxPassoAlt = dMaxPassoAltCalculo; //6400; //2304000; /* //valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)/4    (16*200*(117/11)*56)*/
double dMaxPassoAz = dMaxPassoAzCalculo; //6400; //2304000; /*/valor de resolucao AZ = Passo * MicroPasso * reducao ex(200*16*402)   (16*200*(118/11)*57)*/
double dMaxPassoFoc = 1600;
int dMinTimer = dMinTimerCalculo; /*/passo*/
double dMaxSpeedAlt = dMaxSpeedAltCalculo; // 6400; // 2304000;
double dMaxSpeedAz = dMaxSpeedAzCalculo; // 6400; //2304000;
double dMaxSpeedFoc = 700;
int dReducao = dReducaoCalculo; // Quantidade de micropassos

// Para o controle manual dos motores (Incluído por L.H.Bonani)
int valPotFoc = 0;
int valPotX = 0;
int valPotY = 0;
int ratepadrao;
int maxSpeedMotorFoc = 700; // Incluído por L.H.Bonani
int maxSpeedMotorRA = 13300; // Incluído por L.H.Bonani
int maxSpeedMotorDEC = 13300 ; // Incluído por L.H.Bonani
 
// Para contornar bug de conexão com skysafari (incluído por L.H.Bonani)
bool flagDHora = false;

//Variaveis de persistencia e estrutura de dados ----------------------------------------------------------------------------------------------------------------
DueFlashStorage dueFlashStorage;

// The struct of the configuration.
struct Configuration {
  int32_t Reducao;
  int32_t MaxPassoAlt;
  int32_t MaxPassoAz;
  int32_t MinTimer;
  int32_t AltaM2;
  int32_t AltaM1;
  int32_t AltaM0;
  int32_t SentidoDEC;
  int32_t SentidoRA;
  uint32_t DataHora;
  double latitude;
  double longitude;
  int32_t UTC;
  char* Local;
};

Configuration configuration;
Configuration configurationFromFlash; // create a temporary struct

int Reducao;
int MaxPassoAlt;
int MaxPassoAz;
int MaxSpeedAz = dMaxSpeedAz;
int MaxSpeedAlt = dMaxSpeedAlt;
int SentidoDEC = 0;
int SentidoRA = 0;

int MinTimer;
int AltaM2;
int AltaM1;
int AltaM0;
double latitude;
double longitude;
int UTC;

int fractime;
unsigned long currentMillis, previousMillis, PrimeiroCommanMillis, calculaRADECmountMillis = 0 , previousMillisLed = 0;

//Variaveis de controle para ler comandos LX200  ----------------------------------------------------------------------------------------------------------------
boolean cmdComplete = false, doispontos = true; // whether the string is complete
int pontBuffer = 0;
int numCommand = 0, numCommandexec = 0, flagCommand = 0;
char Command[15][15];

//Variaveis do SETUP
int setupflag = 0;

//Variaveis globais da MEADE
double SideralRate = 60.0; //HZ
int RAbacklash = 0; //(in ArcSec)
int DECbacklash = 0; //(in ArcSec)
int AtivaBack = 1;
int dirAlt, dirAz, dirAltant, dirAzant;

//Variaveis globais de posicao fisica do telescopio  ----------------------------------------------------------------------------------------------------------------
double eixoAltGrausDecimal = 0.0;
double eixoAzGrausDecimal = 0.0;
double ResolucaoeixoAltGrausDecimal = 0.0, ResolucaoeixoAltPassoGrau = 0.0;
double ResolucaoeixoAzGrausDecimal = 0.0, ResolucaoeixoAzPassoGrau = 0.0;
double RAmount = 0.0;
double DECmount = 0.0;
double AZmount = 0.0;
double ALTmount = 0.0;
double AZmountAlvo = 0.1;
double ALTmountAlvo = 0.1;

double DistanciaTotal = 0.0;

//Alvos a serem seguidos ou sincronizado
double RAAlvo = 0.0;
double DECAlvo = 0.0;
double AzAlvo = 0.0;
double AltAlvo = 0.0;
double ErroAlt = 0.0;
double ErroAz = 0.0;

int statusmovimentacao = 0;
int ativaacom = 0;
int gotomount = 0;
//limites da montagem
int HorizonteLimite = 0;
int AltitudeLimite = 90;

//Ajuste fino do tempo
int Segundo;
double Microssegundo = 0 , SegundoFracao = 0.0, MilissegundoSeg = 0.0, MilissegundoI = 0.0;

void setup() {  
  
  ExecutarConfiguracaoInicialLED();  
  ConfigurarTipoDriver();
  ExecutarPrimeiraConfiguracao();
  
  LerMemoriaFlash();

  ConfigurarTipoReducao();
  ConfigurarReducao();
  ConfigurarLocalizacao();
  DefinirSentidoMotores();
  
  setTime(configurationFromFlash.DataHora);

  Serial.print("Bem vindo ao FIREGOTO para setup inicial digitar :HSETUPON# \n");
  SerialPrint("00:00:00#"); //RTA para leitura do driver ASCOM da MEADE autostar I

  IniciaMotores();
  SentidodosMotores();

  previousMillis = millis();
  PrimeiroCommanMillis = previousMillis;
  ErroAlt = ErroAz = 44.0;
  
  DefinirResolucaoEixos();
  DefinirPosicaoInicial();  
  DefinirUsoLCD();
}

void loop() {

  if (ledStateR == LOW) {
    ledStateR = HIGH;
  }
 
  currentMillis = millis();
  CalcPosicaoPasso();

  if (SerialUSB.available() || Serial.available() || Serial3.available()) 
    serialEvent();
  
  if ((numCommand != numCommandexec) && (flagCommand == 0)) {
    SerialPrintDebug(String(numCommandexec));
    SerialPrintDebug(String(numCommand));
    cmdComplete = true;
    executecommand();
    SerialPrintDebug(String(numCommandexec));
    SerialPrintDebug(String(numCommand));
  }
 
  // Para contornar o bug de conexão com skysafari (adicionado por L.H.Bonani)
  if (flagDHora == true) {
    PrintLocalHora();
  }

  if (runRADECMotors == true) {
     runsetupMotors(4000, 1000);
  }

  if ( setupflag == 0 ) {
    if (flagDebug == 1) {
      if (PrimeiroCommanMillis < currentMillis) {
        //Relógio desabilitado para nao comprometer o sync
        PrintLocalHora();
        SerialPrintDebug(String(Hora2DecHora(hour(), minute(), SegundoFracao), 10)) ;
        PrimeiroCommanMillis = PrimeiroCommanMillis + 1001;
      }
    }
  }

  if (setupflag == 1 ) {
    RotinadeSetup();
  }

  //SerialPrint(" Calcula posição \n ");
  if (calculaRADECmountMillis < currentMillis) {
      calculaRADECmountMillis = calculaRADECmountMillis + 113;
      calculaRADECmount();
  }

  AlteraMicroSeg();
  if (previousMillisLed < currentMillis) {
    previousMillisLed = currentMillis + 150;
    digitalWrite(LedR, ledStateR);
    digitalWrite(LedB, ledStateB);
    digitalWrite(LedG, ledStateG);
    ledStateB = LOW;
  }

  // print the string when a newline arrives:
  // protegemount();

  if (ativaacom != 0) {
    if (previousMillis < currentMillis) {
      setaccel();
      previousMillis = currentMillis + 237;
      DistanciaTotal = abs(AltMotor.distanceToGo()) + abs(AzMotor.distanceToGo());
      if ( MaxPassoAlt == dMaxPassoAlt)
      {
        if ( DistanciaTotal > (MaxPassoAz / 90))
        {
          BaixaResolucao();
        }
      }
      else
      {
        if ( DistanciaTotal < (MaxPassoAz / 180))
        {
          AltaResolucao();
        }
      }
      SerialPrintDebug("Distancia Total: ");
      SerialPrintDebug(String(DistanciaTotal));
      SerialPrintDebug(String(MaxPassoAz));
    }

    SetPosition();
    acompanhamento();
  }
  else {
    previousMillis = millis();
  }

  AlteraMicroSeg();

  if (possuiLCD) {
    controlJoystick();
    menu();
  }
}
