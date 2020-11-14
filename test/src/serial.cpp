#include "serial.h"

void Serial::serialSetup()
{
    m_rxPin.pinSetup();
    m_rxPin.setPinAlternateFunction(1);

    m_txPin.pinSetup();
    m_txPin.setPinAlternateFunction(1);

    uint16_t baudRate = Utils::getClockFrequency() / m_bauds;

    #ifdef USART1
        if (m_usart == USART1)
        {
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            USART1->BRR = baudRate;
            USART1->CR1 |= (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE);
        }
    #endif
}
