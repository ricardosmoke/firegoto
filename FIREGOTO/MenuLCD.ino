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
    Joytick & LCD & Menu   by JMLoureiro77

    http://jmloureiro77.blogspot.com.es

  328P Flash 5292 (17%) RAM 409(19%)

*/
//====================================================
// Menu
//====================================================
void menu(){
  switch (mNivel1){
    case 0:
      menu0(); // Tela de Inicio
      break;
    case 1:
      jBut = digitalRead(kPin);
      if (jBut == 0 && millis() > lastInputCheckTime + 350) {
        if (configuracao == false) {
          ativaacom = 0;
          configuracao = true;
          mState = 0;
          mNivel11 = 0;
        } 
        lastInputCheckTime = millis();
      }
      menu1();
      break;
    case 2:
        if(mNivel2==1){
          menu21(); // 1a Estrela
        }else if (mNivel2==2){
          menu22(); // 2a estrela
        }else if (mNivel2==3){
          menu23(); // 3a Estrela
        } else {
          menu2(); // Alinhamento
        }
        break;
    case 3: // Mover RA e DEC
      // Necessário fazer modificações para confirmação da função com o botão do Joystick 
      // para não entrar em conflito com o modo de navegação (incluído por L. H. Bonani)
      jBut = digitalRead(kPin);
      if (jBut == 0 && millis() > lastInputCheckTime + 350) {
        if (manualMoveRADEC == false) {
          ativaacom = 0;
          manualMoveRADEC = true;
          BaixaResolucao();
        } else {
          manualMoveRADEC = false;
          setAccelerationLevel();
          CalculaResolucao();
          CalcPosicaoPasso();
        }
        lastInputCheckTime = millis();
      }
      menu3(); 
      break;
    case 4: // Microfocalizar
      // Necessário fazer modificações para confirmação da função com o botão do Joystick 
      // para não entrar em conflito com o modo de navegação (incluído por L. H. Bonani)
      jBut = digitalRead(kPin);
      if (jBut == 0 && millis() > lastInputCheckTime + 350) {
        if (focStatus == false) {
          focStatus = true;
          lastTrueFocStatus = millis();
          digitalWrite(MotorFoc_Enable, LOW);
        } else {
          focStatus = false;
          digitalWrite(MotorFoc_Enable, HIGH);
        }
        lastInputCheckTime = millis();
      }
      menu4(); 
      break;
      
    case 5: // Menu Park
    
      jBut = digitalRead(kPin);
      if (jBut == 0 && millis() > lastTrueParkStatus + 350) {
        if (parkStatus == false) {
          parkStatus = true;
          lastTrueParkStatus = millis();
          digitalWrite(MotorFoc_Enable, LOW);
        } else {
          parkStatus = false;
          digitalWrite(MotorFoc_Enable, HIGH);
        }
        lastInputCheckTime = millis();
      }
      menu5();
      break;    
      
  }//switch
}

void menu5(){ 
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Estacionar OTA");
  if (parkStatus == false) {
    lcd.setCursor(0,1);
    lcd.print("Off");    
    paraAltMotor();
  } else {    
    lcd.setCursor(0,1);
    lcd.print("On ");    
    Estacionar();   
  }

  lcd.setCursor(0,3);    
  lcd.print("Posicao atual: ");
  lcd.setCursor(16,3);
  lcdALTmount();
}

//----------------------------------------------------
// Tela de Inicio
//----------------------------------------------------
void menu0(){
  // Modificação da relação RA com Az e DEC com ALT, pois originalmente estava invertida (incluído por L. H. Bonani)
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(" FireGoTo - Ricardo ");
  lcd.setCursor(0,1);
  //lcd.print("19/01/21 - 00:00:00");
  LCDClockDisplay();
  lcd.setCursor(0,2);
  lcd.print("RA:  ");
  lcd.setCursor(4,2);
  lcdAZmount();
  lcd.setCursor(0,3);
  lcd.print("DEC: ");
  lcd.setCursor(4,3);
  lcdALTmount();
}

