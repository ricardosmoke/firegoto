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
void IniciaMotores()
{  
  //Iniciar as variaveis do motor de passo
  // Modificado por L.H.Bonani para incluir o motor do focalizador
  pinMode(MotorALT_Direcao, OUTPUT);
  pinMode(MotorALT_Passo, OUTPUT);
  pinMode(MotorALT_Sleep, OUTPUT);
  pinMode(MotorALT_Reset, OUTPUT);
  pinMode(MotorALT_M2, OUTPUT);
  pinMode(MotorALT_M1, OUTPUT);
  pinMode(MotorALT_M0, OUTPUT);
  pinMode(MotorALT_Ativa, OUTPUT);
  pinMode(MotorAZ_Direcao, OUTPUT);
  pinMode(MotorAZ_Passo, OUTPUT);
  pinMode(MotorAZ_Sleep, OUTPUT);
  pinMode(MotorAZ_Reset, OUTPUT);
  pinMode(MotorAZ_M2, OUTPUT);
  pinMode(MotorAZ_M1, OUTPUT);
  pinMode(MotorAZ_M0, OUTPUT);
  pinMode(MotorAZ_Ativa, OUTPUT);
  pinMode(MotorFoc_Passo, OUTPUT);
  pinMode(MotorFoc_Direcao, OUTPUT);
  pinMode(MotorFoc_Enable, OUTPUT);
  //Aciona os pinos por padrão
  digitalWrite(MotorALT_Direcao, LOW);
  digitalWrite(MotorALT_Passo, LOW);
  digitalWrite(MotorALT_Sleep, HIGH);
  digitalWrite(MotorALT_Reset, HIGH);
  digitalWrite(MotorALT_M2, HIGH);
  digitalWrite(MotorALT_M1, HIGH);
  digitalWrite(MotorALT_M0, HIGH);
  digitalWrite(MotorALT_Ativa, LOW);
  digitalWrite(MotorAZ_Direcao, LOW);
  digitalWrite(MotorAZ_Passo, LOW);
  digitalWrite(MotorAZ_Sleep, HIGH);
  digitalWrite(MotorAZ_Reset, HIGH);
  digitalWrite(MotorAZ_M2, HIGH);
  digitalWrite(MotorAZ_M1, HIGH );
  digitalWrite(MotorAZ_M0, HIGH);
  digitalWrite(MotorAZ_Ativa, LOW);
  digitalWrite(MotorFoc_Passo, LOW);
  digitalWrite(MotorFoc_Direcao, LOW);
  digitalWrite(MotorFoc_Enable, HIGH);
  AltMotor.setMaxSpeed(dMaxSpeedAlt);
  AltMotor.setAcceleration(dReducao);
  AzMotor.setMaxSpeed(dMaxSpeedAz);
  AzMotor.setAcceleration(dReducao);
  focMotor.setMaxSpeed(dMaxSpeedFoc);
  focMotor.setAcceleration(8);
  Timer3.start(MinTimer);
  Timer3.attachInterrupt(runmotor);  
}

void SentidodosMotores() // Modificado por L.H.Bonani para correspondência entre RA <-> Az e DEC <-> Alt (originalmente invertida)
{  
  if (SentidoDEC == 1)
  {
    AltMotor.setPinsInverted(true);
  }
  else
  {
    AltMotor.setPinsInverted(false);

  }
  if (SentidoRA == 1)
  {
    AzMotor.setPinsInverted(true);
  }
  else
  {
    AzMotor.setPinsInverted(false);
  }
}

void runmotor () { // Modificado por L.H.Bonani para incluir o motor do focalizador
  if (setupflag == 0 && runRADECMotors == false) {
    AltMotor.run();
    AzMotor.run();
    focMotor.runSpeed();
  } else {
    AltMotor.runSpeed();
    AzMotor.runSpeed();
  }
}

void setaccel()
{
  double tempdis;
  tempdis = abs(AzMotor.distanceToGo());
  AzMotor.setMaxSpeed(tempdis * dReducao );
  tempdis = abs(AltMotor.distanceToGo());
  AltMotor.setMaxSpeed(tempdis * dReducao );
}

void setaccel(int Accel)
{
  AltMotor.setMaxSpeed(Accel);
  AzMotor.setMaxSpeed(Accel);
}

