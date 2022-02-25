#pragma once

#include "command.h"

class CommandManager
{
private:

	std::map<std::wstring, Command> mCommands;

public:

	CommandManager();
	~CommandManager();

	bool CreateCommand(ID3D12Device* device, int frameIndex, std::wstring name);

	Command & GetCommand(std::wstring name);
};
