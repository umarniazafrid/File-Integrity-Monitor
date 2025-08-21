# File Integrity Monitor (FIM)

A lightweight, command-line File Integrity Monitor written in C++. This tool calculates and monitors the cryptographic hashes of files to detect unauthorized modifications, deletions, or additions—a critical practice in system security and compliance.

## Features

*   **Cryptographic Hashing:** Uses the SHA-256 algorithm via the robust Crypto++ library to generate unique file fingerprints.
*   **Baseline Creation:** Scans a target directory recursively and creates a secure baseline of all file hashes.
*   **Integrity Checking:** Compares the current state of files against the saved baseline to identify changes.
*   **Change Detection:** Alerts the user to:
    *   Modified files (hash mismatch)
    *   Deleted files (file not found)
    *   Files that cannot be accessed (permission errors)
*   **Cross-Platform:** Can be compiled and run on Linux, macOS, and Windows.

## How It Works

1.  **Baseline Mode (`-b`):** The tool traverses the specified directory, calculates the SHA-256 hash for each file, and stores the results (file path + hash) in a `baseline.txt` file.
2.  **Check Mode (`-c`):** The tool reads the `baseline.txt` file, recalculates the hashes for all recorded files, and compares them. Any discrepancies are reported immediately to the console.

## Prerequisites

*   A C++17 compatible compiler (GCC, Clang, or MSVC)
*   The Crypto++ library installed on your system.

### Installing Dependencies

**On Ubuntu/Debian:**
sudo apt update
sudo apt install libcrypto++-dev build-essential

**On macOS (using Homebrew):**
```
brew install cryptopp
```

**On Windows:**
1.  Download the Crypto++ library from the [official website](https://www.cryptopp.com/).
2.  Build it following their instructions.
3.  Link the built libraries to your project in Visual Studio.

## Building the Project

1.  Clone or download the `fim.cpp` source file.
2.  Navigate to the directory containing the source code.
3.  Compile the program using the following command:

   **Linux/macOS:**
    ```
    g++ -std=c++17 -o fim fim.cpp -lcryptopp
    ```

    **Windows (Visual Studio Developer Command Prompt):**
    ```
    cl /EHsc fim.cpp /link cryptlib.lib
    ```
    *(Ensure your compiler can find the Crypto++ include and lib directories)*.

## Usage

The program has two primary modes of operation.

### 1. Creating a Baseline

To create an initial baseline of a directory, use the `-b` flag followed by the path to the directory you want to monitor.

```
# Example: Create a baseline of the /etc directory (may require sudo)
sudo ./fim -b /etc

# Example: Create a baseline of a test directory
./fim -b ./my_important_files
```
This will generate a `baseline.txt` file in the current working directory.

### 2. Checking for Changes

To check the integrity of the files against the saved baseline, use the `-c` flag followed by the path to the baseline file.

```
# Check against the previously created baseline
./fim -c baseline.txt
```

The output will display a series of dots (`.`) for unchanged files and clear alerts for any changes found.

### Example Output
```
[+] Monitoring started. Reading baseline...
[+] Baseline loaded. 152 files to check.
.
.[!] ALERT: File has been modified! -> /etc/hosts
    Old Hash: a1b2c3d4e5f67890...
    New Hash: f0e1d2c3b4a59687...
[!] ALERT: File has been deleted! -> /etc/old_config.conf
[+] Monitoring check complete.
```

## Project Structure

*   `fim.cpp` - The main source file containing all the code.
*   `baseline.txt` - Generated output file containing file paths and their hashes. **(Keep this file secure!)**

## Limitations & Enhancements

This is a foundational implementation. For a production environment, consider adding:

*   **Daemon Mode:** Run continuously in the background, checking files at set intervals.
*   **Secure Logging:** Write alerts with timestamps to a log file (`fim.log`).
*   **Configuration File:** Use a config file (e.g., JSON, YAML) to specify directories, exclude patterns, and set options.
*   **Database Storage:** Use SQLite instead of a text file for handling large numbers of files more efficiently.
*   **Alerting:** Integrate with email, SMS, or system messaging APIs to send immediate notifications on change detection.
*   **Digital Signatures:** Sign the baseline file to prevent tampering.

## Security Note

The `baseline.txt` file is a critical asset. If an attacker can modify it, they can hide their changes. Store it in a secure, read-only location outside of the monitored directories.

## License

This project is provided for educational purposes. Feel free to use it as a starting point for your own projects.

## Contributing

Contributions are welcome! Please feel free to fork the repository and submit pull requests with any enhancements or bug fixes.
