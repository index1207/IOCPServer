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

// IOCP�� �ڵ��� ������ �ִ�.
class IocpCore
{
public:
    IocpCore();
    ~IocpCore();

    HANDLE GetHandle() { return _iocpHandle; }

    // iocp�� ����Ѵ�.
    bool Register(IocpObjectPtr iocpObject);

    // iocp�� �ϰ��� ã�´�.
    bool Dispatch(uint32 timeoutMs = INFINITE);
private:
    HANDLE _iocpHandle;
};