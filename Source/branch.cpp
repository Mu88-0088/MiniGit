// branch.cpp
#include "minigit.h"

void MiniGit::branch(const std::string& name) {
    // 1. Validate branch name
    if (name.empty()) {
        std::cerr << "Error: Branch name cannot be empty\n";
        return;
    }

    // 2. Get current commit (check if repository is initialized)
    std::string currentCommit = getHeadCommit();
    if (currentCommit == "null") {
        std::cerr << "Error: Create at least one commit first\n";
        return;
    }

    // 3. Check for existing branch (Git-standard path)
    fs::path branchPath = refsDir / "heads" / name;
    if (fs::exists(branchPath)) {
        std::cerr << "Error: Branch '" << name << "' already exists\n";
        return;
    }

    // 4. Create branch reference
    std::ofstream(branchPath) << currentCommit;
    std::cout << "Created branch '" << name << "' (at commit " 
              << currentCommit.substr(0,7) << ")\n";
}}