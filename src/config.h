/*******************************************************************************
 * Config file for Egret Light Controller board rev. A                         *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef CONFIG_H_
#define CONFIG_H_

/***********************************
 * IO Ports                        *
 ***********************************/
#define IO_DDRA                 0xFE    // PortA [1..7] unused out, 0 sensor input
#define IO_DDRB                 0x1F    // PortB [0..4] unused out, 5 mosi/tx1 -> PD3 in, 6 miso/rx0 -> PD0 in, 7 sck/rx1 -> PD2 in
#define IO_DDRC                 0xFF    // PortC [0..6] unused out 7 switch out
#define IO_DDRD                 0x8C    // PortD [0,1] UART0 in, [2,3] UART1 unused out, [4..6] LED1-3 out, 7 unused out

/* led definition for led.h */
#define PORT_LED0               PORTC
#define PORT_LED1               PORTD
#define PORT_LED2               PORTD
#define PORT_LED3               PORTD
#define PIN_LED0                7       // light switch, simply treat it as led
#define PIN_LED1                4       // Pwr - blue
#define PIN_LED2                5       // Act - green
#define PIN_LED3                6       // Out - red

#define LED_NUM                 4

/* to be used with led_on, led_off,... */
#define LED_LIGHT               0
#define LED_PWR                 1
#define LED_ACT                 2
#define LED_OUT                 3


/***********************************
 * Initialization                  *
 ***********************************/
#define INIT_PORTA              0x00    // pull unused pind to GND, disable pullup on PortA,0 (light sensor)
#define INIT_PORTB              0x00    // pull unused pind to GND, disable pullups for mosi/miso/sck
#define INIT_PORTC              0x00    // pull unused pind to GND, switch off led light
#define INIT_PORTD              0x00    // pull unused pind to GND, switch off leds, disable pullup on UART(s)


/***********************************
 * UART Settings                   *
 ***********************************/
#define BUFFER_SIZE_TX0         32      // in bytes, must be between 1 and 255, should be at least (UART_MSG_SIZE_MAX * 2) + 2
#define BUFFER_SIZE_TX1         1
#define BUFFER_SIZE_RX0         32
#define BUFFER_SIZE_RX1         36
#define UART0_ENABLE                    // undefine to disable uart interface(s)
//#define UART1_ENABLE

/* baud rate */
#define UART0_UBRR              0x005F  // @3.6864MHz -> 2400Baud
#define UART1_UBRR              0x0077


/***********************************
 * Throttle/Display Controller     *
 ***********************************/
#define THROTTLE_VERSION        0       // V4 = 0 or V3X = 1 (see msg.h)


/***********************************
 * Light sensor                    *
 ***********************************/
#define LIGHT_SENS_PORT         PORTA
#define LIGHT_SENS_PIN          0       // PortA,0
#define LIGHT_SENS_THRES_OFF    150     // ADC threshold for switching light off
#define LIGHT_SEND_THRES_ON     60      // ADC threshold for switching light on
//#define LIGHT_AUTO_INTERVAL_MAX 10000   // autolight toggle max. interval in msec


/***********************************
 * Sleep Mode                      *
 ***********************************/
#define SLEEP_BOD_DISABLE               // uncomment to disable brown out detect during sleep (reduces pwr consumption)
#define SLEEP_WAKEUP_PCINT      14      // wakeup on pin change irq PB6 (rx0/mosi)
#define SLEEP_ACT_TIMEOUT       1750    // fall asleep after not receiving any uart message for x milliseconds

#endif /* CONFIG_H_ */
