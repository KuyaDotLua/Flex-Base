// dear FlexUI: Renderer Backend for DirectX9
// This needs to be used along with a Platform Backend (e.g. Win32)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'LPDIRECT3DTEXTURE9' as ImTextureID. Read the FAQ about ImTextureID!
//  [X] Renderer: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can use unmodified FlexUI_impl_* files in your project. See examples/ folder for examples of using this. 
// Prefer including the entire FlexUI/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// If you are new to Dear FlexUI, read documentation from the docs/ folder + read the top of FlexUI.cpp.
// Read online: https://github.com/ocornut/FlexUI/tree/master/docs

#pragma once
#include "FlexUI.h"      // FlexUI_IMPL_API

struct IDirect3DDevice9;

FlexUI_IMPL_API bool     FlexUI_ImplDX9_Init(IDirect3DDevice9* device);
FlexUI_IMPL_API void     FlexUI_ImplDX9_Shutdown();
FlexUI_IMPL_API void     FlexUI_ImplDX9_NewFrame();
FlexUI_IMPL_API void     FlexUI_ImplDX9_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing Dear FlexUI state.
FlexUI_IMPL_API bool     FlexUI_ImplDX9_CreateDeviceObjects();
FlexUI_IMPL_API void     FlexUI_ImplDX9_InvalidateDeviceObjects();
