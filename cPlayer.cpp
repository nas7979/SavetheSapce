#include "DXUT.h"
#include "cPlayer.h"
#include "cContinue.h"

void cPlayer::Init()
{
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Player");
	AddComponent<cCollider>()->AddCollider(Vec2(0, 0), 2);
	GetComponent<cCollider>()->AddCollWith(Obj_Enemy);
	GetComponent<cCollider>()->AddCollWith(Obj_EnemyBullet);
	GetComponent<cCollider>()->AddCollWith(Obj_Map);
	m_Owner->m_Scale = Vec2(0.5, 0.5);

	m_LastAngle = Vec2(0, 1);
	m_CheatInv = false;
	m_Invincible = 0;
	m_ShieldColor = 0xff00ff00;
	m_Shield = 750;
	m_OriginalSpeed = 150;
	m_Speed = 150;
	m_Retreat = false;
	m_Dead = false;
	m_Life = 5;
	m_Armor = false;
	OBJECT->m_Player = m_Owner;
	GAME->PlayerInit();
}

void cPlayer::Update()
{

	if (GetIsInvincible() || m_Armor)
	{
		cParticle* Part;
		Vec2 Dir;
		for (float i = 0; i < 50; i++)
		{
			Dir = RotateVector2(Vec2(1, 0), i * 7.2);
			Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos + Dir * Random(8.f, 10.f), 0.7, Obj_Particle)->GetComponent<cParticle>();
			Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
			Part->GetComponent<cRenderer>()->m_Color = m_ShieldColor;
			Part->m_PosVel = -Dir * 50;
			Part->m_Owner->SetAlarm("Destroy", Random(0.05f, 0.1f));
		}
	}
	if (GAME->GetCell(m_Owner->m_Pos) == Cell_Player)
	{
		m_Shield -= 5 * DT;
	}

	if (INPUT->KeyDown(VK_F1))
	{
		m_CheatInv = !m_CheatInv;
	}
	if (INPUT->KeyDown(VK_F2))
	{
		if(m_Life != 5)
			m_Life++;
	}
	if (INPUT->KeyDown(VK_F3))
	{

	}
	m_Invincible -= DT;
	if (m_Invincible <= 0 && GAME->m_ItemColor == 0xffffff00)
	{
		ResetItem();
	}
	Vec2 PrevPos = m_Owner->m_Pos;
	Vec2 Right = RotateVector2(Vec2(0, 1), m_Owner->m_Rot);
	if (INPUT->KeyDown(VK_LEFT))
	{
		if(m_LastAngle.x != 0)
			m_LastAngle = Vec2(0, Sign(Right.x));
		m_Owner->m_Rot = 180;
	}
	if (INPUT->KeyDown(VK_RIGHT))
	{
		if (m_LastAngle.x != 0)
			m_LastAngle = Vec2(0, Sign(Right.x));
		m_Owner->m_Rot = 0;
	}
	if (INPUT->KeyDown(VK_UP))
	{
		if (m_LastAngle.y != 0)
			m_LastAngle = Vec2(Sign(Right.y), 0);
		m_Owner->m_Rot = 270;
	}
	if (INPUT->KeyDown(VK_DOWN))
	{
		if (m_LastAngle.y != 0)
			m_LastAngle = Vec2(Sign(Right.y), 0);
		m_Owner->m_Rot = 90;
	}

	if (INPUT->KeyPress(VK_LEFT))
	{
		m_Owner->m_Pos += Vec2(-m_Speed * DT, 0);
	}
	if (INPUT->KeyPress(VK_RIGHT))
	{
		m_Owner->m_Pos += Vec2(m_Speed * DT, 0);
	}
	if (INPUT->KeyPress(VK_UP))
	{
		m_Owner->m_Pos += Vec2(0, -m_Speed * DT);
	}
	if (INPUT->KeyPress(VK_DOWN))
	{
		m_Owner->m_Pos += Vec2(0, m_Speed * DT);
	}

	m_Owner->m_Pos.x = Clamp(m_Owner->m_Pos.x, OFFSET, OFFSET + MAPX);
	m_Owner->m_Pos.y = Clamp(m_Owner->m_Pos.y, OFFSET, OFFSET + MAPY);

	Vec2 Dir = Normalize(m_Owner->m_Pos - PrevPos);
	Dir.x = Sign(Dir.x);
	Dir.y = Sign(Dir.y);
	Vec2 CurPos = Vec2((int)PrevPos.x, (int)PrevPos.y);
	if (INPUT->KeyPress(VK_SPACE) && !m_Retreat)
	{
		while (CurPos != Vec2((int)m_Owner->m_Pos.x, (int)m_Owner->m_Pos.y))
		{
			if (GAME->GetCell(CurPos) == Cell_Enemy)
			{
				if (m_DrawingCells.size() == 0)
				{
					m_RespawnPos = CurPos - Dir;
				}
				m_Outlines.push_back(CurPos);
				m_DrawingCells.push_back(CurPos);
				GAME->SetCell(CurPos, Cell_Drawing);
				IMAGE->BeginSurface("Drawing");
				IMAGE->Render(IMAGE->m_Pixel, CurPos, 0, Vec2(1, 1), 0.1, Vec2(0, 0));
				IMAGE->EndSurface();
				if ((GAME->GetCell(CurPos + m_LastAngle) == Cell_Player || GAME->GetCell(CurPos - m_LastAngle) == Cell_Player))
				{
					m_Retreat = true;
					break;
				}
			}

			if ((GAME->GetCell(CurPos + Dir * 2) == Cell_Drawing && GAME->GetCell(CurPos + Dir) == Cell_Enemy) || GAME->GetCell(CurPos + Dir) == Cell_Inside)
			{
				m_Owner->m_Pos = CurPos;
				break;
			}

			if (GAME->GetCell(CurPos + Dir) != Cell_Enemy && GAME->GetCell(CurPos - Dir) == Cell_Enemy && m_DrawingCells.size() != 0)
			{
				m_Outlines.pop_back();
				m_DrawingCells.pop_back();
				GAME->SetCell(CurPos, Cell_Enemy);
				IMAGE->ClearSurface("Drawing");
				IMAGE->BeginSurface("Drawing");
				for (auto& iter : m_DrawingCells)
				{
					IMAGE->Render(IMAGE->m_Pixel, iter, 0, Vec2(1, 1), 0.1, Vec2(0, 0));
				}
				IMAGE->EndSurface();
			}

			if (GAME->GetCell(CurPos + Dir) == Cell_Player && m_DrawingCells.size() != 0)
			{
				cParticle* Part;
				D3DCOLOR Color = D3DCOLOR_XRGB(255, (int)((m_Speed / m_OriginalSpeed) * 255), (int)((m_Speed / m_OriginalSpeed) * 255));
				for (auto& iter : m_DrawingCells)
				{
					Part = OBJECT->AddObject<cParticle>("Particle", iter, 0.3, Obj_Particle)->GetComponent<cParticle>();
					Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
					Part->GetComponent<cRenderer>()->m_Color = Color;
					Part->m_PosVel = RandomVector2() * 200;
					Part->m_PosFri = 0.95;
					Part->m_ScaVel = Vec2(-4, -4);
					Part->m_Owner->m_Scale = Vec2(1, 1) * Random(1.f, 3.f);
					Part->m_Owner->m_Rot = Random(0, 359);
				}
				SOUND->Play("Charge Up 2", -500);
				m_Speed = m_OriginalSpeed;
				m_Glow = true;
				GAME->m_Combo = 1;
				for (float i = 0; i < 5; i++)
				{
					SCHEDULE->AddSchedule(0.3 * i, [&]()->void {
						m_Glow = !m_Glow;
					});
				}
				IMAGE->BeginSurface("Outline");
				IMAGE->RenderSurface("Drawing", Vec2(0, 0), Vec2(1, 1), 0.1, 0xff00ff00);
				IMAGE->EndSurface();
				IMAGE->ClearSurface("Drawing");
				IMAGE->ClearSurface("Glow");
				for (auto& iter : m_DrawingCells)
				{
					GAME->SetCell(iter, Cell_Player);
				}
				m_DrawingCells.clear();

				if (BossFill(CurPos + m_LastAngle, Vec2((int)OBJECT->m_Boss->m_Pos.x, (int)OBJECT->m_Boss->m_Pos.y)) != 0)
				{
					for (auto& iter : m_DrawingCells)
					{
						GAME->SetCell(iter, Cell_Enemy);
					}
					m_DrawingCells.clear();
					Fill(CurPos - m_LastAngle);
				}
				m_Owner->m_Pos = CurPos + Dir;
				IMAGE->BeginSurface("Glow");
				for (auto& iter : m_DrawingCells)
				{
					IMAGE->Render(IMAGE->m_Pixel, iter, 0, Vec2(1, 1), 0.1, Vec2(0, 0));
					GAME->m_CellCount++;
				}
				m_DrawingCells.clear();
				IMAGE->EndSurface();
				IMAGE->BeginSurface("Ground");
				IMAGE->RenderSurface("Glow", Vec2(0, 0), Vec2(1, 1), 0.1);
				IMAGE->EndSurface();

				IMAGE->BeginSurface("Outline");
				for (auto& iter = m_Outlines.begin(); iter != m_Outlines.end();)
				{
					for (int x = -1; x <= 1; x++)
					{
						for (int y = -1; y <= 1; y++)
						{
							if (x == 0 && y == 0)
								continue;
							if (GAME->GetCell(*iter + Vec2(x, y)) == Cell_Enemy)
							{
								goto Out;
							}
						}
					}
					GAME->SetCell(*iter, Cell_Inside);
					IMAGE->Render(IMAGE->m_Pixel, *iter, 0, Vec2(1, 1), 0.1, Vec2(0, 0));
					iter = m_Outlines.erase(iter);
					GAME->m_CellCount++;
					continue;
				Out:
					iter++;
				}
				IMAGE->EndSurface();
				break;
			}
			CurPos += Dir;
		}
	}
	else
	{
		m_Speed = m_OriginalSpeed;
		if (m_DrawingCells.size() != 0)
		{
			m_Retreat = true;
		}

		if (m_Retreat)
		{
			for (int i = 0; i < max(DT * 600, 1); i++)
			{
				m_Owner->m_Pos = m_DrawingCells.back();
				GAME->SetCell(m_Owner->m_Pos, Cell_Enemy);
				m_DrawingCells.pop_back();
				m_Outlines.pop_back();
				IMAGE->ClearSurface("Drawing");
				IMAGE->BeginSurface("Drawing");
				for (auto& iter : m_DrawingCells)
				{
					IMAGE->Render(IMAGE->m_Pixel, iter, 0, Vec2(1, 1), 0.1, Vec2(0, 0));
				}
				IMAGE->EndSurface();
				if (m_DrawingCells.size() == 0)
				{
					m_Retreat = false;
					m_Owner->m_Pos = m_RespawnPos;
					break;
				}
			}
		}
		else
		{
			while (GAME->GetCell(m_Owner->m_Pos) != Cell_Player)
			{
				m_Owner->m_Pos -= Dir;
			}
		}
	}
}

