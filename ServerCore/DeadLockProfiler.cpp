#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock); // 멀티스레드 환경이므로 락을 걸어줘야 한다.
	
	// 아이디를 찾거나 발급한다.
	int32 lockId = 0; // 현재 스레드 아이디
	auto findIter = _nameToId.find(name);
	if (findIter == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findIter->second;
	}
	// 잡고 있는 락이 있었다면
	if (!LLockStack.empty())
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부를 다시 확인한다.
		const auto prevId = LLockStack.top();
		if (lockId != prevId)
		{
			auto& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}
	LLockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (LLockStack.empty())
	{
		CRASH("MULTIFLE_UNLOCK");
	}

	auto lockId = _nameToId[name];
	if (LLockStack.top() != lockId)
	{
		CRASH("INVALID_UNLOCK")
	}

	LLockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const auto lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; ++lockId)
	{
		Dfs(lockId);
	}

	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	if (_discoveredOrder[here] != -1)
	{
		return;
	}

	_discoveredOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회한다
	auto findIter = _lockHistory.find(here);
	if (findIter == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}

	auto& nextSet = findIter->second;
	for (auto there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here가 there보다 먼저 발견되었다면, there은 here의 후손이다. (순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
		{
			continue;
		}

		// 순방향이 아니고, Dfs(there)가 아직 종료되지 않았다면, there은 here의 선조이다. (역방향 간선)
		if (_finished[there] == false)
		{
			cout << format("{} -> {}\n", _idToName[here], _idToName[there]);

			auto now = here;
			while (true)
			{
				cout << format("{} -> {}\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there) break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}
	_finished[here] = true;
}
