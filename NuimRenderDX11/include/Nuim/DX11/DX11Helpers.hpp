#pragma once
#include "Core/Assert.hpp"

#if NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#ifndef NUIM_DX11_CHECK
#define NUIM_DX11_CHECK(x) do { HRESULT hr__ = (x); NUIM_ASSERT(SUCCEEDED(hr__), "DX11 HRESULT failed"); } while(0)
#endif