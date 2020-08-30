#ifndef CRYPTOCONTROL_H
#define CRYPTOCONTROL_H
#include <sys/stat.h>
#include <string>
const size_t NORMAL = 1;
const size_t READ_FILE_ERROR = 2;
const size_t PROCESS_RUNNING_ERROR = 3;
const size_t VERIFY_ENCRYPTO_ERROR = 4;
const size_t VERIFY_PLAIN_ERROR = 5;
const size_t OUT_FILE_EXIST_ERROR = 6;
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
