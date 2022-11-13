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
* Get a double value from the serial port, and send the '_OK_' ack string.
* The value must conains six decimals, in a string with 9 bytes, including sign and decimal dot.
* Examples: '-0.036526', '+5.238388'
*
* \return double.
*/

#include <math.h>

String readString; //armazena informaÃƒÂ§ÃƒÂµes mesmo com acompanhamento ativo
char informacao;
char posicao;

/*
 * Get a int value from the serial port, and send the '_OK_' ack string.
 * Examples: '-00', '+05' or 001 102
 *
 * \return int.
 */

int serialGet04dig() {
  if (readString.length() < 4) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }
  }
  if (readString.length() == 4) {
    int r = readString.toInt();
    readString = "";
    return (r);
  } else {
    return (-2);
  }

}

/*
 * Get a int value from the serial port, and send the '_OK_' ack string.
 * Examples: '-00', '+05' or 001 102
 *
 * \return int.
 */

int serialGet03dig() {
  if (readString.length() < 3) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }
  }
  if (readString.length() == 3) {
    int r = readString.toInt();
    readString = "";
    return (r);

  } else {
    return (-2);
  }

}

/*
 * Get a int value from the serial port, and send the '_OK_' ack string.
 * Examples: '00', '05'
 * not signal
 *
 * \return int.
 */
int serialGet02dig() {
  if (readString.length() < 2) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }
  }
  if (readString.length() == 2) {
    int r = readString.toInt();
    readString = "";
    return (r);
  } else {
    return (-2);
  }
}
/*
 * Get a int value from the serial port, and send the '_OK_' ack string.
 * Examples: '-', '+'
 * not signal
 *
 * \return int 1 ou -1.
 */
int serialGetsignal() {
  if (readString.length() < 1) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }
  }
  if (readString.length() == 1) {
    if (readString == "-" || readString == "S" || readString == "s" || readString == "O" || readString == "o") {
      readString = "";
      return (-1);
    } else {
      readString = "";
      return (1);
    };
  } else {
    return (-2);
  }
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

double modulo(double digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  if (  digits < 0) {
    digits = digits * (-1);
  }
  return (digits);
}

int LeHHMMSS(int *HH, int *MM, int *SS) //Pede e LÃƒÅ  Hora MM SS
{ char fim = 0;
  if (posicao == 0)
  {
    Serial.println("Insira a cordenada AR do objeto: HH MM SS");
    Serial.print("hora = ");
    posicao = -1;
    return (fim);

  }
  if (informacao == 0) {
    *HH = serialGet02dig();
    if (*HH != -2) {
      posicao = 1;
      informacao = 1;
      Serial.println(*HH);
      return (fim);

    }
  }

  if (posicao == 1)
  {
    Serial.print("minuto = ");
    posicao = -1;
    return (fim);

  }
  if (informacao == 1) {
    *MM = serialGet02dig();
    Serial.print("FIM = "); Serial.println(*MM);

    return (fim);


    if (*MM != -2) {
      posicao = 2;
      informacao = 2;
      Serial.println(*MM);
      return (fim);

    }
  }

  if (posicao == 2)
  {
    Serial.print("segundo = ");
    posicao = -1;
    return (fim);

  }

  if (informacao == 2) {
    *SS = serialGet02dig();
    if (*SS != -2) {
      Serial.println(*SS);
      informacao = 0;
      posicao = 0;
      fim = 1;
      return (fim);
    }
  }
}


int LeGGMMSS(int *GG, int *MM, int *SS) //Pede e LÃƒÅ  Grau MM SS
{
  if (posicao == 0)
  {
    Serial.println("Insira a cordenada AR do objeto: HH MM SS");
    Serial.print("hora = ");
    return (0);
    posicao = -1;
  }
  if (informacao == 0) {
    *GG = serialGet02dig();
    if (*GG != -2) {
      posicao = 1;
      informacao = 1;
      Serial.println(*GG);
    }
  }

  if (posicao == 1)
  {
    Serial.print("minuto = ");
    posicao = -1;
  }
  if (informacao == 1) {
    *MM = serialGet02dig();
    if (*MM != -2) {
      posicao = 2;
      informacao = 2;
      Serial.println(*MM);
    }
  }

  if (posicao == 2)
  {
    Serial.print("segundo = ");
    posicao = -1;
  }
  if (informacao == 2) {
    *SS = serialGet02dig();
    if (*SS != -2) {
      Serial.println(*SS);
      informacao = 0;
      posicao = 0;
      return (1);
    }
  }
}

void printMyVar(String myVarName, int myVarValue) {
  String myString = " \n O valor atual da variável "+myVarName+" é ";
  SerialPrint(myString);SerialPrint(String(myVarValue));
}

void printMyVarf(String myVarName, float myVarValue) {
  String myString = " \n O valor atual da variável "+myVarName+" é ";
  SerialPrint(myString);SerialPrint(String(myVarValue));
}

int LeHHDD(int *HH, int *DD) //Pede e LÃƒÅ  Hora e fraÃƒÂ§ÃƒÂ£o decimal da hora
{
}

int LeGGDD(int *GG, int *DD) //Pede e LÃƒÅ  Grau e fraÃƒÂ§ÃƒÂ£o decimal de Grau
{
}


