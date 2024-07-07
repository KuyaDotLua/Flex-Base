#include "memory.h"
#include <iostream>
#include "skStr.h"
#include <conio.h> // para usar getch()
#include <fstream>
#include <string>
#include <windows.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>

#include <direct.h>

i_memory& i_memory::reeq()
{
	static i_memory ins;
	return ins;
}

// Función para ocultar la contraseña
std::string getHiddenInput()
{
    std::string input;
    char ch;
    while (true)
    {
        ch = _getch(); // Obtener el siguiente carácter sin mostrarlo
        if (ch == '\r') // Si es la tecla Enter, terminar la entrada
        {
            std::cout << std::endl;
            break;
        }
        else if (ch == '\b') // Si es la tecla de retroceso, borrar el último carácter
        {
            if (!input.empty())
            {
                input.pop_back();
                std::cout << "\b \b"; // Borrar el carácter en la pantalla
            }
        }
        else
        {
            input += ch;
            std::cout << '*'; // Mostrar un asterisco en lugar del carácter
        }
    }
    return input;
}

void logo()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;


}



void i_memory::initialize()
{
 
    logo();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
 

    std::string username;
    std::string password;
    std::cout << "[+] Welcome To FlexBase!\n";
    Sleep(500);
    std::cout << "[+] This Source Made By MND1337\n";
    Sleep(1500);
    std::cout << "[+] Github: https://github.com/KuyaDotLua\n";
    Sleep(2000);
    system("start https://github.com/KuyaDotLua");
    std::cout << "[+] Waiting For FiveM\n";
    Sleep(2000); 
    {
        window = FindWindowA("grcWindow", 0);
        if (!window)
        {
            logo();
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "Fivem not Found!";
            Sleep(2000);
        }
        logo();
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        GetWindowThreadProcessId(window, &pid);
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    }
}

bool i_memory::find_build(const wchar_t* processName) {

	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}



uintptr_t i_memory::get_base_address(std::string a1)
{
	std::wstring pname = std::wstring(a1.begin(), a1.end());
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnap == INVALID_HANDLE_VALUE)
		return 0;
	MODULEENTRY32 ent;
	ent.dwSize = sizeof(ent);
	if (Module32First(hSnap, &ent))
	{
		do
		{
			if (!_wcsicmp(ent.szModule, pname.c_str()))
			{
				std::wcout << "Version: " << ent.szModule << std::endl;
				CloseHandle(hSnap);
				return (uintptr_t)ent.modBaseAddr;
			}
		} while (Module32Next(hSnap, &ent));
	}
	CloseHandle(hSnap);
	return 0;
}


HWND i_memory::get_process_wnd(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
	uint32_t processId = 0;

	if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
		SetLastError((uint32_t)-1);
		pParams->first = hwnd;
		return FALSE;
	}

	return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}
