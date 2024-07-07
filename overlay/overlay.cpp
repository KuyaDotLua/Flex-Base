#include "overlay.h"
#include <sstream>
#include "million.h"
#include "../Hotkey/hotkey.h"
#include "IconsFontAwesome6.h"
#include "IconsFontAwesome6Brands.h"
#include <process.h>
#include <filesystem>
#include "../esp/esp.h"
int slider_int = 0;
int slider_int2 = 0;
int slider_int3s = 0;
bool test = false;
bool test2 = false;
bool test3 = false;
bool test4 = false;
const char* items[3] = { "head","chest", "legs" };

DWORD find_fivem()
{
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snap, &pe))
	{
		do
		{
			if (std::wstring(pe.szExeFile).find(L"GTAProcess.exe") != std::wstring::npos)
			{
				CloseHandle(snap);
				return pe.th32ProcessID;
			}
		} while (Process32Next(snap, &pe));
	}

	CloseHandle(snap);
	return 0;
}

void inject(DWORD pid, const std::string& path)
{
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!process)
	{
		exit(3);
	}

	void* buffer = VirtualAllocEx(process, nullptr, path.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!buffer)
	{
		exit(3);
	}

	if (!WriteProcessMemory(process, buffer, path.c_str(), path.size(), nullptr))
	{
		VirtualFreeEx(process, buffer, 0, MEM_RELEASE);
		CloseHandle(process);

		exit(3);
	}

	HANDLE thread = CreateRemoteThread(process, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, buffer, 0, nullptr);
	WaitForSingleObject(thread, INFINITE);

	DWORD threadResult = 0;
	GetExitCodeThread(thread, &threadResult);

	exit(3);


	VirtualFreeEx(process, buffer, 0, MEM_RELEASE);
	CloseHandle(process);
}

/*
const char* const KeyNames[] = {
	"VK_RBUTTON",
	"VK_LBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"VK_BACK",
	"VK_TAB",
	"VK_CLEAR",
	"VK_RETURN",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_ALT",
	"VK_PAUSE",
	"VK_CAPITAL",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
};
*/

const char* const KeyNames[] = {
	"Right Click",
	"ALT",
	"Control",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"Left Click",
};

bool aimbot = false;
bool esp = false;
bool autotarget = false;
bool autoshot = false;
bool drawfov = false;
bool healthcheck = false;
bool amourcheck = false;

ImFont* LexendRegular;

int fov = 0;
int smooth = 0;

int Hotkey = 2;

bool active = true;
bool active2 = false;
int subtab2 = 0;
int maintab = 99;

uint64_t selectedped = 0;

//pero ara dale otra vez 
int selectedHeal = 200;
int selectedArmor = 100;
i_overlay& i_overlay::here()
{
	static i_overlay ins;
	return ins;
}



bool i_overlay::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(p_Device, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

std::int64_t __stdcall i_overlay::wndproc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam)
{

	std::int64_t FlexUI_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	FlexUI_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam);
	if (i_overlay::here().showmenu)
		return 1;

	return DefWindowProcA(i_overlay::here().MyWnd, message, wparam, lparam);
}

ImFont* font;
ImFont* fon2t;
ImFont* Consolas;
int my_image_widtsh = 408;
int my_image_heighst = 94;
PDIRECT3DTEXTURE9 my_texture = NULL;
int maintabs = 99;
void styledark()
{
	FlexUIStyle* Style = &FlexUI::GetStyle();
	ImVec4* Colors = Style->Colors;
	Style->Colors[FlexUICol_ScrollbarGrab] = ImColor(237, 143, 243, 0);
	Style->Colors[FlexUICol_FrameBg] = ImColor(34, 34, 34);
	Style->Colors[FlexUICol_CheckMark] = ImColor(34, 34, 34);

	Style->Colors[FlexUICol_ScrollbarBg] = ImColor(237, 143, 243, 0);
	/*Style->Colors[FlexUICol_WindowBg] = ImColor(17, 17, 17);
	Style->Colors[FlexUICol_ChildBg] = ImColor(22, 22, 22);
	Style->Colors[FlexUICol_FrameBg] = ImColor(17, 17, 17);
	Style->Colors[FlexUICol_CheckMark] = ImColor(237, 143, 243);
	Style->Colors[FlexUICol_FrameBgActive] = ImColor(17, 17, 17);
	Style->Colors[FlexUICol_FrameBgHovered] = ImColor(17, 17, 17);
	Style->Colors[FlexUICol_PopupBg] = ImColor(12, 12, 12);
	Style->Colors[FlexUICol_TextDisabled] = ImColor(255, 255, 255);
	Style->Colors[FlexUICol_ButtonActive] = ImColor(100, 150, 255);
	Style->Colors[FlexUICol_Button] = ImColor(27, 27, 27);
	Style->Colors[FlexUICol_ButtonActive] = ImColor(237, 143, 243);
	Style->Colors[FlexUICol_ButtonHovered] = ImColor(237, 143, 243);
	Style->Colors[FlexUICol_Border] = ImColor(26, 26, 26);
	Style->Colors[FlexUICol_Separator] = ImColor(237, 143, 243);
	Style->Colors[FlexUICol_ScrollbarGrab] = ImColor(237, 143, 243);






	Style->ScrollbarSize = 2.0f;
	Style->WindowRounding = 5;
	Style->FrameRounding = 5;
	Style->FrameBorderSize = 0;
	Style->ChildBorderSize = 1;
	*/
	FlexUIIO& ios = FlexUI::GetIO(); (void)ios;

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	ImFont* FontAwesome = FlexUI::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
	ImFont* FontAwesomeBrands = FlexUI::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);
	ios.Fonts->AddFontDefault();
	font = ios.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Impact.ttf", 80.0f, nullptr, ios.Fonts->GetGlyphRangesJapanese());
	fon2t = ios.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 20.0f, nullptr, ios.Fonts->GetGlyphRangesJapanese());

}

