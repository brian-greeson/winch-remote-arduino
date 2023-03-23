#ifndef BUTTONS_H
#define BUTTONS_H

struct ButtonStates{
  bool buttonUp;
  bool buttonSelect;
  bool buttonDown;
  bool buttonEnable;
  bool buttonUpUsed;
  bool buttonSelectUsed;
  bool buttonDownUsed;
  bool buttonEnableUsed;
};

struct DebounceCounts{
  bool buttonUpState;
  bool buttonDownState;
  bool buttonSelectState;
  bool buttonEnableState;
  uint64_t buttonUpTime;
  uint64_t buttonSelectTime;
  uint64_t buttonDownTime;
  uint64_t buttonEnableTime;
} ;

#endif