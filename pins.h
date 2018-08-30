  
#ifndef PINS_H
#define	PINS_H

#ifdef BM
#define SCL_TRIS  TRISC0
#define SDA_TRIS  TRISC1

#define dirTRIS   TRISA2
#define ms1TRIS   TRISC2
#define ms2TRIS   TRISB4
#define ms3TRIS   TRISB5
#define resetTRIS TRISB6

#define dirLAT    LATA2
#define ms1LAT    LATC2
#define ms2LAT    LATB4
#define ms3LAT    LATB5
#define resetLAT  LATB6

#else

#define SCL_TRIS  TRISD0
#define SDA_TRIS  TRISD1
#endif /* BM */

#ifdef B1
#define stepTRIS  TRISC5
#define stepLAT   LATC5
#define stepPORT  PORTC
#define stepMASK  0x20

#define faultTRIS TRISA5
#define faultPORT PORTA
#define faultMASK  0x80

#define limitTRIS TRISA4
#define limitPORT PORTA
#define limitMASK  0x10
#endif /* B1 */

#ifdef B3
#define stepRTRIS  TRISC5
#define stepETRIS  TRISC4
#define stepXTRIS  TRISC3

#define stepRLAT   LATC5
#define stepELAT   LATC4
#define stepXLAT   LATC3

#define stepRPORT  PORTC
#define stepRBIT   0x20
#define stepEPORT  PORTC
#define stepEBIT   0x10
#define stepXPORT  PORTC
#define stepXBIT   0x08

#define faultRTRIS  TRISC6
#define faultETRIS  TRISC7
#define faultXTRIS  TRISB7

#define faultRLAT   LATC6
#define faultELAT   LATC7
#define faultXLAT   LATB7

#define faultRPORT  PORTC
#define faultRBIT   0x40
#define faultEPORT  PORTC
#define faultEBIT   0x80
#define faultXPORT  PORTB
#define faultXBIT   0x80

#define limitRTRIS  TRISA5
#define limitXTRIS  TRISA4

#define limitRPORT  PORTA
#define limitRBIT   0x20
#define limitXPORT  PORTA
#define limitXBIT   0x10
#endif	/* B3 */

#ifdef U6
#define motAPORT   PORTA  // tube 1
#define motBPORT   PORTB  // tube 2
#define motCPORT   PORTC  // tube 3
#define motPPORT   PORTD  // paster
#define motZPORT   PORTA  // camera Z
#define motFPORT   PORTC  // focus

#define motAOFS    0
#define motBOFS    0
#define motCOFS    0
#define motPOFS    4
#define motZOFS    4
#define motFOFS    4

#define limitZTRIS TRISE0
#define limitZPORT PORTE
#define limitZBIT  0x01

#define led1TRIS   TRISE1
#define led2TRIS   TRISE2
#define led3TRIS   TRISD2
#define led4TRIS   TRISD3
 
#define led1LAT    LATE1
#define led2LAT    LATE2
#define led3LAT    LATD2
#define led4LAT    LATD3
 
#define tp1TRIS    TRISB4
#define tp1LAT     LATB4
#define tp2TRIS    TRISB5
#define tp2LAT     LATB5
#endif

#endif	/* PINS_H */
