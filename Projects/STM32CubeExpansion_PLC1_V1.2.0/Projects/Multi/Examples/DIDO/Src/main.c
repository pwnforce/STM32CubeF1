/**
  ******************************************************************************
  * @file               main.c
  * @author             CL
  * @version            V1.1.0
  * @date               25-April-2016
  * @brief              Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "x_nucleo_plc01a1.h"


/** @addtogroup X_NUCLEO_PLC01A1_Examples   
  * @brief 
  * @{ 
  */

/** @addtogroup DIDO (Data In Data Out) 
  * @brief 
  * @{ 
  */
    
 /* Uncomment this for OUTPUT_CYCLING ENABLE */
//#define OUTPUT_CYCLING
    
/* Private variables ---------------------------------------------------------*/
static uint8_t INPUT_READ_FLAG = 0;//Flag for reading input channel status
//static uint8_t ChannelsOn = 0x00;//Number of channels in on state
static uint16_t WD_Reset = 0;//Watchdog count (in ms)
static uint8_t Tick_10 = 0;//10ms count
static uint8_t* Input_Data;//Input channels state
static uint8_t oData = 0;
static uint8_t* Relay_Status;//Output relay status
static uint8_t* Current_Limiter_Status;//Input current limiter status
static uint32_t FreezeTime = 0;//Time in ms for which outpus to be freezed
#ifdef OUTPUT_CYCLING
/* The frequency range of Output_cycling is expressed in Hz */ 
static uint16_t OC_Frequency = 50;
/* 10% duty cycle to 90% duty cycle (%) */
static uint8_t OC_Duty = 50;
#endif /* OUTPUT_CYCLING */

/* Private function prototypes -----------------------------------------------*/
static void initializePlc(void);
static void PLC_Handler(void);
uint8_t* CURRENT_LIMITER_Handler(void);
void RELAY_Handler(uint8_t*);


/* Private functions ---------------------------------------------------------*/
/**
 * @brief   Main function is to show how to use X_NUCLEO_PLC01A1 expansion board
 *          to receive data from Input Current Limiter device and to modify 
 *          and transmit it to intelligent relay
 * @param  None
 * @retval int
 */
int main(void)
{

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  initializePlc();
  
  /* Reset relay at startup to avoid FAULT */
  BSP_RELAY_Reset();
  
  /* wait for 100 ms at startup */
  while (WD_Reset < 100);
  
  /* Enable Relay Outputs */
  BSP_RELAY_EN_Out();  
  
#ifdef OUTPUT_CYCLING  
  BSP_OuputCycling_Init(OC_Frequency,OC_Duty);  
#endif
  
  while (1)
  { 
    PLC_Handler();
  }
}


/**
* @brief  Initialize current limiter and relay.
*         Configures IOs and peripherals
* @param  None
* @retval None
*/
static void initializePlc(void)
{
  /* Initialize Relay and Current Limiter */
  BSP_Relay_Init();
  BSP_CurrentLimiter_Init();  
}

/**
* @brief  Handles the relay and current limiter functions and demos
* @param  None
* @retval None
*/
static void PLC_Handler(void)
{
  if (INPUT_READ_FLAG)          
  {
    INPUT_READ_FLAG = 0;
    
#ifdef OUTPUT_CYCLING      
    /* Reset & set CS1 to refresh VNI watchdog */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);               
#else
    /* Handler for input current limiter */
    Input_Data = CURRENT_LIMITER_Handler();
    
    /* Handler for output relay */    
    RELAY_Handler(Input_Data);
#endif /* OUTPUT_CYCLING */  
  }      
}


/** 
  * @brief  Receive input data from Current Limiter and checks for error status
  * @param  None
  * @retval Pointer to input channels state
*/
uint8_t* CURRENT_LIMITER_Handler(void)
{
  static uint8_t* clData = NULL;
  
  clData = BSP_CURRENT_LIMITER_Read();
  
  Current_Limiter_Status = clData;
  
  if (BSP_GetCurrentLimiterStatus(Current_Limiter_Status) != CURRENT_LIMITER_OK)
  {
    /* Set input error code here */
  }
  
  return (clData+1);
}

/** 
  * @brief  Select output control function and set outputs state
  * @param  Pointer to input channels state
  * @retval None
*/
void RELAY_Handler(uint8_t* iData)
{
  /* Uncomment the relevant function as required */ 

  /* Set Output same as input */
  oData = BSP_Signal_Mirror(*iData);
  /* Freeze selected outputs for a given time */
//  oData = BSP_Output_Freeze(*iData,0xFF,&FreezeTime);
  /* Regroup outputs */
//  oData = BSP_Output_Regroup(0xFF);          
  /* Get the sum of input channels that are high */
//  ChannelsOn = BSP_Inputs_Sum(*iData); oData = 0x00;
  /* Set Outputs same as the required states */
//  oData = BSP_Output_ON(0xFF);  
  /* Set Outputs same as required states */
//  oData = BSP_Output_OFF(0xFF); 
  /* Set Outputs state according to the inputs state AND with required logic  */
//  oData = BSP_Inputs_AND(*iData,0x0F); 
  /* Set Outputs state according to the inputs state OR with required logic */
//  oData = BSP_Inputs_OR(*iData,0x0F); 
  /* Set Outputs state according to the inputs state NOT */
//  oData = BSP_Inputs_NOT(*iData); 
  /* Set Outputs state according to the inputs state XOR */
//  oData = BSP_Inputs_XOR(*iData,0x00); 
  
  Relay_Status = BSP_RELAY_SetOutputs(&oData);
  
  if (BSP_GetRelayStatus(Relay_Status) != RELAY_OK)
  {
    /* Set output error code here */
  }
}


/**
 * @brief  Systick callback implementation for systick interrupt
 * @param  None
 * @retval None
 */
void HAL_SYSTICK_Callback(void)
{
  if (WD_Reset <= 100)
    WD_Reset++;
  else
  {
    Tick_10++;
    if (Tick_10 == 10)
    {
      Tick_10 = 0;
      INPUT_READ_FLAG = 1;
    }
  }
  
  if (FreezeTime != 0)
    FreezeTime--;
}


#ifdef OUTPUT_CYCLING
/**
  * @brief  PWM Pulse finished callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* All channels OFF */
  oData = 0x00;
  /* frequency range is betwenn 50 Hz to 2kHz with duty cycle 10% to 90% */
  BSP_RELAY_SetOutputs(&oData);
}

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* All channels ON */
  oData = 0xFF;
  /* frequency range is betwenn 50 Hz to 2kHz with duty cycle 10% to 90% */
  BSP_RELAY_SetOutputs(&oData);
}

#endif /* OUTPUT_CYCLING */


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{

  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */


}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
