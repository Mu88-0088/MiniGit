#include "minigit.h"
#include <fstream>

void MiniGit::checkout(const std::string& target) {
    // 1. Check if target is a branch (refs_<branch> exists)
    std::string branchPath = minigitDir + "refs_" + target;
    if (fs::exists(branchPath)) {
        // Update HEAD to point to the branch
        std::ofstream head(headFile);
        head << "ref: refs_" << target;
        std::cout << "Switched to branch '" << target << "'\n";
        return;
    }

    // 2. Check if target is a commit ID (exists in commitsDir)
    std::string commitPath = commitsDir + target;
    if (fs::exists(commitPath)) {
        // Detach HEAD to point directly to the commit
        std::ofstream head(headFile);
        head << target;
        std::cout << "Detached HEAD at commit " << target << "\n";
        return;
    }

    // 3. Error if target is invalid
    std::cerr << "Error: No branch or commit named '" << target << "'\n";
}