void setMotorResolution(int Reducao, bool tmcD) {
  if(Reducao==64) {
    if (tmcD) {
      AltaM0 = LOW;
      AltaM1 = HIGH;
    } else {
      Serial.print("Configuração de 64 micropassos para DVR8825 não suportada. Configurando para 32 micropassos. \n");
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = HIGH;
    }
  }
  if(Reducao==32) {
    if (tmcD) {
      AltaM0 = HIGH;
      AltaM1 = LOW;
    } else {
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = HIGH;
    }
  }
  if(Reducao==16) {
    if (tmcD) {
      AltaM0 = HIGH;
      AltaM1 = HIGH;
    } else {
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = LOW;
    }
  }
  if(Reducao==8) {
    if (tmcD) {
      AltaM0 = LOW;
      AltaM1 = LOW;
    } else {
      AltaM2 = LOW;
      AltaM1 = HIGH;
      AltaM0 = HIGH;
    }
  }
  if(Reducao==4) {
    if (tmcD) {
      Serial.print("Configuração de 4 micropassos para TMC2209 não suportada. Configurando para 8 micropassos. \n");
      AltaM0 = LOW;
      AltaM1 = LOW;
    } else {
      AltaM2 = LOW;
      AltaM1 = HIGH;
      AltaM0 = LOW;
    }
  }
  if(Reducao==2) {
    if (tmcD) {
      Serial.print("Configuração de 2 micropassos para TMC2209 não suportada. Configurando para 8 micropassos. \n");
      AltaM0 = LOW;
      AltaM1 = LOW;
    } else {
      AltaM2 = LOW;
      AltaM1 = LOW;
      AltaM0 = HIGH;
    }
  }
}

void addbackslash()
{
  if (AzMotor.distanceToGo() >= 0)
  {
    dirAz = 1;
  } else {
    dirAz = 0;
  }
  if (AltMotor.distanceToGo() >= 0)
  {
    dirAlt = 1;
  } else {
    dirAlt = 0;
  }
  if (AtivaBack == 1)
  {
    AtivaBack = 0;
    if (dirAlt != dirAltant)
    {
      if (dirAlt == 0)
      {
        Serial.println("alt:0");
        AltMotor.setCurrentPosition(AltMotor.currentPosition() + RAbacklash);
      } else
      {
        AltMotor.setCurrentPosition(AltMotor.currentPosition() - RAbacklash);
        Serial.println("alt:1");

      }
    }
    if (dirAz != dirAzant)
    {
      if (dirAz == 0)
      {
        AzMotor.setCurrentPosition( AzMotor.currentPosition() + DECbacklash);
        Serial.println("az: 0 ");

      } else
      {
        AzMotor.setCurrentPosition( AzMotor.currentPosition() - DECbacklash);
        Serial.println("az: 1");

      }
    }
  }
}


void syncro()
{  
  AltMotor.setCurrentPosition((int)ALTmountAlvo);
  AzMotor.setCurrentPosition((int)AZmountAlvo);
  CalcPosicaoPasso();  
}

void SetPosition()
{
  AltMotor.moveTo((int)ALTmountAlvo);
  AzMotor.moveTo((int)AZmountAlvo);
}


int paramotors()
{
  AZmountAlvo = AZmount;
  ALTmountAlvo = ALTmount;
  return (1);
}

int paraAzMotor()
{
  AZmountAlvo = AZmount;
  setaccel(1);
  AZmount = AzMotor.currentPosition();
  AzMotor.moveTo(AZmount);
  return (1);
}

int paraAltMotor()
{
  ALTmountAlvo = ALTmount;
  setaccel(1);
  ALTmount = AltMotor.currentPosition();
  AltMotor.moveTo(ALTmount);
  return (1);
}

void CalculaResolucao(){   
     ResolucaoeixoAltGrausDecimal = 360.0 / MaxPassoAlt ;
     ResolucaoeixoAzGrausDecimal = 360.0 / MaxPassoAz ;
     ResolucaoeixoAltPassoGrau = (MaxPassoAlt  / 360.0);
     ResolucaoeixoAzPassoGrau = (MaxPassoAz  / 360.0);
}


