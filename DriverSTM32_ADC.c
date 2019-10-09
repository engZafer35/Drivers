/******************************************************************************
* #Author       : Zafer Satılmış
* #Revision     : 1.0
* #Date         : May 24, 2019 - 3:54:27 PM
* #File Name    : DriverSTM32_ADC.c 
* #File Path    :/DriverSTM32_ADC.c
*******************************************************************************/
/******************************************************************************
*
*
*******************************************************************************/
/*********************************INCLUDES*************************************/
#include <string.h>
#include "DriverSTM32_ADC.h"
#include "stm32f4xx_hal_gpio.h"
/****************************** MACRO DEFINITIONS *****************************/
#if ((USE_BUS_1 != 1) && (USE_BUS_2 != 1) && (USE_BUS_3 != 1))
    #error "Firstly Select A ADC BUS in DriverSTM32_ADC.h file !!!"
#endif

#define IS_VALID_BUS_ID(x)      (x < EN_ADC_BUSS_MAX_NUM)
#define IS_VALID_CHANNEL_ID(x)  (x < EN_ADC_CHANNEL_MAX_NUM)
/*************************** FUNCTION PROTOTYPES ******************************/

/******************************* TYPE DEFINITIONS *****************************/

/********************************** VARIABLES *********************************/
#if (USE_BUS_1 == 1)
static ADC_HandleTypeDef hadc1 = {NULL, 0,};
#endif
#if (USE_BUS_2 == 1)
static ADC_HandleTypeDef hadc2 = {NULL, 0,};
#endif 
#if (USE_BUS_3 == 1)
static ADC_HandleTypeDef hadc3 = {NULL, 0,};
#endif
/***************************** STATIC FUNCTIONS  ******************************/
/**
 * @brief set gbip as a analog input
 * @param gpio port
 * @param gpio pin
*/
static void initChannelPin(GPIO_TypeDef* GPIOx, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


/**
 * @brief  read adc
 * @param  adc handler
 * @return adc value, -1 failure
*/
static uint32_t readADC(ADC_HandleTypeDef *adcHandle)
{    
    uint32_t retVal =  -1; // Error 
    
    if (NULL != adcHandle)
    {
        if (HAL_OK == HAL_ADC_Start(adcHandle))
        {            
            if (HAL_OK == HAL_ADC_PollForConversion(adcHandle, 10))
            {                
                if ((HAL_ADC_GetState(adcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
                {
                    retVal =  HAL_ADC_GetValue(adcHandle);
                }                                                
            } 
            
            HAL_ADC_Stop(adcHandle);             
        }                     
    }
    
    return  retVal;
}

/***************************** PUBLIC FUNCTIONS  ******************************/
/**
 * @brief  init ADC Bus. Func will handle ADC1, ADC2, ADC3
 * @note    First of all call this function
 * @param   Bus ID
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvADCInitBus(EN_ADC_BUS busID, ADC_HandleTypeDef *hAdcX)
{
    RETURN_STATUS retVal = EN_FAILURE;
    
    if(FALSE == IS_VALID_BUS_ID(busID))
    {
        return EN_FAILURE;
    }
    
#if (USE_BUS_1 == 1)  
    if ((EN_ADC_BUS_1 == busID) && (NULL == hadc1.Instance)) // check bus_1 is empty
    {
        if (IS_NULL_PTR(hAdcX)) //use default settings
        {
            hadc1.Instance                      = ADC1;
            hadc1.Init.ClockPrescaler           = CLOCK_PRESCALLER;
            hadc1.Init.Resolution               = RESOLUTION;
            hadc1.Init.ScanConvMode             = SCAN_CONV_MODE;
            hadc1.Init.ContinuousConvMode       = CON_CONV_MODE;
            hadc1.Init.DiscontinuousConvMode    = DIS_CONT_CONV_MODE;
            hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
            hadc1.Init.ExternalTrigConv         = ADC_EXTERNALTRIGCONV_T1_CC1;
            hadc1.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
            hadc1.Init.NbrOfConversion          = NBR_OF_CONVERSION;
            hadc1.Init.DMAContinuousRequests    = DISABLE;
            hadc1.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
            
            if (HAL_OK == HAL_ADC_Init(&hadc1)) 
            {
                retVal = EN_SUCCESS;
            }     
        }
        else //use user settings
        {
            if (hAdcX->Instance == ADC1) //check whether user send correct channel
            {
                memcpy(&hadc1, hAdcX, sizeof(hadc1));
            }        
        }    
    }
#endif
#if (USE_BUS_2 == 1)
#if ((USE_BUS_1 == 0))   
    if ((EN_ADC_BUS_2 == busID) && (NULL == hadc2.Instance)) // check bus_2 is empty  
#else    
    else if ((EN_ADC_BUS_2 == busID) && (NULL == hadc2.Instance)) // check bus_2 is empty
#endif
    {
        if (IS_NULL_PTR(hAdcX)) //use default settings
        {
            hadc2.Instance                      = ADC2;
            hadc2.Init.ClockPrescaler           = CLOCK_PRESCALLER;
            hadc2.Init.Resolution               = RESOLUTION;
            hadc2.Init.ScanConvMode             = SCAN_CONV_MODE;
            hadc2.Init.ContinuousConvMode       = CON_CONV_MODE;
            hadc2.Init.DiscontinuousConvMode    = DIS_CONT_CONV_MODE;
            hadc2.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
            hadc2.Init.ExternalTrigConv         = ADC_EXTERNALTRIGCONV_T1_CC1;
            hadc2.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
            hadc2.Init.NbrOfConversion          = NBR_OF_CONVERSION;
            hadc2.Init.DMAContinuousRequests    = DISABLE;
            hadc2.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
            
            if (HAL_OK == HAL_ADC_Init(&hadc2)) 
            {
                retVal = EN_SUCCESS;
            }     
        }     
        
        else
        {
            if (hAdcX->Instance == ADC1) //check whether user send correct channel
            {
                memcpy(&hadc2, hAdcX, sizeof(hadc2));
                retVal = EN_SUCCESS;
            }        
        }    
    }
#endif //(USE_BUS_2 == ENABLE)

#if (USE_BUS_3 == 1)
#if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0) && (USE_BUS_3 == 1)) 
    if ((EN_ADC_BUS_3 == busID) && (NULL == hadc3.Instance)) // check bus_2 is empty
#else     
    else if ((EN_ADC_BUS_3 == busID) && (NULL == hadc3.Instance)) // check bus_2 is empty
#endif        
    {
        if (IS_NULL_PTR(hAdcX))
        {
            hadc3.Instance                      = ADC3;
            hadc3.Init.ClockPrescaler           = CLOCK_PRESCALLER;
            hadc3.Init.Resolution               = RESOLUTION;
            hadc3.Init.ScanConvMode             = SCAN_CONV_MODE;
            hadc3.Init.ContinuousConvMode       = CON_CONV_MODE;
            hadc3.Init.DiscontinuousConvMode    = DIS_CONT_CONV_MODE;
            hadc3.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
            hadc3.Init.ExternalTrigConv         = ADC_EXTERNALTRIGCONV_T1_CC1;
            hadc3.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
            hadc3.Init.NbrOfConversion          = NBR_OF_CONVERSION;
            hadc3.Init.DMAContinuousRequests    = DISABLE;
            hadc3.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
            
            if (HAL_OK == HAL_ADC_Init(&hadc3)) 
            {
                retVal = EN_SUCCESS;
            }       
        }
        else
        {
            if (hAdcX->Instance == ADC1) //check whether user send correct channel
            {
                memcpy(&hadc3, hAdcX, sizeof(hadc3));
                retVal = EN_SUCCESS;
            }        
        }
    }
#endif //(USE_BUS_3 == ENABLE)    
        
    return retVal;        
}

/**
 * @brief  init ADC Channel. 
 * @note   Firstly drvADCInitAdcBus() should be called.
 * @param  Bus ID
 * @pram   channel ID
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvADCInitChannel(EN_ADC_BUS busID, EN_ADC_CHANNEL chnlID)
{
    RETURN_STATUS retVal = EN_SUCCESS;
    
    if(IS_VALID_BUS_ID(busID) && IS_VALID_CHANNEL_ID(chnlID))
    {        
        switch(chnlID)
        {
            case EN_ADC_CHANNEL_0:
            {
                initChannelPin(GPIOA, GPIO_PIN_0);
                break;
            }                
            case EN_ADC_CHANNEL_1:
            {
                initChannelPin(GPIOA, GPIO_PIN_1);
                break;
            }                    
            case EN_ADC_CHANNEL_2:
            {
                initChannelPin(GPIOA, GPIO_PIN_2);
                break;
            }                 
            case EN_ADC_CHANNEL_3:
            {
                initChannelPin(GPIOA, GPIO_PIN_3);
                break;
            }     
            case EN_ADC_CHANNEL_4:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID) 
                #endif 
                    { 
                        initChannelPin(GPIOA, GPIO_PIN_4);
                    } 
                #endif
                #if (USE_BUS_3 == 1)
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif 
                    {
                        #if defined(GPIOF)
                            initChannelPin(GPIOF, GPIO_PIN_6);
                        #else
                            retVal = EN_FAILURE; //mcu doesnt have F port
                        #endif
                    }    
                #endif                
                break;
            } 
            case EN_ADC_CHANNEL_5:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID)
                #endif                 
                    {
                        initChannelPin(GPIOA, GPIO_PIN_5);
                    } 
                #endif
                
                #if (USE_BUS_3 == 1)
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif 
                    {
                        #if defined(GPIOF)
                            initChannelPin(GPIOF, GPIO_PIN_7);
                        #else
                            retVal = EN_FAILURE; //mcu doesnt have F port
                        #endif
                    }     
                #endif                
                break;
            } 
            case EN_ADC_CHANNEL_6:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID)
                #endif 
                    {
                        initChannelPin(GPIOA, GPIO_PIN_6);
                    } 
                #endif
                #if (USE_BUS_3 == 1)
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif                                 
                    {
                        #if defined(GPIOF)
                            initChannelPin(GPIOF, GPIO_PIN_8);
                        #else
                            retVal = EN_FAILURE; //mcu doesnt have F port
                        #endif
                    }  
                #endif                
                break;
            } 
            case EN_ADC_CHANNEL_7:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID)
                #endif 
                    {
                        initChannelPin(GPIOA, GPIO_PIN_7);
                    }
                #endif                
                #if (USE_BUS_3 == 1)
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif 
                    {
                        #if defined(GPIOF)
                            initChannelPin(GPIOF, GPIO_PIN_9);
                        #else
                            retVal = EN_FAILURE; //mcu doesnt have F port
                        #endif
                    }     
                #endif                    
                break;
            } 
            case EN_ADC_CHANNEL_8:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID)
                #endif 
                    {
                        initChannelPin(GPIOB, GPIO_PIN_0);
                    }
                #endif 

                #if (USE_BUS_3 == 1)
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif                                
                    {
                        #if defined(GPIOF)
                            initChannelPin(GPIOF, GPIO_PIN_10);
                        #else
                            retVal = EN_FAILURE; //mcu doesnt have F port
                        #endif
                    } 
                #endif                    
                break;
            } 
            case EN_ADC_CHANNEL_9:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID)
                #endif 
                    {
                        initChannelPin(GPIOB, GPIO_PIN_1);
                    } 
                #endif
                
                #if (USE_BUS_3 == 1)
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif
                    {
                        #if defined(GPIOF)
                            initChannelPin(GPIOF, GPIO_PIN_3);
                        #else
                            retVal = EN_FAILURE; //mcu doesnt have F port
                        #endif
                    }   
                #endif                    
                break;
            } 
            case EN_ADC_CHANNEL_10:
            {
                initChannelPin(GPIOC, GPIO_PIN_0);
                break;
            } 
            case EN_ADC_CHANNEL_11:
            {
                initChannelPin(GPIOC, GPIO_PIN_1);
                break;
            } 
            case EN_ADC_CHANNEL_12:
            {
                initChannelPin(GPIOC, GPIO_PIN_2);
                break;
            } 
            case EN_ADC_CHANNEL_13:
            {
                initChannelPin(GPIOC, GPIO_PIN_3);
                break;
            } 
            case EN_ADC_CHANNEL_14:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID)
                #endif         
                    {
                        initChannelPin(GPIOC, GPIO_PIN_4);
                    } 
                #endif
                
                #if (USE_BUS_3 == 1) 
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif
                    {
                    #if defined(GPIOF)
                        initChannelPin(GPIOF, GPIO_PIN_4);
                    #else
                        retVal = EN_FAILURE; //mcu doesnt have F port
                    #endif
                    }    
                #endif                    
                break;
            } 
            case EN_ADC_CHANNEL_15:
            {
                #if ((USE_BUS_1 == 1) || (USE_BUS_2 == 1)) 
                #if ((USE_BUS_1 == 1) && (USE_BUS_2 == 1))        
                    if ((EN_ADC_BUS_1 == busID) || (EN_ADC_BUS_2 == busID))
                #elif ((USE_BUS_1 == 1) && (USE_BUS_2 == 0))
                    if (EN_ADC_BUS_1 == busID)
                #elif ((USE_BUS_1 == 0) && (USE_BUS_2 == 1))
                    if (EN_ADC_BUS_2 == busID)
                #endif
                    {  
                        initChannelPin(GPIOC, GPIO_PIN_5);
                    } 
                #endif
                
                #if (USE_BUS_3 == 1)
                #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0)) 
                    if (EN_ADC_BUS_3 == busID) 
                #else
                    else if (EN_ADC_BUS_3 == busID) 
                #endif
                    {
                        #if defined(GPIOF)
                            initChannelPin(GPIOF, GPIO_PIN_5);
                        #else
                            retVal = EN_FAILURE; //mcu doesnt have F port
                        #endif
                    }  
                #endif
                break;
            } 
            default:
            case EN_ADC_CHANNEL_MAX_NUM:
            {
                retVal = EN_FAILURE;
            }
        }            
    }
    
    return retVal;
}

/**
 * @brief   read adc
 * @note    Firstly drvADCInitAdcBus() and drvADCInitAdcChannel() should be call. 
 * @param   Bus 
 * @pram    Channel ID
 * @return  if everything is OK, return adc value
 *          otherwise return -1(FAILURE)
 */
int drvADCRead(EN_ADC_BUS busID, EN_ADC_CHANNEL chnlID)
{
    int retVal = -1;
    ADC_ChannelConfTypeDef config = {0 };
    ADC_HandleTypeDef *tmpHandler = NULL;
    
    if (IS_VALID_BUS_ID(busID) && IS_VALID_CHANNEL_ID(chnlID))
    {
        config.SamplingTime = SAMPLING_TIME;
        config.Rank         = 1;
        config.Channel      = chnlID;
        
        #if (USE_BUS_1 == 1)        
                if(EN_ADC_BUS_1 == busID) tmpHandler = &hadc1;
        #endif
                
        #if (USE_BUS_2 == 1)
        #if (USE_BUS_1 == 0)   
                if(EN_ADC_BUS_2 == busID) tmpHandler = &hadc2;
        #else    
                else if(EN_ADC_BUS_2 == busID) tmpHandler = &hadc2;
        #endif        
        #endif
        #if (USE_BUS_3 == 1)
        #if ((USE_BUS_1 == 0) && (USE_BUS_2 == 0) && (USE_BUS_3 == 1)) 
                if(EN_ADC_BUS_3 == busID) tmpHandler = &hadc3;
        #else     
                else if(EN_ADC_BUS_3 == busID) tmpHandler = &hadc3;
        #endif 
        #endif        
      
        if (HAL_OK == HAL_ADC_ConfigChannel(tmpHandler, &config)) 
        {
            retVal = readADC(tmpHandler);
        }        
    }
    
    return retVal;
}

/******************************** End Of File *********************************/