//----------------------------------------------------
// Menu Configuracoes
//----------------------------------------------------
void menu1(){
  if (refresh){lcd.clear();refresh=0;}
  /**
   * 0. Micropassos RA:
   * 1. Micropassos DEC:
   * 2. Redução do driver:
   * 3. Velocidade do Motor:
   * 4. Sentido RA:
   * 5. Sentido DEC:
   * 6. Sair
   *
   * State 0: show 0, 1, 2
   * State 1: show 1, 2, 3
   * State 2: show 2, 3, 4
   * State 3: show 3, 4, 5
   * State 4: show 4, 5, 6
   * 
   */
  if (configuracao == false) {
      lcd.setCursor(0,0);
      lcd.print(">Configuracoes");
      //lcd.setCursor(0,1);
      menuState0(); // 
      subState00();
  } else { // entra no modo de configuração
    if (configPassosRA == false && configPassosDEC == false && configReducaoDriver == false && 
        configVelocidadeMotores == false && configSentidoRA == false && configSentidoDEC == false && configDriver == false) {
      lcd.setCursor(0,0);
      lcd.print(">Configuracoes");
      //lcd.setCursor(0,1);
      if (mState == 0) {
        menuState0(); 
        if (mNivel11 == 0) { // Micropassos RA
           subState01();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configPassosRA == false) {
                   configPassosRA = true;
                   refresh = 1;
                   menu10();
               } 
           } 
        } else if (mNivel11 == 1) { // Micropassos DEC
           subState02();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configPassosDEC == false) {
                   configPassosDEC = true;
                   refresh = 1;
                   menu11();
               } 
           } 
        } else if (mNivel11 == 2) { // Redução Driver
           subState03();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configReducaoDriver == false) {
                   configReducaoDriver = true;
                   refresh = 1;
                   menu12();
               } 
           } 
        }
      } else if (mState == 1) {
        menuState1(); 
        if (mNivel11 == 1) { // Micropassos DEC
           subState01();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configPassosDEC == false) {
                   configPassosDEC = true;
                   refresh = 1;
                   menu11();
               } 
           } 
        } else if (mNivel11 == 2) { // Redução Driver
           subState02();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configReducaoDriver == false) {
                   configReducaoDriver = true;
                   refresh = 1;
                   menu12();
               } 
           } 
        } else if (mNivel11 == 3) { // Velocidade dos Motores
           subState03();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configVelocidadeMotores == false) {
                   configVelocidadeMotores = true;
                   refresh = 1;
                   menu13();
               } 
           } 
        }
      } else if (mState == 2) {
        menuState2(); 
        if (mNivel11 == 2) { // Redução Driver
           subState01();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configReducaoDriver == false) {
                   configReducaoDriver = true;
                   refresh = 1;
                   menu12();
               } 
           } 
        } else if (mNivel11 == 3) { // Velocidade dos Motores
           subState02();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configVelocidadeMotores == false) {
                   configVelocidadeMotores = true;
                   refresh = 1;
                   menu13();
               } 
           } 
        } else if (mNivel11 == 4) { // Sentido RA
           subState03();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configSentidoRA == false) {
                   configSentidoRA = true;
                   refresh = 1;
                   menu14();
               } 
           } 
        }
      } else if (mState == 3) {
        menuState3(); 
        if (mNivel11 == 3) { // Velocidade dos Motores
           subState01();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configVelocidadeMotores == false) {
                   configVelocidadeMotores = true;
                   refresh = 1;
                   menu13();
               } 
           } 
        } else if (mNivel11 == 4) { // Sentido RA
           subState02();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configSentidoRA == false) {
                   configSentidoRA = true;
                   refresh = 1;
                   menu14();
               } 
           } 
        } else if (mNivel11 == 5) { // Sentido DEC
           subState03();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configSentidoDEC == false) {
                   configSentidoDEC = true;
                   refresh = 1;
                   menu15();
               } 
           } 
        }
      } else if (mState == 4) {
        menuState4(); 
        if (mNivel11 == 4) { // Sentido RA
           subState01();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configSentidoRA == false) {
                   configSentidoRA = true;
                   refresh = 1;
                   menu14();
               } 
           } 
        } else if (mNivel11 == 5) { // Sentido DEC
           subState02();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configSentidoDEC == false) {
                   configSentidoDEC = true;
                   refresh = 1;
                   menu15();
               } 
           } 
        } else if (mNivel11 == 6) { // Driver
           subState03();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configDriver == false) {
                   configDriver = true;
                   refresh = 1;
                   menu16();
               } 
           } 
        }
      } else if (mState == 5) {
        menuState5(); 
        if (mNivel11 == 5) { // Sentido DEC
           subState01();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configSentidoDEC == false) {
                   configSentidoDEC = true;
                   refresh = 1;
                   menu15();
               } 
           } 
        } else if (mNivel11 == 6) { // Driver
           subState02();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configDriver == false) {
                   configDriver = true;
                   refresh = 1;
                   menu16();
               } 
           } 
        } else if (mNivel11 == 7) { // Motores
           subState03();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (runRADECMotors == false) {
                   configStep4Setup();
                   runRADECMotors = true;
                   rRADECMot = " On";
                   refresh = 1;
               } else {
                   runRADECMotors = false;
                   AzMotor.setSpeed(0);
                   AltMotor.setSpeed(0);
                   rRADECMot = "Off";
                   refresh = 1;
               }
               lastInputCheckTime = millis();
           } 
        }
      } else if (mState == 6) {
        menuState6(); 
        if (mNivel11 == 6) { // Driver
           subState01();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (configDriver == false) {
                   configDriver = true;
                   refresh = 1;
                   menu16();
               } 
           } 
        } else if (mNivel11 == 7) { // Motores
           subState02();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               if (runRADECMotors == false) {
                   configStep4Setup();
                   runRADECMotors = true;
                   rRADECMot = " On";
                   refresh = 1;
               } else {
                   runRADECMotors = false;
                   rRADECMot = "Off";
                   refresh = 1;
               }
               lastInputCheckTime = millis();
           } 
        } else if (mNivel11 == 8) { // Sair
           subState03();
           jBut = digitalRead(kPin);
           if (jBut == 0 && millis() > lastInputCheckTime + 350) {
               configuracao = false;
               refresh = 1;
               lastInputCheckTime = millis();
           } 
        }
      }
    }
  }
}

