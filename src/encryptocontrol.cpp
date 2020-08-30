#include <encryptocontrol.h>
#include <assert.h>
#include <aes256.h>
#include <picosha2.h>
std::string getFileName(std::string fullPathAndName)
{
    return fullPathAndName.substr(fullPathAndName.find_last_of('/')+1);
}
template <typename RaIter>
std::string getSHA256Code(RaIter first, RaIter last)
{
    picosha2::hash256_one_by_one hasher;
    hasher.init();
    hasher.process(first,last);
    hasher.finish();
    std::string hex_str;
    picosha2::get_hash_hex_string(hasher, hex_str);
    return hex_str;
}
EnCryptoControl::EnCryptoControl(std::string inFullPathAndName, std::string key, size_t version)
{
    this->key.clear();
    this->plain.clear();
    this->encrypted.clear();
    this->plainLength = 0;
    this->encryptedLength = 0;
    this->effective = NORMAL;
    for(size_t i = 0;i<key.length();i++)
    {
        this->key.reserve(key.length());
        this->key.push_back(key[i]);
    }
    this->fileName = getFileName(inFullPathAndName);
    this->version = version;
    readFile(inFullPathAndName);
}

bool EnCryptoControl::readFile(std::string inFullPathAndName)
{
    FILE* inFile = fopen(inFullPathAndName.c_str(),"rb");
    if(!inFile)
    {
        return false;
        this->effective = READ_FILE_ERROR;
    }
    fseek(inFile, 0, SEEK_END);
    this->plainLength = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);

    this->plain.clear();
    this->plain.reserve(this->plainLength);
    for(size_t i = 0;i<this->plainLength;i++)
    {
        unsigned char buffer[1]={0};
        fread(buffer,sizeof (unsigned char),1,inFile);
        this->plain.push_back(buffer[0]);
    }
    fclose(inFile);
    std::string plainSHA256Code = getSHA256Code(this->plain.begin(), this->plain.end());
    for(int i = 0;i<64;i++)
    {
        this->plain.push_back(plainSHA256Code[i]);
    }
    return true;
}

bool EnCryptoControl::createHeadMessage(std::vector<unsigned char> &headMessage)
{
    //前512位为文件名
    for(size_t i = 0; i< (this->fileName.length());i++)
    {
        headMessage[i] = this->fileName[i];
    }
    //然后512~512+128-1位为版本信息
    headMessage[512+128-1] = version;
    //然后512+128~512+128+64-1位为原文件的hash校验码
    std::string encryptedSHA256Code = getSHA256Code(this->encrypted.begin(), this->encrypted.end());
    for(size_t i = 0; i<64;i++)
    {
        headMessage[512+128+i] = encryptedSHA256Code[i];
    }
    return true;
}

bool EnCryptoControl::encrypt()
{
    if(this->effective!=NORMAL)
    {
        return false;
    }
    Aes256 aes(this->key);
    aes.encrypt(this->key,this->plain,this->encrypted);
    std::vector<unsigned char> headMessage(1024,0);
    createHeadMessage(headMessage);
    this->encrypted.insert(this->encrypted.end(),headMessage.begin(),headMessage.end());
    this->encryptedLength = this->encrypted.size();
    return true;
}

bool EnCryptoControl::isEffective()
{
    if(this->effective==NORMAL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

size_t EnCryptoControl::getEffective()
{
    return this->effective;
}

bool EnCryptoControl::outFile(std::string outFullPath)
{
    if(this->effective!=NORMAL)
    {
        return false;
    }
    struct stat fileStatus;
    if(stat ((outFullPath+this->fileName+std::string(".KD_Crypto")).c_str(), &fileStatus) == 0)
    {
        this->effective = OUT_FILE_EXIST_ERROR;
        return false;
    }
    FILE* outFile = fopen((outFullPath+this->fileName+std::string(".KD_Crypto")).c_str(),"wb");
    if(!outFile)
    {
        this->effective = READ_FILE_ERROR;
        return false;
    }
    unsigned char buffer[1]={0};
    for(auto it = this->encrypted.begin();it!=this->encrypted.end();it++)
    {
        buffer[0] = *it;
        fwrite(buffer,sizeof (unsigned char),1,outFile);
    }
    fclose(outFile);
    return true;
}
