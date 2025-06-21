// commit.cpp
#include "minigit.h"
#include <chrono>
#include <iomanip>
#include <openssl/sha.h>

// Generates a content-based ID while keeping your format
std::string MiniGit::generateCommitID() {
    static int counter = 1;
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()).count();
    
    std::string unique_str = "commit_" + std::to_string(counter++) + "_" + std::to_string(timestamp);
    
    // Hybrid approach: Add SHA-1 of content for uniqueness
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((const unsigned char*)unique_str.c_str(), unique_str.size(), hash);
    
    std::stringstream ss;
    ss << "custom_";
    for (int i = 0; i < 4; ++i) {  // Shorten hash for readability
        ss << std::hex << std::setw(2) << std::setfill('0') 
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

// Improved time handling (UTC)
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::gmtime(&time);  // UTC time
    
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S UTC", &tm);
    return std::string(buf);
}

// Safer branch reference reading
std::string MiniGit::getCurrentBranch() {
    std::ifstream head(headFile);
    if (!head) return "master";  // Default branch
    
    std::string ref;
    if (std::getline(head, ref)) {
        if (ref.find("ref: ") == 0) {
            return ref.substr(5);
        }
    }
    return "master";
}

// Commit implementation (preserves your logic)
void MiniGit::commit(const std::string& message) {
    // Verify staging file exists and has content
    std::ifstream stage(stagingFile);
    if (!stage || stage.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "[ERROR] Nothing to commit (staging area empty)\n";
        return;
    }

    // Generate ID
    std::string commitID = generateCommitID();
    std::ofstream commitFile(commitsDir + commitID);

    // Write metadata 
    commitFile << "msg: " << message << "\n"
               << "timestamp: " << getCurrentTime() << "\n"
               << "parent: " << getHeadCommit() << "\n";

    // Copy staged files
    std::string line;
    while (std::getline(stage, line)) {
        commitFile << line << "\n";
    }

    // Update references
    updateHead(commitID);
    std::ofstream(stagingFile, std::ios::trunc).close();  // Clear staging

    std::cout << "[DONE] Commit " << commitID << ": " << message << "\n";
}