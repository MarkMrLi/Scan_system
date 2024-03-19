#ifndef TREENODE_H
#define TREENODE_H
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>

#define MAX_PATH_LENGTH 350
#define MAX_NAME_LENGTH 260
#define DIRR 0
#define FILEE 1

extern SYSTEMTIME defaultNodeTime ;
extern SYSTEMTIME defaultNodeTime2 ;
class TreeNode
{
public:
    TreeNode();
    bool tag;           //判断是文件还是目录，0表示目录，1表示文件
    char path[MAX_PATH_LENGTH];
    char name[MAX_NAME_LENGTH];
    long _Deepth;

    long file_size;       // 数据域
    SYSTEMTIME stLocal;    //时间
    TreeNode* firstChild;  // 指向第一个孩子的指针
    TreeNode* nextSibling; // 指向下一个兄弟的指针

    int dirdeep;
    long treedeep;
    // 构造函数
    TreeNode(bool tt,char *pa,long size = 0,SYSTEMTIME time = defaultNodeTime) : tag(tt),file_size(size) ,firstChild(nullptr), nextSibling(nullptr)
    {
        strncpy(path,pa,sizeof(path));
        this->stLocal = time;
        dirdeep = 0;
        treedeep = 0;
    }

    // 函数用于在树中查找目标目录
    TreeNode* findDirectory(TreeNode* currentNode, const char* targetPath);

    // 函数用于统计目录信息
    std::string countDirectoryInfo();

    // 辅助函数用于更新文件信息统计
    void updateFileInfo(const SYSTEMTIME& time, long size);

    // 析构函数，用于释放孩子和兄弟节点的内存
    ~TreeNode() {
        // if(tag == DIRR)
        // {
            // 递归释放孩子节点
            if (firstChild != nullptr) {
                delete firstChild;
            }
            // 释放兄弟节点
            if (nextSibling != nullptr) {
                delete nextSibling;
            }
        // }
    }
    // Function to print deleted nodes and their children to a file
    void printDeletedNodesToFile(std::ofstream& outputFile)
    {

        printDeletedNodesToFileHelper(this, outputFile);


    }
private:
    // SYSTEMTIME earliestTime;
    // SYSTEMTIME latestTime;
    int fileCount;

    // 辅助函数用于比较两个SYSTEMTIME结构的时间先后
    //bool isEarlier(const SYSTEMTIME& time1, const SYSTEMTIME& time2);



    // 辅助函数用于打印时间
    void printTime(const SYSTEMTIME& time);
    // Helper function for recursive traversal and printing
    static void printDeletedNodesToFileHelper(TreeNode* currentNode, std::ofstream& outputFile)
    {
        if (currentNode == nullptr)
            return;

        if (currentNode->tag == DIRR)
        {
            outputFile << "Deleted Directory: " << currentNode->path << " (" << currentNode->name << ")" << std::endl;
        }
        else
        {
            outputFile << "Deleted File: " << currentNode->path << " (" << currentNode->name << ")" << std::endl;
        }

        // Recursively traverse child nodes
        printDeletedNodesToFileHelper(currentNode->firstChild, outputFile);

        // Traverse next sibling nodes
        printDeletedNodesToFileHelper(currentNode->nextSibling, outputFile);
    }
};

#endif // TREENODE_H
