# 使用 C 语言编写的学生成绩管理系统——用户手册

> 本“学生成绩管理系统”是哈尔滨工业大学（深圳）2020 年秋季学期“高级语言实验设计（实验）”课程的第 9 次作业暨结课作业。

## 系统概述

本学生成绩管理系统采用高效率的 C 语言进行程序编写，基于命令行的操作界面进行使用，操作简单，易于上手。

本学生成绩管理系统的主要功能有：

- 学生成绩录入 / 导出（支持标准的 .CSV 格式）；
- 按学生成绩排名（降序、升序）；
- 按学生学号升序列表；
- 按学生姓名字母序升序排列；
- 按学号 / 姓名查找成绩与排名
- 统计分析各学科各成绩段的人数；
- 统计分析各学科及各个学生的平均分。

## 功能使用方法

### 录入学生成绩

#### 从键盘录入

运行程序后键入 1，按下 Enter，系统将进入录入模式。

> 提示：如果在这之前已经录入过成绩，程序将询问您是否抹掉原有数据重新录入。此时，按下 N 将保留原有数据，不再进行录入；按其他字母则将抹掉原有数据。

接下来，系统会要求您键入学生的总数（不超过 30）、学科的数量（不超过 6）。

> 如果您的输入内容不合法（例如，在应该键入数字的地方键入字母），系统将提示您重新键入。

上述两个数据录入成功之后，系统将要求您逐个键入每个学生的学号、姓名、各科成绩。

录入完毕后，系统将对数据库进行保存。保存时，要求您键入一个文件名，这个文件名不应该超过 25 个字符，并且我们建议您使用 `.csv` 扩展名。这个文件将被保存在命令行工作目录下。您可以使用 Excel 软件直接打开这个 CSV 文件查看数据。

> 我们不建议您使用 Excel 编辑这个 CSV 文件。

所有任务结束之后，系统将回到主菜单。

#### 从文件录入

运行程序后键入 11，按下 Enter，系统将进入从文件导入模式。

> 提示：如果在这之前已经录入过成绩，程序将询问您是否抹掉原有数据重新录入。此时，按下 N 将保留原有数据，不再进行录入；按其他字母则将抹掉原有数据。

系统会要求您键入一个文件名。这个文件应该是使用本程序功能 1 导出的数据表，它应该被放在命令行的工作目录下。同样地，它的文件名不应该超过 25 个字符。

完成文件名输入后，系统将开始逐行读取数据。一旦遇到数据有错误的行，系统会提示 `[ WARN]Errors occurred while reading line ...`。

> 请注意，遇到不能读取的行，并不会阻止数据的导入。

无论导入过程中是否出现错误，最终程序都会最终显示 `[ INFO]All done!`，并尝试打印出导入之后的结果。

### 计算各科的成绩和平均分

运行程序后键入 2，按下 Enter。

> 注意：在执行后续所有功能时，如果事先没有使用功能 1 或者功能 11 录入 / 导入成绩，程序将会提示您进行录入 / 导入。

程序将会以易读的方式输出各个科目的成绩和平均分。

### 计算各个学生的成绩和平均分

运行程序后键入 3，按下 Enter。程序将会以易读的方式输出各个学生的成绩和平均分。

### 打印排名表

运行程序后键入 4（成绩降序）或 5（成绩升序），按下 Enter。程序将以易读的方式打印出排名后的成绩表。

程序在计算排名时将自动考虑同分情况。

### 打印按姓名 / 学号排名的成绩表

运行程序后键入 6（按学号升序）或 7（按姓名字母序升序），按下 Enter，程序将以易读的方式打印出成绩表。

### 按学号查找学生的成绩

运行程序后键入 8，按下 Enter，启动学号查找功能。

在程序提示 `[ HINT]Enter the ID you want to search:` 之后，键入需要查询的学号，按下 Enter。

如果所查询的这个学号存在，那么系统将打印其成绩、排名；否则将提示错误信息。

### 按姓名查找学生的成绩

运行程序后键入 9，按下 Enter，启动姓名查找功能。

在程序提示 `[ HINT]Enter the Name you want to search:` 之后，键入需要查询的姓名，按下 Enter。

如果所查询的这个姓名存在，那么系统将打印其成绩、排名；否则将提示错误信息。

### 进行成绩的统计分析

运行程序后键入 10，按下 Enter，启动统计分析功能。

系统将按照“优秀”（Excellent）（不低于 90 分）、“良好”（Good）（80 到 89 分）、“中等”（So-so）（70 到 79 分）、“及格”（Passed）（60 到 69 分）、“不及格”（Failed）（低于 60 分）五个等级对每个学科进行统计，并以表格的形式将结果打印。