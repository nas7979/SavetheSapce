#pragma once
#include "cTimeLine.h"
class cTimeLine_Boss1 : public cTimeLine
{
public:

	// cTimeLine��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;
};

