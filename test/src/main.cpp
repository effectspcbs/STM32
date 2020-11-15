#include "main.h"
#include "utils.h"
#include "hardware.h"
#include "serial.h"
#include "gpio.h"
#include "switch.h"
#include "led.h"

#include <stdlib.h>

int main(void) 
{
    Utils::clockSetup();
    Utils::setSysTickTimer();

    Bank bankA(GPIOA);
    bankA.bankSetup();
    Bank bankB(GPIOB);
    bankB.bankSetup();

    TemporarySwitch tempSwitch(& bankB, 1, pinInputPullUp, 500);
    tempSwitch.tempSwitchSetup();

    Led led(& bankB, 3, pinOutputPushPull);
    led.ledSetup();

    Serial serialPort1(USART1, & bankA, 15, 2, pinAlternateFunctionPP, 9600);
    serialPort1.serialSetup();

    RCC->AHBENR |= RCC_AHBENR_DMAEN;
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP;
    DMA1_Channel2->CCR &= ~(DMA_CCR_DIR | DMA_CCR_PL | DMA_CCR_MSIZE | DMA_CCR_PSIZE | DMA_CCR_MINC);
    DMA1_Channel2->CCR |= (DMA_CCR_DIR | DMA_CCR_PL_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_MINC);

    const char rxb[] = ""
    "ab";
    const size_t size = 20;

    USART1->CR3 |= USART_CR3_DMAT;

    DMA1_Channel2->CMAR = (uint32_t)&rxb;
    DMA1_Channel2->CPAR = (uint32_t)&(USART1->TDR);
    DMA1_Channel2->CNDTR = size;
    DMA1_Channel2->CCR |= DMA_CCR_EN;

    char rxb2 = '\0';

    while (1)
    {
        /**
        tempSwitch.tempSwitchPoll();
        led.blinkLed(500);

        if (tempSwitch.tempSwitchPushed())
        {
            led.ledSwitchState();
        }

        if (tempSwitch.tempSwitchLongPress())
        {
            led.ledSwitchState();
        }
        **/
       /**
        while( !( USART1->ISR & USART_ISR_RXNE ) ) {};
        rxb2 = USART1->RDR;

        while( !( USART1->ISR & USART_ISR_TXE ) ) {};
        USART1->TDR = rxb2;**/
    }
}
