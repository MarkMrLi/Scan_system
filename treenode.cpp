#include "treenode.h"
#include <cstdio>
SYSTEMTIME defaultNodeTime = {2000, 0, 0, 0, 0, 0, 0, 0};
SYSTEMTIME defaultNodeTime2 = {2030, 0, 0, 0, 0, 0, 0, 0};
char* getNextDirectoryName(const char* path, size_t& startPos);
bool isEarlier(const SYSTEMTIME time1, const SYSTEMTIME time2);
TreeNode::TreeNode(){}

// 函数用于在树中查找目标目录
TreeNode* TreeNode::findDirectory(TreeNode* currentNode, const char* targetPath) {
    size_t startPos = 3; // 从路径的起始位置开始查找
    //getNextDirectoryName(targetPath, startPos);
    while (currentNode != nullptr) {
        // 获取下一个目录名

        char* nextDirectoryName = getNextDirectoryName(targetPath, startPos);
        //printf("%s\n",nextDirectoryName);

        // 在当前节点的孩子中查找目录名
        while (currentNode != nullptr) {
            if (stricmp(currentNode->name, nextDirectoryName) == 0) {

                // 如果目录名匹配，则返回当前节点
                if (startPos == strlen(targetPath)) {
                    delete[] nextDirectoryName;
                    return currentNode;
                }
                // 目录名匹配，移动到下一个目录名并继续查找
                currentNode = currentNode->firstChild;

                break;
            }
            // 目录名不匹配，继续在兄弟节点中查找
            currentNode = currentNode->nextSibling;
        }



        // 释放目录名的内存
        delete[] nextDirectoryName;

    }

    return nullptr; // 未找到目标目录
}

char* getNextDirectoryName(const char* path, size_t& startPos) {
    size_t length = strlen(path);
    size_t endPos = startPos;

    while (endPos < length && path[endPos] != '\\') {
        endPos++;
    }



    // 获取目录名的长度
    size_t directoryNameLength = endPos - startPos;

    // 分配内存并复制目录名
    char* directoryName = new char[directoryNameLength + 1];
    strncpy(directoryName, path + startPos, directoryNameLength);
    directoryName[directoryNameLength] = '\0';

    // 更新起始位置，跳过目录分隔符
    startPos = (endPos == length) ? endPos : endPos + 1;
    //startPos = endPos + 1;
    return directoryName;
}

std::string TreeNode::countDirectoryInfo() {
    SYSTEMTIME earliestTime = defaultNodeTime2;
    SYSTEMTIME latestTime = defaultNodeTime;
    long totalSize = 0;
    int fileCount = 0;

    TreeNode* currentNode = this->firstChild;

    while (currentNode != nullptr) {
        if (currentNode->tag == FILEE) { // 判断是否为文件
            // 更新最早和最晚时间

            if (isEarlier(currentNode->stLocal, earliestTime)) {
                earliestTime = currentNode->stLocal;

            }


            if (isEarlier( latestTime,currentNode->stLocal)) {
                latestTime = currentNode->stLocal;

            }

            // 更新总文件大小和文件数
            totalSize += currentNode->file_size;
            fileCount++;
        }

        currentNode = currentNode->nextSibling;
    }

    // 构造统计信息字符串
    std::string result;
    result += "Directory Path: " + std::string(this->path) + "\n";
    result += "Earliest Time: ";
    result += std::to_string(earliestTime.wYear) + "-" + std::to_string(earliestTime.wMonth) + "-" + std::to_string(earliestTime.wDay);
    result += " " + std::to_string(earliestTime.wHour) + ":" + std::to_string(earliestTime.wMinute) + ":" + std::to_string(earliestTime.wSecond) + "\n";
    result += "Latest Time: ";
    result += std::to_string(latestTime.wYear) + "-" + std::to_string(latestTime.wMonth) + "-" + std::to_string(latestTime.wDay);
    result += " " + std::to_string(latestTime.wHour) + ":" + std::to_string(latestTime.wMinute) + ":" + std::to_string(latestTime.wSecond) + "\n";
    result += "Total Files: " + std::to_string(fileCount) + "\n";
    result += "Total Size: " + std::to_string(totalSize) + " bytes\n";

    return result;
}

bool isEarlier(const SYSTEMTIME time1, const SYSTEMTIME time2) {

    int re = memcmp(&time1,&time2,sizeof(SYSTEMTIME));
    if(re > 0)return false;
    else
        return true;

}

void TreeNode::updateFileInfo(const SYSTEMTIME& time, long size) {
    // Update earliest and latest times
    this->stLocal = time;
    this->file_size = size;
}

void TreeNode::printTime(const SYSTEMTIME& time) {
    std::cout << time.wYear << "-" << time.wMonth << "-" << time.wDay;
    std::cout << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << std::endl;
}