void menuState0() {
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(1,1);
  lcd.print("1.mP RA  :");
  lcd.setCursor(12,1);
  lcd.print(mpRA);
  lcd.setCursor(1,2);
  lcd.print("2.mP DEC :");
  lcd.setCursor(12,2);
  lcd.print(mpDEC);
  lcd.setCursor(1,3);
  lcd.print("3.mP DRVR:");
  lcd.setCursor(17,3);
  lcd.print(mpRDVR);
}

void menuState1() {
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(1,1);
  lcd.print("2.mP DEC :");
  lcd.setCursor(12,1);
  lcd.print(mpDEC);
  lcd.setCursor(1,2);
  lcd.print("3.mP DRVR:");
  lcd.setCursor(17,2);
  lcd.print(mpRDVR);
  lcd.setCursor(1,3);
  lcd.print("4.V.Motor:");
  lcd.setCursor(17,3);
  lcd.print(mpVMot);
}

void menuState2() {
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(1,1);
  lcd.print("3.mP DRVR:");
  lcd.setCursor(17,1);
  lcd.print(mpRDVR);
  lcd.setCursor(1,2);
  lcd.print("4.V.Motor:");
  lcd.setCursor(17,2);
  lcd.print(mpVMot);
  lcd.setCursor(1,3);
  lcd.print("5.Dir. RA:");
  lcd.setCursor(12,3);
  lcd.print(dirRA);
}

