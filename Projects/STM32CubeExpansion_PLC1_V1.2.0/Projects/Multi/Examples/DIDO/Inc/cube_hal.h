/**
  ******************************************************************************
  * @file    Projects/Multi/Examples/DataLog/Inc/cube_hal.h
  * @author  CL
  * @version V1.1.0
  * @date    25-April-2016
  * @brief   Header for cube_hal_f0.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CUBE_HAL_H_
#define _CUBE_HAL_H_

/* Includes ------------------------------------------------------------------*/
#ifdef USE_STM32F0XX_NUCLEO
  #include "stm32f0xx_hal.h"
  #include "stm32f0xx_nucleo.h"
  #include "stm32f0xx_hal_conf.h"
  #include "stm32f0xx_hal_def.h"
#endif

#ifdef USE_STM32F4XX_NUCLEO
  #include "stm32f4xx_hal.h"
  #include "stm32f4xx_nucleo.h"
  #include "stm32f4xx_hal_conf.h"
  #include "stm32f4xx_hal_def.h"
#endif

#ifdef USE_STM32F3XX_NUCLEO
  #include "stm32f3xx_hal.h"
  #include "stm32f3xx_nucleo.h"
  #include "stm32f3xx_hal_conf.h"
  #include "stm32f3xx_hal_def.h"
#endif

#ifdef USE_STM32F1XX_NUCLEO
  #include "stm32f1xx_hal.h"
  #include "stm32f1xx_nucleo.h"
  #include "stm32f1xx_hal_conf.h"
  #include "stm32f1xx_hal_def.h"
#endif

void SystemClock_Config(void);

#endif //_CUBE_HAL_H_

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
