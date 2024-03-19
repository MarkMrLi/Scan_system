#ifndef SCAN_H
#define SCAN_H

#include "mystack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "treenode.h"
#include "widget.h"
#include <windows.h>
#include <sys/stat.h>
#include <time.h>
#include <QCoreApplication>
using namespace std;
#define MAX_PATH_LENGTH 350


class scan
{
public:
    scan(char *,Widget *widgetInstance);

    void traverseDirectory(TreeNode *);
    mystack ss;
    char *path;
private:
    Widget *widget;



};

#endif // SCAN_H
