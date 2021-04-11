#pragma once
struct Texture
{
	LPDIRECT3DTEXTURE9 Texture;
	D3DXIMAGE_INFO Info;
};
struct Surface
{
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DSURFACE9 Surface;
};
class cImageManager : public cSingleton<cImageManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();
	void Begin();
	void End();
	void OnLostDevice();
	void OnResetDevice();
	void OnModifyDevice();

	map<string, Texture*> m_Sprites;
	map<string, vector<Texture*>*> m_Animations;
	map<string, Surface*> m_Surfaces;
	LPD3DXSPRITE m_Sprite;
	LPD3DXFONT m_Font;
	Texture* m_Pixel;
	RECT m_Rect;
	Vec2 m_Center;
	Matrix m_Mat;
	LPDIRECT3DSURFACE9 m_Original;

	void Render(Texture* _Sprite, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot = Vec2(0.5, 0.5), D3DCOLOR _Color = 0xffffffff);
	void RenderRect(Texture* _Sprite, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, RECT _Rect, Vec2 _Pivot = Vec2(0.5, 0.5), D3DCOLOR _Color = 0xffffffff);
	void RenderText(string _Text, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot = Vec2(0.5, 0.5), D3DCOLOR _Color = 0xffffffff);
	void RenderSurface(string _Key, Vec2 _Pos, Vec2 _Scale, float _Depth, D3DCOLOR _Color = 0xffffffff);

	void AddSprite(string _Key, string _Path);
	void AddAnimation(string _Key);
	void AddSurface(string _Key);
	Texture* Find(string _Key);
	vector<Texture*>* FindAnimation(string _Key);
	void BeginSurface(string _Key);
	void EndSurface();
	void ClearSurface(string _Key, D3DCOLOR _Color = 0x00000000, DWORD _Flag = D3DCLEAR_TARGET);
};

#define IMAGE cImageManager::GetInstance()