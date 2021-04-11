#include "DXUT.h"
#include "cTextRenderer.h"

void cTextRenderer::Init()
{
}

void cTextRenderer::Update()
{
}

void cTextRenderer::Render()
{
	IMAGE->RenderText(m_Text, Vec2((int)m_Owner->m_Pos.x, (int)m_Owner->m_Pos.y), m_Owner->m_Rot, m_Owner->m_Scale, m_Owner->m_Depth, Vec2(0.5, 0.5), m_Color);
}

void cTextRenderer::Release()
{
}

void cTextRenderer::OnAlarm(string _Key)
{
}

void cTextRenderer::OnCollision(cObject * _Other)
{
}
