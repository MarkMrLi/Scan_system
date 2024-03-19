#include "scan.h"

struct FileInfo {
    char name[MAX_PATH_LENGTH]; // 文件或目录的名称
    time_t lastModified; // 最后修改时间，以秒为单位
    SYSTEMTIME  stLocal;
    long fileSize; // 文件大小，以字节为单位
};
scan::scan(char *pa,Widget *widgetInstance) {this->path = pa;this->widget = widgetInstance;}

void scan::traverseDirectory(TreeNode *root) {
    //

    //一些数据的记录
    int maxdirdeep = 0;
    long maxtreedeep = 0;
    long dirnum = 0;
    long filenum = 0;
    char max_lenth_name[MAX_PATH_LENGTH];

    WIN32_FIND_DATAW findFileData;
    // HANDLE hFind = FindFirstFileW((LPCWSTR)dirPath, &findFileData);
    HANDLE hFind;

    // // 将窄字符转换为宽字符
    wchar_t wPath[MAX_PATH_LENGTH];

    // 使用动态分配的栈

    ss.push(this->path,root);

    while (ss.getsize() > 0) {


        char currentPath[MAX_PATH_LENGTH];
        char pwd[MAX_PATH_LENGTH];
        TreeNode *nownode = ss.getnode();
        if(nownode == root)qDebug()<<"Hello";

        strcpy(currentPath, ss.getit());
        strcpy(pwd,currentPath);
        strcat(pwd,"*");
        //使用通配符匹配目录中的所有文件

        // 转化为宽字符
        MultiByteToWideChar(CP_UTF8, 0, pwd, -1, wPath, MAX_PATH_LENGTH);
        ss.pop();  // 弹出当前目录

        bool flag = true;

        if ((hFind = FindFirstFileW(wPath, &findFileData)) != INVALID_HANDLE_VALUE) {
            do {
                char filePath[MAX_PATH_LENGTH];
                char filename[MAX_NAME_LENGTH];
                snprintf(filePath, sizeof(filePath), "%s%ls\\", currentPath, findFileData.cFileName);
                snprintf(filename,sizeof(filename),"%ls",findFileData.cFileName);

                // printf("%s\n",filePath);

                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    // 如果是子目录，入栈
                    if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
                        dirnum++;
                        if(strlen(filePath) > strlen(max_lenth_name))
                        {
                            strncpy(max_lenth_name,filePath,sizeof(filePath));
                        }

                        //将目录信息写入目录树
                        TreeNode *node = new TreeNode(DIRR,filePath);
                        strcpy(node->name,filename);

                        if(flag)
                        {
                            node->dirdeep = nownode->dirdeep + 1;
                            node->treedeep = nownode->treedeep + 1;
                            maxtreedeep = node->treedeep > maxtreedeep?node->treedeep : maxtreedeep;
                            maxdirdeep = node->dirdeep > maxdirdeep ? node->dirdeep : maxdirdeep;
                            nownode->firstChild = node;
                            flag = false;
                        }
                        else
                        {
                            node->dirdeep = nownode->dirdeep;
                            node->treedeep = nownode->treedeep + 1;
                            maxtreedeep = node->treedeep > maxtreedeep?node->treedeep : maxtreedeep;
                            maxdirdeep = node->dirdeep > maxdirdeep ? node->dirdeep : maxdirdeep;
                            nownode->nextSibling = node;
                        }
                        ss.push(filePath,node);
                        nownode = node;
                    }
                } else {
                    // 处理文件
                    struct FileInfo fileInfo;
                    strncpy(fileInfo.name, filePath, sizeof(fileInfo.name));
                    FILETIME localfiletime;
                    FileTimeToLocalFileTime(&findFileData.ftLastWriteTime,&localfiletime);
                    FileTimeToSystemTime(&localfiletime,&fileInfo.stLocal);
                    //fileInfo.lastModified = (time_t)((findFileData.ftLastWriteTime.dwHighDateTime) << 32 | findFileData.ftLastWriteTime.dwLowDateTime);
                    fileInfo.fileSize = (long)findFileData.nFileSizeLow;

                    filenum++;
                    if(strlen(filePath) > strlen(max_lenth_name))
                    {
                        strncpy(max_lenth_name,filePath,sizeof(filePath));
                    }


                    //将文件信息写入目录树
                    TreeNode *node = new TreeNode(FILEE,filePath,fileInfo.fileSize,fileInfo.stLocal);
                    strcpy(node->name,filename);
                    if(flag)
                    {
                        node->dirdeep = nownode->dirdeep + 1;
                        node->treedeep = nownode->treedeep + 1;
                        maxtreedeep = node->treedeep > maxtreedeep?node->treedeep : maxtreedeep;
                        maxdirdeep = node->dirdeep > maxdirdeep ? node->dirdeep : maxdirdeep;
                        nownode->firstChild = node;
                        flag = false;
                    }
                    else
                    {
                        node->dirdeep = nownode->dirdeep ;
                        node->treedeep = nownode->treedeep + 1;
                        maxtreedeep = node->treedeep > maxtreedeep?node->treedeep : maxtreedeep;
                        maxdirdeep = node->dirdeep > maxdirdeep ? node->dirdeep : maxdirdeep;
                        nownode->nextSibling = node;
                    }
                    nownode = node;
                }

            } while (FindNextFileW(hFind, &findFileData) != 0);

            FindClose(hFind);



        }
    }

    char info[100000];
    snprintf(info, sizeof(info), "Dir Deepth:%d\n\nTree Deepth:%ld\n\nDir Num:%ld\n\nFiles Num:%ld\n\nLongest FileName:\n%s\n\nLenth:%d",maxdirdeep,maxtreedeep,dirnum,filenum,max_lenth_name,strlen(max_lenth_name));

    // 发射信号，更新 textBrowser
    emit widget->updateTextBrowser(info);

    // 处理未处理的事件，让UI界面刷新
    QCoreApplication::processEvents();
    // 释放栈的内存
    ss.freestack();
}
