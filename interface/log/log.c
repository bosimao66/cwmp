


/*
备注：
（1）可以拓展：调试信息打印到控制台，分多个级别，可以给一个变量，用于控制什么级别才可以打印
*/




#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>     //用于可变参数个数的函数

#include <time.h>




//#define LOG_DEFAUL_HOME_DIR "/mnt/hgfs/share/PRO/cwmp2/acs"
//#define LOG_DEFAULT_LOG_FILE_FULL_NAME "/mnt/hgfs/share/PRO/cwmp2/acs/log/log.txt"


//默认的日志文件位置
#define LOG_DEFAUL_HOME_DIR "/mnt/hgfs/share/PRO/cwmp/debug"
#define LOG_DEFAULT_LOG_FILE_FULL_NAME "/mnt/hgfs/share/PRO/cwmp/debug/log.txt"


#define LOG_FILE_MAX_BYTE 1024*64

log_manager_t logLocalManager;


void log_write(int level, const char * file, const char *fmtIn, va_list valIn, const char * fmt, ...);
void log_to_console(int level, const char *fmtIn, va_list valIn, const char * fmt, ...);



void log_init(char *home, char *logFile)
{
    if(home != NULL)
        strcpy(logLocalManager.homeDir, home);
    else
        strcpy(logLocalManager.homeDir, LOG_DEFAUL_HOME_DIR);
    if(logFile != NULL)
        strcpy(logLocalManager.logFileFullName, logFile);
    else    
        strcpy(logLocalManager.logFileFullName, LOG_DEFAULT_LOG_FILE_FULL_NAME);
    int i;
    for(i = 0; i < LOG_LEVEL_END; i++)
        logLocalManager.llo[i] = gLLO[i];

    FILE *fpr, *fpw;
    fpr = fopen(logLocalManager.logFileFullName, "r");
    if(fpr == NULL)//说明文件不存在或者无法打开
    {
        fpw = fopen(logLocalManager.logFileFullName, "w");//创建
        if(fpw == NULL)
            printf("have none userful log file\n");
        else 
            fclose(fpw);
    }
    else
    {
        fseek(fpr, 0, SEEK_END);
        if(ftell(fpr) >= LOG_FILE_MAX_BYTE)//超过限制大小
        {
            fpw = fopen(logLocalManager.logFileFullName, "w");//清空
            if(fpw == NULL)
                printf("have none userful log file\n");
            else 
                fclose(fpw);
        }
        fclose(fpr);
    }
    //写入开场白
    va_list val;
    log_write(LOG_LEVEL_START, logLocalManager.logFileFullName, NULL, val, "\n======================log start=======================");
    log_to_console(LOG_LEVEL_START, NULL, val, "\n======================log start=======================");
}

void log_write(int level, const char * file, const char *fmtIn, va_list valIn, const char * fmt, ...)
{
    if(file == NULL)
        return ;
    if(level < LOG_LEVEL_START || level > LOG_LEVEL_END)
        level = LOG_LEVEL_START;
    if(file == NULL)
        return;
    FILE *fp;
    //检查文件是否存在
    fp = fopen(file, "r");
    if(fp == NULL)
        return;
    //打开文件
    fp = fopen(file, "a");
    if(fp == NULL)
        return;       
    //内容写入日志文件
    if(fmt != NULL)
    {
        va_list val;
        va_start(val, fmt);    
        vfprintf(fp, fmt, val);
        fflush(fp);
        va_end(val);        
    }
    if(fmtIn != NULL)
         vfprintf(fp, fmtIn, valIn);
    if(level != LOG_LEVEL_SHOW_SIMPLE)
         fprintf(fp, "\n");
   
    
    fclose(fp);

}

void log_to_console(int level, const char *fmtIn, va_list valIn, const char * fmt, ...)
{
    if(fmt != NULL)
    {
        va_list val;
        va_start(val, fmt);
        vprintf(fmt, val);
        va_end(val);
    }
    if(fmtIn != NULL)
        vprintf(fmtIn, valIn);
    if(level != LOG_LEVEL_SHOW_SIMPLE)
        printf("\n");
    fflush(stdout);
}

//日志记录

#define LOG_TO_CONSOLE_ENABLE 1
void log_record(int level, const char *info, const char *fmt, ...)
{
    va_list val;
    va_start(val, fmt);
  
    //获取时间
    char bufTime[64] = {0};
    time_t current_time;
    time(&current_time);
    // 将时间转换为本地时间
    struct tm *local_time = localtime(&current_time);
    // 直接输出日期和时间字符串
    int len = strlen(asctime(local_time));
    strncpy(bufTime, asctime(local_time), 64);
    if(len < 64 - 1)
    bufTime[strlen(asctime(local_time)) - 1] = 0;
   // printf("======================%s\n", bufTime);
   
    //简单显示
    if(level == LOG_LEVEL_SHOW_SIMPLE)
        log_write(LOG_LEVEL_SHOW_SIMPLE, logLocalManager.logFileFullName, fmt, val, NULL);    
    //附加：打印级别、时间戳
    else
        log_write(level, logLocalManager.logFileFullName, fmt, val, "[%s %d][%s]%s", 
                logLocalManager.llo[level].name, logLocalManager.llo[level].cnt++, bufTime, info);
    va_end(val);
    
#if(LOG_TO_CONSOLE_ENABLE)
    va_start(val, fmt);
      //简单显示
    if(level == LOG_LEVEL_SHOW_SIMPLE)
        log_to_console(level, fmt, val, NULL);    
    //附加：打印级别、时间戳
    else
        log_to_console(level, fmt, val, "[%s %d][%s]%s",  logLocalManager.llo[level].name, 
                        logLocalManager.llo[level].cnt++, bufTime, info);
    va_end(val);
#endif

}



void log_test(void)
{
    log_init(NULL, NULL);
	LOG_ALARM("this is log alarm!\n");
    LOG_DEBUG("this is log debug !\n");

}






