void menuState3() {
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(1,1);
  lcd.print("4.V.Motor:");
  lcd.setCursor(17,1);
  lcd.print(mpVMot);
  lcd.setCursor(1,2);
  lcd.print("5.Dir. RA:");
  lcd.setCursor(12,2);
  lcd.print(dirRA);
  lcd.setCursor(1,3);
  lcd.print("6.Dir.DEC:");
  lcd.setCursor(12,3);
  lcd.print(dirDEC);
}

void menuState4() {
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(1,1);
  lcd.print("5.Dir. RA:");
  lcd.setCursor(12,1);
  lcd.print(dirRA);
  lcd.setCursor(1,2);
  lcd.print("6.Dir.DEC:");
  lcd.setCursor(12,2);
  lcd.print(dirDEC);
  lcd.setCursor(1,3);
  lcd.print("7.Driver :");
  lcd.setCursor(13,3);
  lcd.print(sDriver);
}

void menuState5() {
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(1,1);
  lcd.print("6.Dir.DEC:");
  lcd.setCursor(12,1);
  lcd.print(dirDEC);
  lcd.setCursor(1,2);
  lcd.print("7.Driver :");
  lcd.setCursor(13,2);
  lcd.print(sDriver);
  lcd.setCursor(1,3);
  lcd.print("8.Motores:");
  lcd.setCursor(17,3);
  lcd.print(rRADECMot);
}

void menuState6() {
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(1,1);
  lcd.print("7.Driver :");
  lcd.setCursor(13,1);
  lcd.print(sDriver);
  lcd.setCursor(1,2);
  lcd.print("8.Motores:");
  lcd.setCursor(17,2);
  lcd.print(rRADECMot);
  lcd.setCursor(1,3);
  lcd.print("9.Sair Config.");
}

void subState00() {
  lcd.setCursor(0,3);
  lcd.print(" ");
}

void subState01() {
  lcd.setCursor(0,1);
  lcd.print("*");
}

void subState02() {
  lcd.setCursor(0,2);
  lcd.print("*");
}

void subState03() {
  lcd.setCursor(0,3);
  lcd.print("*");
}

//-------------------------------------------------1.0
void menu10(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Micropassos RA");
  lcd.setCursor(0,2);
  lcd.print("Valor:");
}

//-------------------------------------------------1.1
void menu11(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Micropassos DEC");
  lcd.setCursor(0,2);
  lcd.print("Valor:");
}

//-------------------------------------------------1.2
void menu12(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Reducao do Driver");
  lcd.setCursor(0,2);
  lcd.print("Valor:");
}

//-------------------------------------------------1.3
void menu13(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Velocidade do Motor");
  lcd.setCursor(0,1);
  lcd.print("Maior<->Mais lento");
  lcd.setCursor(0,2);
  lcd.print("Menor<->Mais rapido");
  lcd.setCursor(0,3);
  lcd.print("Valor:");
}

//-------------------------------------------------1.4
void menu14(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Sentido Motor RA");
  lcd.setCursor(0,2);
  lcd.print("Valor:");
}

//-------------------------------------------------1.5
void menu15(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Sentido Motor DEC");
  lcd.setCursor(0,2);
  lcd.print("Valor:");
}

//-------------------------------------------------1.6
void menu16(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Driver");
  lcd.setCursor(0,2);
  lcd.print(" Tipo:");
}

//----------------------------------------------------
// Menu Alinhamento
//----------------------------------------------------
void menu2(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Alinhamento");
}
//-------------------------------------------------2.1

void menu21(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">1a estrela");
}
//-------------------------------------------------2.2

void menu22(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">2a estrela");
}

//-------------------------------------------------2.3
void menu23(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">3a estrela");
}

