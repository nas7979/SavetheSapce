#include "DXUT.h"
#include "cEnemy.h"
#include "cItem.h"

void cEnemy::Init()
{
	AddComponent<cMapCollider>()->m_OnCollision = [&](Vec2 _Pos, Vec2 _CellPos, short _Cell)->void {
		switch (_Cell)
		{
		case Cell_Drawing:
		{
			cPlayer* Player = OBJECT->m_Player->GetComponent<cPlayer>();
			if (Player->GetIsInvincible())
				return;
			Player->m_Speed -= 20;
			cParticle* Part;
			for (int i = 0; i < 100; i++)
			{
				Part = OBJECT->AddObject<cParticle>("Particle", _CellPos, 0.3, Obj_Particle)->GetComponent<cParticle>();
				Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
				Part->GetComponent<cRenderer>()->m_Color = D3DCOLOR_XRGB(255, (int)((Player->m_Speed / Player->m_OriginalSpeed) * 255), (int)((Player->m_Speed / Player->m_OriginalSpeed) * 255));
				Part->m_PosVel = RandomVector2() * 200;
				Part->m_PosFri = 0.95;
				Part->m_ScaVel = Vec2(-4, -4);
				Part->m_Owner->m_Scale = Vec2(1, 1) * Random(1.f, 3.f);
				Part->m_Owner->m_Rot = Random(0, 359);
			}
			Part = OBJECT->AddObject<cParticle>("Particle", _CellPos, 0.4, Obj_Particle)->GetComponent<cParticle>();
			Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Circle");
			Part->GetComponent<cRenderer>()->m_Color = D3DCOLOR_XRGB(255, (int)((Player->m_Speed / Player->m_OriginalSpeed) * 255), (int)((Player->m_Speed / Player->m_OriginalSpeed) * 255));
			Part->m_ScaVel = Vec2(2, 2);
			Part->m_ScaFri = 0.95;
			Part->m_Owner->m_Scale = Vec2(0.01, 0.01);
			Part->m_AlpVel = -600;
			if (Player->m_Speed < 20)
				Player->m_Speed = 20;
		}
		case Cell_Player:
		{
			m_Owner->m_Pos = _Pos;
			if (m_WaitForAttack >= 3)
				Attack();
			m_WaitForAttack = 0;
			m_Gravity = RotateVector2(Vec2(1, 0), Random(0, 359));
			if (GAME->GetCell(_Pos + Vec2(1, 0)) == Cell_Player && GAME->GetCell(_Pos + Vec2(-1, 0)) == Cell_Player)
			{
				m_Dir.y *= -1;
				return;
			}
			if (GAME->GetCell(_Pos + Vec2(0, 1)) == Cell_Player && GAME->GetCell(_Pos + Vec2(0, -1)) == Cell_Player)
			{
				m_Dir.x *= -1;
				return;
			}
			m_Dir *= -1;
		}
		break;

		case Cell_Inside:
			m_Hp = 0;
			Death();
			break;
		}
	};
	AddComponent<cCollider>()->AddCollWith(Obj_Map);
	GetComponent<cCollider>()->AddCollWith(Obj_PlayerBullet);
	m_Move = true;
	m_Dir = RotateVector2(Vec2(1, 0), Random(0, 359));
	m_Owner->m_Rot = Direction(m_Dir);
	m_Gravity = RotateVector2(Vec2(1, 0), Random(0, 359));
	m_WaitForAttack = 0;
}

void cEnemy::Update()
{
	m_WaitForAttack++;
	if (m_Move)
	{
		if (OBJECT->m_Player->GetComponent<cPlayer>()->GetIsInvincible() == false)
		{
			m_Gravity = Normalize(OBJECT->m_Player->m_Pos - m_Owner->m_Pos);
		}
		m_Owner->m_Pos += m_Dir * m_Speed * DT;
		m_Owner->m_Rot = Direction(m_Dir);
		m_Dir += m_Gravity * DT * 0.5;
		D3DXVec2Normalize(&m_Dir, &m_Dir);
	}
}

void cEnemy::Render()
{
}

void cEnemy::Release()
{
}

void cEnemy::OnAlarm(string _Key)
{
	if (_Key == "Damage")
	{
		GetComponent<cRenderer>()->m_Color = 0xffffffff;
		return;
	}
}

void cEnemy::OnCollision(cObject * _Other)
{
	if (_Other->m_Tag == Obj_PlayerBullet)
	{
		_Other->m_isDestroyed = true;
		DealDamage(_Other->GetComponent<cBullet>()->m_Damage);
		return;
	}
	if (_Other->m_Tag == Obj_Map)
	{
		m_Owner->m_Pos += m_Speed * -m_Dir * DT;
		if(m_WaitForAttack >= 3)
			Attack();
		m_WaitForAttack = 0;
		m_Dir *= -1;
		m_Gravity = RotateVector2(Vec2(1, 0), Random(0, 359));
		return;
	}
}

void cEnemy::Death()
{
	m_Owner->m_isDestroyed = true;
	if (m_Hp <= 0)
	{
		cParticle* Part = OBJECT->AddObject<cParticle>("Score", m_Owner->m_Pos, 0.05, Obj_Particle)->GetComponent<cParticle>();
		Part->AddComponent<cTextRenderer>()->m_Text = "+" + to_string((int)(m_Score * GAME->m_Combo));
		Part->GetComponent<cTextRenderer>()->m_Color = 0xffff0000;
		Part->m_Owner->m_Scale = Vec2(0.5, 0.55) * Clamp(m_Score * GAME->m_Combo / 1000000, 1, 2);
		Part->m_ScaVel = Vec2(0, -0.01);
		Part->m_ScaFri = 1.05;
		GAME->AddScore(m_Score * GAME->m_Combo);
		GAME->m_Combo++;

		switch (Random(1, 15))
		{
		case 1: OBJECT->AddObject<cItem>("Item_Speed", m_Owner->m_Pos, 0.7, Obj_Item); break;
		case 2: OBJECT->AddObject<cItem>("Item_Random", m_Owner->m_Pos, 0.7, Obj_Item); break;
		case 3: OBJECT->AddObject<cItem>("Item_Armor", m_Owner->m_Pos, 0.7, Obj_Item); break;
		case 4: OBJECT->AddObject<cItem>("Item_Life", m_Owner->m_Pos, 0.7, Obj_Item); break;
		case 5: OBJECT->AddObject<cItem>("Item_Invincible", m_Owner->m_Pos, 0.7, Obj_Item); break;
		}
	}
	OBJECT->m_Boss->GetComponent<cBoss>()->m_RespawnQueue.push(m_Owner->m_Name);
}

void cEnemy::DealDamage(float _Damage)
{
	if (m_Hp <= 0)
		return;
	m_Hp -= _Damage;
	m_Owner->SetAlarm("Damage", 0.075);
	SOUND->Play("MG " + to_string(Random(1, 14)));
	GetComponent<cRenderer>()->m_Color = 0xffff0000;
	if (m_Hp <= 0)
	{
		Death();
	}
}

cBullet * cEnemy::FireBullet(Vec2 _Pos, float _Dir, float _Size, float _Damage, float _Speed, D3DCOLOR _Color, ObjectTag _Tag)
{
	cBullet* a = OBJECT->AddObject<cBullet>("Bullet", _Pos, 0.4, _Tag)->GetComponent<cBullet>();
	a->m_Damage = _Damage;
	a->m_Speed = _Speed;
	a->m_Owner->m_Rot = _Dir;
	a->m_Owner->m_Scale = Vec2(_Size, _Size);
	a->GetComponent<cRenderer>()->m_Color = _Color;
	return a;
}
