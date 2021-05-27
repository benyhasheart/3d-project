#pragma once

class IGameLoop
{
public:
	IGameLoop() = default;
	virtual ~IGameLoop()= default;

public:
	virtual bool Initialize() = 0;
	virtual bool Update() = 0;
	virtual bool Render() = 0;
	virtual bool Release() = 0;
};

