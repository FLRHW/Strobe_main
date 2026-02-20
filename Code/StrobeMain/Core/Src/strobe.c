/*
 * strobe.c
 *
 *  Created on: Sep 17, 2025
 *      Author: Felipe
 */

// ------------------------- 		INCLUDES 		-------------------------

#include <stdint.h>
#include <stdbool.h>
//#include "fonts.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_adc.h"
//#include "stm32g4xx_ll_spi.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_tim.h"
//#include "ILI9341_driver.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_usart.h"
//#include "stm32g4xx_ll_rcc.h"
//#include "stm32g4xx_ll_system.h"
//#include "stm32g4xx_ll_gpio.h"
//#include "stm32g4xx_ll_exti.h"
//#include "stm32g4xx_ll_bus.h"
//#include "stm32g4xx_ll_cortex.h"
//#include "stm32g4xx_ll_pwr.h"
//#include "stm32g4xx_ll_i2c.h"


// ------------------------- 		DEFINES 		-------------------------

//#define	MAX_BIN				7
//#define	SETTLE_CNT			4
//#define	HIST				5
//#define	SEL_TIME			5000			// Time to change the requested parameter, in ms
//#define HOLD_TIME			200				// Time to ignore button press request after a previous press, in ms
//#define INC_DEC_TIME		50
//#define SCREEN_REFRESH_PER	50				// Time between subsequent screen updates, in ms

#define LED_TOGGLE_PER		500				// Time to toggle LED status, in ms
#define UART_TX_PER			200				// Time between subsequent uart transmissions, in ms
#define PERINI				20				// Default period, in us
#define FREQINI				100				// Default frequency, 1 = 0.01 Hz / 100 = 1 Hz
#define SUPINI				600				// Default supply voltage, in V
#define PERMIN				10				// Minimum period, in us
#define FREQMIN				5				// Minimum frequency 1 = 0.01 Hz
#define SUPMIN				400				// Minimum supply voltage, in V
#define PERMAX				800000			// Maximum period, in us
#define FREQMAX				20000			// Maximum frequency 1 = 0.01 Hz
#define SUPMAX				1000			// Maximum supply voltage, in V
#define BUSINI				300
#define POWERINI			1
#define IPEAKINI			0
#define BUSMIN				400
#define POWERMIN			0
#define IPEAKMIN			0
#define BUSMAX				1000
#define POWERMAX			1000
#define IPEAKMAX			900
#define RX_SIZE				10				// Amount of bytes to be received from the uart
#define MEASCNT				3				// Amount of measured parameters
#define ADC1CHNUM			4				// Number of ADC1 inputs in use
#define ADC2CHNUM			3				// Number of ADC2 inputs in use
#define	TICK_L_INI			371250//37499625
#define	TICK_H_INI			3750//375
#define	LIM_PERCENT			50
#define	TICK_ADJ			15

//#define SEP					1


// ------------------------- 		MACROS			-------------------------

#define LED_ON			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8)
#define LED_OFF			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8)
#define LED_TOG			LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_8);

#define LAMP_ON			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11)
#define LAMP_OFF		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11)
//#define	LAMP_STATUS		LL_GPIO_IsOutputPinSet(GPIOA, LL_GPIO_PIN_11)

#define ON_7			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7)
#define OFF_7			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7)
#define ON_12			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12)
#define OFF_12			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12)
#define ON_15			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_15)
#define OFF_15			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15)




#define GET_BYTE(x, n)  ((uint8_t)((x) >> (8 * (n)) & 0xFF))	// Returns the nth byte

// ------------------------- 		ENUMS			-------------------------

enum ParamTypes { PERIOD, FREQUENCY, SUPPLY, PARAMS };
enum MeasTypes { BUS, POWER, IPEAK, MEASUREMENTS };
//typedef enum { INCREASE, DECREASE, BUTTONP, ACTYPES } ReqTypes;


// ------------------------- 		VARIABLES 		-------------------------

volatile uint32_t miliseconds = 0;
//uint32_t highlight_expiry[PARAMS];
//uint8_t bin_hist[PARAMS][HIST];			// Stores the latest bin results for each channel
uint16_t adc1_bkp[ADC1CHNUM];				// Copy of latest ADC1 values - PFC output voltage, PFC input voltage, PFC input current, PFC output current
uint16_t adc2_bkp[ADC2CHNUM];				// Copy of latest ADC2 values - Output cap/Lamp positive voltage, Lamp/Emitter current and Collector/Lamp negative voltage
//uint8_t dotpos[PARAMS] = {3, 2, 0};
//uint8_t changepos[PARAMS];
volatile bool new_rx = false;
volatile bool outen = false;
volatile bool tick_upd = false;

