#include "../../include/drivers/mouse.h"
//! Default constructor


using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
MouseEventHandler::MouseEventHandler() {}
void MouseEventHandler::OnActivate(){}
void MouseEventHandler::OnMouseDown(uint8_t button){}
void MouseEventHandler::OnMouseUp(uint8_t button){}
void MouseEventHandler::OnMouseMove(int x, int y){}

MouseDriver::MouseDriver(InterruptManager *manager, MouseEventHandler* handler) :
InterruptHandler(0x2C, manager),
dataport(0x60)   ,
commandport(0x64)
{
    this->handler = handler;
}
//! Destructor
MouseDriver::~MouseDriver() {}

void MouseDriver::Activate() {
    offset = 0;
    buttons = 0;
    
    commandport.Write(0xA8); //active mouse
    commandport.Write(0x20); //get the current state
    uint8_t status = (dataport.Read() | 2);
    commandport.Write(0x60); //set back
    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4); // activate the mouse
    dataport.Read();
}
void printf(char *);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
    uint8_t status = commandport.Read();
    if((!(status & 0x20)))
	return esp;

    buffer[offset] = dataport.Read();

    if(handler == 0)
	return esp;

    offset = (offset + 1) % 3;
    if(offset == 0) {
	if(buffer[1] != 0 || buffer[2] != 0) {
	    handler->OnMouseMove((int8_t)buffer[1], -((int8_t)buffer[2]));
	}
	
	 for(uint8_t i = 0; i < 3; i++) {
	     if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i))){
		 if(buttons & (0x1 << i))
		     handler->OnMouseUp(i+1);
		 else
		     handler->OnMouseDown(i+1);
	     }
	 }
	 buttons = buffer[0];
    }
    return esp;
}