void cPlayer::Render()
{
	int Color = (m_Speed / m_OriginalSpeed) * 255;
	IMAGE->RenderSurface("Drawing", Vec2(0, 0), Vec2(1, 1), 0.9, D3DCOLOR_XRGB(255, Color, Color));
	if (m_Glow)
	{
		IMAGE->RenderSurface("Glow", Vec2(0, 0), Vec2(1, 1), 0.9);
	}
}

void cPlayer::Release()
{
	OBJECT->m_Player = nullptr;
	IMAGE->ClearSurface("Outline");
	IMAGE->ClearSurface("Ground");
	IMAGE->ClearSurface("Drawing");
}

void cPlayer::OnAlarm(string _Key)
{
	if (_Key == "Respawn")
	{
		if (m_Life == 0)
		{
			OBJECT->AddObject<cContinue>("Continue", Vec2(0, 0), 0.1, Obj_None);
			return;
		}
		m_Owner->m_Pos = m_RespawnPos;
		cParticle* Part;
		Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.1, Obj_Particle)->GetComponent<cParticle>();
		Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Circle");
		Part->GetComponent<cRenderer>()->m_Color = 0x0000ff00;
		Part->m_Alp = 1;
		Part->m_AlpVel = 500;
		Part->m_AlpFri = 1;
		Part->m_ScaVel = Vec2(-0.1, -0.1);
		Part->m_ScaFri = 1.05;
		Part->m_Owner->SetAlarm("Destroy", 0.75);
		Part->m_Owner->m_Scale = Vec2(0.3, 0.3);

		SCHEDULE->AddSchedule(0.5, [&]()->void {
			cParticle* Part;
			Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.1, Obj_Particle)->GetComponent<cParticle>();
			Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Player");
			Part->GetComponent<cRenderer>()->m_Color = 0x00ffffff;
			Part->m_Alp = 1;
			Part->m_AlpVel = 500;
			Part->m_AlpFri = 1;
			Part->m_ScaVel = Vec2(-2, -2);
			Part->m_ScaFri = 1.05 ;
			Part->m_Owner->SetAlarm("Destroy", 0.5);
			Part->m_Owner->m_Scale = Vec2(3, 3);
			Part->m_Owner->m_Rot = m_Owner->m_Rot;
		});
		m_Owner->SetAlarm("Spawn", 1);
		return;
	}
	if (_Key == "Spawn")
	{
		m_Enable = true;
		GetComponent<cCollider>()->m_Enable = true;
		GetComponent<cRenderer>()->m_Enable = true;
		return;
	}
	if (_Key == "Camera")
	{
		CAMERA->Move(Vec2(240, 135));
		CAMERA->Scale(1);
		return;
	}
	if (_Key == "Continue")
	{

		int Rand = Random(0, m_Outlines.size() - 1);
		int i = 0;
		for (auto& iter : m_Outlines)
		{
			if (i == Rand)
			{
				m_Owner->m_Pos = iter;
				break;
			}
			i++;
		}
		cParticle* Part;
		Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.1, Obj_Particle)->GetComponent<cParticle>();
		Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Circle");
		Part->GetComponent<cRenderer>()->m_Color = 0x0000ff00;
		Part->m_Alp = 1;
		Part->m_AlpVel = 500;
		Part->m_AlpFri = 1;
		Part->m_ScaVel = Vec2(-0.1, -0.1);
		Part->m_ScaFri = 1.05;
		Part->m_Owner->SetAlarm("Destroy", 0.75);
		Part->m_Owner->m_Scale = Vec2(0.3, 0.3);

		SCHEDULE->AddSchedule(0.5, [&]()->void {
			cParticle* Part;
			Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.1, Obj_Particle)->GetComponent<cParticle>();
			Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Player");
			Part->GetComponent<cRenderer>()->m_Color = 0x00ffffff;
			Part->m_Alp = 1;
			Part->m_AlpVel = 500;
			Part->m_AlpFri = 1;
			Part->m_ScaVel = Vec2(-2, -2);
			Part->m_ScaFri = 1.05;
			Part->m_Owner->SetAlarm("Destroy", 0.5);
			Part->m_Owner->m_Scale = Vec2(3, 3);
			Part->m_Owner->m_Rot = m_Owner->m_Rot;
		});
		m_Owner->SetAlarm("Spawn", 1);
		return;
	}
}