volatile uint16_t adc1_out[ADC1CHNUM];		// DMA dumps ADC readings here
volatile uint16_t adc2_out[ADC2CHNUM];		// DMA dumps ADC readings here
volatile uint8_t uart_rx_buf[RX_SIZE];
//uint8_t meas_data[RX_SIZE];

int32_t param_req[PARAMS] = {PERINI, FREQINI, SUPINI};// Stores desired Period, Frequency and Supply voltage
const uint16_t param_min[PARAMS] = {PERMIN, FREQMIN, SUPMIN};
const uint32_t param_max[PARAMS] = {PERMAX, FREQMAX, SUPMAX};
//uint8_t maxpos_par[PARAMS] = {6, 5, 4};	// Stores maximum number of digits for each parameter

int32_t meas[PARAMS] = {BUSINI, POWERINI, IPEAKINI};							// Buffer for decoded "meas_data"
const uint16_t meas_min[MEASUREMENTS] = {BUSMIN, POWERMIN, IPEAKMIN};
const uint16_t meas_max[MEASUREMENTS] = {BUSMAX, POWERMAX, IPEAKMAX};
uint8_t maxpos_meas[MEASCNT] = {4, 4, 3};	// Stores maximum number of digits for each measurement

int32_t tick_l = TICK_L_INI;
int32_t tick_h = TICK_H_INI;
int32_t tick_l_tmp = TICK_L_INI;
int32_t tick_h_tmp = TICK_H_INI;


// ------------------------- 	INTERRUPT HANDLERS 	-------------------------

void uDMA1_Channel3_IRQHandler(void) {		// USART TX
	if (LL_DMA_IsActiveFlag_TC3(DMA1)) {
	    LL_DMA_ClearFlag_TC3(DMA1);
	    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
	}
}

void uDMA1_Channel4_IRQHandler(void) {		// USART RX
	ON_15;
	ON_7;
	OFF_7;
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	if (LL_DMA_IsActiveFlag_TC4(DMA1)){
        new_rx = true;
    	LL_DMA_ClearFlag_TC4(DMA1);
    }
    if (LL_DMA_IsActiveFlag_TE4(DMA1)){
        LL_DMA_ClearFlag_TE4(DMA1);
        // Handle DMA error
    }
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, RX_SIZE);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
	OFF_15;
}

void uTIM6_IRQHandler(void) {				// Currently running @ 5 kHz. Triggers screen update and other timed events
//	static uint16_t irq_cnt = 0;			// Counts from 0-4999 each time TIM6 IRQ is run (@ 5 kHz | 200 uS intervals)
	static uint8_t msec_cnt = 0;
	if (LL_TIM_IsActiveFlag_UPDATE(TIM6)) {
        LL_TIM_ClearFlag_UPDATE(TIM6);
		if (msec_cnt++ == 4) {
        	miliseconds++;
        	msec_cnt = 0;
		}
 //       if (irq_cnt++ == 4999) {
 //       	irq_cnt = 0;
 //       }
	}
}

void uTIM2_IRQHandler(void) {				// Triggers lamp output
//	if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
        LL_TIM_ClearFlag_UPDATE(TIM2);
		if (outen) {						// Lamp was enabled
			LAMP_OFF;
			outen = 0;
			LL_TIM_SetAutoReload(TIM2, tick_l);
		} else {							// Lamp was disabled
			LL_TIM_SetAutoReload(TIM2, tick_h);
			LAMP_ON;
			outen = 1;
		}
//	}
	if (tick_upd) {
		tick_upd = false;
		tick_l = tick_l_tmp;
		tick_h = tick_h_tmp;
	}
}


// ------------------------- 	OTHER FUNCTIONS		-------------------------

void UART1_Transmit_DMA(uint8_t* buff, uint8_t len) {
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)buff);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, len);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}


void calc_freq_per (void) {
	uint32_t limit_period = LIM_PERCENT *(1000000/param_req[1]); // Calculates maximum period for the new requested frequency
	if (param_req[0] > limit_period) {
		param_req[0] = limit_period;
	}
	tick_h_tmp = ((375 * param_req[0]) / 10) + TICK_ADJ;
	tick_l_tmp = (3750000000 / param_req[1]) - tick_h_tmp;

	tick_upd = true;
	if ((tick_h_tmp != tick_h) || (tick_l_tmp != tick_l)) {
		outen = true;
		LL_TIM_GenerateEvent_UPDATE(TIM2); 		// Forces interruption - removes latency specially if switching from a low freq to high freq
	}
}
// EXPLAIN THE SOURCE FOR THE ABOVE NUMBERS HERE (375, 10, ETC)
// NEEDS DEBUG, inputs 35.6 Hz & 399.010 ms generate odd output