bool homemenu = true;
void emenus()
{
	FlexUIStyle& style = FlexUI::GetStyle();

	// Set the alpha component of the window background color (RGBA)
	style.Colors[FlexUICol_WindowBg].w = 0.5f; // Set alpha to 0.5 (50% transparency)
	style.Colors[FlexUICol_ChildBg].w = 0.2f; // Set alpha to 0.5 (50% transparency)
	style.Colors[FlexUICol_ScrollbarGrab] = ImColor(237, 143, 243, 0);
	style.Colors[FlexUICol_FrameBg] = ImColor(34, 34, 34);
	style.Colors[FlexUICol_CheckMark] = ImColor(255, 0, 0);

	style.Colors[FlexUICol_ScrollbarBg] = ImColor(237, 143, 243, 0);
	FlexUI::SetNextWindowSize(ImVec2(430, 470));
	FlexUI::Begin("#TICKBASE", nullptr, FlexUIWindowFlags_NoScrollbar | FlexUIWindowFlags_NoTitleBar | FlexUIWindowFlags_NoResize);
	FlexUI::SetCursorPos(ImVec2(0, 0));
	FlexUI::BeginChild("#Title", ImVec2(430, 130));
	FlexUI::PushFont(font);
	FlexUI::SetCursorPos(ImVec2(0, 0));
	FlexUI::Image((void*)my_texture, ImVec2(430, 100));
	FlexUI::PopFont();
	FlexUI::EndChild();
	FlexUI::SetCursorPos(ImVec2(0, 95));
	FlexUI::BeginChild("#EndTitle", ImVec2(430, 35));
	FlexUI::PushFont(fon2t);
	FlexUI::PushStyleVar(FlexUIStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
	FlexUI::PushStyleColor(FlexUICol_Button, ImVec4(ImColor(34, 34, 34, 140)));
	FlexUI::PushStyleColor(FlexUICol_ButtonHovered, ImVec4(ImColor(34, 34, 34, 255)));
	FlexUI::PushStyleColor(FlexUICol_ButtonActive, ImVec4(ImColor(34, 34, 34, 255)));


	if (FlexUI::Button(ICON_FA_HOUSE"")) {
		maintab = 99;
		homemenu = true;
	}
	FlexUI::SameLine();
	FlexUI::TextColored(ImColor(255, 0, 0), "[v 1.0]");

	FlexUI::PopFont();
	FlexUI::EndChild();
	FlexUI::SetCursorPos(ImVec2(0, 525));
	FlexUI::BeginChild("#EndMenu", ImVec2(430, 35));
	FlexUI::EndChild();



	FlexUI::SetCursorPos(ImVec2(0, 130));

	FlexUI::BeginChild("#Menu", ImVec2(430, 395));

	FlexUI::Begin("##watermark", nullptr, FlexUIWindowFlags_NoBackground | FlexUIWindowFlags_NoTitleBar | FlexUIWindowFlags_NoResize | FlexUIWindowFlags_NoMove | FlexUIWindowFlags_NoCollapse);
	FlexUI::TextColored(ImColor(255, 0, 0), "Made By MND#1337\n If you paid you got scammed");
	FlexUI::End();
	if (homemenu)
	{
		FlexUI::SetCursorPos(ImVec2(0, 0));

		if (FlexUI::Button(ICON_FA_USER"         Self", ImVec2(430, 35))) { homemenu = false; maintab = 1; }
		FlexUI::SetCursorPos(ImVec2(0, 35));
		if (FlexUI::Button(ICON_FA_WIFI"        Network", ImVec2(430, 35))) { homemenu = false; maintab = 2; }
		FlexUI::SetCursorPos(ImVec2(0, 70));
		if (FlexUI::Button(ICON_FA_CAR"        Vehicle", ImVec2(430, 35))) { homemenu = false; maintab = 3; }
		FlexUI::SetCursorPos(ImVec2(0, 105));
		if (FlexUI::Button(ICON_FA_ROCKET"        Weapons", ImVec2(430, 35))) { homemenu = false; maintab = 4; }
		FlexUI::SetCursorPos(ImVec2(0, 140));
		if (FlexUI::Button(ICON_FA_CODE"        Lua", ImVec2(430, 35))) { homemenu = false; maintab = 5; }
		FlexUI::SetCursorPos(ImVec2(0, 175));
		if (FlexUI::Button(ICON_FA_BOOK"        Resources", ImVec2(430, 35))) { homemenu = false; maintab = 6; }
		FlexUI::SetCursorPos(ImVec2(0, 210));
		if (FlexUI::Button(ICON_FA_PHONE"         Mods", ImVec2(430, 35))) { homemenu = false; maintab = 7; }
		FlexUI::SetCursorPos(ImVec2(0, 245));
		if (FlexUI::Button(ICON_FA_COGS"        Settings", ImVec2(430, 35))) { homemenu = false; maintab = 8; }

		FlexUI::SetCursorPos(ImVec2(0, 280));

		FlexUI::PushStyleColor(FlexUICol_Text, ImVec4(ImColor(255, 0, 0, 255)));
		if (FlexUI::Button((("Unload Cheat")), ImVec2(430, 35)))
		{
			exit(0);
		}
	}












	if (maintab == 5)
	{


		FlexUI::SetCursorPos(ImVec2(0, 0));

		if (FlexUI::Button("Test print", ImVec2(430, 35)))
		{
		}
	}
	if (maintab == 8)
	{



		FlexUI::SliderInt(("Heal Amount"), &selectedHeal, 1, 200);
		FlexUI::SliderInt(("Armor Amount"), &selectedArmor, 1, 100);
		FlexUI::SliderInt(skCrypt(("Fov")), &i_exploits::exp().fovvalue, 50.0f, 150);
		FlexUI::SliderInt(skCrypt(("Run Speed")), &i_exploits::exp().playerspeed, 1.0f, 100);
		FlexUI::SliderInt(skCrypt(("Swim Speed")), &i_exploits::exp().swimspeed2, 1.0f, 100);
		FlexUI::SliderInt(skCrypt(("Noclip Speed")), &i_exploits::exp().noclipspeed, 1.0f, 100);
		FlexUI::SliderInt(skCrypt("Damage Amount"), &i_exploits::exp().damageamount, 1.0f, 100);

	}
	if (maintab == 7)
	{
		FlexUI::SetCursorPos(ImVec2(0, 0));

		if (FlexUI::Button("TP Legion Square", ImVec2(430, 35)))
		{
			D3DXVECTOR3 pos = i_memory::reeq().Read<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90);
			uintptr_t navig = i_memory::reeq().Read<uintptr_t>(i_dupa::skid().i_localplayer + 0x30);
			float xcoord = 190.52f;
			float ycoord = -873.23f;
			float zcoord = 31.5f;

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x30, D3DXVECTOR3(0, 0, 0));

			i_memory::reeq().Write<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90, D3DXVECTOR3(xcoord, ycoord, zcoord));

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x50, D3DXVECTOR3(xcoord, ycoord, zcoord));
		}
		FlexUI::SetCursorPos(ImVec2(0, 35));

		if (FlexUI::Button("TP Humane", ImVec2(430, 35)))
		{
			D3DXVECTOR3 pos = i_memory::reeq().Read<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90);
			uintptr_t navig = i_memory::reeq().Read<uintptr_t>(i_dupa::skid().i_localplayer + 0x30);
			float xcoord = 3641.22f;
			float ycoord = 3743.51f;
			float zcoord = 28.52f;

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x30, D3DXVECTOR3(0, 0, 0));

			i_memory::reeq().Write<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90, D3DXVECTOR3(xcoord, ycoord, zcoord));

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x50, D3DXVECTOR3(xcoord, ycoord, zcoord));
		}
		FlexUI::SetCursorPos(ImVec2(0, 70));

		if (FlexUI::Button("TP Paleto Bay", ImVec2(430, 35)))
		{
			D3DXVECTOR3 pos = i_memory::reeq().Read<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90);
			uintptr_t navig = i_memory::reeq().Read<uintptr_t>(i_dupa::skid().i_localplayer + 0x30);
			float xcoord = -138.52f;
			float ycoord = 6356.99f;
			float zcoord = 31.49f;

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x30, D3DXVECTOR3(0, 0, 0));

			i_memory::reeq().Write<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90, D3DXVECTOR3(xcoord, ycoord, zcoord));

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x50, D3DXVECTOR3(xcoord, ycoord, zcoord));
		}
		FlexUI::SetCursorPos(ImVec2(0, 105));

		if (FlexUI::Button("TP Sandy Shores", ImVec2(430, 35)))
		{
			D3DXVECTOR3 pos = i_memory::reeq().Read<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90);
			uintptr_t navig = i_memory::reeq().Read<uintptr_t>(i_dupa::skid().i_localplayer + 0x30);
			float xcoord = 1837.16f;
			float ycoord = 3695.66f;
			float zcoord = 34.27f;

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x30, D3DXVECTOR3(0, 0, 0));

			i_memory::reeq().Write<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90, D3DXVECTOR3(xcoord, ycoord, zcoord));

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x50, D3DXVECTOR3(xcoord, ycoord, zcoord));
		}
		FlexUI::SetCursorPos(ImVec2(0, 140));

		if (FlexUI::Button("TP Los Santos Airport", ImVec2(430, 35)))
		{
			D3DXVECTOR3 pos = i_memory::reeq().Read<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90);
			uintptr_t navig = i_memory::reeq().Read<uintptr_t>(i_dupa::skid().i_localplayer + 0x30);
			float xcoord = -1037.52f;
			float ycoord = -2737.02f;
			float zcoord = 13.77f;

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x30, D3DXVECTOR3(0, 0, 0));

			i_memory::reeq().Write<D3DXVECTOR3>(i_dupa::skid().i_localplayer + 0x90, D3DXVECTOR3(xcoord, ycoord, zcoord));

			i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x50, D3DXVECTOR3(xcoord, ycoord, zcoord));
		}


	}
	if (maintab == 4)
	{








		FlexUI::Checkbox(skCrypt("No Recoil"), &i_exploits::exp().norecoil);
		FlexUI::Checkbox(skCrypt("No Spread"), &i_exploits::exp().nospread);
		FlexUI::Checkbox(skCrypt("No Reload"), &i_exploits::exp().noreload);
		FlexUI::Checkbox(skCrypt("Infinite Ammo"), &i_exploits::exp().infiniteammo);
		FlexUI::Checkbox(skCrypt("Damage Multiplier"), &i_exploits::exp().damageboos);

		FlexUI::SetCursorPos({ 0, 210 });
		if (FlexUI::Button("Refill Ammo", ImVec2(430, 35)))
		{
			uintptr_t WeaponManager = i_memory::reeq().Read<uintptr_t>(i_dupa::skid().i_localplayer + i_dupa::skid().weaponmanager);
			uintptr_t weaponinfo = i_memory::reeq().Read<uintptr_t>(WeaponManager + 0x20);
			uintptr_t AmmoInfo = i_memory::reeq().Read<uintptr_t>(weaponinfo + 0x60);

			uintptr_t AmmoCount = i_memory::reeq().Read<uintptr_t>(AmmoInfo + 0x8);

			uintptr_t AmmoCount2 = i_memory::reeq().Read<uintptr_t>(AmmoCount + 0x0);
			int actualammo = i_memory::reeq().Read<uintptr_t>(AmmoCount2 + 0x18);

			i_memory::reeq().Write<float>(AmmoCount2 + 0x18, 30);
		}








	}
	if (maintab == 1)
	{

		FlexUI::Checkbox(skCrypt("Noclip"), &i_exploits::exp().noclip);
		FlexUI::Checkbox(skCrypt("Semi GodMode"), &i_exploits::exp().godmodee);
		FlexUI::Checkbox(skCrypt("Custom Speed"), &i_exploits::exp().customspeed);
		FlexUI::Checkbox(skCrypt("Infinite Stamina"), &i_exploits::exp().infinitestamina);
		FlexUI::Checkbox(skCrypt("Fov Changer"), &i_exploits::exp().customfov);
		FlexUI::Checkbox(skCrypt("Fill Health"), &healthcheck);
		FlexUI::Checkbox(skCrypt("Fill Amour"), &amourcheck);

		if (healthcheck)
		{
			i_memory::reeq().Write<float>(i_dupa::skid().i_localplayer + 0x280, static_cast<float>(selectedHeal));
			healthcheck = false;
		}
		FlexUI::SetCursorPos(ImVec2(0, 245));

		if (amourcheck)
		{
			uintptr_t armadura = i_dupa::skid().armoroffset;
			i_memory::reeq().Write<float>(i_dupa::skid().i_localplayer + armadura, static_cast<float>(selectedArmor));

			amourcheck = false;
		}

	}



	if (maintab == 2)
	{
		FlexUI::SetCursorPos(ImVec2(0, 0));

		if (FlexUI::Button(ICON_FA_CROSSHAIRS"         Aimbot", ImVec2(430, 35))) { homemenu = false; maintab = 100; }
		FlexUI::SetCursorPos(ImVec2(0, 35));

		if (FlexUI::Button(ICON_FA_PHONE"         Visuals", ImVec2(430, 35))) { homemenu = false; maintab = 101; }






		static char searchQuery[256];


		FlexUI::InputText("Search", searchQuery, sizeof(searchQuery));

		uintptr_t ClosestPlayer = 0;
		uintptr_t ReplayInterface = i_dupa::skid().i_replay;
		uintptr_t localplayer = i_dupa::skid().i_localplayer;
		if (ReplayInterface)
		{
			uintptr_t PedReplayInterface = i_memory::reeq().Read<DWORD64>(ReplayInterface + 0x18);
			uintptr_t PedList = i_memory::reeq().Read<DWORD64>(PedReplayInterface + 0x100);
			int entitylist = i_memory::reeq().Read<int>(PedReplayInterface + 0x108);

			bool firstSeparator = true;

			for (int i = 0U; i < entitylist; i++)
			{
				if (!PedList) continue;

				uintptr_t Ped = i_memory::reeq().Read<uintptr_t>(PedList + (i * 0x10));
				if (!Ped) continue;

				if (!i_memory::reeq().Read<uintptr_t>(Ped + i_dupa::skid().playerinfo))
					continue;

				uint64_t playerinfos = i_memory::reeq().Read<uint64_t>(Ped + i_dupa::skid().playerinfo);
				int playeridaddr = i_memory::reeq().Read<int>(playerinfos + 0x88);
				std::string playeridstr = std::to_string(playeridaddr);

				if (!searchQuery[0] || playeridstr.find(searchQuery) != std::string::npos)
				{
					if (!firstSeparator)
					{
						FlexUI::Separator();
					}
					firstSeparator = false;

					FlexUI::Text(playeridstr.c_str());
					if (FlexUI::IsItemClicked())
					{
						selectedped = Ped;
					}
				}
			}
		}

	}



	//network option


	if (maintab == 3)
	{
		FlexUI::SetCursorPos(ImVec2(0, 0));

		if (FlexUI::Button("Repair Engine", ImVec2(430, 35)))
		{
			uintptr_t localplayer = i_dupa::skid().i_localplayer;
			uintptr_t actualcar = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
			i_memory::reeq().Write<float>(actualcar + i_dupa::skid().i_enginehealth, 1000.f);

		}
		FlexUI::SetCursorPos(ImVec2(0, 35));

		if (FlexUI::Button("Break Engine", ImVec2(430, 35)))
		{
			uintptr_t localplayer = i_dupa::skid().i_localplayer;
			uintptr_t actualcar = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
			i_memory::reeq().Write<float>(actualcar + i_dupa::skid().i_enginehealth, 0.f); //  nice , do it wait
		}
		FlexUI::SetCursorPos(ImVec2(0, 70));

		if (FlexUI::Button("Red Color", ImVec2(430, 35)))
		{
			uintptr_t localplayer = i_dupa::skid().i_localplayer;
			uintptr_t cur_vehicle = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
			auto vehicle_mods_ptr = i_memory::reeq().Read<uintptr_t>(cur_vehicle + 0x48);
			if (vehicle_mods_ptr) {
				auto model_info = i_memory::reeq().Read<uintptr_t>(vehicle_mods_ptr + 0x0020);
				if (model_info) {
					float red = 1.0f;
					i_memory::reeq().Write<float>(model_info + 0xA4, red);
					i_memory::reeq().Write<float>(model_info + 0xA8, red);
				}
			}
		}
		FlexUI::SetCursorPos(ImVec2(0, 105));

		if (FlexUI::Button("Green Color", ImVec2(430, 35)))
		{
			uintptr_t localplayer = i_dupa::skid().i_localplayer;
			uintptr_t cur_vehicle = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
			auto vehicle_mods_ptr = i_memory::reeq().Read<uintptr_t>(cur_vehicle + 0x48);

			if (vehicle_mods_ptr) {
				auto model_info = i_memory::reeq().Read<uintptr_t>(vehicle_mods_ptr + 0x0020);

				if (model_info) {
					float green = 1.0f;  // Set the green color value
					i_memory::reeq().Write<float>(model_info + 0xA0, green);  // Change A4 to A0 for green channel
					i_memory::reeq().Write<float>(model_info + 0xA4, green);  // Change A8 to A4 for green channel
				}
			}
		}
		FlexUI::SetCursorPos(ImVec2(0, 140));

		if (FlexUI::Button("Blue Color", ImVec2(430, 35)))
		{
			uintptr_t localplayer = i_dupa::skid().i_localplayer;
			uintptr_t cur_vehicle = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
			auto vehicle_mods_ptr = i_memory::reeq().Read<uintptr_t>(cur_vehicle + 0x48);

			if (vehicle_mods_ptr) {
				auto model_info = i_memory::reeq().Read<uintptr_t>(vehicle_mods_ptr + 0x0020);

				if (model_info) {
					float blue = 1.0f;  // Set the blue color value
					i_memory::reeq().Write<float>(model_info + 0xA8, blue);  // Change A0 to A8 for blue channel
					i_memory::reeq().Write<float>(model_info + 0xAC, blue);  // Change A4 to AC for blue channel
				}
			}
		}



	}


	if (maintab == 100)
	{
		if (GetAsyncKeyState(VK_DELETE))
		{
			maintab = 2;
		}
		

	}

	if (maintab == 101)
	{
		FlexUI::SetNextWindowSize(ImVec2(200, 300));
		FlexUI::SetNextWindowPos(ImVec2(FlexUI::GetIO().DisplaySize.x - 250, 150));
		FlexUI::Begin(skCrypt("preview"), nullptr, FlexUIWindowFlags_NoTitleBar | FlexUIWindowFlags_NoCollapse | FlexUIWindowFlags_NoScrollbar | FlexUIWindowFlags_NoResize | FlexUIWindowFlags_NoSavedSettings | FlexUIWindowFlags_NoCollapse | FlexUIWindowFlags_NoMove);
		{

			float windowWidth = FlexUI::GetWindowWidth();


			float textWidth = FlexUI::CalcTextSize("Visuals Preview").x;


			float centerX = (windowWidth - textWidth) / 2.0f;


			FlexUI::SetCursorPosX(centerX);


			FlexUI::Text("Visuals Preview");

			//crosshair preview
			if (i_esp::esp().crosshair)
			{

				ImVec2 previewWindowPos = FlexUI::GetWindowPos();
				ImVec2 previewWindowSize = FlexUI::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				ImVec2 lineStart(center_x - 6, center_y);
				ImVec2 lineEnd(center_x + 7, center_y);
				ImVec2 lineStartVertical(center_x, center_y - 6);
				ImVec2 lineEndVertical(center_x, center_y + 7);
				ImU32 lineColor = IM_COL32(255, 255, 255, 255);

				FlexUI::GetForegroundDrawList()->AddLine(lineStart, lineEnd, lineColor);
				FlexUI::GetForegroundDrawList()->AddLine(lineStartVertical, lineEndVertical, lineColor);
			}

			//Corner Box preview
			if (i_esp::esp().box)
			{
				// Calcular el centro de la ventana de preview
				ImVec2 previewWindowPos = FlexUI::GetWindowPos();
				ImVec2 previewWindowSize = FlexUI::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);


				float Width = 100.0f;
				float Height = 100.0f;

				float boxMargin = 5.0f;

				ImVec2 topLeft(center_x - Width / 2 + boxMargin, center_y - Height / 2 + boxMargin);
				ImVec2 bottomRight(center_x + Width / 2 - boxMargin, center_y + Height / 2 - boxMargin);

				float cornerSize = 10.0f;

				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x + cornerSize, topLeft.y), i_esp::esp().colorespon, 1.0f);
				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x, topLeft.y + cornerSize), i_esp::esp().colorespon, 1.0f);
				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x - cornerSize, topLeft.y), ImVec2(bottomRight.x, topLeft.y), i_esp::esp().colorespon, 1.0f);
				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x, topLeft.y), ImVec2(bottomRight.x, topLeft.y + cornerSize), i_esp::esp().colorespon, 1.0f);
				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, bottomRight.y - cornerSize), ImVec2(topLeft.x, bottomRight.y), i_esp::esp().colorespon, 1.0f);
				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, bottomRight.y), ImVec2(topLeft.x + cornerSize, bottomRight.y), i_esp::esp().colorespon, 1.0f);
				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x - cornerSize, bottomRight.y), ImVec2(bottomRight.x, bottomRight.y), i_esp::esp().colorespon, 1.0f);
				FlexUI::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x, bottomRight.y - cornerSize), ImVec2(bottomRight.x, bottomRight.y), i_esp::esp().colorespon, 1.0f);

				FlexUI::GetForegroundDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32(70, 70, 70, 70));
			}
			//Normal Box preview
			if (i_esp::esp().boxnormal)
			{
				ImVec2 previewWindowPos = FlexUI::GetWindowPos();
				ImVec2 previewWindowSize = FlexUI::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				float Width = 100.0f;
				float Height = 100.0f;

				float boxMargin = 5.0f;

				ImVec2 topLeft(center_x - Width / 2 + boxMargin, center_y - Height / 2 + boxMargin);
				ImVec2 bottomRight(center_x + Width / 2 - boxMargin, center_y + Height / 2 - boxMargin);

				ImU32 lineColor = i_esp::esp().colorespon;
				ImU32 fillColor = IM_COL32(70, 70, 70, 50);

				FlexUI::GetForegroundDrawList()->AddRect(topLeft, bottomRight, lineColor, 0);

				FlexUI::GetForegroundDrawList()->AddRectFilled(topLeft, bottomRight, fillColor);
			}
			//Health Bar Preview
			if (i_esp::esp().heathbar)
			{
				ImVec2 previewWindowPos = FlexUI::GetWindowPos();
				ImVec2 previewWindowSize = FlexUI::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				float Width = 92.0f;
				float Height = 5.0f;

				float HealthPed = 200;
				float HealthDistance = 0;

				float HealthPercentage = (HealthPed - 100) / 100.0f; // Ajusta esta lógica según tus necesidades

				ImColor healthColor = ImColor(14, 110, 17, 255);
				ImColor backgroundColor = ImColor(0, 0, 0, 255);

				float barX = center_x + (Width / 2) + HealthDistance + 5; // Ajusta la posición X aquí
				float barY = center_y + Height / 2 - 2; // Ajusta la posición Y aquí, por ejemplo, agregamos 10 píxeles hacia abajo

				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width, Height), backgroundColor);
				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width * HealthPercentage, Height), healthColor);
			}


			//Armor Bar Preview
			if (i_esp::esp().armorbar)
			{
				ImVec2 previewWindowPos = FlexUI::GetWindowPos();
				ImVec2 previewWindowSize = FlexUI::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				float Width = 92.0f;
				float Height = 5.0f;

				float ArmorPed = 200;
				float ArmorDistance = 0;

				float ArmorPercentage = (ArmorPed - 100) / 100.0f; // Ajusta esta lógica según tus necesidades

				ImColor armorColor = ImColor(10, 145, 255, 255);
				ImColor backgroundColor = ImColor(0, 0, 0, 255);

				float barX = center_x + (Width / 2) + ArmorDistance - 102; // Ajusta la posición X aquí
				float barY = center_y + Height / 2 - 2; // Ajusta la posición Y aquí, por ejemplo, agregamos 10 píxeles hacia abajo

				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width, Height), backgroundColor);
				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width * ArmorPercentage, Height), armorColor);
			}

			ImVec2 cursorPos = FlexUI::GetCursorPos();

			if (i_esp::esp().distance)
			{
				float offsetX = 55.0f;

				float offsetY = 190.0f;

				FlexUI::SetCursorPos(ImVec2(cursorPos.x + offsetX, cursorPos.y + offsetY));

				FlexUI::Text("ID: 1 - [ 33m ]");

				FlexUI::SetCursorPos(cursorPos);
			}

			if (i_esp::esp().weapons)
			{
				float offsetX = 77.0f;

				float offsetY = 170.0f;

				FlexUI::SetCursorPos(ImVec2(cursorPos.x + offsetX, cursorPos.y + offsetY));

				FlexUI::Text("pistol");

				FlexUI::SetCursorPos(cursorPos);
			}






		}
		FlexUI::End();
		if (GetAsyncKeyState(VK_DELETE))
		{
			maintab = 2;
		}
		FlexUI::Checkbox(skCrypt("Ignore Ped"), &i_esp::esp().ignoreped);
		FlexUI::Checkbox(skCrypt("3d esp"), &i_esp::esp().dbox);
		FlexUI::Checkbox(skCrypt("Crosshair"), &i_esp::esp().crosshair);
		FlexUI::Checkbox(skCrypt("Corner Box"), &i_esp::esp().box);
		FlexUI::Checkbox(skCrypt("Box"), &i_esp::esp().boxnormal);
		FlexUI::Checkbox(skCrypt("Skeleton"), &i_esp::esp().skeleton);
		FlexUI::Checkbox(skCrypt("Health Bar"), &i_esp::esp().heathbar);
		FlexUI::Checkbox(skCrypt("Armor Bar"), &i_esp::esp().armorbar);
		FlexUI::Checkbox(skCrypt("ID + Distance"), &i_esp::esp().distance);
		FlexUI::Checkbox(skCrypt("Weapon"), &i_esp::esp().weapons);
		FlexUI::Checkbox(skCrypt("Lines"), &i_esp::esp().lines);

	}

















	if (maintab == 3)
	{


	}
	if (maintab == 4)
	{


	}
	if (maintab == 5)
	{


	}
	if (maintab == 6)
	{


	}
	if (maintab == 7)
	{


	}












	FlexUI::PopStyleVar(1);
	FlexUI::PopStyleColor(4);

	FlexUI::EndChild();


	FlexUI::End();
}
HRESULT i_overlay::init_wndparams(HWND hwnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		return E_FAIL;

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hwnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device))) {
		p_Object->Release();
		return E_FAIL;
	}

	FlexUI::CreateContext();


	bool ret = LoadTextureFromFile("C:\\title.png", &my_texture, &my_image_widtsh, &my_image_heighst);
	IM_ASSERT(ret);

	FlexUIIO& io = FlexUI::GetIO();
	io.IniFilename = NULL;

	char windows_directory[MAX_PATH];

	// std::string segoeui_dir = (std::string)windows_directory + "\\Fonts\\segoeui.ttf";

	// global34 = io.Fonts->AddFontFromFileTTF(segoeui_dir.c_str(), 18.0f, &font_config, io.Fonts->GetGlyphRangesCyrillic());
	// ico_combo2 = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());

	global3 = io.Fonts->AddFontFromMemoryTTF(segoui, sizeof(segoui), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());

	FlexUI_ImplWin32_Init(hwnd);
	FlexUI_ImplDX9_Init(p_Device);

	return S_OK;
}