void CalcPosicaoPasso()
{
  ALTmount = AltMotor.currentPosition();
  AZmount = AzMotor.currentPosition();
  eixoAltGrausDecimal = ResolucaoeixoAltGrausDecimal * ALTmount;
  eixoAzGrausDecimal = ResolucaoeixoAzGrausDecimal * (AZmount + (raLess0 - raGreater360)*MaxPassoAz);
  if (eixoAzGrausDecimal < 0.0) { // incluído por L.H.Bonani para aprimoramento do movivento manual -> se o àngulo a for menor que 0º, passa mostrar (360 + a)º
    raLess0++;
    eixoAzGrausDecimal = ResolucaoeixoAzGrausDecimal * (AZmount + (raLess0 - raGreater360)*MaxPassoAz);
  }
  if (eixoAzGrausDecimal > 360) {  // incluído por L.H.Bonani para aprimoramento do movivento manual -> se o àngulo a for maior que 360º, passa mostrar (a - 360)º
    raGreater360++;
    eixoAzGrausDecimal = ResolucaoeixoAzGrausDecimal * (AZmount + (raLess0 - raGreater360)*MaxPassoAz);
  }
}

void controlMotorJoystick() {   // Movimento manual dos motores (incluído por L.H.Bonani)
/*
 * Esquema de mapeamento do potenciômetro do joystick para controle manual de velocidade dos motores
 * 
 * *   0         ilow         slowp   shigh        ihigh         1023
 * - |------------||-----------||xxxxx||------------||------------|  +
 *               ilowp        slow    shighp        ihighp
 *               
 *   Exemplo: (50% da faixa útil do potenciômetro para baixa velocidade e 50% para alta velocidade)
 *            ilow  = 224;   ilowp = ilow + 1;
 *            ihigh = 798;   ihighp = ihigh + 1;
 *            slow  = 449;   slowp = slow + 1;
 *            shigh = 573;   shighp = shigh + 1;
 * 
 *   0           224           450   573            799         1023
 * - |------------||-----------||xxxxx||------------||------------|  +
 *                225         449     574          798
 *   0  ->224 : baixa resolução (velocidade alta) e sentido horário
 *   225->449 : alta resolução (velocidade baixa) e sentido horário
 *   450->573 : velocidade zero
 *   574->798 : alta resolução (velocidade baixa) e sentido anti-horário
 *   799->1023: baixa resolução (velocidade alta) e sentido anti-horário
 *   Observação: o sentido dos motores depende de como foram configurados na rotina de Setup
 *   
 */
  int ilow = 89;   int ilowp = ilow + 1;
  int slow = 449;   int slowp = slow + 1;
  int shigh = 573;  int shighp = shigh + 1;
  int ihigh = 932;  int ihighp = ihigh + 1;
  valPotX =  analogRead(xPin); 
  if (valPotX <= slow){ 
    if (valPotX >= ilowp) { // High resolution
        if (mRA_oeste == false || XhighRes == false) {
            accel = ratepadrao;
            moveoesteManual();
            XhighRes = true;
        }
    } else { // Low Resolution
        if (XhighRes == true) {
            accel = ratepadrao * 128000;
            moveoesteManual();
            XhighRes = false;
        }
    }
    mRA_leste = false;
    mRA_oeste = true; 
  } else if (valPotX > slow && valPotX < shighp) { 
    if (mRA_oeste == true || mRA_leste == true) {
      paraAzMotor();
    }
    mRA_leste = false;
    mRA_oeste = false;
    XhighRes = false;
  } else if (valPotX >= shighp){ 
    if (valPotX <= ihigh) {
       if (mRA_leste == false || XhighRes == false) { // High resolution
            accel = ratepadrao;
            movelesteManual();
            XhighRes = true;
        }
    } else { // Low resolution
        if (XhighRes == true) {
            accel = ratepadrao * 128000;
            movelesteManual();
            XhighRes = false;
        }
    }
    mRA_leste = true;
    mRA_oeste = false; 
  }
  
  valPotY =  analogRead(yPin); 
  if (valPotY <= slow){
    if (valPotY >= ilowp) { // High resolution
        if (mDEC_norte == false || YhighRes == false) {
            accel = ratepadrao;
            movenorteManual();
            YhighRes = true;
        }
    } else { // Low Resolution
        if (YhighRes == true) {
            accel = ratepadrao * 128000;
            movenorteManual();
            YhighRes = false;
        }
    }
    mDEC_norte = true;
    mDEC_sul = false;
    /*
    if (eixoAltGrausDecimal > 89.5) { // incluído por L.H.Bonani para proteção da montagem (2021.10.28)
       paraAltMotor();
    }
    */
  } else if (valPotY > slow && valPotY < shighp) { 
    if (mDEC_sul == true || mDEC_norte == true) {
      paraAltMotor();
    }
    mDEC_norte = false;
    mDEC_sul = false;
    YhighRes = false;
  } else if (valPotY >= shighp){ 
    if (valPotY <= ihigh) { // High Resolution
       if (mDEC_sul == false || YhighRes == false) {
            //paraAltMotor();
            accel = ratepadrao;
            movesulManual();
            YhighRes = true;
        }
    } else {  // Low Resolution
       if (YhighRes == true) {
            accel = ratepadrao * 128000;
            movesulManual();
            YhighRes = false;
        }
    }
    mDEC_norte = false;
    mDEC_sul = true;
    /*
    if (eixoAltGrausDecimal < 0.5) { // incluído por L.H.Bonani para proteção da montagem (2021.10.28)
       paraAltMotor();
    }
    */
  }  
  movimentoRADEC();
}


