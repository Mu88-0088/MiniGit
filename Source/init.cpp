#include "minigit.h"
#include <iostream>
#include <fstream>

void MiniGit::init() {
    // 1. Create repository root
    if (fs::exists(repoDir)) {
        std::cerr << "Error: MiniGit repository already exists\n";
        return;
    }
    fs::create_directory(repoDir);

    // 2. Create standard directories
    fs::create_directory(objectsDir);  // Stores all Git objects (blobs/commits)
    fs::create_directory(refsDir);     // Contains branch references
    fs::create_directory(refsDir / "heads");  // Branch pointers

    // 3. Initialize HEAD (points to master branch)
    std::ofstream(headPath) << "ref: refs/heads/master";

    // 4. Create empty staging area
    std::ofstream(indexPath).close();

    std::cout << "Initialized empty MiniGit repository in " << repoDir << "\n";
}