void i_overlay::cleanup_d3d()
{
	if (p_Device != NULL) {
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL) {
		p_Object->Release();
	}
}

void set_window_target()
{
	i_overlay& overlay = i_overlay::here();
	i_memory& memory = i_memory::reeq();

	while (true) {
		overlay.GameWnd = memory.get_process_wnd(memory.pid);

		if (overlay.GameWnd) {
			ZeroMemory(&overlay.GameRect, sizeof(overlay.GameRect));
			GetWindowRect(overlay.GameWnd, &overlay.GameRect);

			DWORD dwStyle = GetWindowLong(overlay.GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER) {
				overlay.GameRect.top += 32;
				overlay.Height -= 39;
			}

			overlay.ScreenCenterX = overlay.Width / 2;
			overlay.ScreenCenterY = overlay.Height / 2;

			MoveWindow(overlay.MyWnd, overlay.GameRect.left, overlay.GameRect.top, overlay.Width, overlay.Height, true);
		}
	}
}

char Licence[50] = "";
void i_overlay::setup_window()
{
	(HANDLE)_beginthreadex(nullptr, 0, reinterpret_cast<_beginthreadex_proc_type>(&set_window_target), 0, 0, 0);
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		wndproc,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("Chrome"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyWnd = CreateWindowA(("Chrome"), ("Chrome"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
}


void i_overlay::style()
{
	FlexUIStyle* Style = &FlexUI::GetStyle();
	ImVec4* Colors = Style->Colors;


	FlexUI::StyleColorsDark();
	Style->WindowRounding = 4.f;
	Style->WindowBorderSize = 0;
	Style->ChildRounding = 3.0f;
	Style->Colors[FlexUICol_WindowBg] = ImColor(96, 106, 211);
	Style->Colors[FlexUICol_Border] = ImColor(23, 23, 23);
	Style->Colors[FlexUICol_Button] = ImColor(20, 24, 28);
	Style->Colors[FlexUICol_ButtonActive] = ImColor(237, 143, 243);
	Style->Colors[FlexUICol_ButtonHovered] = ImColor(237, 143, 243);



	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	ImFont* FontAwesome = FlexUI::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
	ImFont* FontAwesomeBrands = FlexUI::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);

}

void i_overlay::CustomStyleColor()
{
	FlexUIStyle& s = FlexUI::GetStyle();


	s.WindowBorderSize = 0;

	s.PopupBorderSize = 0;
	s.FrameBorderSize = 1.0f;
	s.FrameRounding = 4.f;

}

void i_overlay::menu()
{
	emenus();
}

void i_overlay::signal()
{
	ZeroMemory(&Message, sizeof(MSG));
	style();
	static RECT old_rc;
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();


		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		}
		if (showmenu)
		{

		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		FlexUIIO& io = FlexUI::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom) {

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		i_overlay::here().render();

		Sleep(10);
	}
	FlexUI_ImplDX9_Shutdown();
	FlexUI_ImplWin32_Shutdown();
	FlexUI::DestroyContext();
	i_overlay::here().cleanup_d3d();
	DestroyWindow(i_overlay::here().MyWnd);


}

std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

void i_overlay::RectFilled(float x, float y, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
{
	FlexUI::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

void i_overlay::DrawHealthBarhorizontal(ImVec2 pos, ImVec2 pos2, ImVec2 dim, ImColor col)
{

	FlexUI::GetForegroundDrawList()->AddLine(ImVec2(pos.x + dim.y, pos.y), ImVec2(pos2.x, pos.y), col, dim.x);

}

void i_overlay::DrawRectFlexUI(int x, int y, int w, int h, ImVec4 color, float thickness)
{
	FlexUI::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), FlexUI::ColorConvertFloat4ToU32(color), 0, 0, thickness);
}

void i_overlay::DrawLeftProgressBar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;
	RGBA healthcol = { R, G, 0, 255 };

	DrawFilledRect(x - (w / 2) - 3, y, thick, (h)*m_health / 100, &healthcol);
}

