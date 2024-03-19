**磁盘文件扫描：**

扫描磁盘中的所有文件和目录，构建目录树，并且统计磁盘文件属性。运行结果如图6-1所示：
![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/5b0a1c8a-90fd-42f0-b7a0-824756d6fb43)


图6-1获取磁盘文件属性运行结果

该功能统计c:\\windows目录下的所有目录和文件信息，并且建立目录树，将统计信息打印在显示框中，以便使用者查看，主要信息包括：目录深度，目录树深度，目录数量，文件数量，最长文件名以及长度。

空间复杂度分析：如果使用常规的递归，运行过程中的空间复杂度难以计算，会出现内存不足而导致程序崩溃的情况，我在这里采用栈模拟常规的递归调用，只需维护一个记录还未扫描到的目录信息的栈，假设目录总数为m，则空间复杂度为O（m）

时间复杂度分析：由于所有文件和目录都会被扫描到且不会重复扫描到，文件和目录的总数为n，时间复杂度则为O（n）

**SQL****语句生成：**

完成磁盘文件扫描后，就可以进行SQL语句的生成，我们在输入框中打入需要将文件存放的路径，点击确认生成，程序就会自动生成整个目录的所有文件和目录的SQL插入语句，并生成sqldir.txt和sqlfile.txt文件，便于我们将目录和文件信息插入到数据库中。并且会在屏幕上显示文件的存放路径。运行结果如图6-2 ，6-3 所示。
![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/6f32bbce-096e-4ef1-8698-c28ca61b0048)


图6-2 SQL语句生成运行
![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/0d113cc5-1c4a-47e9-b98a-2eeae6cd60bc)


图6-3生成文件信息

时间复杂度分析：采用DFS算法遍历整棵目录树，区分节点处的是目录或者文件，生成对应的插入语句，时间复杂度为O(n)

数据库表结构如下：

~~~sql
CREATE TABLE directories (

        id INT AUTO_INCREMENT PRIMARY KEY,

        name VARCHAR(255) NOT NULL, -- 目录名

        path VARCHAR(512) NOT NULL, -- 完整路径

        subdirectories_count INT, -- 子目录个数

        files_count INT, -- 文件个数

        depth INT, -- 目录深度

        parent_directory_id INT, -- 外键，指向父目录的ID

        FOREIGN KEY (parent_directory_id) REFERENCES directories(id)

    );
~~~

~~~sql
CREATE TABLE files (

        id INT AUTO_INCREMENT PRIMARY KEY,

        name VARCHAR(255) NOT NULL, -- 文件名

        path VARCHAR(512) NOT NULL, -- 完整路径

        size BIGINT NOT NULL, -- 文件大小（以字节为单位）

        depth INT, -- 文件深度

        last_modified TIMESTAMP, -- 最后修改时间

        directory_id INT, -- 外键，指向所属目录的ID

        FOREIGN KEY (directory_id) REFERENCES directories(id)

    );
~~~
生成数据库表结构如下图6-4所示：
![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/685c7afd-3eda-4a86-8b76-41b2000e6bf4)


图6-4数据库结构展示

**统计目录信息：**

在这里程序支持两种统计目录信息的操作：指定某个目录统计信息，从文件中导入多个目录统计多个目录信息

1.统计某个目录信息

输入要统计目录的绝对路径，点击【统计】程序就在对话框中打印出该目录的信息，包括：目录路径，最早更新时间，最晚文件更新时间，总文件数和总文件大小，展示画面如图6-5所示：

![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/6b1618d2-831e-4f9f-8edb-ec5626c8e60b)


图6-5统计指定目录信息

2.从文件中导入目录信息

在对话框中输入导入文件地址和统计信息存放路径，点击【确认】，对话框中会弹出完成信息【Processing complete. Results written to D:\output.txt】，展示如图6-6所示：
![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/fe8839a5-436d-48cf-a19c-7ccb0d405022)



图6-6从文件中导入目录信息

统计信息文件展示如图6-7所示：

![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/4b56c2ae-6ebc-4364-b239-20b76f0725d2)


图6-7目录统计信息

时间复杂度分析：

假设要统计m个目录信息，目录树中有n个节点，如果采用遍历的操作时间复杂度为O(mn),在我的程序中，将一个完整的目录路径，分解为多个目录名，只要依次比较目录文件名进入特定的分支即可，处理后的时间复杂度为O(m logn)大大降低了时间复杂度。

**文件模拟操作：**

该模块分为两个基本操作：1.输入文件模拟操作文件对文件进行模拟操作2.输入要统计目录路径统计文件操作对目录信息的变化

1.文件模拟操作

在输入框中输入文件模拟操作文件的【绝对路径】,点击【确认修改】即可完成对文件的修改的操作

时间复杂度分析：文件模拟操作主要分为两步：1）寻找到文件在目录树中的位置2）对文件进行修改操作，寻找文件的步骤类似于统计目录信息中的操作时间复杂度为O(m logn)，对文件修改操作的时间复杂度为O(1)，所以总的时间复杂度为O(m logn)

2.统计文件修改前后目录信息变化

在输入框中输入mystat文件路径和文件更改信息存放路径，点击【修改检查】，程序会在对话框中打印出总的文件变化数和总文件大小变化数，并且把完整的差异信息文件存放路径打印，执行操作展示如图6-8所示：

![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/e13fe4be-df12-4fbe-8e68-2fbe195a1c3b)


图6-8统计文件修改前后目录信息变化

差异文件信息如图6-9所示：

![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/369557cc-0623-4ad5-97b8-1955d8acb99f)

图6-9文件差异信息

**目录模拟操作：**

本模块分为两个操作1）目录模拟操作2）再次统计文件信息比较差异

1）文件模拟操作

在输入框中分别输入mydir.txt地址和删除文件信息地址，点击【确认删除】，程序会自动完成删除操作，并且将完成信息打印在对话框中，操作展示如图6-10所示：

![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/853a1901-ac7a-4a1b-b1ee-7ee7c77003cb)



图6-10删除目录操作展示

2）再次统计文件信息比较差异

点击【再次统计文件信息比较差异】，程序会将差异信息打印到日志文件中，如图6-11所示，统计信息文件如图6-12所示：

![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/f0b73f64-c2c1-4dc3-a09c-f8c6629881e8)

图6-11再次统计文件信息比较差异
![image](https://github.com/MarkMrLi/Scan_system/assets/110333285/6e76ed0e-3c77-4ba1-bce9-a70c9e8dcf8b)


图6-12统计信息文件