//----------------------------------------------------
// Menu Mover DEC e RA
//----------------------------------------------------
void menu3(){ // Modificado para funcionar com a confirmação do funcionamento por meio do botão do Joystick (incluído por L.H.Bonani)
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Mover RA e DEC");
    lcd.setCursor(0,2);
    lcd.print("RA:  ");
    lcd.setCursor(4,2);
    lcdAZmount();
    lcd.setCursor(0,3);
    lcd.print("DEC: ");
    lcd.setCursor(4,3);
    lcdALTmount();
    if (manualMoveRADEC == true) {
     lcd.setCursor(0,1);
     lcd.print("Manual On ");
     ledStateB = HIGH;
     controlMotorJoystick();
    } else {
     lcd.setCursor(0,1);
     lcd.print("Manual Off");
     ledStateB = LOW;
    }
}
//-------------------------------------------------2.1

//----------------------------------------------------
// Menu Microfocalizador
//----------------------------------------------------
void menu4(){ // Funcionalidades para o funcionamento do focalizador (incluído por L.H.Bonani)
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Microfocalizar");
  if (focStatus == false) {
    lcd.setCursor(0,1);
    lcd.print("Focalizador Off");
  } else {
    lcd.setCursor(0,1);
    lcd.print("Focalizador On ");
    controlFocuser();
  }
  lcd.setCursor(0,2);
  lcd.print("RA:  ");
  lcd.setCursor(4,2);
  lcdAZmount();
  lcd.setCursor(0,3);
  lcd.print("DEC: ");
  lcd.setCursor(4,3);
  lcdALTmount();
}

void movimentoRADEC() { // Rotina para identificação dos movimentos durante o controle manual dos motores (incluído por L.H.Bonani)
    if (mDEC_norte == true && mDEC_sul == false) {
      if (mRA_leste == false && mRA_oeste == false) { // Norte (N)
           lcd.setCursor(11,1);
           lcd.print("> Move N ");
      } else if (mRA_leste == true && mRA_oeste == false) { // Nordeste (NE)
           lcd.setCursor(11,1);
           lcd.print("> Move NE");
      } else if (mRA_leste == false && mRA_oeste == true) { // Noroeste (NW)
           lcd.setCursor(11,1);
           lcd.print("> Move NW");
      } 
  } else if (mDEC_norte == false && mDEC_sul == true) {
      if (mRA_leste == false && mRA_oeste == false) { // Sul (S)
           lcd.setCursor(11,1);
           lcd.print("> Move S ");
      } else if (mRA_leste == true && mRA_oeste == false) { // Sudeste (SE)
           lcd.setCursor(11,1);
           lcd.print("> Move SE");
      } else if (mRA_leste == false && mRA_oeste == true) { // Sudoeste (SW)
           lcd.setCursor(11,1);
           lcd.print("> Move SW");
      } 
  } else if (mDEC_norte == false && mDEC_sul == false) {
      if (mRA_leste == false && mRA_oeste == false) { // Sem movimento
           lcd.setCursor(11,1);
           lcd.print("         ");
      } else if (mRA_leste == true && mRA_oeste == false) { // Leste (E)
           lcd.setCursor(11,1);
           lcd.print("> Move  E");
      } else if (mRA_leste == false && mRA_oeste == true) { // Oeste (W)
           lcd.setCursor(11,1);
           lcd.print("> Move  W");
      } 
  } 
}

