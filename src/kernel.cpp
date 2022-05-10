#include "../include/common/types.h"
#include "../include/gdt.h"
#include "../include/hardwarecommunication/interrupts.h"
#include "../include/hardwarecommunication/pci.h"
#include "../include/drivers/driver.h"
#include "../include/drivers/keyboard.h"
#include "../include/drivers/mouse.h"
#include "../include/drivers/vga.h"
#include "../include/gui/desktop.h"
#include "../include/gui/window.h"

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

// the screen is 25 height * 80 width
void printf(char* str) {
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0;
    for(int i = 0; str[i] != '\0'; i++) {
	switch (str[i]) {
	    case '\n':
		y++;
		x = 0;
		break;
	    default:
		VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
		x++;
		break;
	}
	if(x >= 80) {
	    y++;
	    x = 0;
	}
	if(y >= 25) {
	    for(y = 0; y < 25; y++) 
		for(x = 0; x < 80; x++) 
		    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
	    x = 0;
	    y = 0;
	}
    }
}

void printfHex(uint8_t key) {
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0F];
    printf(foo);
}

class PrintfKeyboardEverntHandler : public KeyboardEventHandler {
public:
    void OnKeyDown(char c){
	char* foo = " ";
	foo[0] = c;
	printf(foo);
    }
    void OnKeyUp(char c){
	
    }
};

class MouseToConsole: public MouseEventHandler{
    int8_t x, y;
public:
    MouseToConsole() {
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;
	x = 40;
	y = 12;
	VideoMemory[80*12+40] = ((VideoMemory[80*12+40] &0xF000) >> 4)
				| ((VideoMemory[80*12+40] &0x0F00) << 4)
				| ((VideoMemory[80*12+40] &0x00FF));

    }
    void OnMouseUp(int button){
	
    }
    void OnMouseDown(int button){
	
    }
    void OnMouseMove(int xoffset, int yoffset) {
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;
	VideoMemory[80*y+x] = ((VideoMemory[80*y+x] &0xF000) >> 4)
			    | ((VideoMemory[80*y+x] &0x0F00) << 4)
			    | ((VideoMemory[80*y+x] &0x00FF));
	x += xoffset;
	if(x < 0) x = 0;
	if(x >= 80) x = 79;
	y += yoffset;
	if(y < 0) y = 0;
	if(y >= 25) y = 24;
	VideoMemory[80*y+x] = ((VideoMemory[80*y+x] &0xF000) >> 4)
			    | ((VideoMemory[80*y+x] &0x0F00) << 4)
			    | ((VideoMemory[80*y+x] &0x00FF));
    }
};


typedef void (*constructor)();
extern "C" constructor start_ctors; 
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
  for(constructor* i = &start_ctors; i != &end_ctors; i++) {
    (*i)();
  }
}


extern "C" void kernelMain(void* multiboot_struct, uint32_t magicnumber) {
  printf("Hello World!\n");
  GlobalDescriptorTable gdt;
  InterruptManager interrupts(&gdt);
  printf("Initializing Hardware, Stage 1 \n");

  Desktop desktop(320,200,0x00,0x00,0xA8);
  DriverManager drvManager;


  // PrintfKeyboardEverntHandler kbhandler;
  //KeyboardDriver keyboard(&interrupts, &kbhandler);
  KeyboardDriver keyboard(&interrupts, &desktop);
  drvManager.AddDriver(&keyboard);

  //MouseToConsole mouseHandler;
  //MouseDriver mouse(&interrupts, &mouseHandler);
  MouseDriver mouse(&interrupts, &desktop);
  drvManager.AddDriver(&mouse);

  PeripheralComponentInterconnectController PCIController;
  PCIController.SelectDrivers(&drvManager, &interrupts);

  VideoGraphicsArray vga;
  
  printf("Initializing Hardware, Stage 2 \n");
  drvManager.ActivateAll();
  printf("Initializing Hardware, Stage 3 \n");

  vga.SetMode(320, 200, 8);

  Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00,0x00);
  desktop.AddChild(&win1);
  Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8,0x00);
  desktop.AddChild(&win2);
  interrupts.Activate();
  
  while(1){
    desktop.Draw(&vga);
  }
}
