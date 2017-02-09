

#include "base/pvf/pvfobject.hh"

#include <iostream>

PVFObject::PVFObject(PVFObjectParams *params) : SimObject(params)
{
    std::cout << "PVFObject initialized." << params << std::endl;
}

PVFObject *
PVFObjectParams::create() 
{
    return new PVFObject(this);
}