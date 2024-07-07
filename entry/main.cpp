#include <Windows.h>
#include "../main/menu.h"
#include <thread>
#include <chrono>
#pragma comment(lib, "Urlmon.lib")
#include <urlmon.h>

//Made By MND1337
int main()
{
	SetConsoleTitle(L"FlexBase V1 [FREE]");
	std::string dwnld_URL = "https://camo.githubusercontent.com/f6d94e4477403fec4de7fd5dfc95e6faed2e4b15b15d299d9d9a4a300030e1d7/68747470733a2f2f692e696d6775722e636f6d2f62657a5447536c2e706e67";
	std::string savepath = "C:\\title.png";
	URLDownloadToFileA(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);

	i_dupa::skid().start();
	return i_overlay::here().Message.wParam;
}