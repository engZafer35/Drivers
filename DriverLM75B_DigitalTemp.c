/******************************************************************************
* #Author       : Zafer Satılmış
* #Revision     : 1.0
* #Date         : May 28, 2019 - 11:10:18 AM
* #File Name    : DrvLM75B_DigitalTemp.c
* #File Path    : /DrvLM75B_DigitalTemp.c
*******************************************************************************/
/******************************************************************************
*
*
*******************************************************************************/
/*********************************INCLUDES*************************************/
#include "DriverLM75B_DigitalTemp.h"
/****************************** MACRO DEFINITIONS *****************************/
#define TEMP_REGISTER_ADD   (0x00) //tempeture register add
#define CONF_REGISTER_ADD   (0x01) //configuration register add
#define THYS_REG_ADD        (0x02) //hysteresis register add
#define TOH_REG_ADD         (0x03) //threshold temp register add

#define SIZEOF_TEMP_REG     (2)
#define SIZEOF_CONF_REG     (1)
#define SIZEOF_THYS_REG     (2)
#define SIZEOF_TOH_REG      (2)

#define I2C_CONN_TIMEOUT	(100)

/*************************** FUNCTION PROTOTYPES ******************************/

/******************************* TYPE DEFINITIONS *****************************/

/********************************** VARIABLES *********************************/

/*
 * select correct I2C bus struct. HAL library used
 */
extern I2C_HandleTypeDef I2C_LINE;

/***************************** STATIC FUNCTIONS  ******************************/

/***************************** PUBLIC FUNCTIONS  ******************************/
/**
 * @brief  read temperature
 * @param  void
 * @return if everything is OK, return temperature
 *         otherwise return 0XFFFF;
 */
float drvLM75GetTemp(void)
{
    float retTemp = 0XFFFF;
    uint8_t buff[SIZEOF_TEMP_REG];

    if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, TEMP_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_TEMP_REG, I2C_CONN_TIMEOUT))
    {
        retTemp =  (buff[0] << 8) + buff[1];
        retTemp =  retTemp/256;
    }

    return retTemp;
}

