#include "DXUT.h"
#include "cItem.h"

void cItem::Init()
{
	AddComponent<cMapCollider>()->m_OnCollision = [&](Vec2 _Pos, Vec2 _CellPos, short _Cell)->void {
		switch (_Cell)
		{
		case Cell_Player:
		{
			m_Owner->m_Pos = _Pos;
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
			if (GetComponent<cCollider>()->m_Enable)
			{
				GetComponent<cMapCollider>()->m_Enable = false;
				m_Dir = Normalize(OBJECT->m_Boss->m_Pos - m_Owner->m_Pos);
			}
			break;
		}
	};
	GetComponent<cMapCollider>()->m_Radius = 1;
	AddComponent<cCollider>()->AddCollider(Vec2(0, 0), 20);
	GetComponent<cCollider>()->AddCollWith(Obj_Player);

	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find(m_Owner->m_Name);
	m_Dir = Normalize(OBJECT->m_Boss->m_Pos - m_Owner->m_Pos);
}

void cItem::Update()
{
	m_Owner->m_Pos += m_Dir * 75 * DT;
	if (GAME->GetCell(m_Owner->m_Pos) == Cell_Enemy)
	{
		GetComponent<cMapCollider>()->m_Enable = true;
	}
}

void cItem::Render()
{
}

void cItem::Release()
{
}

void cItem::OnAlarm(string _Key)
{
}

void cItem::OnCollision(cObject * _Other)
{
	if (GAME->GetCell(_Other->m_Pos) == Cell_Player)
		return;
	cPlayer* Player = _Other->GetComponent<cPlayer>();
	m_Owner->m_isDestroyed = true;
	cParticle* Part = OBJECT->AddObject<cParticle>("Score", m_Owner->m_Pos, 0.05, Obj_Particle)->GetComponent<cParticle>();
	Part->AddComponent<cTextRenderer>()->m_Color = 0xffff0000;
	Part->m_Owner->m_Scale = Vec2(0.5, 0.55);
	Part->m_ScaVel = Vec2(0, -0.01);
	Part->m_ScaFri = 1.03;
	if (m_Owner->m_Name == "Item_Random")
	{
		switch (Random(1, 4))
		{
		case 1: m_Owner->m_Name = "Item_Speed"; break;
		case 2: m_Owner->m_Name = "Item_Armor"; break;
		case 3: m_Owner->m_Name = "Item_Life"; break;
		case 4: m_Owner->m_Name = "Item_Invincible"; break;
		}
	}
	if (m_Owner->m_Name == "Item_Speed")
	{
		Player->ResetItem();
		Player->m_Speed = 250;
		Player->m_OriginalSpeed = 250;
		Player->m_ShieldColor = 0xff0000ff;
		GAME->m_ItemColor = 0xff0000ff;
		GAME->m_ItemText = "SPEED";
		Part->GetComponent<cTextRenderer>()->m_Text = "SPEED+";
		return;
	}
	if (m_Owner->m_Name == "Item_Armor")
	{
		Player->ResetItem();
		Player->m_Armor = true;
		Player->m_ShieldColor = 0xffff0000;
		GAME->m_ItemColor = 0xffff0000;
		GAME->m_ItemText = "ARMOR";
		Part->GetComponent<cTextRenderer>()->m_Text = "ARMOR";
		return;
	}
	if (m_Owner->m_Name == "Item_Invincible")
	{
		Player->ResetItem();
		Player->m_Invincible = 5;
		GAME->m_ItemColor = 0xffffff00;
		GAME->m_ItemText = "INVINCIBLE";
		Part->GetComponent<cTextRenderer>()->m_Text = "INVINCIBLE";
		return;
	}
	if (m_Owner->m_Name == "Item_Life")
	{
		if(Player->m_Life != 5)
			Player->m_Life++;
		Part->GetComponent<cTextRenderer>()->m_Text = "LIFE+";
		return;
	}
}
