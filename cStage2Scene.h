#pragma once
class cStage2Scene : public cScene
{
public:

	// cScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void RenderBack() override;
	virtual void Release() override;

	float m_BackPos;
};

