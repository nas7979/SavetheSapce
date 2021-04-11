#pragma once
class cMainGame
{
public:
	void Init();
	void Update();
	void Render();
	void Release();
	void OnLostDevice();
	void OnResetDevice();
	void OnModifyDevice();
};