void controlJoystick(){ // Modificação de controlJoystick() por L.H.Bonani
  leeJoystick();
  if(PQCP) {
    PQCP=0;
    if (manualMoveRADEC == false && focStatus == false && configuracao == false)  {
     lcd.noBlink();
     if (joyPos==4) {
        if (mNivel1>0){
            mNivel1--;    //para cima
            refresh=1;
         } else {
            mNivel1=5;    // menu focalizador
            refresh=1;
         }
      } else if (joyPos==3) {
         if (mNivel1<5){
            mNivel1++;    // para baixo
            refresh=1;
         } else {
            mNivel1=0;    // menu inicial
            refresh=1;
         }
      } 
    } else if (configuracao == true) { 
      if (configPassosRA == false && configPassosDEC == false && configReducaoDriver == false && 
        configVelocidadeMotores == false && configSentidoRA == false && configSentidoDEC == false && configDriver == false) {
        lcd.noBlink();
        if (joyPos==4) {
           if (mNivel11>0){
              mNivel11--;    //para cima
              if (mNivel11 == 5 && mState == 6) {
                 mState = 5;
              } else if (mNivel11 == 4 && mState == 5) {
                 mState = 4;
              } else if (mNivel11 == 3 && mState == 4) {
                 mState = 3;
              } else if (mNivel11 == 2 && mState == 3) {
                 mState = 2;
              } else if (mNivel11 == 1 && mState == 2) {
                 mState = 1;
              } else if (mNivel11 == 0 && mState == 1) {
                 mState = 0;
              }
              refresh=1;
           } else {
              mNivel11=0;    // menu config (estado 0) -- cursor em 0
              mState = 0;
              refresh=1;
           }
        } else if (joyPos==3) {
           if (mNivel11<8){
              mNivel11++;    // para baixo
              if (mNivel11 == 3 && mState == 0) {
                 mState = 1;
              } else if (mNivel11 == 4 && mState == 1) {
                 mState = 2;
              } else if (mNivel11 == 5 && mState == 2) {
                 mState = 3;
              } else if (mNivel11 == 6 && mState == 3) {
                 mState = 4;
              } else if (mNivel11 == 7 && mState == 4) {
                 mState = 5;
              } else if (mNivel11 == 8 && mState == 5) {
                 mState = 6;
              }
              refresh=1;
           } else {
              mNivel11=8;    // menu inicial
              mState = 6;
              refresh=1;
           }
        } 
      } else {
        if (configPassosRA == true) { // Array RA possui comprimento 8
           lcd.setCursor(7,2);
           mpRA = array2String(micropassosRA);
           lcd.print(mpRA);
           lcd.setCursor((7+indexRA),2);
           lcd.blink();
           if (joyPos==4) { // para cima
              if (micropassosRA[indexRA] == 9) {
                 micropassosRA[indexRA] = 0;
              } else {
                 micropassosRA[indexRA]++;
              }
           } else if (joyPos==3) { // para baixo
              if (micropassosRA[indexRA] == 0) {
                 micropassosRA[indexRA] = 9;
              } else {
                 micropassosRA[indexRA]--;
              }
           } else if (joyPos==2) { // para esquerda
              if (indexRA == 0) { // primeira posição do array
                 indexRA = 7; 
              } else {
                 indexRA--;
              }
              lcd.setCursor((7+indexRA),2);
           } else if (joyPos==1) { // para direita
              if (indexRA == 7) { // última posição do array
                 indexRA = 0; 
              } else {
                 indexRA++;
              }
              lcd.setCursor((7+indexRA),2);
           } else {
              jBut = digitalRead(kPin);
              if (jBut == 0 && millis() > lastInputCheckTime + 350) {
                 configPassosRA = false;
                 refresh = 1;
                 setMaxPassoAz(mpRA);
                 dMaxPassoAz =  MaxPassoAz;
                 lastInputCheckTime = millis();
              }
           } 
        } else if (configPassosDEC == true) { // Array DEC possui comprimento 8
           lcd.setCursor(7,2);
           mpDEC = array2String(micropassosDEC);
           lcd.print(mpDEC);
           lcd.setCursor((7+indexDEC),2);
           lcd.blink();
           if (joyPos==4) { // para cima
              if (micropassosDEC[indexDEC] == 9) {
                 micropassosDEC[indexDEC] = 0;
              } else {
                 micropassosDEC[indexDEC]++;
              }
           } else if (joyPos==3) { // para baixo
              if (micropassosDEC[indexDEC] == 0) {
                 micropassosDEC[indexDEC] = 9;
              } else {
                 micropassosDEC[indexDEC]--;
              }
           } else if (joyPos==2) { // para esquerda
              if (indexDEC == 0) { // primeira posição do array
                 indexDEC = 7; 
              } else {
                 indexDEC--;
              }
              lcd.setCursor((7+indexDEC),2);
           } else if (joyPos==1) { // para direita
              if (indexDEC == 7) { // última posição do array
                 indexDEC = 0; 
              } else {
                 indexDEC++;
              }
              lcd.setCursor((7+indexDEC),2);
           } else {
              jBut = digitalRead(kPin);
              if (jBut == 0 && millis() > lastInputCheckTime + 350) {
                 configPassosDEC = false;
                 refresh = 1;
                 setMaxPassoAlt(mpDEC);
                 dMaxPassoAlt = MaxPassoAlt;
                 lastInputCheckTime = millis();
              }
           } 
        } else if (configReducaoDriver == true) { // Usando 3 dígitos para a configuração do driver
           lcd.setCursor(7,2);
           mpRDVR = setReducaoDVR(vReducao);
           lcd.print(mpRDVR);
           lcd.setCursor(7,2);
           lcd.noBlink();
           if (joyPos==4) { // para cima
              if (indexRDVR == lengthRDVR - 1) {
                 indexRDVR = 0;
              } else {
                 indexRDVR++;
              }
              vReducao = redDriver[indexRDVR];
           } else if (joyPos==3) { // para baixo
              if (indexRDVR == 0) {
                 indexRDVR = lengthRDVR - 1;
              } else {
                 indexRDVR--;
              }
              vReducao = redDriver[indexRDVR];
           } else {
              jBut = digitalRead(kPin);
              if (jBut == 0 && millis() > lastInputCheckTime + 350) {
                 configReducaoDriver = false;
                 refresh = 1;
                 setReducao(mpRDVR);
                 setMotorResolution(Reducao, tmcFlag);
                 IniciaMotores();
                 lastInputCheckTime = millis();
              }
           } 
        } else if (configVelocidadeMotores == true) { // Usando 3 dígitos para a configuração da velocidade, de 000 a 799, pois soma-se 200
           lcd.setCursor(7,3);
           mpVMot = array2String3(velocidadeMotor);
           Serial.println(mpVMot);
           lcd.print(mpVMot);
           lcd.setCursor((7+indexVMot),2);
           lcd.blink();
           if (joyPos==4) { // para cima
              if (indexVMot == 0) {
                 if (velocidadeMotor[indexVMot] == 7) {
                    velocidadeMotor[indexVMot] = 0;
                 } else {
                    velocidadeMotor[indexVMot]++;
                 }
              } else {
                 if (velocidadeMotor[indexVMot] == 9) {
                    velocidadeMotor[indexVMot] = 0;
                 } else {
                    velocidadeMotor[indexVMot]++;
                 }
              }
           } else if (joyPos==3) { // para baixo
              if (indexVMot == 0) {
                 if (velocidadeMotor[indexVMot] == 0) {
                    velocidadeMotor[indexVMot] = 7;
                 } else {
                    velocidadeMotor[indexVMot]--;
                 }
              } else {
                 if (velocidadeMotor[indexVMot] == 0) {
                    velocidadeMotor[indexVMot] = 9;
                 } else {
                    velocidadeMotor[indexVMot]--;
                 }
              }
           } else if (joyPos==2) { // para esquerda
              if (indexVMot == 0) { // primeira posição do array
                 indexVMot = 2; 
              } else {
                 indexVMot--;
              }
              lcd.setCursor((7+indexVMot),2);
           } else if (joyPos==1) { // para direita
              if (indexVMot == 2) { // última posição do array
                 indexVMot = 0; 
              } else {
                 indexVMot++;
              }
              lcd.setCursor((7+indexVMot),2);
           } else {
              jBut = digitalRead(kPin);
              if (jBut == 0 && millis() > lastInputCheckTime + 350) {
                 configVelocidadeMotores = false;
                 refresh = 1;
                 setMinTimer(mpVMot);
                 IniciaMotores();
                 lastInputCheckTime = millis();
              }
           } 
        } else if (configSentidoRA == true) {
           lcd.setCursor(7,2);
           lcd.print(dirRA);
           lcd.noBlink();
           if (joyPos==4 || joyPos ==3) { // para cima
              if (sentidoMotorRA == 0) {
                  sentidoMotorRA = 1;
                  dirRA = "1(AntiH)";
              } else {
                  sentidoMotorRA = 0;
                  dirRA = "0(Horar)";
              }
           } else {
              jBut = digitalRead(kPin);
              if (jBut == 0 && millis() > lastInputCheckTime + 350) {
                 configSentidoRA = false;
                 refresh = 1;
                 setSentidoRA(String(sentidoMotorRA));
                 SentidodosMotores();
                 lastInputCheckTime = millis();
              }
           } 
        } else if (configSentidoDEC == true) {
           lcd.setCursor(7,2);
           lcd.print(dirDEC);
           lcd.noBlink();
           if (joyPos==4 || joyPos==3) { // para cima
              if (sentidoMotorDEC == 0) {
                  sentidoMotorDEC = 1;
                  dirDEC = "1(AntiH)";
              } else {
                  sentidoMotorDEC = 0;
                  dirDEC = "0(Horar)";
              }
           } else {
              jBut = digitalRead(kPin);
              if (jBut == 0 && millis() > lastInputCheckTime + 350) {
                 configSentidoDEC = false;
                 refresh = 1;
                 setSentidoDEC(String(sentidoMotorDEC));
                 SentidodosMotores();
                 lastInputCheckTime = millis();
              }
           } 
        } else if (configDriver == true) {
           lcd.setCursor(7,2);
           lcd.print(sDriver);
           lcd.noBlink();
           if (joyPos==4 || joyPos==3) { // para cima
              if (tmcFlag) {
                  sDriver = dvr;
                  tmcFlag = false;
              } else {
                  sDriver = tmc;
                  tmcFlag = true;
              }
           } else {
              jBut = digitalRead(kPin);
              if (jBut == 0 && millis() > lastInputCheckTime + 350) {
                 configDriver = false;
                 refresh = 1;
                 setMotorResolution(Reducao, tmcFlag);
                 IniciaMotores();
                 lastInputCheckTime = millis();
              }
           } 
        }
      } 
    }
  } 
}

