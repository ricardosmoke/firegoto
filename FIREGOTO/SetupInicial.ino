void ConfigurarTipoDriver() {
    if (tmcFlag) {
        sDriver = tmc;
    } else {
        sDriver = dvr;
    }
}

void ExecutarConfiguracaoInicialLED() {
  //Pinos Led RGB
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  digitalWrite(LedR, ledStateR);
  digitalWrite(LedB, ledStateB);
  digitalWrite(LedG, ledStateG);
  //Pinos Joystick
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(kPin, INPUT_PULLUP);
  
  if (ledStateR == LOW) {
    ledStateR = HIGH;
  } else {
    // ledStateR = LOW;
  }
  digitalWrite(LedR, ledStateR);

  Serial.begin(9600);
  Serial3.begin(9600);
  SerialUSB.begin(9600);
  Wire1.begin();  
}

void ExecutarPrimeiraConfiguracao() {  
  /* Flash is erased every time new code is uploaded. Write the default configuration to flash if first time */
  // running for the first time?
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(0); // flash bytes will be 255 at first run
  Serial.print("Primeira Execucao: ");
  
  if (codeRunningForTheFirstTime) {
    Serial.println("yes");
    /* OK first time running, set defaults */
    configuration.Reducao = dReducao;
    configuration.MaxPassoAlt = dMaxPassoAlt;
    configuration.MaxPassoAz = dMaxPassoAz;
    configuration.MinTimer = dMinTimer;
    configuration.latitude = -25.40;;
    configuration.longitude = -49.20;
    configuration.SentidoDEC = 0;
    configuration.SentidoRA = 0;
    setTime(00, 00, 00, 01, 01, 2021);
    MilissegundoSeg = second();
    configuration.DataHora = now();
    configuration.UTC = -3;
    strcpy (configuration.Local, "Minha Casa");
    // write configuration struct to flash at adress 4
    byte b2[sizeof(Configuration)]; // create byte array to store the struct
    memcpy(b2, &configuration, sizeof(Configuration)); // copy the struct to the byte array
    dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
    // write 0 to address 0 to indicate that it is not the first time running anymore
    dueFlashStorage.write(0, 0);
  }
  else {
    Serial.println("no");
  }
}

void ConfigurarTipoReducao() {
  Reducao = configurationFromFlash.Reducao;   
   // Alteração da configuração dos motores em função do tipo de driver (incluído por L. H. Bonani) 
   if(Reducao==64) {
    if (tmcFlag) {
      AltaM0 = LOW;
      AltaM1 = HIGH;
      indexRDVR = 0;
    } else {
      Serial.print("Configuração de 64 micropassos para DVR8825 não suportada. Configurando para 32 micropassos. \n");
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = HIGH;
      indexRDVR = 0;
    }
  }
  if(Reducao==32) {
    if (tmcFlag) {
      AltaM0 = HIGH;
      AltaM1 = LOW;
      indexRDVR = 1;
    } else {
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = HIGH;
      indexRDVR = 0;
    }
  }
  if(Reducao==16) {
    if (tmcFlag) {
      AltaM0 = HIGH;
      AltaM1 = HIGH;
      indexRDVR = 2;
    } else {
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = LOW;
      indexRDVR = 1;
    }
  }
  if(Reducao==8) {
    if (tmcFlag) {
      AltaM0 = LOW;
      AltaM1 = LOW;
      indexRDVR = 3;
    } else {
      AltaM2 = LOW;
      AltaM1 = HIGH;
      AltaM0 = HIGH;
      indexRDVR = 2;
    }
  }
  if(Reducao==4) {
    if (tmcFlag) {
      Serial.print("Configuração de 4 micropassos para TMC2209 não suportada. Configurando para 8 micropassos. \n");
      AltaM0 = LOW;
      AltaM1 = LOW;
      indexRDVR = 3;
    } else {
      AltaM2 = LOW;
      AltaM1 = HIGH;
      AltaM0 = LOW;
      indexRDVR = 3;
    }
  }
  if(Reducao==2) {
    if (tmcFlag) {
      Serial.print("Configuração de 2 micropassos para TMC2209 não suportada. Configurando para 8 micropassos. \n");
      AltaM0 = LOW;
      AltaM1 = LOW;
      indexRDVR = 3;
    } else {
      AltaM2 = LOW;
      AltaM1 = LOW;
      AltaM0 = HIGH;
      indexRDVR = 4;
    }
  }
  vReducao = Reducao;
  mpRDVR = setReducaoDVR(vReducao);
  
}

void DefinirSentidoMotores() {

  SentidoRA = configurationFromFlash.SentidoRA;
  sentidoMotorRA = SentidoRA;
  
  if (sentidoMotorRA == 0) {
    dirRA = "0(Horar)";
  } else {
    dirRA = "1(AntiH)";
  }

  SentidoDEC = configurationFromFlash.SentidoDEC;
  sentidoMotorDEC = SentidoDEC;

  if (sentidoMotorDEC == 0) {
    dirDEC = "0(Horar)";
  } else {
    dirDEC = "1(AntiH)";
  }
}

void ConfigurarLocalizacao() {
  latitude = configurationFromFlash.latitude;
  longitude = configurationFromFlash.longitude;
  UTC = configurationFromFlash.UTC;
}

void ConfigurarReducao() {
  MaxPassoAlt = configurationFromFlash.MaxPassoAlt;
  setDECmicroSteps(MaxPassoAlt);
  mpDEC = array2String(micropassosDEC);
  
  MaxPassoAz = configurationFromFlash.MaxPassoAz;
  setRAmicroSteps(MaxPassoAz);
  mpRA = array2String(micropassosRA);

  dReducao = Reducao;  
  dMaxPassoAlt = MaxPassoAlt;
  dMaxPassoAz =  MaxPassoAz;
  MinTimer = configurationFromFlash.MinTimer;
  setVelocidadeMotores(MinTimer - 200);
  mpVMot = array2String3(velocidadeMotor);
}

void DefinirPosicaoInicial() {
  ALTmountAlvo = dMaxPassoAlt / 4;
  syncro();
}

void DefinirUsoLCD() {
  if (possuiLCD) {
    lcd.begin(Wire1); 
    lcd.backlight();
    lcd.clear();
  }
}

void LerMemoriaFlash() {
  byte* b = dueFlashStorage.readAddress(4); // byte array which is read from flash at adress 4
  memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct
}

void DefinirResolucaoEixos() {
  ResolucaoeixoAltGrausDecimal = 360.0 / MaxPassoAlt ;
  ResolucaoeixoAzGrausDecimal = 360.0 / MaxPassoAz ;
  ResolucaoeixoAltPassoGrau = (MaxPassoAlt  / 360.0);
  ResolucaoeixoAzPassoGrau = (MaxPassoAz  / 360.0);
  ratepadrao = (int)(MaxPassoAz / 86400);
}