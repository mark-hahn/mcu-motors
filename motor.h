
#ifndef MOTOR_H
#define	MOTOR_H

#include <xc.h>
#include "types.h"

// steps are in 1/8 step (bipolar) or one phase (unipolar)
//    for bipolar:
//       steps/rev:        1600
//       dist/rev:           40 mm
//       max distance:      800 mm
//       max step count: 32,000
//
//    for unipolar:
//       steps/rev:        2048
//       dist/rev:           40 mm
//       max distance:      625 mm
//       max step count: 32,000

#ifdef B1
#define NUM_MOTORS 1
#endif
#ifdef B3
#define NUM_MOTORS 3
#endif
#ifdef U6
#define NUM_MOTORS 6
#endif

// global for use in main chk loop
extern uint8  motorIdx;
extern struct motorState      *ms;
extern struct motorSettings   *sv;
extern volatile unsigned char *mp; // motor port (like &PORTA)
extern uint8                   mm; // motor mask (0xf0 or 0x0f or step bit)

extern volatile unsigned char *stepPort[NUM_MOTORS];
extern uint8                   stepMask[NUM_MOTORS];

// faultPort == 0 means no fault pin
extern volatile unsigned char *faultPort[NUM_MOTORS];
extern uint8                   faultMask[NUM_MOTORS];

// limitPort == 0 means no limit switch
extern volatile unsigned char *limitPort[NUM_MOTORS];
extern uint8                   limitMask[NUM_MOTORS];

#define setBiStepLo()           *stepPort[motorIdx] &= ~stepMask[motorIdx]
#define setBiStepHiInt(_motIdx) *stepPort[_motIdx]  |=  stepMask[_motIdx]
  
#define setUniPort(_phase) (*mp = (*mp & ~mm) | motPhaseValue[motorIdx][_phase]);
#define setUniPortInt(_motIdx, _phase)                                   \
  (*stepPort[_motIdx] = (*stepPort[_motIdx] & ~stepMask[_motIdx]) |   \
    motPhaseValue[_motIdx][_phase]);

struct motorState {
  bool   i2cCmdBusy;
  uint8  stateByte;
  int16  curPos;
  int16  targetPos;
  int16  homeTestPos;
  bool   dir;
  bool   targetDir;
  uint8  ustep;  // bipolar only
  uint8  phase;  // unipolar only
  uint16 speed;
  uint16 targetSpeed;
  int8   stepDist;
  bool   stepPending;
  bool   stepped;
  uint16 nextStepTicks;
  uint16 lastStepTicks;
  bool   resetAfterSoftStop;
} mState[NUM_MOTORS];

#define POS_UNKNOWN_CODE -9999 // not homed since lastmotor off

// constants loadable from command
struct motorSettings {
  uint16 maxSpeed;
  uint16 minSpeed;
  uint16 noAccelSpeedLimit;
  uint16 accellerationRate;
  uint16 homingSpeed;
  uint16 homingBackUpSpeed;
  uint16 homeOfs;
  uint16 homePos;
};

#define NUM_SETTING_WORDS 8

union settingsUnion{
  uint16 reg[NUM_SETTING_WORDS];
  struct motorSettings val;
} mSet[NUM_MOTORS];

void motorInit(void);
void chkMotor(void);
bool withinDecellDist(void);
void softStopCommand(bool reset);
void haveFault();        // bipolar only
void setStepPhase(void); // unipolar only
bool limitClosed(void);
void setStep(void);
void stopStepping(void);
void resetMotor(void);
void motorOnCmd(void);
void processMotorCmd(void);
void clockInterrupt(void);
uint16 getLastStep(void);
void   setNextStep(uint16 ticks);

#endif	/* MOTOR_H */

