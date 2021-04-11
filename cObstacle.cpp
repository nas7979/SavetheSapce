#include "DXUT.h"
#include "cObstacle.h"

void cObstacle::Init()
{
	Texture* Tex = IMAGE->Find("Asteroid " + to_string(Random(1, 6)));
	AddComponent<cRenderer>()->m_Sprite = Tex;
	m_Owner->m_Scale = Vec2(0.2, 0.2) * Random(0.75f, 1.25f);
	AddComponent<cCollider>()->AddCollider(Vec2(0, 0), Tex->Info.Height * 0.4f);
	GetComponent<cCollider>()->AddCollWith(Obj_PlayerBullet);
	GetComponent<cCollider>()->AddCollWith(Obj_EnemyBullet);

	m_Owner->m_Rot = Random(0, 359);
}

void cObstacle::Update()
{
	if (GAME->GetCell(m_Owner->m_Pos) == Cell_Inside)
	{
		m_Owner->m_isDestroyed = true;
	}
}

void cObstacle::Render()
{
}

void cObstacle::Release()
{
	cParticle* Part;
	for (int i = 0; i < 10; i++)
	{
		Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.6, Obj_Particle)->GetComponent<cParticle>();
		Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Asteroid " + to_string(Random(1, 6)));
		Part->m_PosVel = RandomVector2() * 200;
		Part->m_PosFri = 0.95;
		Part->m_RotVel = Random(-200, 200);
		Part->m_RotFri = 0.95;
		Part->m_ScaVel = Vec2(-0.1, -0.1);
		Part->m_ScaFri = 1.05;
		Part->m_Owner->m_Scale = Vec2(0.1, 0.1) * Random(0.75f, 1.5f);
		Part->m_Owner->m_Rot = Random(0, 359);
	}
}

void cObstacle::OnAlarm(string _Key)
{
}

void cObstacle::OnCollision(cObject * _Other)
{
}
