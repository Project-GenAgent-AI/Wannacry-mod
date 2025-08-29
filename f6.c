#include <windows.h>
#include <stdio.h>

void generate_key_from_computer_name(char *outputBuffer, size_t bufferSize)
{
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD nameLength = sizeof(computerName) / sizeof(wchar_t);
    unsigned int seed = 1;
    size_t i, len;

    // Initialize output buffer safely
    if (bufferSize == 0) return;
    outputBuffer[0] = '\0';

    // Get computer name
    if (!GetComputerNameW(computerName, &nameLength)) {
        // If failed, fill output with empty string
        return;
    }
    computerName[nameLength] = L'\0'; // Null-terminate

    // Calculate seed based on computer name characters
    len = wcslen(computerName);
    for (i = 0; i < len; i++) {
        seed *= (unsigned int)computerName[i];
    }

    // Seed the C rand() function
    srand(seed);

    // Generate length for alphabetic characters (8 to 15)
    int alphaCount = (rand() % 8) + 8;

    // Generate lowercase alphabetic characters
    for (i = 0; i < (size_t)alphaCount && i < bufferSize - 1; i++) {
        outputBuffer[i] = (char)((rand() % 26) + 'a');
    }

    // Generate numeric characters for remainder (11 to ~18 chars)
    int numCount = (rand() % 8) + 11;
    for (; i < (size_t)(alphaCount + numCount) && i < bufferSize - 1; i++) {
        outputBuffer[i] = (char)((rand() % 10) + '0');
    }

    // Null-terminate string
    outputBuffer[i] = '\0';
}
int main()
{
    char key[64]; // Buffer to hold generated key

    generate_key_from_computer_name(key, sizeof(key));
    printf("Generated Key: %s\n", key);

    return 0;
}