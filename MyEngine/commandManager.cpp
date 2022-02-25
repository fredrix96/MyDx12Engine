#include "commandManager.h"

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

bool CommandManager::CreateCommand(ID3D12Device* device, int frameIndex, std::wstring name)
{
	Command cmd;
	bool isCreated = cmd.CreateCommandAllocators(device, name);
	
	if (isCreated)
	{
		isCreated = cmd.CreateCommandList(device, frameIndex, name);

		if (isCreated)
		{
			mCommands.emplace(name, cmd);
		}
	}

	return isCreated;
}

Command & CommandManager::GetCommand(std::wstring name)
{
	return mCommands.at(name);
}

