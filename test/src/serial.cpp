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

void Serial::enableDmaTx(DMA_TypeDef* periph, DMA_Channel_TypeDef* channel)
{
    m_dmaTxChannel = new DmaChannel(periph, channel);
    m_dmaTxChannel->dmaChannelSetup();

    #ifdef USART1
        if (m_usart == USART1)
        {
            SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP;
            USART1->CR3 |= USART_CR3_DMAT;
        }
    #endif

    m_dmaTxChannel->setDmaChannelSettings(readFromMemory, lowPriority, memSize8bits, periphSize8bits, memIncEnabled, transferCompleteIntEnabled, periphIncDisabled, mem2MemDisabled, circModeDisabled);
}

void Serial::print(const char* str, size_t len)
{
    if (m_dmaTxChannel->transferCompleted())
    {
        m_dmaTxChannel->clearTransferRegisters();
    }

    DMA1_Channel2->CMAR = (uint32_t)str;
    DMA1_Channel2->CPAR = (uint32_t)&(USART1->TDR);
    DMA1_Channel2->CNDTR = len;
    DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void Serial::startDmaTx(char* address, size_t length)
{
    if (m_dmaTxChannel->transferCompleted())
    {
        m_dmaTxChannel->clearTransferRegisters();
    }

    DMA1_Channel2->CMAR = (uint32_t)address;
    DMA1_Channel2->CPAR = (uint32_t)&(USART1->TDR);
    DMA1_Channel2->CNDTR = length;
    DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void Serial::clearDmaRegisters()
{
    m_dmaTxChannel->clearTransferRegisters();
}