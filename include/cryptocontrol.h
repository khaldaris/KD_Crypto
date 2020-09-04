#ifndef CRYPTOCONTROL_H
#define CRYPTOCONTROL_H
#include <sys/stat.h>
#include <string>
const size_t NORMAL = 1;
const size_t READ_FILE_ERROR       = 2;   //文件读取错误，请检查权限
const size_t PROCESS_RUNNING_ERROR = 3;   //程序运行错误，请重试
const size_t VERIFY_ENCRYPTO_ERROR = 4;   //加密后的文件正确性校验错误
const size_t VERIFY_PLAIN_ERROR    = 5;   //解密后的文件正确性校验错误
const size_t OUT_FILE_EXIST_ERROR  = 6;   //输出的路径下存在同名的文件
class CryptoControl
{
public:
    CryptoControl();
    virtual bool encrypt();
    virtual bool decrypt();
    virtual bool isEffective();
    virtual size_t getEffective();
    virtual bool outFile(std::string outFullPath);
    virtual ~CryptoControl();
};

#endif // CRYPTOCONTROL_H
