#pragma once
class cTextRenderer : public cComponent
{
public:

	// cComponent��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;

	string m_Text;
	D3DCOLOR m_Color = 0xffffffff;
};

