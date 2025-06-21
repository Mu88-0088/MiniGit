// init.cpp
// Implements the `init` command for MiniGit

#include "minigit.h"

// Constructor (can be extended if needed)
MiniGit::MiniGit() {
    // Empty for now; init logic is in the init() method
}

// Initializes the MiniGit repository
void MiniGit::init() {
    // Create the main .minigit directory if it doesn't exist
    if (!fs::exists(minigitDir)) fs::create_directory(minigitDir);

    // Create subdirectories for commits and file objects
    if (!fs::exists(objectsDir)) fs::create_directory(objectsDir);
    if (!fs::exists(commitsDir)) fs::create_directory(commitsDir);

    // Initialize HEAD to point to master branch
    std::ofstream head(headFile);
    head << "ref: master";

    // Create initial ref for master pointing to null (no commits yet)
    std::ofstream master(minigitDir + "refs_master");
    master << "null";

    // Empty staging area
    std::ofstream stage(stagingFile);
    stage << "";

    std::cout << "Initialized empty MiniGit repository in .minigit/\n";
}
