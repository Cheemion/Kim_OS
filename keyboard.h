#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"
class KeyboardDriver : public InterruptHandler, public Driver
{
    Port8Bit dataport;
    Port8Bit commandport;
    
public:
    //! Default constructor
    KeyboardDriver(InterruptManager* manager);
    //! Destructor
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};
#endif
