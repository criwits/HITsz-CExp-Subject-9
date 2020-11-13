/** 头文件和宏定义区 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define STU_MAX 30
#define NAME_MAX 25
#define SUBJ_MAX 6

/** 定义结构体 **/
typedef struct database
{
    int iInputIndex;
    char cName[NAME_MAX];
    int iStudentID;
    float fScore[SUBJ_MAX];
    int iRanking;
} DATABASE;

/** 工具性函数区 **/
/* 总分计算函数 */
float GetTotal(float *array, int n)
{
    int i;
    float fTotal = 0.0;
    for (i = 0; i < n; i++)
        fTotal += array[i];
    return fTotal;
}

/* 平均分计算函数 */
float GetAverage(float *array, int n)
{
    return GetTotal(array, n) / n;
}

/* 升序排序函数 */
_Bool Ascending(float a, float b)
{
    return a < b;
}

/* 降序排序函数 */
_Bool Descending(float a, float b)
{
    return a > b;
}

/* 递归字母序排序函数 */
void RecursionSortName(DATABASE *a, DATABASE *b, int step)
{
    if (a->cName[step] < b->cName[step])
    {
        DATABASE tmp;
        tmp = *a;
        *a = *b;
        *b = tmp;
        return;
    }
    if (a->cName[step] == b->cName[step])
        RecursionSortName(a, b, step + 1);
    return;
}

