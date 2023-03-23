#include "heltec.h" 
#include "Wire.h"
#include "buttons.h"
#include "messages.h"
#include "settings.h"

#define SCREEN_REFRESH_RATE 100
#define TELEMTRY_RATE 1000
#define RADIO_TIMEOUT 500
#define MESSAGE_RATE 250
#define BAND    915E6  

struct RemoteMessage remoteMessage;

struct BaseMessage baseMessage;
uint8_t debugVal = 0;
struct WinchState{
  int8_t speed;
  uint8_t direction;
  uint8_t throttle;
  uint8_t enable;
  uint8_t error;
  int16_t lineRemaining;
  uint8_t batteryLevel;
  uint64_t lastRemoteMessageTime;
  uint64_t lastBaseMessageTIme;
} winchState;

struct RemoteState{
  uint8_t rewind;
  uint8_t throttle;
  uint8_t enable;
  uint8_t error;
  struct ButtonStates buttonStates;
  uint8_t waitForTelemety;
  uint8_t newMessage;
  uint64_t lastTelemetry;
} remoteState;

uint64_t lastScreenRefresh = 0;
int lastPacketSize = 0;


void setup() { 
  remoteState.enable = remoteState.rewind = remoteState.throttle = 0;
  // Ensure throttle is off!  
  // Start the board
  Heltec.begin(true , true , false , true , BAND);

  // Setup Radio
  radioInit();

  // Setup Display
  displayInit();

  // Let things settle
  delay(100);

  // Setup buttons
  buttonsInit();

  delay(100);
}


void loop() {
  if(millis() - lastScreenRefresh > SCREEN_REFRESH_RATE){
    lastScreenRefresh = millis();
    displayStatus();
  }

  // Button Check
  getButtons();
  if(remoteState.buttonStates.buttonEnable == true){
    remoteState.enable = true;
    remoteState.rewind = false;

  } else {
    remoteState.enable = false;
    remoteState.rewind = remoteState.buttonStates.buttonUp;
  }

/* Uncomment if you want to add more button things
  // if(remoteState.buttonStates.buttonUp && !remoteState.buttonStates.buttonUpUsed){
  //   remoteState.buttonStates.buttonUpUsed = true;
  //   remoteState.rewind = true;
  // }
  // if(remoteState.buttonStates.buttonDown && !remoteState.buttonStates.buttonDownUsed){
  //   remoteState.buttonStates.buttonDownUsed = true;
  //   // DO BUTTON THINGS
  // }
  // if (remoteState.buttonStates.buttonSelect && !remoteState.buttonStates.buttonSelectUsed) {
  //   remoteState.buttonStates.buttonSelectUsed = true;
  //   // DO BUTTON THINGS
  // }
*/

  // Messaging
  // Get a telemetry packet at the desired rate
  if(millis() - remoteState.lastTelemetry > TELEMTRY_RATE) {
    delay(5); // so we don't overload the lora module
    requestTelemetry();
    radioRecieve();
    remoteState.waitForTelemety = true;
  }

  // Listen if we are waiting for a telemtry packet
  if(remoteState.waitForTelemety){
    if(millis() - remoteMessage.time > RADIO_TIMEOUT)
    {
      // Timeout if a long time has passed since sending the request packet
      remoteState.waitForTelemety = false;
    } else if(remoteState.newMessage) {
      // We recieved a message, update state
      winchState.lineRemaining = baseMessage.lineRemaining;
      winchState.batteryLevel = baseMessage.batteryLevel;
      winchState.lastBaseMessageTIme = baseMessage.time;
      winchState.speed = baseMessage.speed;
      winchState.direction = baseMessage.direction;

      // Go back to tx mode
      remoteState.waitForTelemety = false;
    }
  } else {
      // Send message at the set interval
      if(millis() - remoteMessage.time > MESSAGE_RATE ) {
      sendMessage();
      }
    }  
  
}

// Stop the winch now
void eStop(){
  remoteState.throttle = 0;
  remoteState.enable = false;
  remoteState.error = ERR_ESTOP;
}