void setAccelerationLevel() {
  switch(accelRateLevel) {
      case 3:
        accel = ratepadrao * 2;
        break;
      case 2:
        accel = ratepadrao * 8;
        break;
      case 1:
        accel = ratepadrao * 128;
        break;
      case 0:
        accel = ratepadrao * 128000;
        break;
  }
}


void controlFocuser() { 
    int ilow = 224;   int ilowp = ilow + 1;
    int slow = 449;   int slowp = slow + 1;
    int shigh = 573;  int shighp = shigh + 1;
    int ihigh = 798;  int ihighp = ihigh + 1;
    valPotFoc =  analogRead(yPin);  // Alteração do eixo do joystick do "x" para o "y" (por Rômulo Almeida).
    if (valPotFoc >= slowp && valPotFoc <= shighp) {
        if (lastTrueFocStatus + 10000 <= millis()) {
          focStatus = false;
          digitalWrite(MotorFoc_Enable, HIGH);
        } 
        motorSpeedFoc = 0;
      focMotor.setSpeed(motorSpeedFoc);
}else{
  lastTrueFocStatus = millis();     
    if (valPotFoc <= slow){
      if (valPotFoc >= ilowp) {
         motorSpeedFoc = map(valPotFoc,slow,ilowp,0,maxSpeedMotorFoc/16); 
      } else {
         motorSpeedFoc = map(valPotFoc,ilow,0,maxSpeedMotorFoc/2,maxSpeedMotorFoc); 
      }
      focMotor.setSpeed(-motorSpeedFoc);
    } else if (valPotFoc > slow && valPotFoc < shighp) { 
      motorSpeedFoc = 0;
      focMotor.setSpeed(motorSpeedFoc);
    } else if (valPotFoc >= shighp){ 
      if (valPotFoc <= ihigh) {
        motorSpeedFoc = map(valPotFoc,shighp,ihigh,0,maxSpeedMotorFoc/16); 
      } else {
        motorSpeedFoc = map(valPotFoc,ihighp,1023,maxSpeedMotorFoc/2,maxSpeedMotorFoc); 
      }
      focMotor.setSpeed(motorSpeedFoc);
    }
    
  } focMotor.runSpeed();
}


void BaixaResolucao() { // Modificado por L.H.Bonani para permitir o uso do TMC2209
  if ( MaxPassoAz == dMaxPassoAz) {
    if (tmcFlag) {
      MaxPassoAz = dMaxPassoAz / dReducao * 8;
      MaxPassoAlt = dMaxPassoAlt / dReducao * 8;    
      digitalWrite(MotorALT_M1, LOW);
      digitalWrite(MotorALT_M0, LOW);
      digitalWrite(MotorAZ_M1, LOW );
      digitalWrite(MotorAZ_M0, LOW);  
      AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao * 8);
      AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao * 8);
      AltMotor.setAcceleration(dReducao * 32);
      AzMotor.setAcceleration(dReducao * 32);
    } else {
      MaxPassoAz = dMaxPassoAz / dReducao;
      MaxPassoAlt = dMaxPassoAlt / dReducao;
      digitalWrite(MotorALT_M2, LOW);
      digitalWrite(MotorALT_M1, LOW);
      digitalWrite(MotorALT_M0, LOW);
      digitalWrite(MotorAZ_M2, LOW);
      digitalWrite(MotorAZ_M1, LOW );
      digitalWrite(MotorAZ_M0, LOW);
      AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao);
      AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao);
      AltMotor.setAcceleration(dReducao * 4);
      AzMotor.setAcceleration(dReducao * 4);
    }
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}


