#include "interrupts.h"
#include "types.h"

void printf(char *str);
InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::ActiveInterruptManager = 0;
void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interrupt,
    uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType)
{
    // address of pointer to code segment (relative to global descriptor table)
    // and address of the handler (relative to segment)
    interruptDescriptorTable[interrupt].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt].handlerAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = CodeSegment;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interrupt].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    interruptDescriptorTable[interrupt].reserved = 0;
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
    asm volatile("lidt %0" : : "m" (idt));
}
void InterruptManager::Deactivate(){

    if(ActiveInterruptManager == this) {
	ActiveInterruptManager = 0;
	__asm__ volatile("cli");
    }
}

void InterruptManager::Activate(){

    if(ActiveInterruptManager != 0)
	ActiveInterruptManager->Deactivate();
    
    ActiveInterruptManager = this;
    __asm__ volatile("sti");
}
InterruptManager::~InterruptManager() {}


uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    printf(" INTERRUPT\n");
    
    return esp;
}

uint32_t InterruptManager::handleInterrupt(uint8_t  interruptNumber, uint32_t esp) {
    if(ActiveInterruptManager != 0) {
	return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }
    return esp;
}
