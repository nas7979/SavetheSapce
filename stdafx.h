#pragma once
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <queue>
#include <functional>
#include <dsound.h>
#include "SDKsound.h"
#include "SDKwavefile.h"
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "dsound.lib")

#define g_Device DXUTGetD3D9Device()
#define DT GAME->m_DT
#define NSDT GAME->m_NSDT

using Vec2 = D3DXVECTOR2;
using Matrix = D3DXMATRIX;
using namespace std;
#include "Math.h"
using namespace Math;

#include "cSingleton.h"
#include "cImageManager.h"
#include "cObjectManager.h"
#include "cSoundManager.h"
#include "cSceneManager.h"
#include "cInputManager.h"
#include "cCameraManager.h"
#include "cGameManager.h"
#include "cScheduleManager.h"

#include "cScene.h"
#include "cObject.h"
#include "cComponent.h"
#include "cParticle.h"
#include "cRenderer.h"
#include "cTextRenderer.h"
#include "cCollider.h"
#include "cMapCollider.h"
#include "cAnimation.h"

#include "cPlayer.h"
#include "cBullet.h"
#include "cEnemy.h"
#include "cBoss.h"