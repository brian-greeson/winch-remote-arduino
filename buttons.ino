#include "settings.h"
#include "buttons.h"

#define DEBOUNCE_THRESHOLD 10

struct DebounceCounts debounceCounts;
struct ButtonStates   debounceStates;

void buttonsInit(){
  pinMode(BUTTON_ENABLE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(POT_THROTTLE, INPUT);

  remoteState.buttonStates.buttonDown = remoteState.buttonStates.buttonUp = remoteState.buttonStates.buttonSelect = remoteState.buttonStates.buttonEnable = 0;
  debounceCounts.buttonDownTime = debounceCounts.buttonUpTime = debounceCounts.buttonSelectTime = debounceCounts.buttonEnableTime = 0;

  while( !digitalRead(BUTTON_REWIND_PIN) || !digitalRead(BUTTON_ENABLE_PIN)){
    // Hang if enable or rewind buttons are active during boot!! (ask me how I know)
    // TODO: ALert user of the fault.
  }
}

void getButtons(){
  remoteState.throttle = getThrottleReading();
  remoteState.buttonStates.buttonEnable = getEnableButton();
  remoteState.buttonStates.buttonUp = getUpButton();
  remoteState.buttonStates.buttonDown= getDownButton();
  remoteState.buttonStates.buttonSelect= getSelectButton();

  if(remoteState.buttonStates.buttonEnable == false){
      remoteState.buttonStates.buttonEnableUsed = false;
  }
  if(remoteState.buttonStates.buttonUp == false){
      remoteState.buttonStates.buttonUpUsed = false;
  }
  if(remoteState.buttonStates.buttonDown  == false){
      remoteState.buttonStates.buttonDownUsed = false;
  }
  if(remoteState.buttonStates.buttonSelect  == false){
      remoteState.buttonStates.buttonSelectUsed = false;
  }

  
}

uint8_t getThrottleReading(){
  uint16_t raw = analogRead(POT_THROTTLE);

  uint8_t ret = map(raw, 0, 4095 ,0,255);
  return ret ;
}

uint8_t getEnableButton(){
  uint64_t timeNow = millis();
  uint8_t ret = 0;
  bool btnState = !digitalRead(BUTTON_ENABLE_PIN);

  if (btnState != debounceStates.buttonEnable) {
    debounceCounts.buttonEnableTime = timeNow;
    }


  if ((timeNow - debounceCounts.buttonEnableTime) > DEBOUNCE_THRESHOLD) {
   ret = btnState;
   }
 debounceStates.buttonEnable = btnState;

  return ret;
}

uint8_t getDownButton(){
  uint64_t timeNow = millis();
  uint8_t ret = 0;
  bool btnState = !digitalRead(BUTTON_DOWN_PIN);

  if (btnState != debounceStates.buttonDown) {
    debounceCounts.buttonDownTime = timeNow;
    }
  if ((timeNow - debounceCounts.buttonDownTime) > DEBOUNCE_THRESHOLD) {
   ret = btnState;
   }
  debounceStates.buttonDown = btnState;

  return ret;
}

uint8_t getUpButton(){
  uint64_t timeNow = millis();
  uint8_t ret = 0;
  bool btnState = !digitalRead(BUTTON_UP_PIN);


  if (btnState != debounceStates.buttonUp) {
    debounceCounts.buttonUpTime = timeNow;
    }


  if ((timeNow - debounceCounts.buttonUpTime) > DEBOUNCE_THRESHOLD) {
   ret = btnState;
   }
  debounceStates.buttonUp = btnState;

  return ret;
}

uint8_t getSelectButton(){
  uint64_t timeNow = millis();
  uint8_t ret = 0;
  bool btnState = !digitalRead(BUTTON_SELECT_PIN);


  if (btnState != debounceStates.buttonSelect) {
    debounceCounts.buttonSelectTime = timeNow;
    }


  if ((timeNow - debounceCounts.buttonSelectTime) > DEBOUNCE_THRESHOLD) {
   ret = btnState;
   }
  debounceStates.buttonSelect = btnState;

  return ret;
}




