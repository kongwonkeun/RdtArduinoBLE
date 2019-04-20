/*
 * Tc5Timer.h
 *
 * Created: 2019-04-19 13:29:48
 *  Author: kong
 */

#ifndef TC5TIMER_H_
#define TC5TIMER_H_

#include "LinkedList.h"

class Tc5Timer
{
public:
    Tc5Timer();
    ~Tc5Timer();
    void begin(uint32_t interval);
    void end();
    bool isSyncing();
    void addCallback(void(*callback)());
    void runCallback();
    void reset();
    void start();
    void stop();
private:
    uint32_t m_tickInterval;
    LinkedList<void(*)()> m_callbacks;
};

extern Tc5Timer x_tc5_tick;

#endif /* TC5TIMER_H_ */