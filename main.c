/** ͷ�ļ��ͺ궨���� **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define STU_MAX 30
#define NAME_MAX 25
#define SUBJ_MAX 6

/** ����ṹ�� **/
typedef struct database
{
    int iInputIndex;
    char cName[NAME_MAX];
    int iStudentID;
    float fScore[SUBJ_MAX];
    int iRanking;
} DATABASE;

/** �����Ժ����� **/
/* �ּܷ��㺯�� */
float GetTotal(float *array, int n)
{
    int i;
    float fTotal = 0.0;
    for (i = 0; i < n; i++)
        fTotal += array[i];
    return fTotal;
}

/* ƽ���ּ��㺯�� */
float GetAverage(float *array, int n)
{
    return GetTotal(array, n) / n;
}

/* ���������� */
_Bool Ascending(float a, float b)
{
    return a < b;
}

/* ���������� */
_Bool Descending(float a, float b)
{
    return a > b;
}

/* �ݹ���ĸ�������� */
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

/** �����Ժ����� **/
/* ����¼�뺯�� */
void InputRecord(DATABASE *data_list, int *stu_number, int *sub_number, int *is_recorded)
{
    //�ж��Ƿ��Ѿ�¼�����
    if (*is_recorded)
    {
        //�洢�û���ѡ��
        char cChoice;
        //��ʾ�û��Ѿ�¼������Ƿ�����¼�롣
        printf("[ WARN]There's already data in the memory. Wipe them and continue?\n");
        printf("[ HINT]Press n for no and other for yes: ");
        //��ȡ�û�ѡ��
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
    //Ĩ��ԭ�����ݡ�
    memset(data_list, 0, sizeof(DATABASE) * (STU_MAX + 1));
    int iIsScanf = 1, i, j;
    printf("[ HINT]Please enter the number of students: ");
    //Ϊ����Ƿ����룬�˴�ʹ�� iIsScanf �����ж� scanf() �Ƿ�ɹ���ȡֵ��
    //���δ�ܳɹ�¼�룬�� fflush() ��ջ�������Ȼ����ʾ��¼��
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
    //���¼�롣
    for (i = 1; i <= *stu_number; i++)
    {
        data_list[i].iInputIndex = i;
        printf("[ HINT]Please enter the ID of student %d: ", i);
        iIsScanf = 1;
        //����Ƿ����룬����³���ԡ�
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
    //������¼�롱flag��Ϊ 1��
    *is_recorded = 1;
    //����������
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
    //��ȡ����ֵ�����������ǰһ����һ������ô TA �������ͺ�ǰһ����һ����
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
    //������ֵ���� data_list��
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

/* �б��ӡ���� */
void ListRecord(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    //����Ƿ��Ѿ�¼�롣
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
    //�����ӡ��
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

/* ���㵥����Ŀ�ֺܷ�ƽ���ֺ��� */
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

/* ���㵥��ѧ���ܷ�ƽ���ֺ��� */
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

/* ���ɼ������������� */
void SortByS(_Bool (*comparing)(float a, float b), DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i, j;
    //�����ƻ�Դ���ݿ⣬�����µ�������ʱ�����ݿ⡣
    DATABASE dataTemp;
    DATABASE dataSorted[stu_number + 1];
    //���������ݿ⸴�Ƶ���ʱ���ݿ⡣
    memcpy(dataSorted, data_list, sizeof(dataSorted));
    //ѡ������
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
    //��ӡ��������ʱ���ݿ⡣
    ListRecord(dataSorted, stu_number, sub_number, is_recorded);
    return;
}

/* ��ѧ������������ */
void SortInAByID(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    int i, j;
    //ת�����ݡ�
    DATABASE dataTemp;
    DATABASE dataSorted[stu_number + 1];
    memcpy(dataSorted, data_list, sizeof(dataSorted));
    //����
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

/* ��ѧ��������ĸ������������ */
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
            //ִ�еݹ�����
            RecursionSortName(&dataSorted[j], &dataSorted[i], 0);
        }
    }
    ListRecord(dataSorted, stu_number, sub_number, is_recorded);
    return;
}

/* ��ѧ�Ų��Һ��� */
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
    //������ҡ�
    for (i = 1; i <= stu_number; ++i)
    {
        //�����ִ������ѧ����
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
            //����������
            return;
        }
    }
    printf("[ERROR]Can't find the student with given ID!\n\n");
    return;
}

/* ���������Һ��� */
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
        //����ҵ���
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
            //����������
            return;
        }
    }
    printf("[ERROR]Can't find the student with given Name!\n\n");
    return;
}

/* ͳ�Ʒ������� */
void StatisticAnalysis(DATABASE *data_list, int stu_number, int sub_number, int is_recorded)
{
    if (!is_recorded)
    {
        printf("[ERROR]Please use (1) to input record or (11) to load data first!\n\n");
        return;
    }
    //0 �� 5 �ֱ�ָ ���㡢���á�һ�㡢���񡢲�����
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

/* ���ļ����� */
void InputFromFile(DATABASE *data_list, int *stu_number, int *sub_number, int *is_recorded)
{
    //�ж��Ƿ��Ѿ�¼�����
    if (*is_recorded)
    {
        //�洢�û���ѡ��
        char cChoice;
        //��ʾ�û��Ѿ�¼������Ƿ�����¼�롣
        printf("[ WARN]There's already data in the memory. Wipe them and continue?\n");
        printf("[ HINT]Press n for no and other for yes: ");
        //��ȡ�û�ѡ��
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
    //Ĩ��ԭ�����ݡ�
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
    //��ʼ�Ķ��ļ�
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


/** ������ **/
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