void AltaResolucao () {  // Modificado por L.H.Bonani para permitir o uso do TMC2209
  if ( MaxPassoAz != dMaxPassoAz) {
    MaxPassoAz = dMaxPassoAz;
    MaxPassoAlt = dMaxPassoAlt;
    if (tmcFlag) {
       digitalWrite(MotorALT_M1, AltaM1);
       digitalWrite(MotorALT_M0, AltaM0);
       digitalWrite(MotorAZ_M1, AltaM1);
       digitalWrite(MotorAZ_M0, AltaM0);
       AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao / 8);
       AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao / 8);
       AltMotor.setAcceleration(dReducao/8*12);
       AzMotor.setAcceleration(dReducao/8*12);
    } else {
       digitalWrite(MotorALT_M2, AltaM2);
       digitalWrite(MotorALT_M1, AltaM1);
       digitalWrite(MotorALT_M0, AltaM0);
       digitalWrite(MotorAZ_M2, AltaM2);
       digitalWrite(MotorAZ_M1, AltaM1);
       digitalWrite(MotorAZ_M0, AltaM0);
       AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao);
       AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao);
       AltMotor.setAcceleration(dReducao);
       AzMotor.setAcceleration(dReducao);
    }
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = LOW;
  }
}

void BaixaResolucaoAz ()
{
  if ( MaxPassoAz == dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz / dReducao;
    digitalWrite(MotorAZ_M2, LOW);
    digitalWrite(MotorAZ_M1, LOW );
    digitalWrite(MotorAZ_M0, LOW);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao);
    AzMotor.setAcceleration(dReducao * 4);
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}
void BaixaResolucaoAlt ()
{
  if ( MaxPassoAlt == dMaxPassoAlt)
  {
    MaxPassoAlt = dMaxPassoAlt / dReducao;
    digitalWrite(MotorALT_M2, LOW);
    digitalWrite(MotorALT_M1, LOW);
    digitalWrite(MotorALT_M0, LOW);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao);
    AltMotor.setAcceleration(dReducao * 4);
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}


void AltaResolucaoAz ()
{
  if ( MaxPassoAz != dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz;
    digitalWrite(MotorAZ_M2, AltaM2);
    digitalWrite(MotorAZ_M1, AltaM1);
    digitalWrite(MotorAZ_M0, AltaM0);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    AzMotor.setAcceleration(dReducao * dReducao * 2);
    ledStateG = LOW;
  }
}
void AltaResolucaoAlt ()
{
  if ( MaxPassoAlt != dMaxPassoAlt)
  {
    MaxPassoAlt = dMaxPassoAlt;
    digitalWrite(MotorALT_M2, AltaM2);
    digitalWrite(MotorALT_M1, AltaM1);
    digitalWrite(MotorALT_M0, AltaM0);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    AltMotor.setAcceleration(dReducao * dReducao * 2);
    ledStateG = LOW;
  }
}

void protegemount()
{
  /*
  if ((eixoAltGrausDecimal < 1 ) || (eixoAltGrausDecimal > 90))
  {
    paramotors();
    ativaacom = 0;
  }


  if ((eixoAzGrausDecimal < 1) || (eixoAzGrausDecimal > 360))
  {
    paramotors();
    ativaacom = 0;
  }
  */
}

void Estacionar() {
  
    ALTmountAlvo = dMaxPassoAlt;  
    BaixaResolucao();
    accel = ratepadrao * 128000;
    accelRateLevel = 0;  
    setAccelerationLevel();
    CalculaResolucao();
    CalcPosicaoPasso();  
    movenorteManual();    

    if (eixoAltGrausDecimal >= 90) {
      parkStatus = false;
    }
}

void Nivelar() {
  
    ALTmountAlvo = dMaxPassoAlt / 4;  
    BaixaResolucao();
    accel = ratepadrao * 128000;
    accelRateLevel = 0;  
    setAccelerationLevel();
    CalculaResolucao();
    CalcPosicaoPasso();  
    movesulManual();    

    if (eixoAltGrausDecimal <= 0) {
      nivelarStatus = false;
    }
}
