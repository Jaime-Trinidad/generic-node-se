/**
  ******************************************************************************
  * @file    lora_info.c
  * @author  MCD Application Team
  * @brief   To give info to the application about LoRaWAN configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LoRaMac.h"
#include "lora_info.h"
#include "app.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static LoraInfo_t loraInfo = {0, 0};

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported functions --------------------------------------------------------*/
void LoraInfo_Init(void)
{
  loraInfo.ActivationMode = 0;
  loraInfo.Region = 0;
  loraInfo.ClassB = 0;
  loraInfo.Kms = 0;
  /* USER CODE BEGIN LoraInfo_Init_1 */

  /* USER CODE END LoraInfo_Init_1 */

#ifdef  REGION_AS923
  loraInfo.Region |= (1 << LORAMAC_REGION_AS923) ;
#endif /* REGION_AS923 */
#ifdef  REGION_AU915
  loraInfo.Region |= (1 << LORAMAC_REGION_AU915) ;
#endif /* REGION_AU915 */
#ifdef  REGION_CN470
  loraInfo.Region |= (1 << LORAMAC_REGION_CN470) ;
#endif /* REGION_CN470 */
#ifdef  REGION_CN779
  loraInfo.Region |= (1 << LORAMAC_REGION_CN779) ;
#endif /* REGION_CN779 */
#ifdef  REGION_EU433
  loraInfo.Region |= (1 << LORAMAC_REGION_EU433) ;
#endif /* REGION_EU433 */
#ifdef  REGION_EU868
  loraInfo.Region |= (1 << LORAMAC_REGION_EU868) ;
#endif /* REGION_EU868 */
#ifdef  REGION_KR920
  loraInfo.Region |= (1 << LORAMAC_REGION_KR920) ;
#endif /* REGION_KR920 */
#ifdef  REGION_IN865
  loraInfo.Region |= (1 << LORAMAC_REGION_IN865) ;
#endif /* REGION_IN865 */
#ifdef  REGION_US915
  loraInfo.Region |= (1 << LORAMAC_REGION_US915) ;
#endif /* REGION_US915 */
#ifdef  REGION_RU864
  loraInfo.Region |= (1 << LORAMAC_REGION_RU864) ;
#endif /* REGION_RU864 */

  if (loraInfo.Region == 0)
  {
    APP_PRINTF("error: At least one region shall be defined in the MW: check lorawan_conf.h \r\n");
    while (1) {} /* At least one region shall be defined */
  }

#if ( LORAMAC_CLASSB_ENABLED == 1 )
  loraInfo.ClassB = 1;
#elif !defined (LORAMAC_CLASSB_ENABLED)
#error LORAMAC_CLASSB_ENABLED not defined ( shall be <0 or 1> )
#endif /* LORAMAC_CLASSB_ENABLED */

#if (!defined (LORAWAN_KMS) || (LORAWAN_KMS == 0))
  loraInfo.Kms = 0;
  loraInfo.ActivationMode = 3;
#else /* LORAWAN_KMS == 1 */
  loraInfo.Kms = 1;
  loraInfo.ActivationMode = ACTIVATION_BY_PERSONALISATION + (OVER_THE_AIR_ACTIVATION << 1);
#endif /* LORAWAN_KMS */
  /* USER CODE BEGIN LoraInfo_Init_2 */

  /* USER CODE END LoraInfo_Init_2 */
}

LoraInfo_t *LoraInfo_GetPtr(void)
{
  /* USER CODE BEGIN LoraInfo_GetPtr */

  /* USER CODE END LoraInfo_GetPtr */
  return &loraInfo;
}

uint8_t GetBatteryLevel(void)
{
  uint8_t batteryLevel = 3; // Dummy value to use in the basic app, user can add desired implementation depending on the board.
  APP_LOG(TS_ON, VLEVEL_M, "VDDA= %d (Dummy value)\n\r", batteryLevel);

  return batteryLevel;
}

uint16_t GetTemperatureLevel(void)
{
  return 20; // Dummy value to use in the basic app, user can add desired implementation depending on the board.
}

void GetUniqueId(uint8_t *id)
{
  uint32_t val = 0;
  val = LL_FLASH_GetUDN();
  if (val == 0xFFFFFFFF)  /* Normally this should not happen */
  {
    uint32_t ID_1_3_val = HAL_GetUIDw0() + HAL_GetUIDw2();
    uint32_t ID_2_val = HAL_GetUIDw1();

    id[7] = (ID_1_3_val) >> 24;
    id[6] = (ID_1_3_val) >> 16;
    id[5] = (ID_1_3_val) >> 8;
    id[4] = (ID_1_3_val);
    id[3] = (ID_2_val) >> 24;
    id[2] = (ID_2_val) >> 16;
    id[1] = (ID_2_val) >> 8;
    id[0] = (ID_2_val);
  }
  else  /* Typical use case */
  {
    id[7] = val & 0xFF;
    id[6] = (val >> 8) & 0xFF;
    id[5] = (val >> 16) & 0xFF;
    id[4] = (val >> 24) & 0xFF;
    val = LL_FLASH_GetDeviceID();
    id[3] = val & 0xFF;
    val = LL_FLASH_GetSTCompanyID();
    id[2] = val & 0xFF;
    id[1] = (val >> 8) & 0xFF;
    id[0] = (val >> 16) & 0xFF;
  }
}

uint32_t GetDevAddr(void)
{
  uint32_t val = 0;
  val = LL_FLASH_GetUDN();
  if (val == 0xFFFFFFFF)
  {
    val = ((HAL_GetUIDw0()) ^ (HAL_GetUIDw1()) ^ (HAL_GetUIDw2()));
  }
  return val;
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions --------------------------------------------------------*/

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
