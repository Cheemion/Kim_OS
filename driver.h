#ifndef __DRIVER_H
#define __DRIVER_H

class Driver
{
public:
    //! Default constructor
    Driver();

    //! Destructor
    ~Driver();

    virtual void Activate();
    virtual int Reset();
    virtual void Deactivate();
};


class DriverManager
{
private:
    Driver* drivers[265];
    int numDrivers;

public:
    DriverManager();
    void AddDriver(Driver*);
    void ActivateAll();
};
#endif
