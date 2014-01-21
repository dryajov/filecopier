#ifndef COPYENGINEDEFAULT_H
#define COPYENGINEDEFAULT_H

#include "copyengineabstract.h"

class CopyEngineDefault : public CopyEngineAbstract
{
public:
    CopyEngineDefault(QString source, QString dest, QString basePath, bool paused = false);
    virtual ~CopyEngineDefault() {}

    virtual bool copy(QString &errStr, ICopyEngineCallback *callback = 0);
};

#endif // COPYENGINEDEFAULT_H