/** 功能性函数区 **/
/* 数据录入函数 */
void InputRecord(DATABASE *data_list, int *stu_number, int *sub_number, int *is_recorded)
{
    //判断是否已经录入过。
    if (*is_recorded)
    {
        //存储用户的选择。
        char cChoice;
        //提示用户已经录入过，是否重新录入。
        printf("[ WARN]There's already data in the memory. Wipe them and continue?\n");
        printf("[ HINT]Press n for no and other for yes: ");
        //获取用户选择。
        cChoice = getch();
        switch(cChoice)
        {
        case 'n':
        case 'N':
            printf("\n");
            return;
            break;
        default:
            break;
        }
    }
    printf("\n");
    //抹掉原有数据。
    memset(data_list, 0, sizeof(DATABASE) * (STU_MAX + 1));
    int iIsScanf = 1, i, j;
    printf("[ HINT]Please enter the number of students: ");
    //为避免非法输入，此处使用 iIsScanf 变量判断 scanf() 是否成功获取值。
    //如果未能成功录入，用 fflush() 清空缓冲区，然后提示重录。
    do
    {
        fflush(stdin);
        if (!iIsScanf || *stu_number > STU_MAX || *stu_number <= 0)
            printf("[ERROR]Not a proper input, please try again: ");
        iIsScanf = scanf("%d", stu_number);
    } while (!iIsScanf || *stu_number > STU_MAX || *stu_number <= 0);
    printf("[ HINT]Please enter the number of subjects: ");
    do
    {
        fflush(stdin);
        if (!iIsScanf || *sub_number > SUBJ_MAX || *sub_number <= 0)
            printf("[ERROR]Not a proper input, please try again: ");
        iIsScanf = scanf("%d", sub_number);
    } while (!iIsScanf || *sub_number > SUBJ_MAX || *sub_number <= 0);
    //逐个录入。
    for (i = 1; i <= *stu_number; i++)
    {
        data_list[i].iInputIndex = i;
        printf("[ HINT]Please enter the ID of student %d: ", i);
        iIsScanf = 1;
        //避免非法输入，提升鲁棒性。
        do
        {
            fflush(stdin);
            if (!iIsScanf)
                printf("[ERROR]Not a proper input, please try again: ");
            iIsScanf = scanf("%d", &data_list[i].iStudentID);
        } while (!iIsScanf);
        printf("[ HINT]Please enter the name of the student %d: ", i);
        fflush(stdin);
        gets(data_list[i].cName);
        for (j = 0; j < *sub_number; j++)
        {
            printf("[ HINT]Please enter the score %d of the student %d: ", j + 1, i);
            do
            {
                fflush(stdin);
                if (!iIsScanf)
                    printf("[ERROR]Not a proper input, please try again: ");
                iIsScanf = scanf("%f", &data_list[i].fScore[j]);
            } while (!iIsScanf);
        }
    }
    printf("[ INFO]Successfully recorded data of %d students!\n\n", *stu_number);
    //将“已录入”flag设为 1。
    *is_recorded = 1;
    //计算排名。
    DATABASE dataTemp;
    DATABASE dataSorted[*stu_number + 1];
    memcpy(dataSorted, data_list, sizeof(dataSorted));
    for (i = 1; i < *stu_number; i++)
    {
        for (j = i + 1; j <= *stu_number; j++)
        {
            if (GetTotal(dataSorted[j].fScore, *sub_number) > GetTotal(dataSorted[i].fScore, *sub_number))
            {
                dataTemp = dataSorted[j];
                dataSorted[j] = dataSorted[i];
                dataSorted[i] = dataTemp;
            }
        }
    }
    //获取排名值：如果分数跟前一个人一样，那么 TA 的排名就和前一个人一样。
    dataSorted[1].iRanking = 1;
    for (i = 2; i <= *stu_number; i++)
    {
        if (fabs(GetTotal(dataSorted[i].fScore, *sub_number) - GetTotal(dataSorted[i - 1].fScore, *sub_number)) < 1e-3)
        {
            dataSorted[i].iRanking = dataSorted[i - 1].iRanking;
        }
        else
        {
            dataSorted[i].iRanking = i;
        }
    }
    //把排名值传回 data_list。
    for (i = 1; i <= *stu_number; i++)
        data_list[dataSorted[i].iInputIndex].iRanking = dataSorted[i].iRanking;
    FILE *fp;
    char cFileName[NAME_MAX];
    printf("[ HINT]Please enter the filename you want to store this file.\n"
           "[ HINT]The extension .CSV is recommended.\n"
           "[ HINT]Enter the filename here(no more than 25 characters): ");
    fflush(stdin);
    gets(cFileName);
    if (!(fp = fopen(cFileName, "w")))
    {
        printf("[ERROR]Cannot open the file!\n"
               "[ERROR]The data was not saved!\n");
        return;
    }
    fprintf(fp, "%d", *stu_number);
    for (i = 0; i < *sub_number + 2; i++)
    {
        fprintf(fp, ",");
    }
    fprintf(fp, "\n");

    fprintf(fp, "%d", *sub_number);
    for (i = 0; i < *sub_number + 2; i++)
    {
        fprintf(fp, ",");
    }
    fprintf(fp, "\n");

    for (i = 1; i <= *stu_number; i++)
    {
        fprintf(fp, "%d,", data_list[i].iStudentID);
        for (j = 0; j < *sub_number; j++)
        {
            fprintf(fp, "%.2f,", data_list[i].fScore[j]);
        }
        fprintf(fp, "%d,%s\n", data_list[i].iRanking, data_list[i].cName);
    }
    fclose(fp);
    printf("[ INFO]Successfully saved all the data to %s!\n", cFileName);
    return;
}

/* 列表打印函数 */
void ListRecord(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    //检查是否已经录入。
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i, j;
    printf(" Rank   StudentID             Name          ");
    for (i = 0; i < sub_number; i++)
        printf("Score%2d   ", i + 1);
    printf("Total     Average    ");
    printf("\n");
    for (i = 0; i < sub_number; i++)
        printf("=========");
    printf("=====================================================================\n");
    //逐个打印。
    for (i = 1; i <= stu_number; i++)
    {
        printf(" %-7d%-22d%-14s", data_list[i].iRanking, data_list[i].iStudentID, data_list[i].cName);
        for (j = 0; j < sub_number; j++)
            printf("%-10.2f", data_list[i].fScore[j]);
        printf("%-10.2f%-10.2f", GetTotal(data_list[i].fScore, sub_number), GetAverage(data_list[i].fScore, sub_number));
        printf("\n");
    }
    printf("\n[ INFO]Successfully listed data of %d students!\n\n", stu_number);
    return;
}