int leeJoystick(){ // modificado por L.H.Bonani para aumentar a faixa de definição das posições do Joystick
    int x = analogRead(xPin);
    int y = analogRead(yPin);
    int k = digitalRead(kPin);
    if(x>873&&y>449&&y<573){
      joyRead=1;   //x+,0
    }else if(x<124&&y>449&&y<573){
      joyRead=2;  //-x,0
    }else if(y>873&&x>449&&x<573){
      joyRead=3;  //0,+y
    }else if(y<124&&x>449&&x<573){
      joyRead=4;  //0,-y
    }else if(x<124&&y<124){
      joyRead=6; //-x,-y
    }else if(x>873&&y<124){
      joyRead=7; //+x,-y
    }else if(x>873&&y>873){
      joyRead=8; //+x,+y
    }else if(x<124&&y>873){
      joyRead=9; //-x,+y
    }else if(!k){
      joyRead=5; // botão
     // printMyVar("k=",k);
    }else{
      joyRead=0;
    }
  if (joyRead != lastJoyPos){
    lastDebounceTime = millis();
  }
  if(((millis() - lastDebounceTime) > debounceDelay)&&(joyRead!=joyPos)){
    joyPos=joyRead;
    if(!PQCP){
      PQCP=1;
    }
  }
  if(((millis() - lastDebounceTime) > (5*debounceDelay))){
    joyPos=joyRead;                     //repeat time only for UP/DOWN
    if(!PQCP){
      PQCP=1;
    }
  }
  lastJoyPos=joyRead;
}


/* MAPA DE DIRECOES
 *      y-      
    NO  N  NE     
      6 4 7   
 x- O 2 5 1 L x+
      9 3 8   
    SO  S  SE     
        y+      

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
 * 
 */
