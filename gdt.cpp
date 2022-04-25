#include "gdt.h"
#include "types.h"
GlobalDescriptorTable::GlobalDescriptorTable() :
nullSegmentSelector(0, 0, 0),
unusedSegmentSelector(0, 0, 0),
codeSegmentSelector(0, 64*1024*1024, 0x9A), //0x1001,1010, 1 means in memory, 00 means highest priority, 1 means code or data, 1010 means code read or write
dataSegmentSelector(0, 64*1024*1024, 0x92) { // 0x1001,0010, 0010means datasegment read or write
    uint32_t i[2];
    i[0] = (uint32_t)this;
    i[1] = sizeof(GlobalDescriptorTable) << 16; // size of tables
    // Loads the values in the source operand into the global descriptor table register (GDTR) or the interrupt descriptor table register (IDTR). The source operand specifies a 6-byte memory location that contains the base address (a linear address) and the limit (size of table in bytes) of the global descriptor table (GDT) or the interrupt descriptor table (IDT)
    //using this table
    asm volatile("lgdt (%0)": :"p" (((uint8_t *)i)+2));
}
GlobalDescriptorTable::~GlobalDescriptorTable(){
    
}
uint16_t GlobalDescriptorTable::DataSegmentSelector() {
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}
uint16_t GlobalDescriptorTable::CodeSegmentSelector() {
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}


// check GDT format
GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base,uint32_t limit, uint8_t flags) {
    uint8_t* target = (uint8_t*)this;
    //setting limit
    if(limit <= 65536) { // range from 1 byte to 2^16 byte
	target[6] = 0x40; //0100,0000, high 4 bits are settings for GDT
    } else {
	if((limit & 0xFFF) != 0xFFF)
	    limit = (limit >> 12) - 1;
	else
	    limit = limit >> 12;
	target[6] = 0xC0; //1100,0000
    }
    target[0] = limit & 0xFF;
    target[0] = (limit >> 8) & 0xFF;
    target[0] = (limit >> 8) & 0xFF;
    target[6] = target[6] | (limit >> 16);
    //setting ptr
    target[2] = (base >> 0) & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    target[5] = flags;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base() {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}
uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit() {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0)
	result = (result << 12) | 0xFFF;
    
    return result;
}
