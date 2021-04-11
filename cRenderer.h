#pragma once
class cRenderer : public cComponent
{
public:
	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;

	Texture* m_Sprite = nullptr;
	D3DCOLOR m_Color = 0xffffffff;
};