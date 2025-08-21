#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <string>

// alias for filesystem namespace
namespace fs = std::filesystem;

// global baseline map
std::map<std::string, std::string> baselineMap;

// forward declaration for the hashing function
std::string calculateFileHash(const std::string& filePath);

void monitorChanges(const std::string& baselineFilePath) {
    std::cout << "[+] Monitoring started. Reading baseline..." << std::endl;

    std::ifstream baselineFile(baselineFilePath);
    if (!baselineFile.is_open()) {
        std::cerr << "[-] Error: Could not open baseline file: " 
                  << baselineFilePath << std::endl;
        return;
    }

    // Read baseline file into map
    std::string line;
    while (std::getline(baselineFile, line)) {
        size_t delimiterPos = line.find(" : ");
        if (delimiterPos != std::string::npos) {
            std::string filePath = line.substr(0, delimiterPos);
            std::string originalHash = line.substr(delimiterPos + 3);
            baselineMap[filePath] = originalHash;
        }
    }
    baselineFile.close();

    std::cout << "[+] Baseline loaded. " << baselineMap.size() << " files to check." << std::endl;

    // Check each file in baseline
    for (const auto& [filePath, originalHash] : baselineMap) {
        if (!fs::exists(filePath)) {
            std::cout << "\n[!] ALERT: File has been deleted! -> " << filePath << std::endl;
            continue;
        }

        std::string currentHash = calculateFileHash(filePath);

        if (currentHash.empty()) {
            std::cout << "\n[!] ALERT: Could not access/read file -> " << filePath << std::endl;
        } 
        else if (currentHash != originalHash) {
            std::cout << "\n[!] ALERT: File has been modified! -> " << filePath << std::endl;
            std::cout << "    Old Hash: " << originalHash << std::endl;
            std::cout << "    New Hash: " << currentHash << std::endl;
        } 
        else {
            // file unchanged → print a progress dot
            std::cout << "." << std::flush;
        }
    }
    std::cout << "\n[+] Monitoring check complete." << std::endl;
}