#if (OPTIMIZATION == 1)
/**
 * @brief  init LM75 temperature sensor with default value
 * @param  init struct.
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75Init(void)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t buff[2]; //max register 2 byte

    buff[0] = OVERTEMPERATURE_VALUE & 0x00FF;
    buff[1] = OVERTEMPERATURE_VALUE & 0xFF00;
    if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, TOH_REG_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_TOH_REG, I2C_CONN_TIMEOUT))
    {
        buff[0] = HYSTERESIS_TEMP_VALUE & 0x00FF;
        buff[1] = HYSTERESIS_TEMP_VALUE & 0xFF00;
        if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, THYS_REG_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_THYS_REG, I2C_CONN_TIMEOUT))
        {
            //load conf register data
            buff[0] = DF_LM75_WORKING_MODE + DF_OS_WORKING_MODE + DF_OS_POLARITY + DF_OS_FAULT_NUM;

            if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
            {
                retVal = EN_SUCCESS;
            }
        }
    }


    return retVal;

}
#endif

#if ((OPTIMIZATION == 0) || (defined USE_INTERRUPT))
/**
 * @brief  clear LM75B interrupt output
 * @note   if you select interrupt mode for os_working mode
 *         you can use this func to clear os output status
 * @param  void
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75ClrInterrupt(void)
{
    RETURN_STATUS retVal = EN_SUCCESS;

    //clear any register to clear interrupt
    if (0XFFFF == drvLM75GetTemp())
    {
        retVal = EN_FAILURE;
    }
    return retVal;
}
#endif


#if (OPTIMIZATION == 0)
/**
 * @brief  init LM75 temperature sensor.
 * @param  init struct.
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75Init(const LM75_INIT_STR *intStr)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t configReg; //max register 2 byte

    if (IS_SAFELY_PTR(intStr))
    {
        if (EN_SUCCESS == drvLM75SetTohAndThyr(intStr->overTemp, intStr->hysTemp))
        {
            //load conf register data
            configReg = intStr->confReg.osWorkingMode + intStr->confReg.osPolarity + intStr->confReg.osWorkingMode + intStr->confReg.osFaultNum;

            if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&configReg, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
            {
                retVal = EN_SUCCESS;
            }
        }
    }

    return retVal;
}

/**
 * @brief  set threshold and hysteresis temperature value
 * @param  threshold value (Centigrade)
 * @param  hysteresis value (Centigrade)
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75SetTohAndThyr(int16_t toh, int16_t hys)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t buff[2]; //max register 2 byte

    buff[0] = toh & 0x00FF;
    buff[1] = toh & 0xFF00;
    if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, TOH_REG_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_TOH_REG, I2C_CONN_TIMEOUT))
    {
        buff[0] = hys & 0x00FF;
        buff[1] = hys & 0xFF00;
        if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, THYS_REG_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_THYS_REG, I2C_CONN_TIMEOUT))
        {
            retVal = EN_SUCCESS;
        }
    }
    return retVal;
}

/**
 * @brief  set OS working mode
 * @param  mode
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75SetOsWorkingMode(OS_WORKING_MODE mode)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t temp;
    if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
    {
        temp &= 0xFD; //clear os working bit
        temp += mode;
        if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
        {
            retVal = EN_SUCCESS;
        }
    }
    return retVal;
}

/**
 * @brief  set sensor(LM75) working mode
 * @param  mode
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75SetL75WorkingMode(LM75_WORKING_MODE mode)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t temp;
    if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
    {
        temp &= 0xFE; //clear LM75 working mode bit
        temp += mode;
        if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
        {
            retVal = EN_SUCCESS;
        }
    }
    return retVal;
}

/**
 * @brief  set sensor(LM75) working mode
 * @param  mode
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75SetOsPolarity(OS_POLARITY mode)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t temp;
    if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
    {
        temp &= 0xFB; //clear os polarity bit
        temp += mode;
        if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
        {
            retVal = EN_SUCCESS;
        }
    }
    return retVal;
}

/**
 * @brief  set fault number
 * @param  mode
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75SetFaultNum(OS_FAULT_NUM fault)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t temp;

    if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
    {
        temp &= 0xE7; //clear os polarity bit
        temp += fault;
        if (HAL_OK == HAL_I2C_Mem_Write(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&temp, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
        {
            retVal = EN_SUCCESS;
        }
    }
    return retVal;
}

/**
 * @brief  read Toh and Thyr
 * @param  Toh set add
 * @param  hysteresis set add
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75ReadTohAndThyr(float *toh, float *hys)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t buff[SIZEOF_THYS_REG];

    if (IS_SAFELY_PTR(toh) && IS_SAFELY_PTR(hys))
    {
        if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, TOH_REG_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_TOH_REG, I2C_CONN_TIMEOUT))
        {
            (*toh) = (buff[0] << 8) + buff[1];
            (*toh) = (*toh)/256;

            buff[0] = buff[1] = 0;
            if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, THYS_REG_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buff, SIZEOF_THYS_REG, I2C_CONN_TIMEOUT))
            {
                (*hys) = (buff[0]<<8) + buff[1];
                (*hys) = (*hys)/256;

                retVal = EN_SUCCESS;
            }
        }
    }
    return retVal;
}

/**
 * @brief  read conf. register
 * @param  mode
 * @param  hysteresis
 * @return if everything is OK, return SUCCES
 *         otherwise return FAILURE
 */
RETURN_STATUS drvLM75ReadConf(LM75_CONF_REG *confReg)
{
    RETURN_STATUS retVal = EN_FAILURE;
    uint8_t conf;

   if(IS_SAFELY_PTR(confReg))
   {
        if (HAL_OK == HAL_I2C_Mem_Read(&I2C_LINE, (uint16_t)LM75B_I2C_ADD, CONF_REGISTER_ADD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&conf, SIZEOF_CONF_REG, I2C_CONN_TIMEOUT))
        {
            confReg->workingMode    = (LM75_WORKING_MODE)(conf & 0x01);
            confReg->osWorkingMode  = (OS_WORKING_MODE)(conf & 0x02);
            confReg->osPolarity     = (OS_POLARITY)(conf & 0x04);
            confReg->osFaultNum     = (OS_FAULT_NUM)(conf & 0x18);

            retVal = EN_SUCCESS;
        }
    }
    return retVal;
}

#endif //#if (OPTIMIZATION == 0)
/******************************** End Of File *********************************/