/* 计算单个科目总分和平均分函数 */
void CalculateSingleCourseTAndA(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i, j;
    float fTotal = 0;
    printf(" Subject No.     Total        Average\n");
    printf("=======================================================\n");
    for (i = 0; i < sub_number; i++)
    {
        fTotal = 0;
        for (j = 1; j <= stu_number; j++)
            fTotal += data_list[j].fScore[i];
        printf(" %-16d%-13.2f%-13.2f\n", i + 1, fTotal, fTotal / stu_number);
    }
    printf("\n[ INFO]Successfully listed data of %d subjects!\n", sub_number);
    return;
}

/* 计算单个学生总分平均分函数 */
void CalculateSingleStudentTAndA(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i;
    printf(" Student ID      Name           Total       Average\n");
    printf("===============================================================\n");
    for (i = 1; i <= stu_number; i++)
    {
        printf(" %-16d%-15s%-12.2f%-12.2f\n", data_list[i].iStudentID, data_list[i].cName, GetTotal(data_list[i].fScore, sub_number), GetAverage(data_list[i].fScore, sub_number));
    }
    printf("\n[ INFO]Successfully listed data of %d students!\n", stu_number);
    return;
}

/* 按成绩升降序排序函数 */
void SortByS(_Bool (*comparing)(float a, float b), DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i, j;
    //避免破坏源数据库，建立新的排序临时用数据库。
    DATABASE dataTemp;
    DATABASE dataSorted[stu_number + 1];
    //将现有数据库复制到临时数据库。
    memcpy(dataSorted, data_list, sizeof(dataSorted));
    //选择排序。
    for (i = 1; i < stu_number; i++)
    {
        for (j = i + 1; j <= stu_number; j++)
        {
            if ((*comparing)(GetTotal(dataSorted[j].fScore, sub_number), GetTotal(dataSorted[i].fScore, sub_number)))
            {
                dataTemp = dataSorted[j];
                dataSorted[j] = dataSorted[i];
                dataSorted[i] = dataTemp;
            }
        }
    }
    //打印排序后的临时数据库。
    ListRecord(dataSorted, stu_number, sub_number, is_recorded);
    return;
}

/* 按学号升序排序函数 */
void SortInAByID(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i, j;
    //转移数据。
    DATABASE dataTemp;
    DATABASE dataSorted[stu_number + 1];
    memcpy(dataSorted, data_list, sizeof(dataSorted));
    //排序。
    for (i = 1; i < stu_number; i++)
    {
        for (j = i + 1; j <= stu_number; j++)
        {
            if (dataSorted[j].iStudentID < dataSorted[i].iStudentID)
            {
                dataTemp = dataSorted[j];
                dataSorted[j] = dataSorted[i];
                dataSorted[i] = dataTemp;
            }
        }
    }
    ListRecord(dataSorted, stu_number, sub_number, is_recorded);
    return;
}

/* 按学生姓名字母序升序排序函数 */
void SortByName(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i, j;
    DATABASE dataSorted[stu_number + 1];
    memcpy(dataSorted, data_list, sizeof(dataSorted));
    for (i = 1; i < stu_number; i++)
    {
        for (j = i + 1; j <= stu_number; j++)
        {
            //执行递归排序。
            RecursionSortName(&dataSorted[j], &dataSorted[i], 0);
        }
    }
    ListRecord(dataSorted, stu_number, sub_number, is_recorded);
    return;
}

