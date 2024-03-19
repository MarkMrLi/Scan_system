#ifndef MYSTACK_H
#define MYSTACK_H

#include <cstddef>
#include "treenode.h"

class mystack
{
public:
    mystack();

    void push(const char *,TreeNode *);
    void pop();
    char *getit();
    void freestack();
    TreeNode *getnode();
    size_t getsize();


private:
    char **data;
    TreeNode **Nodearr;
    size_t size;
    size_t capacity;
};

#endif // MYSTACK_H