void cPlayer::OnCollision(cObject * _Other)
{
	switch (_Other->m_Tag)
	{
		case Obj_EnemyBullet:
			_Other->m_isDestroyed = true;
		case Obj_Enemy:
		case Obj_Map:
		{
			if (GetIsInvincible())
				return;

			GAME->m_ItemColor = 0xff00ff00;
			GAME->m_ItemText = "NONE";
			if (m_Armor)
			{
				m_Armor = false;
				m_ShieldColor = 0xff00ff00;
				return;
			}

			m_Life--;
			cParticle* Part;
			D3DCOLOR Color = D3DCOLOR_XRGB(255, (int)((m_Speed / m_OriginalSpeed) * 255), (int)((m_Speed / m_OriginalSpeed) * 255));
			for (auto& iter : m_DrawingCells)
			{
				GAME->SetCell(iter, Cell_Enemy);
				Part = OBJECT->AddObject<cParticle>("Particle", iter, 0.3, Obj_Particle)->GetComponent<cParticle>();
				Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
				Part->GetComponent<cRenderer>()->m_Color = Color;
				Part->m_PosVel = RandomVector2() * 200;
				Part->m_PosFri = 0.95;
				Part->m_ScaVel = Vec2(-4, -4);
				Part->m_Owner->m_Scale = Vec2(1, 1) * Random(1.f, 3.f);
				Part->m_Owner->m_Rot = Random(0, 359);
				m_Outlines.pop_back();
			}
			m_DrawingCells.clear();
			IMAGE->ClearSurface("Drawing");

			m_Owner->SetAlarm("Respawn", 3);
			m_Owner->SetAlarm("Camera", 1);
			GAME->m_TimeScale = 0;
			CAMERA->Move(m_Owner->m_Pos);
			CAMERA->Scale(2);
			SCHEDULE->AddSchedule(0.5, []()->void {
				SOUND->Play("Rocket Tower Shoot", -1500);
				CAMERA->Shake(5, 1);
				GAME->m_TimeScale = 1;
			});

			SOUND->Play("Mini Bomb 5", -1500);
			m_OriginalSpeed = 150;
			m_ShieldColor = 0xff00ff00;
			m_Speed = 150;
			m_Enable = false;
			GAME->Explosion(m_Owner->m_Pos, 1, false);
			GetComponent<cCollider>()->m_Enable = false;
			GetComponent<cRenderer>()->m_Enable = false;
		}
		break;
	}
}

