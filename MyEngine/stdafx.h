#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers.
#endif

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include "wrl/client.h"
#include <string>
#include <iostream>
#include <map>

// detect memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define NUM_OF_FRAME_BUFFERS 3 // number of buffers we want, 2 for double buffering, 3 for tripple buffering

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#define DEFAULT_VALUE -1

using namespace DirectX; // we will be using the directxmath library
using namespace Microsoft::WRL; // for stuff like ComPtrs

enum class DESCRIPTOR_TYPE {
    RTV
};

#ifdef _DEBUG
#define ASSERT(expression)                                  \
    if(!(expression))                                       \
    {                                                       \
        OutputDebugStringA("Error!\n");                     \
        DebugBreak();                                       \
    }
#else
#define ASSERT(expression);

#endif