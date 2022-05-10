#ifndef __MYOS__DRIVERS_VGA_H
#define __MYOS__DRIVERS_VGA_H

#include "../common/types.h"
#include "../hardwarecommunication/port.h"
#include "../drivers/driver.h"
#include "../hardwarecommunication/interrupts.h"
using myos::common::int32_t;
namespace myos {
    namespace drivers {
	class VideoGraphicsArray{
	    protected:
	    hardwarecommunication::Port8Bit miscPort;
	    hardwarecommunication::Port8Bit crtcIndexPort;
	    hardwarecommunication::Port8Bit crtcDataPort;
	    hardwarecommunication::Port8Bit sequenceIndexPort;
	    hardwarecommunication::Port8Bit sequenceDataPort;
	    hardwarecommunication::Port8Bit graphicsControllerIndexPort;
	    hardwarecommunication::Port8Bit graphicsControllerDataPort;
	    hardwarecommunication::Port8Bit attributeControllerIndexPort;
	    hardwarecommunication::Port8Bit attributeControllerReadPort;
	    hardwarecommunication::Port8Bit attributeControllerWritePort;
	    hardwarecommunication::Port8Bit attributeControllerResetPort;
	    void WriteResigers(uint8_t* registers);
	    uint8_t* GetFrameBufferSegment();
	    virtual uint8_t GetColorIndex(uint8_t r, uint8_t g, uint8_t b);
	    public:
	    VideoGraphicsArray();
	    ~VideoGraphicsArray();
	    virtual bool SupportsMode(uint32_t widht, uint32_t height, uint32_t colordepth);
	    virtual bool SetMode(uint32_t widht, uint32_t height, uint32_t colordepth);
	    virtual void PutPixel(int32_t x, int32_t y, uint8_t colorIndex);
	    virtual void PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
	    virtual void FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
	};
    }
}

#endif
