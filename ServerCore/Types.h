#pragma once

#include <mutex>
#include <atomic>

using Byte = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

template<class Type>
using Atomic = std::atomic<Type>;
using Mutex = std::mutex;
using CondVar = std::condition_variable;
using UniqueLock = std::unique_lock<std::mutex>;
using LockGuard = std::lock_guard<std::mutex>;

using IocpCorePtr = std::shared_ptr<class IocpCore>;
using IocpObjectPtr = std::shared_ptr<class IocpObject>;
using IocpEventPtr = std::shared_ptr<class IocpEvent>;
using SessionPtr = std::shared_ptr<class Session>;
using ListenerPtr = std::shared_ptr<class Listener>;
using ServerServicePtr = std::shared_ptr<class ServerService>;

#define size16(val) static_cast<int16>(sizeof(val))
#define size32(val) static_cast<int32>(sizeof(val))
#define len16(arr) static_cast<int16>(sizeof(arr)/sizeof(*arr))
#define len32(arr) static_cast<int32>(sizeof(arr)/sizeof(*arr))

#define _STOMP
