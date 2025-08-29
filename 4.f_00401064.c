#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    DWORD wait_ms = 5000; // Wait up to 5 seconds
    DWORD exit_code = 0;

    if (argc < 2) {
        printf("Usage: %s <command_line_to_run>\n", argv[0]);
        return 1;
    }

    // Zero the structs and fill STARTUPINFOA.cb
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    // Combine the arguments into a single command string
    char cmd_line[1024] = {0};
    int i;
    for (i = 1; i < argc; ++i) {
        strcat(cmd_line, argv[i]);
        if (i != argc - 1) strcat(cmd_line, " ");
    }

    // Create the process (hidden window)
    BOOL success = CreateProcessA(
        NULL,           // Program (uses command line)
        cmd_line,       // Command line
        NULL, NULL,     // Security attributes
        FALSE,          // Inherit handles
        CREATE_NO_WINDOW, // No window shown
        NULL,           // Environment
        NULL,           // Current directory
        &si, &pi
    );

    if (!success) {
        printf("Failed to create process.\n");
        return 2;
    }

    // Optionally wait for process
    if (wait_ms > 0) {
        DWORD waitResult = WaitForSingleObject(pi.hProcess, wait_ms);
        if (waitResult != WAIT_OBJECT_0) {
            TerminateProcess(pi.hProcess, -1);
            printf("Process killed (timeout).\n");
            exit_code = -1;
        } else {
            if (GetExitCodeProcess(pi.hProcess, &exit_code))
                printf("Process exited with code: %lu\n", exit_code);
        }
    }

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return exit_code;
}



./f4.exe cmd.exe /c echo hello
