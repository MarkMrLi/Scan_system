#ifndef SQLCREATOR_H
#define SQLCREATOR_H
#include "treenode.h"
#include "stdio.h"
#include "mystack.h"

#define MAX_SQL_LENGTH 500

class SqlCreator
{
public:
    SqlCreator();
    void creatsql(TreeNode *root,const char *path);
};

#endif // SQLCREATOR_H
