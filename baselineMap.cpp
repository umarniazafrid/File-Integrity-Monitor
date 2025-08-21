#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <string>

// alias for filesystem namespace
namespace fs = std::filesystem;

// global baseline map (filePath -> hash)
std::map<std::string, std::string> baselineMap;

// forward declaration of the hashing function
std::string calculateFileHash(const std::string& filePath);

void createBaseline(const std::string& targetDirectory) {
    std::cout << "[+] Creating baseline for: " << targetDirectory << std::endl;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(targetDirectory)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                std::string fileHash = calculateFileHash(filePath);

                if (!fileHash.empty()) { // Only add if hash was successful
                    baselineMap[filePath] = fileHash;
                    std::cout << "  -> Hashed: " << filePath << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[-] Error scanning directory: " << e.what() << std::endl;
        return;
    }

    // save baseline to file
    std::ofstream baselineFile("baseline.txt");
    if (baselineFile.is_open()) {
        for (const auto& [filePath, fileHash] : baselineMap) {
            baselineFile << filePath << " : " << fileHash << std::endl;
        }
        baselineFile.close();
        std::cout << "[+] Baseline created successfully in 'baseline.txt'. "
                  << baselineMap.size() << " files recorded." << std::endl;
    } else {
        std::cerr << "[-] Error: Could not open baseline.txt for writing!" << std::endl;
    }
}
