
#include "main.h"
#include <cstdint>
#include <stdint.h>

/**
 * UART Callback
*/
/*void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size){
    uint8_t buffer[4];
    uint8_t tx_buff[]={65,66,67,68,69,70,71,72,73,74}; //ABCDEFGHIJ in ASCII code
    uint8_t rx_buff[10];
    uint8_t test_data = 101;

    /*while(1) {
        HAL_UART_Receive(&huart5, buffer, sizeof(buffer), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart5, buffer, sizeof(buffer), HAL_MAX_DELAY);
    }*/

   
    /*HAL_UART_Receive_IT(&huart5, rx_buff, 10);
    HAL_Delay(100);
    HAL_UART_Transmit_IT(&huart5, tx_buff, 10);
   */
    
    
    /*
    while (1) {
        HAL_UART_Transmit(&huart5, &test_data, sizeof(test_data), HAL_MAX_DELAY);
    }
    */
// }

/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart5) {
        //uint8_t rx_buff[32];
        //uint8_t receivedData = huart5.Instance->RDR;
        //HAL_UART_Receive(&huart5, rx_buff, sizeof(rx_buff), 100);
        
        // uint8_t tx_buff[]={65,66,67,68,69,70,71,72,73,74}; //ABCDEFGHIJ in ASCII code
        HAL_UART_Receive_IT(&huart5, rx_buff, sizeof(rx_buff));
        HAL_Delay(10);
        HAL_UART_Transmit_IT(&huart5, rx_buff, sizeof(rx_buff));
        //HAL_UART_Transmit(&huart5, rx_buff, sizeof(rx_buff), 100);
    }
} */
