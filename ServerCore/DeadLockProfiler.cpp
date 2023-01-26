#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock); // ��Ƽ������ ȯ���̹Ƿ� ���� �ɾ���� �Ѵ�.
	
	// ���̵� ã�ų� �߱��Ѵ�.
	int32 lockId = 0; // ���� ������ ���̵�
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
	// ��� �ִ� ���� �־��ٸ�
	if (!LLockStack.empty())
	{
		// ������ �߰ߵ��� ���� ���̽���� ����� ���θ� �ٽ� Ȯ���Ѵ�.
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

	// ��� ������ ������ ��ȸ�Ѵ�
	auto findIter = _lockHistory.find(here);
	if (findIter == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}

	auto& nextSet = findIter->second;
	for (auto there : nextSet)
	{
		// ���� �湮�� ���� ���ٸ� �湮�Ѵ�.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here�� there���� ���� �߰ߵǾ��ٸ�, there�� here�� �ļ��̴�. (������ ����)
		if (_discoveredOrder[here] < _discoveredOrder[there])
		{
			continue;
		}

		// �������� �ƴϰ�, Dfs(there)�� ���� ������� �ʾҴٸ�, there�� here�� �����̴�. (������ ����)
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
