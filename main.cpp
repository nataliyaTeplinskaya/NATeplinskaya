#define WINVER 0x0500
#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;
#define INFO_BUFFER_SIZE 32767
#define BUFSIZE MAX_PATH
#define WINVER 0x0502
#define MAX_DATA_LENGTH 16383

int main()
{

    //1.1 ќпределение параметров системы и компьютера

    OSVERSIONINFO os_vers = {0};
    os_vers.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&os_vers);
    DWORD dwMinorVersion = os_vers.dwMinorVersion;
    DWORD dwMajorVersion = os_vers.dwMajorVersion;
    DWORD dwBuild = os_vers.dwBuildNumber;

    printf("Version of OS is %d.%d.%d\n",
                dwMajorVersion,
                dwMinorVersion,
                dwBuild);


    //1.2 ќпределение системного каталога Ц функци€ GetSystemDirectory()

    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    GetSystemDirectory( infoBuf, INFO_BUFFER_SIZE );
    printf("\nSystem directory %s", infoBuf);


    //1.3 ќпределение названи€ компьютера и псевдонима текущего пользовател€ Ц функции GetComputerName(), GetUserName()

    DWORD  bufCharCount = INFO_BUFFER_SIZE;

    GetUserName( infoBuf, &bufCharCount);
    printf("\nUser name:%s", infoBuf);

    bufCharCount = INFO_BUFFER_SIZE;
    GetComputerName( infoBuf, &bufCharCount );
    printf("\nComputer name:        %s", infoBuf );


    //1.4
    char buffer[MAX_PATH];
    DWORD  cchBufferLength = MAX_PATH;
    char Names[MAX_PATH];
    __int64 total, available, free;


    HANDLE search = FindFirstVolume(buffer, sizeof(buffer));

    do {
        printf("\n%s", buffer);
        GetVolumePathNamesForVolumeName(buffer, Names, cchBufferLength, &cchBufferLength);
        printf("\n  First path: %s", Names);
        GetDiskFreeSpaceEx(
                           buffer,
                           (PULARGE_INTEGER)&available,
                           (PULARGE_INTEGER)&total,
                           (PULARGE_INTEGER)&free);

        if (GetDiskFreeSpaceEx(
                               buffer,
                               (PULARGE_INTEGER)&available,
                               (PULARGE_INTEGER)&total,
                               (PULARGE_INTEGER)&free)==0)
        {
            printf("\n  No information available.\n ");
        }
        else
        {
        printf("\n  Total size: %u  bytes ", total);
        printf("\n  Free space: %u  bytes\n", available);
        }

    } while (FindNextVolume(search, buffer, sizeof(buffer)));
    FindVolumeClose(search);

    printf("\n------------------\n");


    //1.5 —писок программ, запускаемых при старте системы, из реестра Windows

    HKEY hKey;
    LPCSTR lpSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    DWORD  ulOptions;
    REGSAM samDesired;
    PHKEY  phkResult;
    DWORD In = 0;

    char Progbuffer[MAX_PATH*4];
    DWORD Pblen = 32767;

    RegOpenKeyExA(HKEY_CURRENT_USER ,lpSubKey, 0, KEY_ALL_ACCESS, &hKey);

    while(RegEnumValueA(hKey, In, Progbuffer, &Pblen, 0, NULL, NULL, NULL)==ERROR_SUCCESS)
    {
        RegEnumValueA(hKey, In, Progbuffer, &Pblen, 0, NULL, NULL, NULL);
        printf("\n%s", Progbuffer);
        In++;
        printf("\n--------------------------------\n");
    }


    //2 ‘ункциональность изменени€ рабочей частоты ÷ѕ

    LARGE_INTEGER freq;
    LARGE_INTEGER t0;
    LARGE_INTEGER t;

    QueryPerformanceCounter(&t0);
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);

    double tact = t.QuadPart - t0.QuadPart;
    double frequency = freq.QuadPart;;
    double us = 1000000 * tact / frequency;

    printf("CPU Frequency = %u Hz\n",  freq);
    printf("CPU clock = %f us\n",  us);


}

