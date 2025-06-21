#include "minigit.h"
#include <unordered_set>
#include <vector>

// Helper to get commit history (for LCA)
std::vector<std::string> getCommitHistory(const std::string &startCommit)
{
    std::vector<std::string> history;
    std::string commit = startCommit;
    while (commit != "null")
    {
        history.push_back(commit);
        std::ifstream f(commitsDir + commit);
        std::string line;
        while (std::getline(f, line))
        {
            if (line.find("parent: ") == 0)
            {
                commit = line.substr(8);
                break;
            }
        }
    }
    return history;
}

void MiniGit::merge(const std::string &branchName)
{
    // 1. Validate branch (your existing code)
    std::string targetRef = minigitDir + "refs_" + branchName;
    if (!fs::exists(targetRef))
    {
        std::cerr << "Error: Branch '" << branchName << "' doesn't exist\n";
        return;
    }

    // 2. Get commits (your existing code)
    std::string currentCommit = getHeadCommit();
    std::ifstream targetFile(targetRef);
    std::string targetCommit;
    targetFile >> targetCommit;

    if (currentCommit == targetCommit)
    {
        std::cerr << "Error: Already up-to-date\n";
        return;
    }

    // 3. Find LCA (improved version of your code)
    auto currentHistory = getCommitHistory(currentCommit);
    auto targetHistory = getCommitHistory(targetCommit);

    std::string lca;
    for (const auto &commit : currentHistory)
    {
        if (std::find(targetHistory.begin(), targetHistory.end(), commit) != targetHistory.end())
        {
            lca = commit;
            break;
        }
    }

    if (lca.empty())
    {
        std::cerr << "Error: No common ancestor\n";
        return;
    }
    std::cout << "Found LCA: " << lca << "\n";

#include "minigit.h"
#include <unordered_set>
#include <vector>

    // ... [keep existing getCommitHistory() helper] ...

    void MiniGit::merge(const std::string &branchName)
    {
        // ... [keep existing validation/LCA code until line 39] ...

        std::cout << "Found LCA: " << lca << "\n";

        // ====== START CONFLICT DETECTION ======
        auto getChangedFiles = [this](const std::string &fromCommit,
                                      const std::string &toCommit) {
            std::unordered_set<std::string> changedFiles;
            std::unordered_map<std::string, std::string> fromFiles, toFiles;

            // Helper to parse commit file listings
            auto parseCommitFiles = [this](const std::string &commit,
                                           auto &outputMap) {
                std::ifstream f(commitsDir + commit);
                std::string line;
                while (std::getline(f, line))
                {
                    if (line.find("parent: ") == 0)
                        continue;
                    size_t space = line.find(' ');
                    if (space != std::string::npos)
                    {
                        outputMap[line.substr(0, space)] = line.substr(space + 1);
                    }
                }
            };

            parseCommitFiles(fromCommit, fromFiles);
            parseCommitFiles(toCommit, toFiles);

            // Compare file versions
            for (const auto &[file, hash] : toFiles)
            {
                if (!fromFiles.count(file) || fromFiles[file] != hash)
                {
                    changedFiles.insert(file);
                }
            }
            return changedFiles;
        };

        // 4. Three-way merge with conflict detection
        std::vector<std::string> conflictingFiles;

        // Get files changed in current branch (since LCA)
        std::unordered_set<std::string> currentChangedFiles = getChangedFiles(lca, currentCommit);
        // Get files changed in target branch (since LCA)
        std::unordered_set<std::string> targetChangedFiles = getChangedFiles(lca, targetCommit);

        // Find files modified in both branches
        for (const auto &file : currentChangedFiles)
        {
            if (targetChangedFiles.count(file))
            {
                conflictingFiles.push_back(file);
            }
        }

        // Handle conflicts (PDF requirement)
        if (!conflictingFiles.empty())
        {
            for (const auto &file : conflictingFiles)
            {
                std::cerr << "CONFLICT: both modified " << file << "\n";
            }
            return;
        }
        // 5. Success message (your existing output)
        std::cout << "Merge successful (simulated)\n";
    }