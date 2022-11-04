/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId CAN1_TaskHandle;
osThreadId CAN2_TaskHandle;
osThreadId LED_TaskHandle;
osThreadId HeartBeat_TaskHandle;
osThreadId Motor_Ctrl_TaskHandle;
osThreadId Imu_taskHandle;
osThreadId Gimbal_taskHandle;
osThreadId Shooting_TaskHandle;
osThreadId Vision_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Start_CAN1_Task(void const * argument);
void Start_CAN2_Task(void const * argument);
void Start_led_Task(void const * argument);
void Start_heart_beat_task(void const * argument);
void Start_motor_ctrl_task(void const * argument);
void Start_imu_task(void const * argument);
void Start_gimbal_task(void const * argument);
void Start_shooting_task(void const * argument);
void Start_vision_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of CAN1_Task */
  osThreadDef(CAN1_Task, Start_CAN1_Task, osPriorityNormal, 0, 128);
  CAN1_TaskHandle = osThreadCreate(osThread(CAN1_Task), NULL);

  /* definition and creation of CAN2_Task */
  osThreadDef(CAN2_Task, Start_CAN2_Task, osPriorityNormal, 0, 128);
  CAN2_TaskHandle = osThreadCreate(osThread(CAN2_Task), NULL);

  /* definition and creation of LED_Task */
  osThreadDef(LED_Task, Start_led_Task, osPriorityNormal, 0, 128);
  LED_TaskHandle = osThreadCreate(osThread(LED_Task), NULL);

  /* definition and creation of HeartBeat_Task */
  osThreadDef(HeartBeat_Task, Start_heart_beat_task, osPriorityNormal, 0, 128);
  HeartBeat_TaskHandle = osThreadCreate(osThread(HeartBeat_Task), NULL);

  /* definition and creation of Motor_Ctrl_Task */
  osThreadDef(Motor_Ctrl_Task, Start_motor_ctrl_task, osPriorityNormal, 0, 128);
  Motor_Ctrl_TaskHandle = osThreadCreate(osThread(Motor_Ctrl_Task), NULL);

  /* definition and creation of Imu_task */
  osThreadDef(Imu_task, Start_imu_task, osPriorityHigh, 0, 128);
  Imu_taskHandle = osThreadCreate(osThread(Imu_task), NULL);

  /* definition and creation of Gimbal_task */
  osThreadDef(Gimbal_task, Start_gimbal_task, osPriorityNormal, 0, 128);
  Gimbal_taskHandle = osThreadCreate(osThread(Gimbal_task), NULL);

  /* definition and creation of Shooting_Task */
  osThreadDef(Shooting_Task, Start_shooting_task, osPriorityNormal, 0, 128);
  Shooting_TaskHandle = osThreadCreate(osThread(Shooting_Task), NULL);

  /* definition and creation of Vision_Task */
  osThreadDef(Vision_Task, Start_vision_task, osPriorityNormal, 0, 128);
  Vision_TaskHandle = osThreadCreate(osThread(Vision_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Start_CAN1_Task */
/**
* @brief Function implementing the CAN1_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_CAN1_Task */
__weak void Start_CAN1_Task(void const * argument)
{
  /* USER CODE BEGIN Start_CAN1_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_CAN1_Task */
}

/* USER CODE BEGIN Header_Start_CAN2_Task */
/**
* @brief Function implementing the CAN2_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_CAN2_Task */
__weak void Start_CAN2_Task(void const * argument)
{
  /* USER CODE BEGIN Start_CAN2_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_CAN2_Task */
}

/* USER CODE BEGIN Header_Start_led_Task */
/**
* @brief Function implementing the LED_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_led_Task */
__weak void Start_led_Task(void const * argument)
{
  /* USER CODE BEGIN Start_led_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_led_Task */
}

/* USER CODE BEGIN Header_Start_heart_beat_task */
/**
* @brief Function implementing the HeartBeat_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_heart_beat_task */
__weak void Start_heart_beat_task(void const * argument)
{
  /* USER CODE BEGIN Start_heart_beat_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_heart_beat_task */
}

/* USER CODE BEGIN Header_Start_motor_ctrl_task */
/**
* @brief Function implementing the Motor_Ctrl_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_motor_ctrl_task */
__weak void Start_motor_ctrl_task(void const * argument)
{
  /* USER CODE BEGIN Start_motor_ctrl_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_motor_ctrl_task */
}

/* USER CODE BEGIN Header_Start_imu_task */
/**
* @brief Function implementing the Imu_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_imu_task */
__weak void Start_imu_task(void const * argument)
{
  /* USER CODE BEGIN Start_imu_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_imu_task */
}

/* USER CODE BEGIN Header_Start_gimbal_task */
/**
* @brief Function implementing the Gimbal_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_gimbal_task */
__weak void Start_gimbal_task(void const * argument)
{
  /* USER CODE BEGIN Start_gimbal_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_gimbal_task */
}

/* USER CODE BEGIN Header_Start_shooting_task */
/**
* @brief Function implementing the Shooting_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_shooting_task */
__weak void Start_shooting_task(void const * argument)
{
  /* USER CODE BEGIN Start_shooting_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_shooting_task */
}

/* USER CODE BEGIN Header_Start_vision_task */
/**
* @brief Function implementing the Vision_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_vision_task */
__weak void Start_vision_task(void const * argument)
{
  /* USER CODE BEGIN Start_vision_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_vision_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