void i_overlay::DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	FlexUI::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), FlexUI::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), 0, 0);
}
void i_overlay::DrawShieldbar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;

	DrawFilledRect(x - (w / 2) - 3, y, thick, (h)*m_health / 100, &Col.darkblue);
}

void i_overlay::DrawHealthBar(ImVec2 pos, ImVec2 dim, ImColor col)
{


	FlexUI::GetForegroundDrawList()->AddLine(pos, ImVec2(pos.x, pos.y - dim.y), col, dim.x);

}
void i_overlay::DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width)
{
	FlexUI::GetForegroundDrawList()->AddLine(x, y, color, width);
}

void i_overlay::DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{

			FlexUI::GetForegroundDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), FlexUI::GetColorU32(color), line.c_str());
		}
		else
		{//

			FlexUI::GetForegroundDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * index), FlexUI::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

void i_overlay::DrawString(float fontSize, int x, int y, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = FlexUI::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		FlexUI::GetForegroundDrawList()->AddText(FlexUI::GetFont(), fontSize, ImVec2(x + 1, y + 1), FlexUI::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		FlexUI::GetForegroundDrawList()->AddText(FlexUI::GetFont(), fontSize, ImVec2(x - 1, y - 1), FlexUI::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		FlexUI::GetForegroundDrawList()->AddText(FlexUI::GetFont(), fontSize, ImVec2(x + 1, y - 1), FlexUI::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		FlexUI::GetForegroundDrawList()->AddText(FlexUI::GetFont(), fontSize, ImVec2(x - 1, y + 1), FlexUI::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	FlexUI::GetForegroundDrawList()->AddText(FlexUI::GetFont(), fontSize, ImVec2(x, y), ImColor(189, 175, 255), text.c_str());
}

static const char* keyNames[] = {
	"",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

void i_overlay::HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = ("Select Key");
	else
		aimkeys = preview_value;

	if (status == 1)
	{
		aimkeys = ("Press key to bind");
	}
	if (FlexUI::Button(aimkeys.c_str(), ImVec2(100, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}


void i_overlay::initialize()
{

	// some stuff xyzus
	setup_window();
	init_wndparams(MyWnd);
	Sleep(2000);
	signal();
}


