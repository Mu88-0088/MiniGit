// log.cpp
#include "minigit.h"
#include <iomanip>

void MiniGit::log() {
    // 1. Get HEAD commit
    std::string commitID = getHeadCommit();
    if (commitID.empty() || commitID == "null") {
        std::cout << "No commits yet\n";
        return;
    }

    // 2. Traverse history
    while (!commitID.empty() && commitID != "null") {
        // Read commit data
        std::ifstream commitFile(commitsDir + commitID);
        if (!commitFile) {
            std::cerr << "Warning: Broken commit chain at " << commitID << "\n";
            break;
        }

        // Parse and display
        std::cout << "\n\033[1;36mCommit:\033[0m " << commitID << "\n";  // Cyan header
        std::string line, nextCommitID;
        
        while (std::getline(commitFile, line)) {
            if (line.find("msg: ") == 0) {
                std::cout << "\033[1;33mMessage:\033[0m " << line.substr(5) << "\n";
            }
            else if (line.find("timestamp: ") == 0) {
                std::cout << "\033[1;32mDate:\033[0m   " << line.substr(11) << "\n";
            }
            else if (line.find("parent: ") == 0) {
                nextCommitID = line.substr(8);
            }
        }
        std::cout << std::string(30, '-') << "\n";  // Clean separator
        commitID = nextCommitID;
    }
}