#include "DXUT.h"
#include "cGameManager.h"

void cGameManager::Init()
{
	m_Score = 0;
	m_TimeScale = 1;

	AddRanking("SHR", 1000000000);
	AddRanking("RIN", 900000000);
	AddRanking("KRI", 800000000);
	AddRanking("SHJ", 700000000);
	AddRanking("IRY", 600000000);
	AddRanking("KZK", 500000000);
	AddRanking("SKR", 400000000);
	AddRanking("TIG", 300000000);
	AddRanking("ITS", 200000000);
	AddRanking("ZKN", 100000000);
}

void cGameManager::Update()
{
	m_NSDT = DXUTGetElapsedTime();
	if (m_NSDT > 1.f / 60.f)
		m_NSDT = 1.f / 60.f;
	m_DT = m_NSDT * m_TimeScale;

	m_ScoreSize = Lerp<float>(m_ScoreSize, 1, 0.1);
	m_ScoreColor = Lerp<float>(m_ScoreColor, 0, 0.1);

	m_Percentage = (m_CellCount / (701 * 501)) * 100;

	if (INPUT->KeyDown(VK_F4))
		SCENE->ChangeScene("MENU");
	if (INPUT->KeyDown(VK_F5))
		SCENE->ChangeScene("STAGE1");
	if (INPUT->KeyDown(VK_F6))
		SCENE->ChangeScene("STAGE2");
	if (INPUT->KeyDown(VK_F7))
		SCENE->ChangeScene("STAGE3");
}

void cGameManager::Render()
{
	if (OBJECT->m_Player)
	{
		char Text[32];
		sprintf(Text, "%.2f%%", (int)(m_Percentage * 100) * 0.01f);
		IMAGE->RenderText("PERCENTAGE", Vec2(840, 20), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xff00ff00);
		IMAGE->RenderText(Text, Vec2(840, 60), 0, Vec2(1, 1), 0.1, Vec2(0.5, 0.5), 0xff00ff00);

		sprintf(Text, "%09d", m_Score);
		IMAGE->RenderText("SCORE", Vec2(840, 120), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xff00ff00);
		IMAGE->RenderText(Text, Vec2(840, 160), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xff00ff00);

		IMAGE->RenderText("SHIELD", Vec2(840, 220), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xff00ff00);
		IMAGE->RenderText(to_string((int)Clamp(OBJECT->m_Player->GetComponent<cPlayer>()->m_Shield, 0, 9999)), Vec2(840, 260), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xff00ff00);

		IMAGE->RenderText("ITEM", Vec2(840, 320), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xff00ff00);
		if(m_ItemText != "INVINCIBLE")
			IMAGE->RenderText(m_ItemText, Vec2(840, 360), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), m_ItemColor);
		else
			IMAGE->RenderText("INVINCIBLE (" + to_string((int)OBJECT->m_Player->GetComponent<cPlayer>()->m_Invincible) + ")", Vec2(840, 360), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), m_ItemColor);


		IMAGE->RenderText("LIFE", Vec2(840, 420), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xff00ff00);
		for (int i = 0; i < OBJECT->m_Player->GetComponent<cPlayer>()->m_Life; i++)
		{
			IMAGE->Render(IMAGE->Find("Player"), Vec2(840 - 20 * (OBJECT->m_Player->GetComponent<cPlayer>()->m_Life - 1) + i * 40, 460), 90, Vec2(2, 2), 0.1, Vec2(0.5, 0.5));
		}
	}
}

void cGameManager::Release()
{
	for (auto& iter : m_Ranking)
	{
		delete iter;
	}
}

