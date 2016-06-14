#include <msp430.h>
#include <stdio.h>
#include <math.h>
#include "sndmodule.h"
#include "dispmodule.h"
#include "driverlib.h"
#include "src/uart.c"
//*****************************************************************************
//
//Select Baud rate
//
//*****************************************************************************
#define BAUD_RATE                               9600
//*****************************************************************************
//
//Initialize received data
//
//*****************************************************************************
int receivedData = 0x00;
//*****************************************************************************
//
//Initialize trasnmit data
//
//*****************************************************************************
int transmitData = 0x00;

int check = 0;
void main(void) {

WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

//CHANGE CLOCK FREQUENCY TO 2MHz, and restore SMCLK to 1MHz
  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  __bic_SR_register(SCG0);                  // Enable the FLL control loop
  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 2 MHz / 32,768 Hz = 62500 = MCLK cycles for DCO to settle
  __delay_cycles(62500);//
  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
	UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
											// Clear XT2,XT1,DCO fault flags
	SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  USCI_A_UART_initParam param1 = {0};
      param1.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
      param1.clockPrescalar = 6;
      param1.firstModReg = 13;
      param1.secondModReg = 0;
      param1.parity = USCI_A_UART_NO_PARITY;
      param1.msborLsbFirst = USCI_A_UART_LSB_FIRST;
      param1.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
      param1.uartMode = USCI_A_UART_MODE;
      param1.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
      USCI_A_UART_enable(USCI_A1_BASE);





      // Load data onto buffer
      USCI_A_UART_transmitData(USCI_A1_BASE,
                               transmitData);
}