void strobe_config(void) {						// Carries out general configuration for the board
	calc_freq_per();

//	LL_Init1msTick(SystemCoreClock);		// Required as auto generated code only initialises HAL system tick (due to USB relying on it)
/*
	// DMA configuration to automatically read ADC 1 conversion results	----------------------------------------------------------
	LL_DMA_ConfigAddresses(
	    DMA1,
	    LL_DMA_CHANNEL_1,
	    LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
	    (uint32_t)adc1_out,
	    LL_DMA_DIRECTION_PERIPH_TO_MEMORY
	);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC1CHNUM);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

	LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);	// Calibrates, enables and starts ADC
	while (LL_ADC_IsCalibrationOnGoing(ADC1));
	LL_mDelay(1);
	LL_ADC_Enable(ADC1);
	while (!LL_ADC_IsActiveFlag_ADRDY(ADC1));
	LL_ADC_REG_StartConversion(ADC1);


	// DMA configuration to automatically read ADC 2 conversion results	----------------------------------------------------------
	LL_DMA_ConfigAddresses(
	    DMA1,
	    LL_DMA_CHANNEL_2,
	    LL_ADC_DMA_GetRegAddr(ADC2, LL_ADC_DMA_REG_REGULAR_DATA),
	    (uint32_t)adc2_out,
	    LL_DMA_DIRECTION_PERIPH_TO_MEMORY
	);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, ADC2CHNUM);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);

	LL_ADC_StartCalibration(ADC2, LL_ADC_SINGLE_ENDED);	// Calibrates, enables and starts ADC
	while (LL_ADC_IsCalibrationOnGoing(ADC2));
	LL_mDelay(1);
	LL_ADC_Enable(ADC2);
	while (!LL_ADC_IsActiveFlag_ADRDY(ADC2));
	LL_ADC_REG_StartConversion(ADC2);

*/
	// Timer 2 configuration --------------------------------------------------------------------------------------------
	LAMP_OFF;
	tick_l = TICK_L_INI;
	tick_h = TICK_H_INI;
	LL_TIM_EnableIT_UPDATE(TIM2);    		// Enable update event interrupt for timer 2
	LL_TIM_GenerateEvent_UPDATE(TIM2); 		// Forces interruption
	LL_TIM_EnableCounter(TIM2);      		// Start counting


	// Timer 6 configuration --------------------------------------------------------------------------------------------
	LL_TIM_EnableIT_UPDATE(TIM6);    		// Enable update event interrupt for timer 6
	LL_TIM_GenerateEvent_UPDATE(TIM6); 		// Forces interruption
	LL_TIM_EnableCounter(TIM6);      		// Start counting


	// DMA configuration for USART TX --------------------------------------------------------------------------------------------
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)&USART1->TDR);
	LL_USART_EnableDMAReq_TX(USART1);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
//	UART1_Transmit_DMA(uart_tx_buf,9);
//	uDMA1_Channel3_IRQHandler();


	// DMA configuration for USART RX --------------------------------------------------------------------------------------------
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
    LL_DMA_ClearFlag_TC4(DMA1);				// Clear DMA transfer and error flags for channel 4
    LL_DMA_ClearFlag_TE4(DMA1);
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_4,    // Set peripheral and memory addresses and data length
        LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_RECEIVE),
        (uint32_t)uart_rx_buf,
        LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, RX_SIZE);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);	    // Enable DMA channel to start reception
    LL_USART_EnableDMAReq_RX(USART1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);  // Transfer complete interrupt
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_4);  // Transfer error interrupt (optional)

    LL_USART_Disable(USART1);				// RESET UART (otherwise power cycle is required for proper execution)
    LL_USART_Enable(USART1);
}


