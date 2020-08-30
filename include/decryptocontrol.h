#ifndef DECRYPTOCONTROL_H
#define DECRYPTOCONTROL_H
#include <vector>

#include <cryptocontrol.h>

class DeCryptoControl:public CryptoControl
{
public:
    DeCryptoControl(std::string fullPathAndName,std::string key);
    ~DeCryptoControl(){};
    bool decrypt();
    bool isEffective();
    size_t getEffective();
    bool outFile(std::string outFullPath);

private:

    bool readFile(std::string inFullPathAndName);
    bool verifyHeadMessage(std::vector<unsigned char> &headMessage);
    size_t effective;
    size_t version;
    std::string fileName;
    size_t plainLength;
    size_t encryptedLength;
    std::vector<unsigned char> key;
    std::vector<unsigned char> plain;
    std::vector<unsigned char> encrypted;
};

#endif // DECRYPTOCONTROL_H
