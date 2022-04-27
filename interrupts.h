#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H
#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager {
protected:
    // see IDT descriptor
    struct GateDescriptor {
	uint16_t handlerAddressLowBits;
	uint16_t gdt_codeSegmentSelector;
	uint8_t reserved;
	uint8_t access;
	uint16_t handlerAddressHighBits;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct InterruptDescriptorTablePointer {
	uint16_t size;
	uint16_t base;
    } __attribute__((packed));

    static void SetInterruptDescriptorTableEntry(uint8_t interruptNumber,
						 uint16_t gdt_codeSegmentSelectorOffset,
						 void (*handler)(),
						 uint8_t DescriptorPrivilegeLevel,
						 uint8_t DescriptorType);
public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();
    void Activate();
    static uint32_t handleInterrupt(uint8_t interrupt, uint32_t esp);
    static uint32_t IgnoreInterruptRequest();
    static uint32_t HandleInterruptRequest0x00();
    static uint32_t HandleInterruptRequest0x01();
};
#endif
