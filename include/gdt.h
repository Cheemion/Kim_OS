#ifndef __MYOS__GDT_H
#define __MYOS__GDT_H
#include "common/types.h"

using myos::common::uint16_t;
using myos::common::uint32_t;
using myos::common::uint8_t;

namespace myos {
class GlobalDescriptorTable {
public:
    class SegmentDescriptor {
    private:
	uint16_t limit_lo; // initial byte
	uint16_t base_lo;
	uint8_t base_hi;
	uint8_t type;
	uint8_t flags_limit_hi;
	uint8_t base_vhi; //base very high, this is the last byte from GDT
    public:
	SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
	uint32_t Base();
	uint32_t Limit();
    } __attribute__((packed));
    
    SegmentDescriptor nullSegmentSelector; // the first segment descriptor never used
    SegmentDescriptor unusedSegmentSelector; //
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;

public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();
    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();
};
}
#endif