/* 按学号查找函数 */
void SearchByID(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int iInputID, iIsScanf = 1;
    printf("[ HINT]Enter the ID you want to search: ");
    do
    {
        fflush(stdin);
        if (!iIsScanf)
            printf("[ERROR]Not a proper input, please try again: ");
        iIsScanf = scanf("%d", &iInputID);
    } while (!iIsScanf);
    int i, j;
    //逐个查找。
    for (i = 1; i <= stu_number; ++i)
    {
        //若发现存在这个学生。
        if (iInputID == data_list[i].iStudentID)
        {
            printf("[ INFO]Here is the result: \n");
            printf(" StudentID             Name          ");
            for (j = 0; j < sub_number; j++)
                printf("Score%2d   ", j + 1);
            printf("Total     Average    Ranking");
            printf("\n");
            for (j = 0; j < sub_number; j++)
                printf("=========");
            printf("======================================================================\n");
            printf(" %-22d%-14s", data_list[i].iStudentID, data_list[i].cName);
            for (j = 0; j < sub_number; j++)
                printf("%-10.2f", data_list[i].fScore[j]);
            printf("%-10.2f%-11.2f%-3d", GetTotal(data_list[i].fScore, sub_number), GetAverage(data_list[i].fScore, sub_number), data_list[i].iRanking);
            printf("\n");
            printf("\n[ INFO]Successfully listed the data of one student.\n\n");
            //结束函数。
            return;
        }
    }
    printf("[ERROR]Can't find the student with given ID!\n\n");
    return;
}

/* 按姓名查找函数 */
void SearchByName(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int iIsScanf = 1;
    char cInputName[NAME_MAX];
    printf("[ HINT]Enter the ID you want to search: ");
    do
    {
        fflush(stdin);
        if (!iIsScanf)
            printf("[ERROR]Not a proper input, please try again: ");
        iIsScanf = scanf("%s", cInputName);
    } while (!iIsScanf);
    int i, j;
    for (i = 1; i <= stu_number; ++i)
    {
        //如果找到。
        if (!strcmp(cInputName, data_list[i].cName))
        {
            printf("[ INFO]Here is the result: \n");
            printf(" StudentID             Name          ");
            for (j = 0; j < sub_number; j++)
                printf("Score%2d   ", j + 1);
            printf("Total     Average    Ranking");
            printf("\n");
            for (j = 0; j < sub_number; j++)
                printf("=========");
            printf("======================================================================\n");
            printf(" %-22d%-14s", data_list[i].iStudentID, data_list[i].cName);
            for (j = 0; j < sub_number; j++)
                printf("%-10.2f", data_list[i].fScore[j]);
            printf("%-10.2f%-11.2f%-3d", GetTotal(data_list[i].fScore, sub_number), GetAverage(data_list[i].fScore, sub_number), data_list[i].iRanking);
            printf("\n");
            printf("\n[ INFO]Successfully listed the data of one student.\n\n");
            //结束函数。
            return;
        }
    }
    printf("[ERROR]Can't find the student with given Name!\n\n");
    return;
}

/* 统计分析函数 */
void StatisticAnalysis(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    //0 — 5 分别指 优秀、良好、一般、及格、不及格。
    int iSubjectNum[sub_number][5];
    int i, j;
    memset(iSubjectNum, 0, sizeof(iSubjectNum));
    for (i = 1; i <= stu_number; ++i)
    {
        for (j = 0; j < sub_number; j++)
        {
            if (data_list[i].fScore[j] >= 90)
                iSubjectNum[j][0]++;
            else if (data_list[i].fScore[j] >= 80)
                iSubjectNum[j][1]++;
            else if (data_list[i].fScore[j] >= 70)
                iSubjectNum[j][2]++;
            else if (data_list[i].fScore[j] >= 60)
                iSubjectNum[j][3]++;
            else
                iSubjectNum[j][4]++;
        }
    }
    printf(" Subject No.      Exce(>=90)        Good[80,90)       So-so[70,80)      Passed[60,70)     Failed(<60)\n");
    printf("================================================================================================================\n");
    for (i = 0; i < sub_number; i++)
    {
        printf(" %-17d", i + 1);
        for (j = 0; j <= 4; j++)
            printf("%-10d%5.2f%% |", iSubjectNum[i][j], iSubjectNum[i][j] * 100 / (float)stu_number);
        printf("\n");
    }
    printf("\n[ INFO]Successfully listed data of all subjects!\n\n");
    return;
}

