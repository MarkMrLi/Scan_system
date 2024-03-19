#include "widget.h"
#include "./ui_widget.h"
#include <qmessagebox.h>
#include "scan.h"
#include <QDir>
#include "treenode.h"
#include "sqlcreator.h"
#include "filesimulator.h"

char *desdir = "C:\\Windows\\";
std::vector<std::string> results1;  // Vector to store each result
std::vector<std::string> results2;  // Vector to store each result
TreeNode *root = new TreeNode(DIRR,desdir);  //目录树根节点

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QMessageBox eixtt;

    eixtt.information(this,"拜拜","感谢使用");


    delete root;
    this->close();


}

void Widget::updateTextBrowser(const QString &text)
{
    ui->textBrowser->setText(text);
}

void Widget::on_pushButton_2_clicked()
{
    QMessageBox info;

    scan traverse(desdir,this);
    strcpy(root->name , "Windows");

    ui->textBrowser->setText("正在扫描中...");
    QCoreApplication::processEvents();  // 处理未处理的事件，让UI界面刷新

    traverse.traverseDirectory(root);
    info.information(this,"提示","扫描完成");

    //this->close();
}




void Widget::on_pushButton_3_clicked()
{
    QMessageBox info;
    SqlCreator sql;
    //qDebug()<<root->firstChild->path;
    const char *path;
    if(ui->sqlpath->text().isEmpty()) {
        path = "D:\\"; // 用你希望的默认值替代 "默认值"
    } else {
        path = qPrintable(ui->sqlpath->text());
    }

    ui->textBrowser->setText("正在生成中...");
    QCoreApplication::processEvents();
    sql.creatsql(root,path);
    std::string aa;
    aa = "已将文件存放到:\n" + std::string(path) + "\\sqldir.txt\n" + std::string(path) + "\\sqlfile.txt";
    ui->textBrowser_2->setText(QString::fromStdString(aa));

    info.information(this,"提示","扫描完成");
    qDebug()<<ui->sqlpath->text();

}


void Widget::on_pushButton_4_clicked()
{
    // 假设要查找的目标目录路径为 "c:\\windows\\UUS\\x86\\"
    //const char* targetDirectory = "c:\\windows\\UUS\\";
    const char *targetDirectory;
    if (ui->lineEdit->text().isEmpty()) {
        // 如果lineEdit的文本为空，使用默认值
        targetDirectory = "c:\\windows\\UUS";
    } else {
        // 使用lineEdit的文本作为目标目录
        targetDirectory = qPrintable(ui->lineEdit->text());
    }


    //TreeNode*
    // 调用查找函数
    TreeNode* foundNode = root->findDirectory(root, targetDirectory);

    // 打印结果
    if (foundNode != nullptr) {

        // 调用countDirectoryInfo，并将结果输出到ui->textBrowser
        QString result = QString::fromStdString(foundNode->countDirectoryInfo());
        ui->textBrowser_3->setText(result);
        fflush(stdout);  // 刷新标准输出缓冲区
    } else {
        printf("False!\n");
        fflush(stdout);  // 刷新标准输出缓冲区
        //qDebug() << "Target directory not found." ;
    }
}


