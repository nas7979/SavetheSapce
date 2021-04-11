#include "DXUT.h"
#include "cScore.h"

void cScore::Init()
{
	m_Alpha = 0;
	m_BonusEnd = false;
	m_TargetBonus = (int)(GAME->m_Percentage * 100);
	m_CurBonus = 8000;
	AddBonus();
}

void cScore::Update()
{
	m_Alpha += 255 * DT;
	if (m_Alpha > 255)
		m_Alpha = 255;

	if (m_BonusEnd)
	{
		if (INPUT->KeyDown(VK_SPACE))
		{
			cParticle* Part = OBJECT->AddObject<cParticle>("Particle", Vec2(0, 0), 0, Obj_Particle)->GetComponent<cParticle>();
			Part->m_Owner->m_Scale = Vec2(10000, 10000);
			Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
			Part->GetComponent<cRenderer>()->m_Color = 0x00000000;
			Part->m_Alp = 0;
			Part->m_AlpVel = 255;
			SCHEDULE->AddSchedule(1, []()->void {
				SCENE->ChangeScene(SCENE->m_NextScene);
			});
		}
	}
}

void cScore::Render()
{
	IMAGE->Render(IMAGE->m_Pixel, Vec2(0, 0), 0, Vec2(10000, 10000), 0.05, Vec2(0, 0), D3DCOLOR_ARGB((int)(m_Alpha * 0.5), 0, 0, 0));

	char Text[16];
	int Cur = 0;
	int Max = m_Bonuses.size() - 1;
	for (auto& iter : m_Bonuses)
	{
		sprintf(Text, "%.2f%%", iter * 0.01);
		if (Cur != Max)
		{
			IMAGE->RenderText(Text, Vec2(280, 75 + Cur * 35), 0, Vec2(0.5, 0.5), 0.04, Vec2(0, 0.5), D3DCOLOR_ARGB((int)(m_Alpha), 255, 255, 255));
			IMAGE->RenderText("+" + to_string((int)(100000 + pow(iter - 8000, 2) * 10)), Vec2(680, 75 + Cur * 35), 0, Vec2(0.5, 0.5), 0.04, Vec2(1, 0.5), D3DCOLOR_ARGB((int)(m_Alpha), 255, 255, 255));
		}
		else
		{
			IMAGE->RenderText(Text, Vec2(280, 75 + Cur * 35), 0, Vec2(0.5, 0.5), 0.04, Vec2(0, 0.5), D3DCOLOR_ARGB((int)(m_Alpha), 255, 0, 0));
			IMAGE->RenderText("+" + to_string(5 * (int)(100000 + pow(iter - 8000, 2) * 10)), Vec2(680, 75 + Cur * 35), 0, Vec2(0.5, 0.5), 0.04, Vec2(1, 0.5), D3DCOLOR_ARGB((int)(m_Alpha), 255, 0, 0));
			IMAGE->RenderText("LIFE BONUS x" + to_string(OBJECT->m_Player->GetComponent<cPlayer>()->m_Life), Vec2(700, 75 + Cur * 35), 0, Vec2(0.5, 0.5), 0.04, Vec2(0, 0.5), D3DCOLOR_ARGB((int)(m_Alpha), 255, 0, 0));
		}
		Cur++;
	}

	IMAGE->RenderText("BONUS SCORE", Vec2(480, 30), 0, Vec2(1, 1), 0.04, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)(m_Alpha), 255, 0, 0));
	IMAGE->RenderText("TOTAL SCORE: " + to_string((int)GAME->m_Score), Vec2(480, 480), 0, Vec2(1, 1), 0.04, Vec2(0.5, 0.5), m_BonusEnd ? D3DCOLOR_ARGB((int)(m_Alpha), 255, 0, 0) : D3DCOLOR_ARGB((int)(m_Alpha), 255, 255, 255));
}

void cScore::Release()
{
}

void cScore::OnAlarm(string _Key)
{
}

void cScore::OnCollision(cObject * _Other)
{
}

void cScore::AddBonus()
{
	SCHEDULE->AddSchedule(0.02, [&]()->void {
		for (int i = 0; i < 15; i++)
		{
			m_Bonuses.push_back(m_CurBonus);
			if (m_Bonuses.size() == 11)
				m_Bonuses.pop_front();

			if (m_CurBonus == m_TargetBonus)
			{
				m_Bonus = 100000 + pow(m_CurBonus - 8000, 2) * 10 * OBJECT->m_Player->GetComponent<cPlayer>()->m_Life;
				GAME->m_Score += m_Bonus;
				m_BonusEnd = true;
				return;
			}
			m_CurBonus++;
		}
		AddBonus();
	});
}
