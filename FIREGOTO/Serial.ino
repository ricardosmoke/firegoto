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
//updates
/*void lebuffercomand()
  {
  if (pontBufferold != pontBuffer)
  {

    inputcmd[numCommand] = buffercmd[pontBufferold];
    if (inputcmd[numCommand] == ':' && inputcmd[1] != 'S')
    {
      numCommand = 0;
      inputcmd[numCommand] = buffercmd[pontBufferold];
      //  printe();
    }
    if (inputcmd[numCommand] == ' ' )
    {
      numCommand = numCommand - 1;
    }

    if (inputcmd[numCommand] == '#' )
    {
      cmdComplete = true;
      numCommand = 0;
      //     printe();
    }
    pontBufferold = pontBufferold + 1;
    numCommand = numCommand + 1;
    if (pontBufferold > 29)
    {
      pontBufferold = 0;
    }

  }
  }*/
void SerialPrint(String str)
{
  Serial.print(str);
  Serial3.print(str);
  SerialUSB.print(str);
  ledStateB = HIGH;
}

/*
  void serialEvent3() {
  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read();
    if (inChar != ' ' )
    {
      pontBuffer = pontBuffer + 1;
      buffercmd[pontBuffer] = inChar;
    }
    if (inChar == ':' && buffercmd[1] != 'S')
    {
      pontBuffer = 0;
      buffercmd[pontBuffer] = inChar;
    }
    if (inChar == '#' &&  pontBuffer > 0 )
    {
      cmdComplete = true;
      pontBuffer = pontBuffer + 1;
      buffercmd[pontBuffer] = inChar;
      memcpy(inputcmd, buffercmd, pontBuffer);
      pontBuffer = 0;
      buffercmd[1] = '@';
      executecommand();
    }
    if (pontBuffer > 29)
    {
      pontBuffer = 0;
    }
  }
  }
*/

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar != ' ' )
    {
      Command[numCommand][pontBuffer] = inChar;
      pontBuffer = pontBuffer + 1;
    }
    if (inChar == ':'  && Command[numCommand][1] != 'S')
    {
      pontBuffer = 0;
      Command[numCommand][0] = inChar;
      pontBuffer = pontBuffer + 1;
    }
    if (inChar == '#')
    {
      pontBuffer = 0;
      numCommand = numCommand + 1;
    }
  }

  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read();
    if (inChar != ' ' )
    {
      Command[numCommand][pontBuffer] = inChar;
      pontBuffer = pontBuffer + 1;
    }
    if (inChar == ':'  && Command[numCommand][1] != 'S')
    {
      pontBuffer = 0;
      Command[numCommand][0] = inChar;
      pontBuffer = pontBuffer + 1;
    }
    if (inChar == '#')
    {
      pontBuffer = 0;
      numCommand = numCommand + 1;
    }
  }

  while (SerialUSB.available()) {
    // get the new byte:
    char inChar = (char)SerialUSB.read();
    if (inChar != ' ' )
    {
      Command[numCommand][pontBuffer] = inChar;
      pontBuffer = pontBuffer + 1;
    }
    if (inChar == ':'  && Command[numCommand][1] != 'S')
    {
      pontBuffer = 0;
      Command[numCommand][0] = inChar;
      pontBuffer = pontBuffer + 1;

    }
    if (inChar == '#' )
    {
      pontBuffer = 0;
      numCommand = numCommand + 1;
    }
  }
  if (numCommand > 14 )
  {
    numCommand = 0;
  }
  if (pontBuffer > 14)
  {
    pontBuffer = 0;
  }
}
void SerialPrintDebug(String str)
{
  if (flagDebug == 1)
  {
    Serial.println(str);
    // SerialUSB.println(str);
  }
}