void Widget::on_pushButton_5_clicked()
{
    // 获取输入文件路径和输出文件路径
    std::string inputFile = ui->lineEdit_2->text().isEmpty() ? "D:\\mystat.txt" : ui->lineEdit_2->text().toStdString();
    std::string outputFile = ui->lineEdit_3->text().isEmpty() ? "D:\\output.txt" : ui->lineEdit_3->text().toStdString();


    // 打开输入文件
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return;
    }

    // 打开输出文件
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        inFile.close(); // 关闭输入文件
        return;
    }

    // 读取文件内容
    bool inDirsSection = false;
    std::string line;
    while (std::getline(inFile, line)) {
        if (line == "stat dirs") {
            inDirsSection = true;
        } else if (line == "end of dirs") {
            break;  // 结束循环，不再读取目录
        } else if (inDirsSection) {
            const char* targetDir = line.c_str();
            printf("Target Directory: %s\n", targetDir);

            // 调用查找函数
            TreeNode* foundNode = root->findDirectory(root, targetDir);

            // 打印结果并写入输出文件
            if (foundNode != nullptr) {
                std::string result = foundNode->countDirectoryInfo();
                //outFile << "Target Directory: " << targetDir << "\n";
                results1.push_back(result);
                outFile << result << "\n\n";
                fflush(stdout);
            } else {
                printf("False!\n");
                fflush(stdout);
            }
        }
    }

    // 关闭文件
    inFile.close();
    outFile.close();

    //QString result = QString::fromStdString(foundNode->countDirectoryInfo());
    // 将输出信息写入到 textBrowser_3 中
    ui->textBrowser_3->setText("Processing complete. Results written to " + QString(outputFile.c_str()));

    //printf("Processing complete. Results written to %s\n", outputFile.c_str());
    fflush(stdout);  // 刷新标准输出缓冲区
}


void Widget::on_pushButton_6_clicked()
{
    // 获取输入文件路径
    std::string inputFile = ui->lineEdit_5->text().isEmpty() ? "D:\\myfile.txt" : ui->lineEdit_5->text().toStdString();

    FileSimulator change(inputFile,root);
    change.simulateFileOperations();
    fflush(stdout);  // 刷新标准输出缓冲区
}


void Widget::on_filechangediff_clicked()
{
    // 获取输入文件路径和输出文件路径
    std::string inputFile = ui->lineEdit_5->text().isEmpty() ? "D:\\mystat.txt" : ui->lineEdit_5->text().toStdString();
    std::string outputFile = ui->lineEdit_6->text().isEmpty() ? "D:\\myfilelog.txt" : ui->lineEdit_6->text().toStdString();


    // 打开输入文件
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return;
    }

    // 打开输出文件
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        inFile.close(); // 关闭输入文件
        return;
    }

    // 读取文件内容
    bool inDirsSection = false;
    std::string line;
    int indx = 0;
    int totalfiles = 0;
    long totalsize = 0;
    // 定义正则表达式模式
    std::regex filesPattern("Total Files: (\\d+)");
    std::regex sizePattern("Total Size: (\\d+) bytes");

    // 使用正则表达式进行匹配
    std::smatch filesMatch, sizeMatch;


    while (std::getline(inFile, line)) {
        if (line == "stat dirs") {
            inDirsSection = true;
        } else if (line == "end of dirs") {
            break;  // 结束循环，不再读取目录
        } else if (inDirsSection) {
            const char* targetDir = line.c_str();
            printf("Target Directory: %s\n", targetDir);

            // 调用查找函数
            TreeNode* foundNode = root->findDirectory(root, targetDir);

            // 打印结果并写入输出文件
            if (foundNode != nullptr) {
                std::string result = foundNode->countDirectoryInfo();
                std::string before_result = results1[indx];
                indx++;
                // outFile << "Target Directory: " << targetDir << "\n";
                // outFile << result << "\n\n";
                if(result != before_result)
                {
                    int totalFiles2;
                    long totalSize2;
                    int totalFiles1;
                    long totalSize1;
                    if (std::regex_search(result, filesMatch, filesPattern) && std::regex_search(result, sizeMatch, sizePattern)) {
                        // 获取匹配到的数字
                        totalFiles2 = std::stoi(filesMatch[1].str());
                        totalSize2 = std::stoi(sizeMatch[1].str());

                        // 输出结果
                        std::cout << "Total Files: " << totalFiles2 << std::endl;
                        std::cout << "Total Size: " << totalSize2 << " bytes" << std::endl;
                    }
                    if (std::regex_search(before_result, filesMatch, filesPattern) && std::regex_search(before_result, sizeMatch, sizePattern)) {
                        // 获取匹配到的数字
                        totalFiles1 = std::stoi(filesMatch[1].str());
                        totalSize1 = std::stoi(sizeMatch[1].str());

                        // 输出结果
                        std::cout << "Total Files: " << totalFiles1 << std::endl;
                        std::cout << "Total Size: " << totalSize1 << " bytes" << std::endl;
                    }
                    totalfiles = totalfiles + totalFiles2 - totalFiles1;
                    totalsize = totalsize + totalSize2 - totalSize1;
                    outFile << "Before\n"<<before_result<<"\n"<<"After\n"<<result << "\n\n";
                }


            } else {
                printf("False!\n");
                fflush(stdout);
            }
        }
    }

    // 关闭文件
    inFile.close();
    outFile.close();

    //QString result = QString::fromStdString(foundNode->countDirectoryInfo());
    // 将输出信息写入到 textBrowser_4 中
    ui->textBrowser_4->setText("Processing complete. Results written to " +
                               QString::fromStdString(outputFile) +
                               "\nTotal Files: " + QString::number(totalfiles) +
                               "\nTotal Size: " + QString::number(totalsize) + " bytes");

    //printf("Processing complete. Results written to %s\n", outputFile.c_str());
    fflush(stdout);  // 刷新标准输出缓冲区
}


