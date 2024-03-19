#include "sqlcreator.h"

SqlCreator::SqlCreator() {}
char* replaceBackslashes(const char* input);
void SqlCreator::creatsql(TreeNode *root,const char *path)
{
    char dirpath[MAX_PATH_LENGTH],filepath[MAX_PATH_LENGTH];
    snprintf(dirpath,sizeof(dirpath),"%s\\sqldir.txt",path);
    snprintf(filepath,sizeof(filepath),"%s\\sqlfile.txt",path);
    FILE *Dir = fopen(dirpath,"w");
    FILE *File = fopen(filepath,"w");

    mystack ss;
    ss.push(path,root);

    while(ss.getsize())
    {
        TreeNode *nownode = ss.getnode();
        ss.pop();

        char *newpath;
        newpath = replaceBackslashes(nownode->path);
        //strncpy(newpath,nownode->path,strlen(nownode->path)-1);

        if(nownode->tag == DIRR)
        {
            TreeNode *p = nownode->firstChild;
            int dircount = 0;
            int filecount = 0;
            while(p != nullptr)
            {
                if(p->tag == DIRR)dircount++;
                if(p->tag == FILEE)filecount++;
                p = p->nextSibling;
            }

            fprintf(Dir,"INSERT INTO directories (name, path, subdirectories_count, files_count, depth)VALUES ('%s', '%s', %d, %d, %d);\n",\
                                                                                                                nownode->name,newpath,dircount,filecount,nownode->dirdeep);
        }
        else
        {
            fprintf(File,"INSERT INTO files (name, path, size, depth, last_modified)VALUES ('%s', '%s', %ld, %d, '%04d-%02d-%02d %02d:%02d:%02d');\n",\
            nownode->name,newpath,nownode->file_size,nownode->dirdeep,nownode->stLocal.wYear,nownode->stLocal.wMonth, nownode->stLocal.wDay,nownode->stLocal.wHour, nownode->stLocal.wMinute, nownode->stLocal.wSecond);
        }
        if(nownode->firstChild != nullptr)
            ss.push(path,nownode->firstChild);
        if(nownode->nextSibling != nullptr)
            ss.push(path,nownode->nextSibling);
    }

    // fprintf(Dir,"Damn");
    // fprintf(File,"Man");

    fclose(Dir);
    fclose(File);

}

// 函数实现
char* replaceBackslashes(const char* input) {
    // 计算新字符串所需的内存大小
    size_t inputLength = strlen(input);
    size_t newLength = 0;

    for (size_t i = 0; i < inputLength; i++) {
        if (input[i] == '\\') {
            newLength++;
        }
        newLength++;
    }

    // 分配新字符串的内存
    char* result = (char*)malloc(newLength + 1);

    if (!result) {
        // 内存分配失败
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // 替换并复制字符
    size_t j = 0;
    for (size_t i = 0; i < inputLength; i++) {
        if (input[i] == '\\') {
            result[j++] = '\\';
        }
        result[j++] = input[i];
    }

    // 在新字符串的末尾添加 null 终止符
    result[newLength] = '\0';

    return result;
}
