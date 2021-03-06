
#ifndef __MOUSE_H
#define __MOUSE_H
#include "../common/types.h"
#include "../hardwarecommunication/port.h"
#include "../drivers/driver.h"
#include "../hardwarecommunication/interrupts.h"
using myos::common::uint8_t;
using myos::common::uint32_t;
namespace myos {
    namespace drivers {

class MouseEventHandler{
public:
    MouseEventHandler();
    virtual void OnMouseDown(uint8_t button);
    virtual void OnMouseUp(uint8_t button);
    virtual void OnMouseMove(int x, int y);
    virtual void OnActivate();
};

class MouseDriver : public myos::hardwarecommunication::InterruptHandler, public Driver
{
    myos::hardwarecommunication::Port8Bit dataport;
    myos::hardwarecommunication::Port8Bit commandport;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons; //last state
    MouseEventHandler* handler;
public:
    //! Default constructor
    MouseDriver(myos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
    //! Destructor
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};
    }
}
#endif
