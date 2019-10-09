/******************************************************************************
* #Author       : Zafer Satılmış
* #Revision     : 1.0
* #Date         : Aug 21, 2019 - 8:08:16 AM
* #File Name    : DrvM4T11_RTC.h
* #File Path    : Driver/DeviceDriver/inc/DrvM4T11_RTC.h
*
*******************************************************************************/

/******************************************************************************
*                                Pin Table Description
*     Pin
*      1   -   OSCI    -  Oscillator input
*      2   -   OCSO    -  Oscillator output
*      3   -   Vbat    -  Battery supply voltage
*      4   -   Vss     -  Ground
*      5   -   SDA     -  Serial data address input/output
*      6   -   SCL     -  Serial clock
*      7   -   FT/OUT  -  Frequency test/output driver (open drain)
*      8   -   Vcc     -  Supply voltage
*
******************************************************************************/
/******************************IFNDEF & DEFINE********************************/
#ifndef __DRV_M4T11_RTC_H__
#define __DRV_M4T11_RTC_H__
/*********************************INCLUDES*************************************/
#include "GlobalDefinitions.h"
/******************************MACRO DEFINITIONS*******************************/
/**
 * @brief Select M4T11 I2C add
 */
#define M4T11_I2C_ADD   (0xD0)

/**
 * select correct I2C bus struct name according to HW. (HAL library used)
 */
#define I2C_LINE (hi2c3)
#ifndef I2C_LINE
    #error "Firstly define I2C_LINE macro in DrvLM75B_DigitalTemp.h file."
#endif
/*******************************TYPE DEFINITIONS ******************************/
/**
 * @brief m4t11 driver time structure
 */
typedef struct _M4T11_RTC_STR
{
    U8 sec;     /* Seconds parameter, from 00 to 59 */
    U8 min;     /* Minutes parameter, from 00 to 59 */
    U8 hour;    /* Hours parameter, 24Hour mode, 00 to 23 */
    U8 wday;    /* Day in a week, from 1 to 7 */
    U8 mday;    /* Date in a month, 1 to 31 */
    U8 mon;     /* Month in a year, 1 to 12 */
    U16 year;   /* Year parameter, 2000 to 3000 */
}M4T11_RTC_STR;
/************************* GLOBAL VARIBALE REFERENCES *************************/

/************************* GLOBAL FUNCTION DEFINITIONS ************************/

/**
 * @brief  get time
 * @param  M4T11_RTC_STR pointer
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvM4T11GetTime(M4T11_RTC_STR *getTime);

/**
 * @brief  set time
 * @param  M4T11_RTC_STR pointer
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvM4T11SetTime(const M4T11_RTC_STR *setTime);

#endif /* __DRV_M4T11_RTC_H__ */

/********************************* End Of File ********************************/
