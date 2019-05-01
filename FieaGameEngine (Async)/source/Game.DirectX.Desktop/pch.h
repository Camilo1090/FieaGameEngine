#pragma once

#define NOMINMAX

// Windows
#include <windows.h>
#include <wrl.h>

// Standard
#include <string>
#include <sstream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

// DirectX
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

// Local
#include "GameException.h"