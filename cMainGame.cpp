#include "DXUT.h"
#include "cMainGame.h"
#include "cMenuScene.h"
#include "cIntroScene.h"
#include "cRankingScene.h"
#include "cStage1Scene.h"
#include "cStage2Scene.h"
#include "cStage3Scene.h"
#include "cLoadingScene.h"

void cMainGame::Init()
{
	IMAGE->CreateInstance();
	SOUND->CreateInstance();
	SCENE->CreateInstance();
	INPUT->CreateInstance();
	OBJECT->CreateInstance();
	CAMERA->CreateInstance();
	GAME->CreateInstance();
	SCHEDULE->CreateInstance();

	SCENE->AddScene("MENU", new cMenuScene);
	SCENE->AddScene("RANKING", new cRankingScene);
	SCENE->AddScene("INTRO", new cIntroScene);
	SCENE->AddScene("STAGE1", new cStage1Scene);
	SCENE->AddScene("STAGE2", new cStage2Scene);
	SCENE->AddScene("STAGE3", new cStage3Scene);
	SCENE->AddScene("LOADING", new cLoadingScene);

	SCENE->ChangeScene("LOADING");

	IMAGE->AddSurface("Drawing");
	IMAGE->AddSurface("Ground");
	IMAGE->AddSurface("Outline");
	IMAGE->AddSurface("Default");
	IMAGE->AddSurface("Glow");
}

void cMainGame::Update()
{
	INPUT->Update();
	SCENE->Update();
	OBJECT->Update();
	CAMERA->Update();
	GAME->Update();
	SCHEDULE->Update();
}

void cMainGame::Render()
{
	IMAGE->ClearSurface("Default");
	IMAGE->BeginSurface("Default");
	g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	SCENE->Render();
	IMAGE->RenderSurface("Outline", Vec2(0, 0), Vec2(1, 1), 0.8);
	IMAGE->RenderSurface("Ground", Vec2(0, 0), Vec2(1, 1), 0.6974);

	IMAGE->EndSurface();
	IMAGE->BeginSurface("Default");
	g_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	SCENE->RenderBack();
	OBJECT->RenderBack();

	IMAGE->EndSurface();

	IMAGE->BeginSurface("Default");
	g_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	OBJECT->Render();
	GAME->Render();

	IMAGE->EndSurface();

	IMAGE->Begin();
	IMAGE->RenderSurface("Default", -(CAMERA->m_Pos + CAMERA->m_ShakePos + Vec2(240,135)) * 2 * CAMERA->m_Scale, Vec2(2, 2) * CAMERA->m_Scale, 0.1);
	IMAGE->End();
}

void cMainGame::Release()
{
	OBJECT->ReleaseInstance();
	INPUT->ReleaseInstance();
	GAME->ReleaseInstance();
	SCHEDULE->ReleaseInstance();
	CAMERA->ReleaseInstance();
	SCENE->ReleaseInstance();
	SOUND->ReleaseInstance();
	IMAGE->ReleaseInstance();
}

void cMainGame::OnLostDevice()
{
	IMAGE->OnLostDevice();
}

void cMainGame::OnResetDevice()
{
	IMAGE->OnResetDevice();
}

void cMainGame::OnModifyDevice()
{
	IMAGE->OnModifyDevice();
}
