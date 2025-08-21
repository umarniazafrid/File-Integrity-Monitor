#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>

namespace fs = std::filesystem;
using namespace CryptoPP;
using namespace std;

// global baseline map
map<string, string> baselineMap;

// calculateFileHash
string calculateFileHash(const string& filePath) {
    string digest;
    SHA256 hash;
    try {
        FileSource fileSource(
            filePath.c_str(),
            true,
            new HashFilter(
                hash,
                new HexEncoder(
                    new StringSink(digest),
                    false // lowercase hex
                )
            )
        );
    } catch (const exception& e) {
        cerr << "Error reading/hashing file: " << filePath << " - " << e.what() << endl;
        return "";
    }
    return digest;
}

// createBaseline
void createBaseline(const string& targetDirectory) {
    cout << "[+] Creating baseline for: " << targetDirectory << endl;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(targetDirectory)) {
            if (entry.is_regular_file()) {
                string filePath = entry.path().string();
                string fileHash = calculateFileHash(filePath);

                if (!fileHash.empty()) {
                    baselineMap[filePath] = fileHash;
                    cout << "  -> Hashed: " << filePath << endl;
                }
            }
        }
    } catch (const exception& e) {
        cerr << "[-] Error scanning directory: " << e.what() << endl;
        return;
    }

    ofstream baselineFile("baseline.txt");
    if (baselineFile.is_open()) {
        for (const auto& [filePath, fileHash] : baselineMap) {
            baselineFile << filePath << " : " << fileHash << endl;
        }
        baselineFile.close();
        cout << "[+] Baseline created successfully in 'baseline.txt'. "
             << baselineMap.size() << " files recorded." << endl;
    } else {
        cerr << "[-] Error: Could not open baseline.txt for writing!" << endl;
    }
}

// monitorChanges
void monitorChanges(const string& baselineFilePath) {
    cout << "[+] Monitoring started. Reading baseline..." << endl;

    ifstream baselineFile(baselineFilePath);
    if (!baselineFile.is_open()) {
        cerr << "[-] Error: Could not open baseline file: " << baselineFilePath << endl;
        return;
    }

    string line;
    while (getline(baselineFile, line)) {
        size_t delimiterPos = line.find(" : ");
        if (delimiterPos != string::npos) {
            string filePath = line.substr(0, delimiterPos);
            string originalHash = line.substr(delimiterPos + 3);
            baselineMap[filePath] = originalHash;
        }
    }
    baselineFile.close();

    cout << "[+] Baseline loaded. " << baselineMap.size() << " files to check." << endl;

    for (const auto& [filePath, originalHash] : baselineMap) {
        if (!fs::exists(filePath)) {
            cout << "\n[!] ALERT: File has been deleted! -> " << filePath << endl;
            continue;
        }

        string currentHash = calculateFileHash(filePath);

        if (currentHash.empty()) {
            cout << "\n[!] ALERT: Could not access/read file -> " << filePath << endl;
        } else if (currentHash != originalHash) {
            cout << "\n[!] ALERT: File has been modified! -> " << filePath << endl;
            cout << "    Old Hash: " << originalHash << endl;
            cout << "    New Hash: " << currentHash << endl;
        } else {
            cout << "." << flush;
        }
    }
    cout << "\n[+] Monitoring check complete." << endl;
}

// main
int main(int argc, char* argv[]) {
    cout << "\n=== Simple File Integrity Monitor (FIM) ===\n" << endl;

    if (argc < 3) {
        cerr << "Usage: " << endl;
        cerr << "  " << argv[0] << " -b <path_to_directory>  (Create Baseline)" << endl;
        cerr << "  " << argv[0] << " -c <path_to_baseline_file>  (Check for Changes)" << endl;
        return 1;
    }

    string mode = argv[1];
    string path = argv[2];

    if (mode == "-b") {
        createBaseline(path);
    } else if (mode == "-c") {
        monitorChanges(path);
    } else {
        cerr << "[-] Invalid mode. Use -b or -c." << endl;
        return 1;
    }

    return 0;
}
