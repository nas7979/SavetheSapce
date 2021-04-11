#include "DXUT.h"
#include "Math.h"

int Math::Random(int _Min, int _Max)
{
	return _Min + (rand() | (rand() << 16)) % (_Max - _Min + 1);
}

float Math::Random(float _Min, float _Max)
{
	return (float)Random((int)(_Min * 1000), (int)(_Max * 1000)) * 0.001;
}

Vec2 Math::RandomVector2()
{
	return RotateVector2(Vec2(Random(-1.f, 1.f), 0), Random(0, 359));
}

float Math::Sign(float _Val)
{
	if (_Val > 0)
		return 1;
	if (_Val < 0)
		return -1;
	return 0;
}

float Math::Clamp(float _Val, float _Min, float _Max)
{
	return max(_Min, min(_Max, _Val));
}

Vec2 Math::RotateVector2(Vec2 _Vec, float _Rot)
{
	float Rot = D3DXToRadian(_Rot);
	return Vec2(_Vec.x * cos(Rot) + _Vec.y * -sin(Rot), _Vec.x * sin(Rot) + _Vec.y * cos(Rot));
}

float Math::Distance(Vec2 _Vec)
{
	return D3DXVec2Length(&_Vec);
}

float Math::Direction(Vec2 _Vec)
{
	return D3DXToDegree(atan2f(_Vec.y, _Vec.x));
}

Vec2 Math::Normalize(Vec2 _Vec)
{
	return *D3DXVec2Normalize(&Vec2(), &_Vec);
}
