#ifndef __MYOS__DRIVERS__KEYBOARD_H
#define __MYOS__DRIVERS__KEYBOARD_H

#include "driver.h"
#include "../common/types.h"
#include "../hardwarecommunication/interrupts.h"
#include "../hardwarecommunication/port.h"

using myos::common::uint32_t;
namespace myos {
    namespace drivers {
	
class KeyboardEventHandler{
public:
    KeyboardEventHandler();
    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);
};

class KeyboardDriver : public myos::hardwarecommunication::InterruptHandler, public Driver
{
    myos::hardwarecommunication::Port8Bit dataport;
    myos::hardwarecommunication::Port8Bit commandport;
    KeyboardEventHandler* handler; 
public:
    //! Default constructor
    KeyboardDriver(myos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
    //! Destructor
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};
    }
}
#endif
