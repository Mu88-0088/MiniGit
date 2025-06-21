// add.cpp
#include "minigit.h"
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>

// Improved hashing using SHA-1 (Git standard)
std::string MiniGit::computeHash(const std::string& content) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(content.c_str()), 
         content.size(), 
         hash);

    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') 
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

void MiniGit::add(const std::string& filename) {
    // 1. Open file in binary mode (cross-platform safety)
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "[ERROR] Cannot open file: " << filename << "\n";
        return;
    }

    // 2. Read content (binary-safe)
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // 3. Compute Git-compatible hash
    std::string hash = computeHash(content);

    // 4. Store in Git-like object structure
    fs::path objDir = objectsDir / hash.substr(0,2);
    fs::create_directory(objDir);
    std::string objectPath = (objDir / hash.substr(2)).string();

    if (!fs::exists(objectPath)) {
        std::ofstream out(objectPath, std::ios::binary);
        out << content;
    }

    // 5. Update staging file (keeping your original approach)
    std::ofstream stage(stagingFile, std::ios::app);
    stage << filename << " " << hash << "\n";

    std::cout << "[INFO] Staged: " << filename << " (" << hash.substr(0,7) << ")\n";
}