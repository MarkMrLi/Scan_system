#include "DirectorySimulator.h"

DirectorySimulator::DirectorySimulator(const std::string& filename,TreeNode *currentnode) {
    std::ifstream file(filename);
    std::string line;
    this->node = currentnode;

    while (std::getline(file, line) && line != "end of dirs") {
        if (line == "selected dirs") {
            while (std::getline(file, line) && line != "end of dirs") {
                std::istringstream iss(line);
                std::string token;

                std::getline(iss, token, ',');
                std::string fullPath = token;

                std::getline(iss, token, ',');
                char operation = token.empty() ? '\0' : token[0];

                std::getline(iss, token, ',');
                std::string time = token;

                std::getline(iss, token, ',');
                std::string size = token;

                // 获取去掉最后一个文件的目录路径
                size_t lastSlashPos = fullPath.find_last_of("\\");
                std::string parentDirectory = (lastSlashPos != std::string::npos) ? fullPath.substr(0, lastSlashPos) : "";
                lastSlashPos = parentDirectory.find_last_of("\\");
                parentDirectory = (lastSlashPos != std::string::npos) ? parentDirectory.substr(0, lastSlashPos) : "";
                directoryOperations.push_back({fullPath,parentDirectory, operation, time, size});
            }
        }
    }
}

void DirectorySimulator::executeDirectoryOperations(const char *filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
        return;
    }
    for (const auto& op : directoryOperations) {
        std::cout << "Operation: " << op.operation << " on directory: " << op.fullPath <<std::endl<<op.parentDirectory<< std::endl;
        // 执行目录操作的代码

        TreeNode *panode = node->findDirectory(node,op.parentDirectory.c_str());
        TreeNode *nownode = node->findDirectory(node,op.fullPath.c_str());

        if(nownode != nullptr)
        {
            outputFile << "Deleted Directory: " << nownode->path << " (" << nownode->name << ")" << std::endl;

            if(panode->firstChild == nownode)
            {
                panode->firstChild = nownode->nextSibling;
                nownode->nextSibling = nullptr;
                nownode->printDeletedNodesToFile(outputFile);
                delete nownode;
            }
            else
            {
                TreeNode *p = panode->firstChild;
                while(p->nextSibling != nownode)
                {

                    p = p->nextSibling;
                }
                printf("%s\n",p->name);printf("%s\n",p->nextSibling->name);
                fflush(stdout);
                p->nextSibling = nownode->nextSibling;
                nownode->nextSibling = nullptr;
                nownode->printDeletedNodesToFile(outputFile);
                delete nownode;
            }
            std::cout<<"Success!"<<std::endl;
        }

    }
    outputFile.close();
}
