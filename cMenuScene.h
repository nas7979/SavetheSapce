#pragma once
class cMenuScene : public cScene
{
public:

	// cScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void RenderBack() override;
	virtual void Release() override;

	string m_Text[4];
	float m_Size[4];
	D3DCOLOR m_Color[4];
	cParticle* m_Part;
	int m_Selected;
	bool m_Mask;
	bool m_MoveAble;
	float m_BackPos;
};

