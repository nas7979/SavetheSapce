#pragma once
struct Schedule
{
	function<void()> Func;
	float Time;
};
class cScheduleManager : public cSingleton<cScheduleManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	list<Schedule*> m_Schedules;

	void AddSchedule(float _Time, function<void()> _Func);
};

#define SCHEDULE cScheduleManager::GetInstance()