void cGameManager::PlayerInit()
{
	IMAGE->ClearSurface("Outline");
	IMAGE->ClearSurface("Drawing");
	IMAGE->ClearSurface("Ground");
	IMAGE->ClearSurface("Glow");
	memset(m_Map, (short)0, sizeof(m_Map));
	m_ItemColor = 0xff00ff00;
	m_ItemText = "NONE";
	m_TimeScale = 1;
	m_CellCount = 0;
	m_Percentage = 0;
	m_Win = false;

	IMAGE->BeginSurface("Outline");
	for (int i = 0; i < 701; i++)
	{
		m_Map[i][0] = Cell_Player;
		m_Map[i][MAPY] = Cell_Player;
		OBJECT->m_Player->GetComponent<cPlayer>()->m_Outlines.push_back(Vec2(i, 0) + Vec2(OFFSET, OFFSET));
		OBJECT->m_Player->GetComponent<cPlayer>()->m_Outlines.push_back(Vec2(i, MAPY) + Vec2(OFFSET, OFFSET));
		IMAGE->Render(IMAGE->m_Pixel, Vec2(OFFSET + i, OFFSET + 0), 0, Vec2(1, 1), 0.1, Vec2(0, 0), 0xff00ff00);
		IMAGE->Render(IMAGE->m_Pixel, Vec2(OFFSET + i, OFFSET + MAPY), 0, Vec2(1, 1), 0.1, Vec2(0, 0), 0xff00ff00);
	}
	for (int i = 0; i < 501; i++)
	{
		m_Map[0][i] = Cell_Player;
		m_Map[MAPX][i] = Cell_Player;
		OBJECT->m_Player->GetComponent<cPlayer>()->m_Outlines.push_back(Vec2(0, i) + Vec2(OFFSET, OFFSET));
		OBJECT->m_Player->GetComponent<cPlayer>()->m_Outlines.push_back(Vec2(MAPX, i) + Vec2(OFFSET, OFFSET));
		IMAGE->Render(IMAGE->m_Pixel, Vec2(OFFSET + 0, OFFSET + i), 0, Vec2(1, 1), 0.1, Vec2(0, 0), 0xff00ff00);
		IMAGE->Render(IMAGE->m_Pixel, Vec2(OFFSET + MAPX, OFFSET + i), 0, Vec2(1, 1), 0.1, Vec2(0, 0), 0xff00ff00);
	}
	IMAGE->EndSurface();
}

void cGameManager::SetCell(Vec2 _Pos, short _Cell)
{
	_Pos -= Vec2(OFFSET, OFFSET);
	_Pos.x = Clamp(_Pos.x, 0, MAPX);
	_Pos.y = Clamp(_Pos.y, 0, MAPY);
	m_Map[(int)_Pos.x][(int)_Pos.y] = _Cell;
}

short cGameManager::GetCell(Vec2 _Pos)
{
	_Pos -= Vec2(OFFSET, OFFSET);
	_Pos.x = Clamp(_Pos.x, 0, MAPX);
	_Pos.y = Clamp(_Pos.y, 0, MAPY);
	return m_Map[(int)_Pos.x][(int)_Pos.y];
}

void cGameManager::AddScore(int _Score)
{
	m_Score += _Score;
	m_ScoreSize = 1 + Clamp(_Score / 1000000, 0.1, 1);
	m_ScoreColor = 255 + 255 * (m_ScoreSize - 1);
}

void cGameManager::AddRanking(string _Key, int _Score)
{
	RankData* a = new RankData;
	a->Key = _Key;
	a->Score = _Score;
	m_Ranking.push_back(a);
	m_Ranking.sort([&](RankData* _Prev, RankData* _Next)->bool {return _Prev->Score > _Next->Score; });
}

void cGameManager::Explosion(Vec2 _Pos, float _Scale, bool _Shake)
{
	SOUND->Play("explosion-" + to_string(Random(1, 6)));
	if(_Shake)
		CAMERA->Shake(_Scale * 2, _Scale * 0.5);
	cParticle* a = OBJECT->AddObject<cParticle>("Particle", _Pos, 0.39, Obj_Particle)->GetComponent<cParticle>();
	a->AddComponent<cRenderer>();
	a->AddComponent<cAnimation>()->AddAnimation("Idle", "Explosion", Random(0.03f, 0.04f));
	a->GetComponent<cAnimation>()->SetAnimation("Idle");
	a->m_Owner->m_Rot = Random(0, 359);
	a->m_Owner->m_Scale = Vec2(1, 1) * _Scale * Random(0.75f, 1.25f);
	for (int i = 0; i < 6; i++)
	{
		a = OBJECT->AddObject<cParticle>("Particle", _Pos + RandomVector2() * 10, 0.4, Obj_Particle)->GetComponent<cParticle>();
		a->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Debris" + to_string(Random(1, 6)));
		a->m_PosVel = RandomVector2() * 200 * _Scale;
		a->m_PosFri = 0.97;
		a->m_RotVel = Random(-500, 500);
		a->m_RotFri = 0.95;
		a->m_ScaVel = Vec2(-0.4, -0.4) *_Scale;
		a->m_Owner->m_Scale = Vec2(0.5, 0.5) * _Scale;
		a->m_Owner->m_Rot = Random(0, 359);
		a->m_FuncTime = 0.1;
		a->m_Func = [](cObject* _This)->void {
			cParticle* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, 0.41, Obj_Particle)->GetComponent<cParticle>();
			a->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Smoke" + to_string(Random(1, 3)));
			a->GetComponent<cRenderer>()->m_Color = 0x60909090;
			a->m_ScaVel = Vec2(-0.02, -0.02);
			a->m_ScaFri = 1.02;
			a->m_RotVel = Random(-200, 200);
			a->m_RotFri - 0.96;
			a->m_Owner->m_Rot = Random(0, 359);
			a->m_Owner->m_Scale = Vec2(0.2, 0.2) * _This->m_Scale.x;
		};
	}
}
