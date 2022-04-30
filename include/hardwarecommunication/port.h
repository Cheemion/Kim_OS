#ifndef __MYOS__HARDWARECOMMUNICATION__PORT_H
#define __MYOS__HARDWARECOMMUNICATION__PORT_H
#include "../common/types.h"

using myos::common::uint16_t;
using myos::common::uint8_t;
using myos::common::uint32_t;
namespace myos {
    namespace hardwarecommunication {

class Port
{
protected:
    uint16_t portnumber;
public:
    //! Default constructor
    Port(uint16_t portnumber);
    ~Port();
private:
};

class Port8Bit : public Port
{
public:
    //! Default constructor
    Port8Bit(uint16_t portnumber);

    //! Destructor
     ~Port8Bit();

    virtual void Write(uint8_t data);
    virtual uint8_t Read();
};

class Port16Bit : public Port
{
public:
    //! Default constructor
    Port16Bit(uint16_t portnumber);

    //! Destructor
    ~Port16Bit();

    virtual void Write(uint16_t data);
    virtual uint16_t Read();
};

class Port32Bit : public Port
{
public:
    //! Default constructor
    Port32Bit(uint16_t portnumber);

    //! Destructor
    ~Port32Bit();

    virtual void Write(uint32_t data);
    virtual uint32_t Read();
};


class Port8BitSlow : public Port8Bit
{
public:
    //! Default constructor
    Port8BitSlow(uint16_t portnumber);

    //! Destructor
     ~Port8BitSlow();

    virtual void Write(uint8_t data);
};
    }
}
#endif
