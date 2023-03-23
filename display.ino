void displayInit(){
   Heltec.display->init();
  // Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->clear();
  Heltec.display->drawString(0,0,"Winch Remote");
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 30, "Waiting for data");
  Heltec.display->display();

}

// TODO: clean this mess up
void displayStatus(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

  // Header info
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "RSSI: " + String(LoRa.packetRssi(), DEC) +"db");  
  Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  if(remoteState.rewind){
    Heltec.display->drawString(128, 0,  "Rewind");
    } else {
    Heltec.display->drawString(128, 0, "Enable: " + String(remoteState.enable, DEC));
  }

  // First Row
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->drawString(64, 9, "Power: " + String(remoteState.throttle, DEC));
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  // Throttle Progress Bar
  Heltec.display->drawProgressBar(0, 21, 127, 5, (remoteState.throttle/ 2.54));

  // Third Row
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->drawString(0, 26, "Line Out:");
  Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  Heltec.display->drawString(127, 27,  String(winchState.lineRemaining)+ "m");
  // Line out Progress Bar
  Heltec.display->drawProgressBar(0, 43, 127, 20, (winchState.lineRemaining/ 10));

  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->setColor(INVERSE);

  if(winchState.lineRemaining > 800){
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->drawString(64, 40, "!! !! !!");
  } else {
    Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  Heltec.display->setFont(ArialMT_Plain_24);

    Heltec.display->drawString(127, 40, "//// ");

  }
  Heltec.display->display();
}