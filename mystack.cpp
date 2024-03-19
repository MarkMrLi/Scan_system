#include "mystack.h"
#include <qDebug.h>

mystack::mystack() {
    this->capacity = 1000000;
    this->data = (char**)malloc(this->capacity * sizeof(char*));
    this->Nodearr = (TreeNode**)malloc(this->capacity * sizeof(TreeNode*));
    this->size = 0;

}

void mystack::push(const char *value,TreeNode *Node){
    if (this->size < this->capacity) {
    this->data[this->getsize()] = _strdup(value); // 使用 _strdup 复制字符串
    this->Nodearr[this->getsize()] = Node;
    this->size++;
    } else {
    fprintf(stderr, "Stack overflow\n");
    exit(EXIT_FAILURE);
    }
}

void mystack::pop(){
        if(this->size>0)
        {
            this->size--;
        }
        else
        {
            qDebug()<<"stack is empty";
        }
}

char *mystack::getit(){
    if(this->size>0)
    {
        return this->data[this->size-1];
    }
    else
    {
        qDebug()<<"There is no item in stack";
    }
}

TreeNode *mystack::getnode(){
    if(this->size>0)
    {
        return this->Nodearr[this->size-1];
    }
    else
    {
        qDebug()<<"There is no item in stack";
    }
}
void mystack::freestack(){
    for(size_t i = 0; i< this->capacity;i++)
    {
        free(this->data[i]);
    }
    free(this->data);
}

size_t mystack::getsize(){
    return this->size;
}