int LeGGDD(int *DD, int *MM, int *AAAA) //Pede e LÃƒÅ  Grau e fraÃƒÂ§ÃƒÂ£o decimal de Grau
{
  if (posicao == 0)
  {
    Serial.println("Insira a dia: DD MM AAAA");
    Serial.print("DIa = (DD)");
    posicao = -1;
  }
  if (informacao == 0) {
    *DD = serialGet02dig();
    if (*DD != -2) {
      posicao = 1;
      informacao = 1;
      Serial.println( *DD);
    }
  }
  if (posicao == 1)
  {
    Serial.print("Mes = (MM)");
    posicao = -1;
  }
  if (informacao == 1) {
    *MM = serialGet02dig();
    if (*MM != -2) {
      posicao = 2;
      informacao = 2;
      Serial.println(*MM);
    }
  }
  if (posicao == 2)
  {
    Serial.print("Ano = (AAAA)");
    posicao = -1;
  }
  if (informacao == 2) {
    *AAAA = serialGet04dig();
    if (*AAAA != -2) {
      posicao = 0;
      informacao = 0;
      Serial.println( *AAAA);
      return (1);
    }
  }
}
int LeSinal()
{
  int sinal = -2;
  if (posicao == 0)
  {
    Serial.println("Insira o hemisferio");
    Serial.print("(N/S ou +/-)");
    posicao = -1;
  }
  if (informacao == 0) {
    sinal = serialGetsignal();
    if (sinal != -2) {
      posicao = 0;
      informacao = 0;
      return (sinal);

    }
  }
}
void EnderecoLCD()
{
  byte LCDerror, LCDaddress;
  int LCDnDevices;

  Serial.println("Buscando LCD...");

  LCDnDevices = 0;
  for(LCDaddress = 1; LCDaddress < 127; LCDaddress++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire1.beginTransmission(LCDaddress);
    LCDerror = Wire1.endTransmission();

    if (LCDerror == 0)
    {
      Serial.print("I2C encontrado no endereco 0x");
      if (LCDaddress<16) 
        Serial.print("0");
      Serial.print(LCDaddress,HEX);
      Serial.println("  !");

      LCDnDevices++;
    }
    else if (LCDerror==4) 
    {
      Serial.print("Erro desconhecido no endereco 0x");
      if (LCDaddress<16) 
        Serial.print("0");
      Serial.println(LCDaddress,HEX);
    }    
  }
  if (LCDnDevices == 0)
    Serial.println("Nenhum LCD encontrado\n");
  else
    Serial.println("Finalizado\n");

  delay(1000);           // wait 5 seconds for next scan
}

void setRAmicroSteps(int number) {
  char cstr[9];
  sprintf(cstr, "%08d", number);
  for (unsigned int i = 0; i < 8; i++) {
    micropassosRA[i] = cstr[i]-'0';
  }
}

void setDECmicroSteps(int number) {
  char cstr[9];
  sprintf(cstr, "%08d", number);
  for (unsigned int i = 0; i < 8; i++) {
    micropassosDEC[i] = cstr[i]-'0';
  }
}

String setReducaoDVR(int number) {
  char cstr[4];
  sprintf(cstr, "%03d", number);
  String str(cstr);
  //Serial.println(str);
  return str;
}

void setVelocidadeMotores(int number) {
  char cstr[4];
  sprintf(cstr, "%03d", number);
  for (unsigned int i = 0; i < 3; i++) {
    velocidadeMotor[i] = cstr[i]-'0';
  }
}

String array2String(int myArray[]) {
  String str = "";
  for (int i = 0; i < sizeMS; i++) {
    str = str + String(myArray[i]);
  }
  return str;
}

String array2String3(int myArray[]) {
  String str = "";
  for (int i = 0; i < sizeSD; i++) {
    str = str + String(myArray[i]);
  }
  return str;
}

String int2string8(int number) {
  String numStr = String(number);
  int slength = numStr.length(); 
  if (slength != 8) {
    for (int i = 0; i < 8 - slength; i++) {
      numStr = "0" + numStr;
    }
  }
  return numStr;
}

void LCDClockDisplay() {
  // digital clock display of the time
  
  lcd.setCursor(0,1);
  
  if(day()<10){
    lcd.print("0");
    lcd.setCursor(1,1);
    lcd.print(day());
  }
  else { 
  lcd.print(day());
  }
  lcd.setCursor(2,1);
  lcd.print("/");
  lcd.setCursor(3,1);
  if(month()<10){
    lcd.print("0");
    lcd.setCursor(4,1);
    lcd.print(month());
  }
  else { 
    lcd.print(month());
  }
  lcd.setCursor(5,1);
  lcd.print("/");
  lcd.setCursor(6,1);
  lcd.print(year());
  lcd.setCursor(10,1);
  lcd.print(" ");
  lcd.setCursor(12,1);
    
  if (timeFlag == false) { // changed by L.H.Bonani in order to show local time instead of UTC
    thisHour =  hour();
  } else {
    if (hour() + UTC < 0) {
       thisHour =  24 + hour() + UTC;
    } else {
       thisHour =  hour() + UTC;
    }
  }
  if(thisHour<10){
    lcd.print("0");
    lcd.setCursor(13,1);
    lcd.print(thisHour);
  }
  else {
    lcd.print(thisHour);
  }
  lcd.setCursor(14,1);
  lcd.print(":");
  lcd.setCursor(15,1);
  if(minute()<10){
    lcd.print("0");
    lcd.setCursor(16,1);
    lcd.print(minute());
  }
  else {
    lcd.print(minute());
  }
  lcd.setCursor(17,1);
  lcd.print(":");
  lcd.setCursor(18,1);
  if(second()<10){
    lcd.print("0");
    lcd.setCursor(19,1);
    lcd.print(second());
  }
  else {
    lcd.print(second());
  }
}
