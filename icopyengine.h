#ifndef ICOPYENGINE_H
#define ICOPYENGINE_H

#include <QString>

class ICopyEngineCallback
{
public:
    virtual void writtenBytes(long long &bytes) = 0;
};

class ICopyEngine
{
public:
    virtual void resume()                                                   = 0;
    virtual void pause()                                                    = 0;
    virtual void cancel()                                                   = 0;
    virtual bool copy(QString &errStr, ICopyEngineCallback *callback = 0)   = 0;

    virtual ~ICopyEngine() {}
};

#endif // ICOPYENGINE_H