bool uart_rx (void) {
	static uint8_t sync_del;
	uint8_t crc = 0;
	bool rx_status = false;
	for(uint8_t i = 0; i < RX_SIZE - 1; i++) {	// Calculates CRC on the new data
		crc ^= uart_rx_buf[i];
		for(uint8_t j = 0; j < 8; j++) {
			crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
		}
	}
	if (crc == uart_rx_buf[RX_SIZE - 1]) {		// CRC is valid
		rx_status = true;
		uint32_t rxtemp;
		for (uint8_t i = 0; i < PARAMS; i++) {	// Converts each sequence of 3 bytes into a 24 bit value
			rxtemp = (uart_rx_buf[3 * i]) | ((uart_rx_buf[3 * i + 1]) << 8) | ((uart_rx_buf[3 * i + 2]) << 16);
			if (rxtemp > param_max[i]) {			// Only stores data if within the limits specified
				param_req[i] = param_max[i];
			} else if (rxtemp < param_min[i]) {
				param_req[i] = param_min[i];
			} else {
				param_req[i] = rxtemp;
			}
		}
	} else {									// Wrong CRC
		rx_status = false;
		LL_USART_DisableDirectionRx(USART1);
					LL_mDelay(sync_del);		// Delay to try new sync at a different time
					sync_del++;
		crc = LL_USART_ReceiveData8(USART1);
		LL_USART_ClearFlag_ORE(USART1);
		LL_USART_ClearFlag_NE(USART1);
		LL_USART_ClearFlag_FE(USART1);
		LL_USART_ClearFlag_PE(USART1);
		LL_USART_EnableDirectionRx(USART1);
/*		LL_USART_Disable(USART1);				// RESET UART (might be required to resume normal operation)
	    LL_USART_Enable(USART1);
*/
	}
	return (rx_status);
}


void uart_tx (void) {
	static uint8_t uart_tx_buf[10];
	uint8_t crc = 0;
		if ((meas[0]+= 1) > 1200) meas[0]=0;
		if ((meas[1]+= 1) > 1200) meas[1]=0;
		if ((meas[2]+= 1) > 1200) meas[2]=0;

	for(uint8_t i = 0; i < 9; i++) {
		uart_tx_buf[i] = GET_BYTE(meas[i / 3], i % 3);
		crc ^= uart_tx_buf[i];
		for(uint8_t j = 0; j < 8; j++) {
			crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
		}
	}
	uart_tx_buf[9] = crc;

	if (!LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_3)) {
		if (LL_USART_IsActiveFlag_TC(USART1)) {         // DMA channel not active// UART transmission complete
			UART1_Transmit_DMA(uart_tx_buf,10);
		}
	}
}

/*
void process_adc (uint8_t ch, uint8_t input) {
	//meas[input] = adc1_bkp[input];

//	meas[0] = adc1_bkp[0];
//	meas[1] = adc1_bkp[1];
//	meas[2] = adc1_bkp[2];
}

*/
void strobe_main() {
	uint32_t LED_tog_t = 0, uart_tr_t = 0;
	LED_OFF;
	strobe_config();


	while (1) {
		if (miliseconds > LED_tog_t) {			// Blinks alive LED at 1 Hz 50 %
			LED_tog_t = miliseconds + LED_TOGGLE_PER;
			LED_TOG;
		}

		if (new_rx) {							// RX flag active
			new_rx = false;
			if(uart_rx() == true) {				// If received data successfully
				// Updates timer and voltage control
				calc_freq_per();
				// calc_pwm_voltage();
			}
		}

		if (miliseconds > uart_tr_t) {			// Time for new transmission
			uart_tr_t = miliseconds + UART_TX_PER;
			uart_tx();
		}
/*
		if (LL_ADC_IsActiveFlag_EOS(ADC1)) {		// Checks end of conversion sequence for ADC 1
			LL_ADC_ClearFlag_EOS(ADC1);
			for (uint8_t ch = 0; ch < ADC1CHNUM; ch++) {
				adc1_bkp[ch] = adc1_out[ch]; // saves a copy of ADC values before allowing new ADC conversion
			}
			LL_ADC_REG_StartConversion(ADC1);

//			meas[0] = adc1_bkp[0];
//			meas[1] = adc1_bkp[1];
//			meas[2] = adc1_bkp[2];

//			process_adc(1, 0);
//			process_adc(1, 1);
//			process_adc(1, 2);
//			process_adc(1, 3);

		}

		if (LL_ADC_IsActiveFlag_EOS(ADC2)) {		// Checks end of conversion sequence for ADC 2
			LL_ADC_ClearFlag_EOS(ADC2);
			for (uint8_t ch = 0; ch < ADC2CHNUM; ch++) {
				adc2_bkp[ch] = adc2_out[ch]; // saves a copy of ADC values before allowing new ADC conversion
			}
			LL_ADC_REG_StartConversion(ADC2);

			meas[1] = adc2_bkp[0];
			meas[2] = adc2_bkp[1];
			meas[0] = adc2_bkp[2];

//			process_adc(2, 0);
//			process_adc(2, 1);
//			process_adc(2, 2);
		}*/

	}
}










