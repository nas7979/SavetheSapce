#pragma once
class cRankingScene : public cScene
{
public:

	// cScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void RenderBack() override;
	virtual void Release() override;

	bool m_Typing;
	string m_Name;
};

