#ifndef IAPP_MODULE_H
#define IAPP_MODULE_H

#include "hw/gfx_conf.h"

class IAppModule {
public:
    virtual ~IAppModule() = default;

    virtual const char* getName() const = 0;

    virtual const char* getIconPath() const = 0; 

    virtual void run() = 0;

    virtual bool isActive() const = 0;
};

#endif 