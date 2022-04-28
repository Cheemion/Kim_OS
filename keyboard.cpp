#include "keyboard.h"
#include "types.h"
//! Default constructor
KeyboardDriver::KeyboardDriver(InterruptManager *manager) :
InterruptHandler(0x21, manager),
dataport(0x60)   ,
commandport(0x64)
{
    while(commandport.Read() & 0x1) // remove previous pressed keys
	dataport.Read();
    commandport.Write(0xAE); //sending keyboard interrupt
    commandport.Write(0x20); //get the current state
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); //change current state
    dataport.Write(status);

    dataport.Write(0xF4); // activate the keyboard
}
//! Destructor
KeyboardDriver::~KeyboardDriver() {}

void printf(char *);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    uint8_t key = dataport.Read();
    if(key < 0x80) { // only focus on the pressing
	//when pressing the key, we got a interrupt with the highest bit being the zero
	//when reselasing the key, we got a interrupt with the least bit being the one
	switch(key) {
	case 0xFA: break;
	case 0x45: case 0xC5: break;
	    default:
		char* foo = "KEYBOARD 0x00";
		char* hex = "0123456789ABCDEF";
		foo[11] = hex[(key >> 4) & 0x0F];
		foo[12] = hex[key & 0x0F];
		printf(foo);
	}
    }
    return esp;
}
