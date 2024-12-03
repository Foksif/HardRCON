#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <sstream>
#include <cstdlib>
#include <windows.h>

using namespace std;

const string RCON_HOST = "";
const int RCON_PORT = 2222;
const string RCON_PASSWORD = "";

std::string xorEncryptDecrypt(const std::string& input, char key) {
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] ^= key;
    }
    return output;
}

std::string encryptedHost = xorEncryptDecrypt(RCON_HOST, 'K');
std::string encryptedPassword = xorEncryptDecrypt(RCON_PASSWORD, 'K');

atomic<bool> isRunning(true);

string executeRconCommand(const string& command) {
    stringstream cmd;
    cmd << "mcrcon.exe -H " << encryptedHost
        << " -P " << RCON_PORT
        << " -p \"" << encryptedPassword
        << "\" \"" << command << "\"";

    FILE* pipe = _popen(cmd.str().c_str(), "r");
    if (!pipe) {
        cerr << "Failed to execute mcrcon command!" << endl;
        return "";
    }

    char buffer[128];
    string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    _pclose(pipe);

    return result;
}


int main() {
    SetConsoleTitle(TEXT("HardRCON | Vanila"));
    SetConsoleOutputCP(CP_UTF8);

    try {
        std::cout << R"(
+----------+
| HardRCON |
+----------+
    )" << std::endl;

        cout << "Connected to Vanila server via RCON." << endl;
        cout << "Type your commands below. Type 'exit' to quit." << endl;

        string userCommand;
        while (isRunning) {
            cout << "> ";
            getline(cin, userCommand);

            if (userCommand == "exit") {
                isRunning = false;
                break;
            }

            string response = executeRconCommand(userCommand);
            cout << "[Response]: " << response << endl;
        }

        cout << "Disconnected from server." << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
