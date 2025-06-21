#include "minigit.h"
#include <algorithm>

void MiniGit::diff(const std::string &commitHash1, const std::string &commitHash2)
{
    // 1. Validate commits
    fs::path commitFile1 = commitsDir + commitHash1;
    fs::path commitFile2 = commitsDir + commitHash2;
    if (!fs::exists(commitFile1) || !fs::exists(commitFile2))
    {
        std::cerr << "Error: Invalid commit hash(es)\n";
        return;
    }

    // 2. Get file lists from commits
    auto getFiles = [this](const fs::path &commitPath) {
        std::vector<std::pair<std::string, std::string>> files; // {filename, hash}
        std::ifstream f(commitPath);
        std::string line;
        while (std::getline(f, line))
        {
            if (line.find("parent: ") == 0)
                continue;
            size_t space = line.find(' ');
            if (space != std::string::npos)
            {
                files.emplace_back(line.substr(0, space), line.substr(space + 1));
            }
        }
        return files;
    };

    auto files1 = getFiles(commitFile1);
    auto files2 = getFiles(commitFile2);

    // 3. Compare files
    std::cout << "Diff between " << commitHash1.substr(0, 7) << " and "
              << commitHash2.substr(0, 7) << ":\n";

    for (const auto &[file, hash1] : files1)
    {
        auto it = std::find_if(files2.begin(), files2.end(),
                               [&file](const auto &p) { return p.first == file; });

        if (it == files2.end())
        {
            std::cout << "[DELETED] " << file << "\n";
        }
        else if (it->second != hash1)
        {
            std::cout << "[MODIFIED] " << file << "\n";
            // Optional: Add line-by-line diff here
        }
    }

    // Check for new files
    for (const auto &[file, hash2] : files2)
    {
        if (std::find_if(files1.begin(), files1.end(),
                         [&file](const auto &p) { return p.first == file; }) == files1.end())
        {
            std::cout << "[NEW] " << file << "\n";
        }
    }
}