#pragma once

/* ---------------
     IocpObject
   --------------- */

// Completion Key
class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
    virtual HANDLE GetHandle() abstract;
    virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};


/*---------------
   	 IOCP Core
  ---------------*/

// IOCP의 핸들을 가지고 있다.
class IocpCore
{
public:
    IocpCore();
    ~IocpCore();

    HANDLE GetHandle() { return _iocpHandle; }

    // iocp에 등록한다.
    bool Register(IocpObjectPtr iocpObject);

    // iocp의 일감을 찾는다.
    bool Dispatch(uint32 timeoutMs = INFINITE);
private:
    HANDLE _iocpHandle;
};