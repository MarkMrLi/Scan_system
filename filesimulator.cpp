#include "FileSimulator.h"

void ConvertUnixTimeToSystemTime(long int unixTime, SYSTEMTIME& systemTime);
// 构造函数
FileSimulator::FileSimulator(const std::string& filename,TreeNode *root) : filename(filename) {
    this->root = root;
    readAndParseFile();
}

// 读取文件内容并解析操作
void FileSimulator::readAndParseFile() {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;




    // 读取文件内容
    while (std::getline(file, line) && line != "end of files") {
        // 检查是否是第一行，如果是，则跳过
        if (line == "selected files") {
            continue;
        }
        std::istringstream iss(line);
        std::string token;


        // 使用 std::getline 以逗号分隔读取每个值
        std::getline(iss, token, ',');
        std::string fullPath = token;

        std::getline(iss, token, ',');
        char operation = token.empty() ? '\0' : token[0];

        std::getline(iss, token, ',');
        //std::cout<<token;


        long int time = (operation == 'D')?0:std::stol(token);

        std::getline(iss, token, ',');
        long int size = (operation == 'D')?0:std::stol(token);

        // 获取去掉最后一个文件的目录路径
        size_t lastSlashPos = fullPath.find_last_of("\\");
        std::string parentDirectory = (lastSlashPos != std::string::npos) ? fullPath.substr(0, lastSlashPos) : "";

        // 存储文件操作信息
        operations.push_back({fullPath, parentDirectory, operation, time, size});
    }
}

// 模拟文件操作
void FileSimulator::executeFileOperations() {
    for (const auto& op : operations) {
        std::cout << "Executing operation: " << op.operation << " on file: " << op.fullPath << std::endl;

        TreeNode *panode = root->findDirectory(root,op.parentDirectory.c_str());
        TreeNode *nownode = root->findDirectory(root,op.fullPath.c_str());
        SYSTEMTIME systemTime;
        TreeNode* newnode = nullptr;  // Declare the pointer outside the switch

        // 根据操作类型执行相应操作

        switch (op.operation) {
        case 'D':
            if(nownode == nullptr)break;
            std::cout << "Deleting file." << std::endl;

            if(panode->firstChild == nownode)
            {
                panode->firstChild = nownode->nextSibling;
                nownode->nextSibling = nullptr;
                delete nownode;
            }
            else
            {
                TreeNode *p = panode->firstChild;
                while(p->nextSibling != nownode)
                {

                    p = p->nextSibling;
                }
                //printf("%s\n",p->name);printf("%s\n",p->nextSibling->name);
                //fflush(stdout);
                p->nextSibling = nownode->nextSibling;
                nownode->nextSibling = nullptr;
                delete nownode;
            }
            // 执行删除文件的操作
            break;
        case 'M':
            if(nownode == nullptr)break;
            std::cout << "Modifying time and size." << std::endl;


            ConvertUnixTimeToSystemTime(op.time, systemTime);

            nownode->updateFileInfo(systemTime,op.size);

            // 执行修改时间和大小的操作
            break;
        case 'A':
            if(panode == nullptr)break;
            if(nownode)break;
            std::cout << "Adding new file." << std::endl;

            ConvertUnixTimeToSystemTime(op.time, systemTime);
            newnode = new TreeNode(FILEE, const_cast<char*>(op.fullPath.c_str()), op.size, systemTime);
            if(panode->firstChild != nullptr)
            {
                panode = panode->firstChild;
                while(panode->nextSibling != nullptr)
                {
                    panode = panode->nextSibling;
                }
                panode->nextSibling = newnode;
            }
            else
            {
                panode->firstChild = newnode;
            }

            // 执行新增文件的操作
            break;
        default:
            std::cerr << "Unknown operation: " << op.operation << std::endl;
        }
    }
}

// 执行文件模拟操作
void FileSimulator::simulateFileOperations() {
    executeFileOperations();
}

void ConvertUnixTimeToSystemTime(long int unixTime, SYSTEMTIME& systemTime) {
    FILETIME fileTime;

    // Convert Unix time to FILETIME
    fileTime.dwLowDateTime = (DWORD)(unixTime & 0xFFFFFFFF);
    fileTime.dwHighDateTime = (DWORD)((unixTime >> 32) & 0xFFFFFFFF);

    // Convert FILETIME to SYSTEMTIME
    FileTimeToSystemTime(&fileTime, &systemTime);
}
