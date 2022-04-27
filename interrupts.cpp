#include "interrupts.h"

void printf(char *str);
InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType) {
    const uint8_t IDT_DESC_PRESENT = 0x80; //0x1000,0000, 1 means present
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    //DescriptorPriviledgeLeve = 0,1,2,3
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt) :
picMasterCommand(0x20),
picMasterData(0x21),
picSlaveCommand(0xA0),
picSlaveData(0xA1)
{
    uint32_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE; // this means interrupt Gate

    for(uint16_t i = 0; i < 256; i++) 
	SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    // a bunch of settings
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20); // any interrupt, ad 0x20 to it , range from 0x20 to 0x27
    picSlaveData.Write(0x28); // any interrupt, ad 0x28 to it, the same
    
    picMasterData.Write(0x04); // telling this is the master 
    picSlaveData.Write(0x02); // telling this is the slave

    picMasterData.Write(0x01); // telling this is the master 
    picSlaveData.Write(0x01); // telling this is the slave

    picMasterData.Write(0x00); // telling this is the master 
    picSlaveData.Write(0x00); // telling this is the slave

    
    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    __asm__ volatile("lidt %0": : "m" (idt));
}
void InterruptManager::Activate(){
    __asm__ volatile("sti");
}
InterruptManager::~InterruptManager(){
    
}
uint32_t InterruptManager::handleInterrupt(uint8_t interrupt, uint32_t esp) {
    printf("interrupt");
    return esp;
}
