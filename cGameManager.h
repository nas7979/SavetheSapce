#pragma once
#define MAPX 700
#define MAPY 500
#define OFFSET 20
enum CellState
{
	Cell_Enemy,
	Cell_Drawing,
	Cell_Player,
	Cell_Inside
};
struct RankData
{
	string Key;
	int Score;
};
class cGameManager : public cSingleton<cGameManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	float m_DT;
	float m_NSDT;
	float m_TimeScale;
	int m_Score;
	int m_Combo;
	short m_Map[MAPX + 1][MAPY + 1];
	float m_ScoreColor;
	float m_ScoreSize;
	float m_CellCount;
	float m_Percentage;
	bool m_Win;
	bool m_Continued;
	string m_ItemText;
	D3DCOLOR m_ItemColor;
	list<RankData*> m_Ranking;

	void PlayerInit();
	void SetCell(Vec2 _Pos, short _Cell);
	short GetCell(Vec2 _Pos);
	void AddScore(int _Score);
	void AddRanking(string _Key, int _Score);
	void Explosion(Vec2 _Pos, float _Scale, bool _Shake = true);
};

#define GAME cGameManager::GetInstance()