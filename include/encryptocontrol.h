#ifndef ENCRYPTOCONTROL_H
#define ENCRYPTOCONTROL_H
#include <vector>

#include <cryptocontrol.h>

class EnCryptoControl:public CryptoControl
{
public:
    EnCryptoControl(std::string inFullPathAndName,std::string key,size_t version);
    ~EnCryptoControl(){};
    bool encrypt();
    bool isEffective();
    size_t getEffective();
    bool outFile(std::string outFullPath);

private:

    bool readFile(std::string inFullPathAndName);
    bool createHeadMessage(std::vector<unsigned char> &headMessage);
    size_t effective;
    size_t version;
    std::string fileName;
    size_t plainLength;
    size_t encryptedLength;
    std::vector<unsigned char> key;
    std::vector<unsigned char> plain;
    std::vector<unsigned char> encrypted;
};

#endif // ENCRYPTOCONTROL_H
