// minigit.h
#ifndef MINIGIT_H
#define MINIGIT_H

#include <chrono>           // For timestamp
#include <openssl/sha.h>    // For SHA-1 hashing
#include <iomanip>          // For hex formatting
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

 namespace fs = std::filesystem;

struct Blob
{
    std::string filename; // Original path
    std::string hash;     // SHA-1 of content
};

struct Commit
{
    std::string id; // Commit hash (SHA-1 of metadata + files)
    std::string message;
    std::string timestamp;
    std::string parent; // Single parent (extend to vector for merges later)
    std::vector<Blob> files;
};

class MiniGit
{
  public:
    // Core commands (PDF requirements)
    void init();
    void add(const std::string &filename);
    void commit(const std::string &message);
    void log();
    void branch(const std::string &name);
    void checkout(const std::string &ref);
    void merge(const std::string &branchName);
    void diff(const std::string& commitHash1, const std::string& commitHash2);

  private:
    // Internal utilities
    std::string getHeadCommit()
    {
        std::string branch = getCurrentBranch();
        std::ifstream branchFile(minigitDir + "refs_" + branch);
        std::string commitID;
        std::getline(branchFile, commitID);
        return commitID; // Returns "null" if empty
    }
    std::vector<std::string> getCommitHistory(const std::string &startCommit);
    std::string getCurrentBranch()
    {
        std::ifstream head(headFile);
        std::string ref;
        std::getline(head, ref);                      // Gets "ref: refs_master"
        return ref.substr(ref.find_last_of("_") + 1); // Extracts "master"
    }
    std::string hashObject(const std::string &content);
    std::string getCurrentHead();
    void updateRef(const std::string &ref, const std::string &commitId);
    void writeObject(const std::string &hash, const std::string &content);

    // Repository paths (const for safety)
    const fs::path repoDir = ".minigit";
    const fs::path objectsDir = repoDir / "objects";
    const fs::path refsDir = repoDir / "refs";
    const fs::path headPath = repoDir / "HEAD";
    const fs::path indexPath = repoDir / "index";

    // State
    std::unordered_map<std::string, std::string> stagingArea; // filename -> hash
};

#endif // MINIGIT_H