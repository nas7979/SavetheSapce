#pragma once
class cLoadingScene : public cScene
{
public:

	// cScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void RenderBack() override;
	virtual void Release() override;

	int m_Wait;
};

