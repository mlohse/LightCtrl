/*******************************************************************************
 * ADC driver for light sensor                                                 *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/**
 * @brief adc_init initializes the adc for reading pin 'LIGHT_SENS_PIN'
 * @see config.h
 */
void adc_init(void);

/**
 * @brief adc_start continuously runs adc, result is buffered and can be read
 *                  any time by calling @see adc_read()
 */
void adc_start(void);

/**
 * @brief adc_stop shuts down adc
 */
void adc_stop(void);

/**
 * @brief adc_read read last adc result
 * @return return 10-bit adc result
 * @note if adc is not running, the last result will be returned (which may be
 *       old), if adc was never started result will be '0'
 */
uint16_t adc_read(void);

#endif /* ADC_H_ */
