#ifndef __MYOS__HARDWARECOMMUNICSTION__INTERRUPTS_H
#define __MYOS__HARDWARECOMMUNICSTION__INTERRUPTS_H
#include "../common/types.h"
#include "../gdt.h"
#include "../multitasking.h"
#include "../hardwarecommunication/port.h"

using myos::common::uint8_t;
using myos::common::uint16_t;

using myos::common::uint32_t;
using myos::common::uint64_t;
namespace myos {
    namespace hardwarecommunication {

class InterruptManager;

class InterruptHandler {
protected:
    uint8_t interruptNumber;
    InterruptManager* interruptManager;
public:
    InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
    ~InterruptHandler();
public:
    virtual uint32_t HandleInterrupt(uint32_t esp);
};


class InterruptManager {
friend class InterruptHandler;
public:
    static InterruptManager* ActiveInterruptManager;
    InterruptHandler* handlers[256];
    TaskManager* taskManager;
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
	uint32_t base;
    } __attribute__((packed));
    uint16_t hardwareInterruptOffset;
    static void SetInterruptDescriptorTableEntry(uint8_t interruptNumber,
						 uint16_t codeSegmentSelectorOffset,
						 void (*handler)(),
						 uint8_t DescriptorPrivilegeLevel,
						 uint8_t DescriptorType);
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;
    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
public:
    InterruptManager(uint16_t hardwareInterruptOffset,myos::GlobalDescriptorTable* gdt, myos::TaskManager* taskManager);
    ~InterruptManager();
    void Activate();
    void Deactivate();
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
    uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);

    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
    static void HandleInterruptRequest0x0C();
};
    }
}
#endif