void Widget::on_pushButton_7_clicked()
{
    ui->textBrowser_5->setText("正在删除中。。。");
    // 获取输入文件路径和输出文件路径
    std::string inputFile = ui->lineEdit_7->text().isEmpty() ? "D:\\mydir.txt" : ui->lineEdit_7->text().toStdString();
    std::string outputFile = ui->lineEdit_8->text().isEmpty() ? "D:\\deletedir.txt" : ui->lineEdit_8->text().toStdString();


    DirectorySimulator change(inputFile,root);

    change.executeDirectoryOperations(outputFile.c_str());
    ui->textBrowser_5->setText("Processing complete. Results written to " + QString(outputFile.c_str()));
}


void Widget::on_pushButton_8_clicked()
{
    // 获取输入文件路径和输出文件路径
    // std::string inputFile = ui->lineEdit_2->text().toStdString();
    // std::string outputFile = ui->lineEdit_3->text().toStdString();
    std::string inputFile = "D:\\mystat.txt";
    std::string outputFile = "D:\\mydirlog.txt";

    // 打开输入文件
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return;
    }

    // 打开输出文件
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        inFile.close(); // 关闭输入文件
        return;
    }

    // 读取文件内容
    bool inDirsSection = false;
    std::string line;
    while (std::getline(inFile, line)) {
        if (line == "stat dirs") {
            inDirsSection = true;
        } else if (line == "end of dirs") {
            break;  // 结束循环，不再读取目录
        } else if (inDirsSection) {
            const char* targetDir = line.c_str();
            printf("Target Directory: %s\n", targetDir);

            // 调用查找函数
            TreeNode* foundNode = root->findDirectory(root, targetDir);

            // 打印结果并写入输出文件
            if (foundNode != nullptr) {
                std::string result = foundNode->countDirectoryInfo();
                //outFile << "Target Directory: " << targetDir << "\n";
                results2.push_back(result);
                // outFile << result << "\n\n";
                fflush(stdout);
            } else {
                printf("False!\n");
                fflush(stdout);
            }
        }
    }

    int ind1 = 0;
    int ind2 = 0;
    while(ind1 < results1.size() && ind2 < results2.size())
    {
        if(results1[ind1] == results2[ind2])
        {
            ind1++;
            ind2++;
            continue;
        }
        else
        {
            outFile << results1[ind1] <<"\n\n";
            ind1++;
            continue;
        }
    }

    // 关闭文件
    inFile.close();
    outFile.close();

    //QString result = QString::fromStdString(foundNode->countDirectoryInfo());
    // 将输出信息写入到 textBrowser_3 中
    ui->textBrowser_5->setText("Processing complete. Results written to " + QString(outputFile.c_str()));

    //printf("Processing complete. Results written to %s\n", outputFile.c_str());
    fflush(stdout);  // 刷新标准输出缓冲区
}

