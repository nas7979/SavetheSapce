#pragma once
namespace Math
{
	int Random(int _Min, int _Max);
	float Random(float _Min, float _Max);
	Vec2 RandomVector2();
	float Sign(float _Val);
	float Clamp(float _Val, float _Min, float _Max);
	Vec2 RotateVector2(Vec2 _Vec, float _Rot);
	float Distance(Vec2 _Vec);
	float Direction(Vec2 _Vec);
	Vec2 Normalize(Vec2 _Vec);
	template<typename T>
	T Lerp(T _S, T _E, float _T)
	{
		return _S + (_E - _S) * _T;
	}
}