#ifndef I2C_H
#define	I2C_H

#include <xc.h>
#include "types.h"
#include "motor.h"

// move/home commands start immediately even when already busy
// acceleration is 0..7: 4000, 8000, 16000, 24000, 32000, 40000, 50000, 60000
// all position and distance are in steps (bi: 1/8 ustep, uni: phase)
// all speed is in steps/sec (except speed-move cmd)
// homing with no limit switch just sets current position to settings value

// steps are in 1/8 ustep (bipolar) or one phase (unipolar)
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
//
// writes ...
//   (first word of recv buffer is buf len)
//
//   -- move ommands --
//   1aaa aaaa  goto command, top 7 bits of goto addr
//      aaaa aaaa followed by bottom 8 bits
//   01ss ssss (speed-move cmd) set max speed = s*256 steps/sec and move to addr
//     0aaa aaaa top 7 bits of move addr
//     aaaa aaaa bottom 8 bits
//
//   -- one-byte commands --
//   0001 0000  start homing
//   0001 0001  next read position is end position of homing (test pos)
//   0001 0010  soft stop, deccelerates, no reset
//   0001 0011  soft stop, deccelerates first, then reset
//   0001 0100  hard stop (immediate reset)
//   0001 0101  motor on (hold place, reset off)
//   0001 0110  set curpos to home pos value setting (fake homing)
//
//   -- 21 byte settings command --
//   0001 1111  load settings, 16-bit values
//      acceleration rate table index 0..7 (steps/sec/sec), 0 is off
//      default speed
//      no-acceleration speed limit (and start speed when stopped)
//      max pos     (min pos is always zero))
//      homing speed
//      homing decelleration rate table index 0..7 
//      homing back-up speed
//      home offset distance
//      home pos value (set cur pos to this value after homing, usually 0)
//      limit sw controls

// -- 4-byte state response --
// error code and bit cleared on status read, only on motor being read
// Error codes 
//   MOTOR_FAULT_ERROR   0x10
//   I2C_OVERFLOW_ERROR  0x20
//   CMD_DATA_ERROR      0x30
//   CMD_NOT_DONE_ERROR  0x40
//   STEP_NOT_DONE_ERROR 0x50
//   BOUNDS_ERROR   0x60
//   NOT_HOMED_ERROR     0x70
//
// state response bytes
//   1) vccc eboz  state byte
//      v: version (1-bit)
//    ccc: error code (see above) (only set on specific motor causing error)
//      e: error bit              (set on all motors when any error in mcu)
//      b: busy state
//      o: motor on (not in reset)
//      z: at home
//   2) aaaa aaaa  current position, top 8 bits (might be result of cmd 0x11)
//   3) aaaa aaaa  followed by bottom 8 bits
//   4) cccc cccc  8-bit cksum, sum of first 3 bytes


#define RECV_BUF_SIZE   (NUM_SETTING_WORDS*2 + 1) // + opcode byte
#define NUM_SEND_BYTES   4  //  state, posH, posL, cksum

#define I2C_ADDR_MASK 0xf0 // motor idx in d3-d1 (d2-d0 in real addr)

#ifdef B1
#define I2C_ADDR      0x10  // real addr:0x08, ext box mcu (B1), motor always 0
#endif
#ifdef B3
#define I2C_ADDR      0x20  // real addr:0x10, head mcu (B3)for bipolar motors
#endif
#ifdef U6
#define I2C_ADDR      0x30  // real addr:0x18, head mcu (U6) for unipolar motors
#endif

extern volatile uint8 i2cRecvBytes[NUM_MOTORS][RECV_BUF_SIZE + 1];
extern volatile uint8 i2cRecvBytesPtr;
extern volatile uint8 i2cSendBytes[NUM_SEND_BYTES];
extern volatile uint8 i2cSendBytesPtr;

void i2cInit(void);
void i2cInterrupt(void);

#endif	/* I2C_H */
