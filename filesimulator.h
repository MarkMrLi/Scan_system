#ifndef FILE_SIMULATOR_H
#define FILE_SIMULATOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "treenode.h"

struct FileOperation {
    std::string fullPath;
    std::string parentDirectory;
    char operation;
    long int time;
    long int size;
};

class FileSimulator {
public:
    // 构造函数，接受文件名
    FileSimulator(const std::string& filename,TreeNode *root);

    // 执行文件模拟操作
    void simulateFileOperations();

private:
    // 读取文件内容并解析操作
    void readAndParseFile();

    // 模拟文件操作
    void executeFileOperations();

private:
    std::string filename;
    std::vector<FileOperation> operations;
    TreeNode *root;
};

#endif // FILE_SIMULATOR_H
