#ifndef __MYOS__DRIVERS__KEYBOARD_H
#define __MYOS__DRIVERS__KEYBOARD_H
#include <../common/types.h>
#include <hardwarecommunication/interrupts.h> 
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace myos {
    namespace drivers {
	class KeyboardEventHandler{
	public:
	    KeyboardEventHandler();
	    virtual void OnKeyDown(char);
	    virtual void OnKeyUp(char);
	};

	class KeyboardDriver : public InterruptHandler, public Driver
	{
	    Port8Bit dataport;
	    Port8Bit commandport;
	    KeyboardEventHandler* handler; 
	public:
	    //! Default constructor
	    KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler);
	    //! Destructor
	    ~KeyboardDriver();
	    virtual uint32_t HandleInterrupt(uint32_t esp);
	    virtual void Activate();
	};
    }
}
#endif