/* 从文件读入 */
void InputFromFile(DATABASE *data_list, int *stu_number, int *sub_number, int *is_recorded)
{
    //判断是否已经录入过。
    if (*is_recorded)
    {
        //存储用户的选择。
        char cChoice;
        //提示用户已经录入过，是否重新录入。
        printf("[ WARN]There's already data in the memory. Wipe them and continue?\n");
        printf("[ HINT]Press n for no and other for yes: ");
        //获取用户选择。
        cChoice = getch();
        switch(cChoice)
        {
        case 'n':
        case 'N':
            printf("\n");
            return;
            break;
        default:
            break;
        }
    }
    printf("\n");
    //抹掉原有数据。
    memset(data_list, 0, sizeof(DATABASE) * (STU_MAX + 1));
    char cFileName[NAME_MAX];
    printf("[ HINT]Please enter the name of your data file.\n"
           "[ HINT]Standard CSV files are supported.\n"
           "[ HINT]Enter the filename: ");
    fflush(stdin);
    gets(cFileName);
    FILE *fp;
    if(!(fp = fopen(cFileName, "r")))
    {
        printf("[ERROR]Cannot open the file!\n");
        return;
    }
    //开始阅读文件
    char cBuffer[16];
    fgets(cBuffer, 16, fp);
    *stu_number = atoi(cBuffer);
    fgets(cBuffer, 16, fp);
    *sub_number = atoi(cBuffer);
    int i, j, iReadCount = 0;
    for (i = 1; i <= *stu_number; i++)
    {
        iReadCount = 0;
        iReadCount += fscanf(fp, "%d,", &data_list[i].iStudentID);
        for (j = 0; j < *sub_number; j++)
            iReadCount += fscanf(fp, "%f,", &data_list[i].fScore[j]);
        iReadCount += fscanf(fp, "%d,%s\n", &data_list[i].iRanking, data_list[i].cName);
        if (iReadCount != 3 + *sub_number)
            printf("[ WARN]Errors occurred while reading line %d!\n", i + 2);
    }
    fclose(fp);
    *is_recorded = 1;
    printf("[ INFO]All done!\n");
    return;
}


/** 主函数 **/
int main(void)
{
    int iStudentNumber = STU_MAX;
    int iIsRecorded = 0;
    int iSubjects = 1;
    DATABASE dataList[STU_MAX + 1];
    int iChoice, iIsScanf = 1;
    do
    {
        printf("1. Input record\n"
               "2. Calculate total and average score of every course\n"
               "3. Calculate total and average score of every student\n"
               "4. Sort in descending order by total score of every student\n"
               "5. Sort in ascending order by total score of every student\n"
               "6. Sort in ascending order by StudentID\n"
               "7. Sort in dictionary order by name\n"
               "8. Search by StudentID\n"
               "9 .Search by name\n"
               "10.Statistic analysis for every course\n"
               "11.List Record\n"
               "0. Exit\n");
        printf("[ HINT]Please enter your choice: ");
        do
        {
            fflush(stdin);
            if (!iIsScanf)
                printf("[ERROR]Not a proper input, please try again: ");
            iIsScanf = scanf("%d", &iChoice);
        } while (!iIsScanf);
        switch (iChoice)
        {
        case 1:
            printf("\n");
            InputRecord(dataList, &iStudentNumber, &iSubjects, &iIsRecorded);
            break;
        case 11:
            printf("\n");
            InputFromFile(dataList, &iStudentNumber, &iSubjects, &iIsRecorded);
            ListRecord(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 2:
            printf("\n");
            CalculateSingleCourseTAndA(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 3:
            printf("\n");
            CalculateSingleStudentTAndA(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 4:
            printf("\n");
            SortByS(Descending, dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 5:
            printf("\n");
            SortByS(Ascending, dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 6:
            printf("\n");
            SortInAByID(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 7:
            printf("\n");
            SortByName(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 8:
            printf("\n");
            SearchByID(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 9:
            printf("\n");
            SearchByName(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 10:
            printf("\n");
            StatisticAnalysis(dataList, iStudentNumber, iSubjects, iIsRecorded);
            break;
        case 0:
            return 0;
            break;
        default:
            printf("\n[ERROR]Not a proper choice!\n\n");
            break;
        }
    } while (1);
    return 0;
}

