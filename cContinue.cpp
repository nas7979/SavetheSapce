#include "DXUT.h"
#include "cContinue.h"

void cContinue::Init()
{
	m_Time = 6;
	m_Size = 2;
	m_Last = 0;
	GAME->m_TimeScale = 0;
	OBJECT->m_Stop = true;
	m_Alpha = 0;
}

void cContinue::Update()
{
	if (m_Last != (int)m_Time)
	{
		m_Last = m_Time;
		m_Size = 2;
	}
	m_Size = Lerp<float>(m_Size, 1, 0.1);

	if (m_Alpha != 0)
		m_Alpha += 255 * NSDT;

	if (m_Time <= 0 && m_Last != -100)
	{
		m_Last = -100;
		m_Time = -100;
		m_Alpha = 1;
		SCHEDULE->AddSchedule(1, []()->void {
			SCENE->ChangeScene("MENU");
		});
	}
	else
	{
		m_Time -= NSDT;
		if (INPUT->KeyDown(VK_SPACE))
		{
			GAME->m_Continued = true;
			m_Owner->m_isDestroyed = true;
			OBJECT->m_Player->SetAlarm("Continue", 1);
			OBJECT->m_Player->GetComponent<cPlayer>()->m_Life = 5;
		}
	}
}

void cContinue::Render()
{
	IMAGE->Render(IMAGE->m_Pixel, Vec2(0, 0), 0, Vec2(100000, 100000), 0.02, Vec2(0.5, 0.5), 0x90000000);
	IMAGE->Render(IMAGE->m_Pixel, Vec2(0, 0), 0, Vec2(100000, 100000), 0.001, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)m_Alpha, 0, 0, 0));
	IMAGE->RenderText("CONTINUE?", Vec2(480, 80), 0, Vec2(2, 2), 0.01, Vec2(0.5, 0.5), 0xffffff00);
	IMAGE->RenderText("PRESS SPACE TO CONTINUE", Vec2(480, 150), 0, Vec2(0.5, 0.5), 0.01, Vec2(0.5, 0.5), 0xffffff00);

	IMAGE->RenderText(to_string((int)Clamp(m_Time, 0, 5)), Vec2(480, 270), 0, Vec2(3, 3) * m_Size, 0.01, Vec2(0.5, 0.5), 0xffffff00);
}

void cContinue::Release()
{
	OBJECT->m_Stop = false;
	GAME->m_TimeScale = 1;
}

void cContinue::OnAlarm(string _Key)
{
}

void cContinue::OnCollision(cObject * _Other)
{
}
