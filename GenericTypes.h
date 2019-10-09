/******************************************************************************
* #Author 		: Zafer Satılmış
* #Revision		: 1.0
* #Date			: Apr 30, 2019 - 2:40:38 PM
* #File Name	: GenericTypes.h
* #File Path	: GenericTypes.h
*
*******************************************************************************/

/******************************************************************************
*
******************************************************************************/
/******************************IFNDEF ? DEFINE********************************/
#ifndef __GENERIC_TYPES_H__
#define __GENERIC_TYPES_H__

/*********************************INCLUDES*************************************/
#include "stm32f4xx_hal.h"
/******************************MACRO DEFINITIONS*******************************/
#define DBG_ERR(format, args...) ;
#define DBG_WARNING(format, args...) ;
#define DBG_INFO(format, args...) ;

#define IS_NULL_PTR(x)     (NULL == x)
#define IS_SAFELY_PTR(x)   (NULL != x)

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#define SYSTEM_VERSION "V1.01"

/*******************************TYPE DEFINITIONS ******************************/
typedef void (*VoidCallback)(void);

typedef enum
{
    EN_FAILURE = -1,
    EN_SUCCESS = 0
}RETURN_STATUS;

typedef unsigned int 	BOOL;
typedef BOOL 		    FLAG;


/************************* GLOBAL VARIBALE REFERENCES *************************/

/************************* GLOBAL FUNCTION DEFINITIONS ************************/

#endif /* __GENERIC_TYPES_H__ */

/********************************* End Of File ********************************/
