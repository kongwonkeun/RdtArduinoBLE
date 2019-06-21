/*
 * Tc5Timer.cpp
 *
 * Created: 2019-04-19 13:30:25
 *  Author: kong
 */

#include <Arduino.h>
#include "Tc5Timer.h"

void TC5_Handler(void)
{
    x_tc5_tick.runCallback();
    TC5->COUNT16.INTFLAG.bit.MC0 = 1;
}

Tc5Timer::Tc5Timer() : m_tickInterval(0)
{
}

Tc5Timer::~Tc5Timer()
{
    end();
}

void Tc5Timer::begin(uint32_t interval)
{
    m_tickInterval = interval;
    GCLK->CLKCTRL.reg = (uint16_t)(GCLK_CLKCTRL_CLKEN
        | GCLK_CLKCTRL_GEN_GCLK0
        | GCLK_CLKCTRL_ID(GCM_TC4_TC5)
    );
    while (GCLK->STATUS.bit.SYNCBUSY);
    reset();
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
    // TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE;
    // TC5->COUNT16.CC[0].reg  = (uint16_t)((SystemCoreClock / 1000) * m_tickInterval); // 1MHz at Reset
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_ENABLE; // prescaler = 1024
    TC5->COUNT16.CC[0].reg  = (uint16_t)((46875 / 1000) * m_tickInterval); // 48MHz/1024 = 46875 (1sec)
    while (isSyncing());
    NVIC_DisableIRQ(TC5_IRQn);
    NVIC_ClearPendingIRQ(TC5_IRQn);
    NVIC_SetPriority(TC5_IRQn, 0);
    NVIC_EnableIRQ(TC5_IRQn);
    TC5->COUNT16.INTENSET.bit.MC0 = 1;
    while (isSyncing());
}

void Tc5Timer::end()
{
    stop();
}

bool Tc5Timer::isSyncing()
{
    return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

void Tc5Timer::addCallback(void(*callback)())
{
    m_callbacks.add(callback);
}

void Tc5Timer::runCallback()
{
    unsigned int size = m_callbacks.size();
    void (*f)();
    for (unsigned int i = 0; i < size; i++) {
        f = m_callbacks.get(i);
        f();
    }
}

void Tc5Timer::reset()
{
    TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while (isSyncing());
    while (TC5->COUNT16.CTRLA.bit.SWRST);
}

void Tc5Timer::start()
{
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (isSyncing());
}

void Tc5Timer::stop()
{
    TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (isSyncing());
}

Tc5Timer x_tc5_tick;

/* EOF */