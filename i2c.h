#ifndef I2C_H
#define	I2C_H

#include <xc.h>
#include "types.h"
#include "motor.h"

// ===== HARDWARE =====
//
// One PIC mcu can host multiple motors -- each motor has its own i2c addr
// Each motor state is independent (except for error which stops entire mcu)
//
// At the time of this writing the code can be built for:
//     One bipolar motor     (define BM and B1) (PIC16F15345)
//     Three bipolar motors  (define BM and B3) (PIC16F15345)
//     Six unipolar motors   (define U6)        (PIC16F15375)
// The PIC models can be different. The MCU pins are set in pins.h.
//
// This MCU code has a specific meaning for a step.
// For bipolar it is a 1/8 micro-step and for unipolar it is one of four phases
//
// If using a 20-tooth GT2 pulley, then one step is 1/40 mm for bipolar
// and 1/50 mm for unipolar motor (28BYJ-48)
// steps are 1/8 micro-step (bipolar) or one phase (unipolar)
//    for bipolar: (typical 200 step motor and 20-tooth GT2 pulley)
//       steps/rev:        1600
//       dist/rev:           40 mm
//       max step count: 32,767
//       max distance:      800 mm
//    for unipolar:  (28BYJ-48 and 20-tooth GT2 pulley)
//       steps/rev:        2048
//       dist/rev:           40 mm
//       max step count: 32,767
//       max distance:      625 mm
// all speed is in steps/sec (except speed-move cmd)
// acceleration is 0..7: none, 8000, 16000, 24000, 32000, 40000, 50000, 60000
// for 1/40 mm steps: none, 200, 400, 600, 800, 1000, 1250, 1500 mm/sec/sec
// move, home, and stop commands start immediately even when motor is busy
//
// ===== SOFTWARE =====

//   Each MCU is an I2C slave.  Each motor in the MCU has its own I2C address.
//   Any I2C write to MCU is a command.  Any read returns 4-byte status.
// 
//   -- I2C addresses --
//   Config for one bipolar motor    (BM,B1) uses 0x08
//   Config for three bipolar motors (BM,B3) uses 0x10, 0x11, 0x12
//   Config for six unipolar motors  (U6)    uses 0x18 ... 0x1d
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
//   -- 2-byte move command --
//   1aaa aaaa     top 7 bits of move addr
//      aaaa aaaa  bottom 8 bits

//   -- 3-byte speed-move command --
//   01ss ssss     set speed setting to value s times 256
//     0aaa aaaa   top 7 bits of move addr,
//     aaaa aaaa   bottom 8 bits
//
//   -- 5-byte accel-speed-move command --  
//   0000 1ccc    set acceleration idx setting
//     ssss ssss  top 8 bits of speed,
//     ssss ssss  bottom 8 bits
//     0aaa aaaa  top 7 bits of move addr,
//     aaaa aaaa  bottom 8 bits
//
//   -- 3-byte to 19-byte settings command --
//   write may be short, only setting first entries
//   0001 1111  load settings, 16-bit values
//      acceleration rate table index 0..7 (steps/sec/sec), 0 is off
//      default speed
//      no-acceleration speed limit (and start speed when stopped)
//      max pos     (min pos is always zero))
//      homing speed
//      homing back-up speed
//      home offset distance
//      home pos value (set cur pos to this value after homing, usually 0)
//      limit switch controls
//
//   -- 1 byte limit sw controls --/
//   in settings above
//   zero-byte is normal behavior
//   00sslee
//      ss  start dir: 0: rev, 1: fwd, 2: limit sw, 3: not limit sw
//       l  switch pin active voltage, 0: lo,  1: hi
//      ee  force ending side: 0: no, 1: sw pin active, 2: sw pin not active
//
// -- 4-byte status read --
// this is the only read format on i2c -- always 4 bytes including cksum
// Error code and error bit are cleared on status read, only on addressed motor
//   1) vccc eboz  state byte
//        v: version (1-bit)
//      ccc: error code (see above) (only set on specific motor causing error)
//        e: error bit              (set on all motors when any error in mcu)
//        b: busy state
//        o: motor on (not in reset)
//        z: homed
//   2) aaaa aaaa  current position, top 8 bits (might be result of cmd 0x11)
//   3) aaaa aaaa  followed by bottom 8 bits
//   4) cccc cccc  8-bit cksum, sum of first 3 bytes
//
// motor position is stored when homing first closes limit switch
// Command 0x11 tells next status read to return this test position
// This status read will have a magic state byte value of 0x04
//
// Error codes for state byte above 
//   MOTOR_FAULT_ERROR   0x10  // over-heated or over-current driver chip
//   I2C_OVERFLOW_ERROR  0x20  // i2c byte received before last taken
//   CMD_DATA_ERROR      0x30  // command format incorrect
//   CMD_NOT_DONE_ERROR  0x40  // new command arrived before old cmd finished
//   STEP_NOT_DONE_ERROR 0x50  // step rate too fast for MCU
//   BOUNDS_ERROR        0x60  // position < 0 or > max setting when moving
//   NOT_HOMED_ERROR     0x70  // attempt to do a move when not homed yet
//


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
