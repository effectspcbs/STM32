#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stm32f030x6.h>

// Port mode register values
#define GPIO_MODER_INPUT    0x0
#define GPIO_MODER_OUTPUT   0x1
#define GPIO_MODER_AF       0x2
#define GPIO_MODER_ANALOG   0x3

// Output mode type register values
#define GPIO_OTYPER_PP      0x0
#define GPIO_OTYPER_OD      0x1

// Output speed register values
#define GPIO_OSPEEDR_LOW    0x0
#define GPIO_OSPEEDR_MED    0x1
#define GPIO_OSPEEDR_HIGH   0x3

// Port PU/PD register
#define GPIO_PUPDR_NONE     0x0
#define GPIO_PUPDR_UP        0x1
#define GPIO_PUPDR_DOWN     0x2

enum pinSettings
{
    pinInputFloating,
    pinInputPullUp,
    pinInputPullDown,
    pinInputAnalog,
    pinOutputOpenDrain,
    pinOutputPushPull,
};

class Bank
{
    private:
        GPIO_TypeDef* m_bank;

    public:
        Bank(GPIO_TypeDef* bank): m_bank(bank) { }

        /**
         * @brief Enable the peripheral clock if it's not on
         */
        void bankSetup();

        /**
         * @brief Set the MODER register
         * 
         * @param pin 
         * @param mode 
         * 0 : Input mode
         * 1 : Output mode
         * 2 : Alternate function mode
         * 3 : Analog mode
         */
        void setPinMode(uint8_t pin, uint8_t mode);

        /**
         * @brief Set the OTYPER register
         * 
         * @param pin 
         * @param type 
         * 0 : Push-Pull output
         * 1 : Open-drain output
         */
        void setPinType(uint8_t pin, uint8_t otype);

        /**
         * @brief Set the PUPDR register
         * 
         * @param pin 
         * @param pud 
         * 0 : No pull-up or pull-down resistor
         * 1 : Use a pull-up resistor
         * 2 : Use a pull-down resistor
         */
        void setPinPud(uint8_t pin, uint8_t pud);

        /**
         * @brief Set the OSPEEDR register
         * 
         * @param pin 
         * @param speed 
         * 0 : Low Speed
         * 1 : Mid SPeed
         * 2 : Low Speed
         * 3 : High Speed
         */
        void setPinSpeed(uint8_t pin, uint8_t ospeed);
};

class Pin
{
    private:
        Bank* m_pinBank;
        uint8_t m_pin;
        pinSettings m_pinSettings;

    public:
        Pin(Bank* bank, uint8_t pin, pinSettings settings) : m_pinBank(bank), m_pin(pin), m_pinSettings(settings) { }

        void pinSetup();
};

#endif