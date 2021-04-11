#include "DXUT.h"
#include "cImageManager.h"

void cImageManager::Init()
{
	D3DXCreateSprite(g_Device, &m_Sprite);
	D3DXCreateFont(g_Device, 64, 32, 1, 1, 0, DEFAULT_CHARSET, 0, 0, 0, L"System", &m_Font);
	g_Device->GetRenderTarget(0, &m_Original);
}

void cImageManager::Update()
{
}

void cImageManager::Render()
{
}

void cImageManager::Release()
{
	m_Sprite->Release();
	m_Font->Release();
	m_Original->Release();
	for (auto& iter : m_Sprites)
	{
		iter.second->Texture->Release();
		delete iter.second;
	}
	for (auto& iter : m_Animations)
	{
		delete iter.second;
	}
	for (auto& iter : m_Surfaces)
	{
		iter.second->Surface->Release();
		iter.second->Texture->Release();
		delete iter.second;
	}
}

void cImageManager::Begin()
{
	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DX_FILTER_NONE);
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DX_FILTER_NONE);
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);

	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	g_Device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
}

void cImageManager::End()
{
	m_Sprite->End();
}

void cImageManager::OnLostDevice()
{
	m_Sprite->OnLostDevice();
	m_Font->OnLostDevice();
}

void cImageManager::OnResetDevice()
{
	m_Sprite->OnResetDevice();
	m_Font->OnResetDevice();
	g_Device->GetRenderTarget(0, &m_Original);
	m_Original->Release();
	D3DSURFACE_DESC Desc;
	m_Original->GetDesc(&Desc);
	for (auto& iter : m_Surfaces)
	{
		if (iter.second->Surface == nullptr)
		{
			g_Device->CreateTexture(Desc.Width, Desc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &iter.second->Texture, nullptr);
			iter.second->Texture->GetSurfaceLevel(0, &iter.second->Surface);
		}
	}
}

void cImageManager::OnModifyDevice()
{
	if (IMAGE == nullptr)
		return;
	m_Original->Release();
	for (auto& iter : m_Surfaces)
	{
		iter.second->Surface->Release();
		iter.second->Surface = nullptr;
		iter.second->Texture->Release();
		iter.second->Texture = nullptr;
	}
}

void cImageManager::Render(Texture * _Sprite, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot, D3DCOLOR _Color)
{
	_Pos += Vec2(480, 270);
	m_Center = Vec2((float)_Sprite->Info.Width * _Pivot.x, (float)_Sprite->Info.Height * _Pivot.y);
	D3DXMatrixTransformation2D(&m_Mat, &m_Center, 0, &_Scale, &m_Center, D3DXToRadian(_Rot), &(_Pos - m_Center));
	m_Mat._43 = _Depth;
	m_Sprite->SetTransform(&m_Mat);
	m_Sprite->Draw(_Sprite->Texture, nullptr, nullptr, nullptr, _Color);
}

void cImageManager::RenderRect(Texture * _Sprite, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, RECT _Rect, Vec2 _Pivot, D3DCOLOR _Color)
{
	_Pos += Vec2(480, 270);
	m_Center = Vec2((float)_Sprite->Info.Width * _Pivot.x, (float)_Sprite->Info.Height * _Pivot.y);
	D3DXMatrixTransformation2D(&m_Mat, &m_Center, 0, &_Scale, &m_Center, D3DXToRadian(_Rot), &(_Pos - m_Center));
	m_Mat._43 = _Depth;
	m_Sprite->SetTransform(&m_Mat);
	m_Sprite->Draw(_Sprite->Texture, &_Rect, nullptr, nullptr, _Color);
}

void cImageManager::RenderText(string _Text, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot, D3DCOLOR _Color)
{
	_Pos += Vec2(480, 270);
	m_Font->DrawTextA(m_Sprite, _Text.c_str(), -1, &m_Rect, DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_CALCRECT, _Color);
	m_Center = Vec2((float)(m_Rect.right - m_Rect.left) * _Pivot.x, (float)(m_Rect.bottom - m_Rect.top) * _Pivot.y);
	D3DXMatrixTransformation2D(&m_Mat, &m_Center, 0, &_Scale, &m_Center, D3DXToRadian(_Rot), &(_Pos - m_Center));
	m_Mat._43 = _Depth;
	m_Sprite->SetTransform(&m_Mat);
	m_Font->DrawTextA(m_Sprite, _Text.c_str(), -1, nullptr, DT_NOCLIP | DT_CENTER | DT_VCENTER, _Color);
}

void cImageManager::RenderSurface(string _Key, Vec2 _Pos, Vec2 _Scale, float _Depth, D3DCOLOR _Color)
{
	D3DXMatrixTransformation2D(&m_Mat, &Vec2(0,0), 0, &_Scale, &Vec2(0,0), 0, &(_Pos));
	m_Mat._43 = _Depth;
	m_Sprite->SetTransform(&m_Mat);
	m_Sprite->Draw(m_Surfaces[_Key]->Texture, nullptr, nullptr, nullptr, _Color);
}

void cImageManager::AddSprite(string _Key, string _Path)
{
	Texture* a = new Texture;
	if (D3DXCreateTextureFromFileExA(g_Device, _Path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, &a->Info, 0, &a->Texture) == S_OK)
	{
		m_Sprites[_Key] = a;
		return;
	}
	delete a;
}

void cImageManager::AddAnimation(string _Key)
{
	vector<Texture*>* Vec = new vector<Texture*>;
	char Key[32];
	for (int i = 1; true; i++)
	{
		sprintf(Key, "%s%d", _Key.c_str(), i);
		auto Find = m_Sprites.find(Key);
		if (Find != m_Sprites.end())
			Vec->push_back((*Find).second);
		else
			break;
	}
	m_Animations[_Key] = Vec;
}

void cImageManager::AddSurface(string _Key)
{
	Surface* a = new Surface;
	D3DSURFACE_DESC Desc;
	m_Original->GetDesc(&Desc);
	g_Device->CreateTexture(Desc.Width, Desc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &a->Texture, nullptr);
	a->Texture->GetSurfaceLevel(0, &a->Surface);
	m_Surfaces[_Key] = a;
}

Texture * cImageManager::Find(string _Key)
{
	return m_Sprites[_Key];
}

vector<Texture*>* cImageManager::FindAnimation(string _Key)
{
	return m_Animations[_Key];
}

void cImageManager::BeginSurface(string _Key)
{
	g_Device->SetRenderTarget(0, m_Surfaces[_Key]->Surface);
	IMAGE->Begin();
}

void cImageManager::EndSurface()
{
	IMAGE->End();
	g_Device->SetRenderTarget(0, m_Original);
}

void cImageManager::ClearSurface(string _Key, D3DCOLOR _Color, DWORD _Flag)
{
	g_Device->SetRenderTarget(0, m_Surfaces[_Key]->Surface);
	g_Device->Clear(0, nullptr, _Flag, _Color, 0, 0);
	g_Device->SetRenderTarget(0, m_Original);
}
