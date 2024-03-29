#include <drivers/amd_am79c973.h>
using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

amd_am79c973::amd_am79c973(myos::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,
        myos::hardwarecommunication::InterruptManager* interrupts)
        : Driver(), InterruptHandler(dev->interrupt + interrupts->hardwareInterruptOffset, interrupts),
            MACAddress0Port(dev->portBase),
            MACAddress2Port(dev->portBase + 0x02),
            MACAddress4Port(dev->portBase + 0x04),
            registerDataPort(dev->portBase + 0x10),
            registerAddressPort(dev->portBase + 0x12),
            resetPort(dev->portBase + 0x14),
            busControlRegisterDataPort(dev->portBase + 0x16)
{
    currentSendBuffer = 0;
    currentRecvBuffer = 0;
    
    uint64_t MAC0 = MACAddress0Port.Read() % 256;
    uint64_t MAC1 = MACAddress0Port.Read() / 256;
    uint64_t MAC2 = MACAddress2Port.Read() % 256;
    uint64_t MAC3 = MACAddress2Port.Read() / 256;
    uint64_t MAC4 = MACAddress4Port.Read() % 256;
    uint64_t MAC5 = MACAddress4Port.Read() / 256;
    
    uint64_t MAC = MAC5 << 40
                 | MAC4 << 32
                 | MAC3 << 24
                 | MAC2 << 16
                 | MAC1 << 8
                 | MAC0;
    
    // 32 bit mode
    registerAddressPort.Write(20);
    busControlRegisterDataPort.Write(0x102);
    
    // STOP reset
    registerAddressPort.Write(0);
    registerDataPort.Write(0x04);
    
    // initBlock
    initBlock.mode = 0x0000; // promiscuous mode = false
    initBlock.reserved1 = 0;
    initBlock.numSendBuffers = 3;
    initBlock.reserved2 = 0;
    initBlock.numRecvBuffers = 3;
    initBlock.physicalAddress = MAC;
    initBlock.reserved3 = 0;
    initBlock.logicalAddress = 0;
    
    sendBufferDescr = (BufferDescriptor*)((((uint32_t)&sendBufferDescrMemory[0]) + 15) & ~((uint32_t)0xF));
    initBlock.sendBufferDescrAddress = (uint32_t)sendBufferDescr;
    recvBufferDescr = (BufferDescriptor*)((((uint32_t)&recvBufferDescrMemory[0]) + 15) & ~((uint32_t)0xF));
    initBlock.recvBufferDescrAddress = (uint32_t)recvBufferDescr;
    
    for(uint8_t i = 0; i < 8; i++)
    {
        sendBufferDescr[i].address = (((uint32_t)&sendBuffers[i]) + 15 ) & ~(uint32_t)0xF;
        sendBufferDescr[i].flags = 0x7FF
                                 | 0xF000;
        sendBufferDescr[i].flags2 = 0;
        sendBufferDescr[i].avail = 0;
        
        recvBufferDescr[i].address = (((uint32_t)&recvBuffers[i]) + 15 ) & ~(uint32_t)0xF;
        recvBufferDescr[i].flags = 0xF7FF
                                 | 0x80000000;
        recvBufferDescr[i].flags2 = 0;
        sendBufferDescr[i].avail = 0;
    }

    registerAddressPort.Write(1);
    registerDataPort.Write(  (uint32_t)(&initBlock) & 0xFFFF );
    registerAddressPort.Write(2);
    registerDataPort.Write(  ((uint32_t)(&initBlock) >> 16) & 0xFFFF );
}


amd_am79c973::~amd_am79c973(){

}
void printf(char*);
common::uint32_t amd_am79c973::HandleInterrupt(common::uint32_t esp){
    printf("INTERRUPT FROM AMD am79c973\n");
    registerAddressPort.Write(0);
    uint32_t temp = registerDataPort.Read();
    if((temp & 0x8000) == 0x8000) printf("AMD am79c973 Error \n");
    if((temp & 0x2000) == 0x2000) printf("AMD am79c973 COLLISION Error \n");
    if((temp & 0x1000) == 0x1000) printf("AMD am79c973 Miss Frame \n");
    if((temp & 0x0800) == 0x0800) printf("AMD am79c973 Memory Error \n");
    if((temp & 0x0400) == 0x0400) printf("AMD am79c973 Data received \n");
    if((temp & 0x0200) == 0x0200) printf("AMD am79c973 Data SENT \n");

    // acknoledge
    registerAddressPort.Write(0);
    registerAddressPort.Write(temp);

    if((temp & 0x0200) == 0x0200) printf("AMD am79c973 Init Done \n");
    return esp;
}

void amd_am79c973::Activate(){
    registerAddressPort.Write(0);
    registerDataPort.Write(0x41);

    registerAddressPort.Write(4);
    uint32_t temp = registerDataPort.Read();
    registerAddressPort.Write(4);
    registerDataPort.Write(temp | 0xC00);
    
    registerAddressPort.Write(0);
    registerDataPort.Write(0x42);
}

int amd_am79c973::Reset(){
    resetPort.Read();
    resetPort.Write(0);
    return 10;
}