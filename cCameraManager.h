#pragma once
class cCameraManager : public cSingleton<cCameraManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	Vec2 m_Pos;
	float m_Scale;
	float m_ScaleTo;
	Vec2 m_PosTo;
	Vec2 m_ShakePos;
	float m_ShakeForce;
	float m_ShakeTime;

	void Shake(float _Force, float _Time);
	void Move(Vec2 _To);
	void Scale(float _To);
};

#define CAMERA cCameraManager::GetInstance()