void cPlayer::Fill(Vec2 _Pos)
{
	GAME->SetCell(_Pos, Cell_Inside);
	m_DrawingCells.push_back(_Pos);
	if (GAME->GetCell(_Pos + Vec2(1, 0)) == Cell_Enemy)
		Fill(_Pos + Vec2(1, 0));
	if (GAME->GetCell(_Pos + Vec2(-1, 0)) == Cell_Enemy)
		Fill(_Pos + Vec2(-1, 0));
	if (GAME->GetCell(_Pos + Vec2(0, 1)) == Cell_Enemy)
		Fill(_Pos + Vec2(0, 1));
	if (GAME->GetCell(_Pos + Vec2(0, -1)) == Cell_Enemy)
		Fill(_Pos + Vec2(0, -1));
}

short cPlayer::BossFill(Vec2 _Pos, Vec2 _BossPos)
{
	short BossFound = 0;
	if (_Pos == _BossPos)
	{
		return 1;
	}

	GAME->SetCell(_Pos, Cell_Inside);
	m_DrawingCells.push_back(_Pos);
	if (GAME->GetCell(_Pos + Vec2(1, 0)) == Cell_Enemy)
	{
		BossFound += BossFill(_Pos + Vec2(1, 0), _BossPos);
		if (BossFound != 0)
			goto Out;
	}
	if (GAME->GetCell(_Pos + Vec2(-1, 0)) == Cell_Enemy)
	{
		BossFound += BossFill(_Pos + Vec2(-1, 0), _BossPos);
		if (BossFound != 0)
			goto Out;
	}
	if (GAME->GetCell(_Pos + Vec2(0, 1)) == Cell_Enemy)
	{
		BossFound += BossFill(_Pos + Vec2(0, 1), _BossPos);
		if (BossFound != 0)
			goto Out;
	}
	if (GAME->GetCell(_Pos + Vec2(0, -1)) == Cell_Enemy)
	{
		BossFound += BossFill(_Pos + Vec2(0, -1), _BossPos);
		if (BossFound != 0)
			goto Out;
	}
	Out:
	return BossFound;
}

bool cPlayer::GetIsInvincible()
{
	return m_Enable == false || m_Retreat == true || m_CheatInv == true || m_Invincible > 0 || (GAME->GetCell(m_Owner->m_Pos) == Cell_Player && m_Shield > 0);
}

void cPlayer::ResetItem()
{
	m_Armor = false;
	m_Invincible = 0;
	m_Speed = m_OriginalSpeed = 150;
	m_ShieldColor = 0xff00ff00;
	GAME->m_ItemText = "NONE";
	GAME->m_ItemColor = 0xff00ff00;
}
