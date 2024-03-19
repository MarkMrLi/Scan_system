#ifndef DIRECTORY_SIMULATOR_H
#define DIRECTORY_SIMULATOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "treenode.h"

class DirectoryOperation {
public:
    std::string fullPath;
    std::string parentDirectory;
    char operation;
    std::string time;
    std::string size;
};

class DirectorySimulator {
private:
    std::vector<DirectoryOperation> directoryOperations;
    TreeNode *node;

public:
    DirectorySimulator(const std::string& filename,TreeNode *currentnode);
    void executeDirectoryOperations(const char *);
};

#endif // DIRECTORY_SIMULATOR_H
