#include <decryptocontrol.h>
#include <assert.h>
#include <aes256.h>
#include <picosha2.h>
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
DeCryptoControl::DeCryptoControl(std::string fullPathAndName,std::string key)
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
    this->fileName = "";
    this->version = 0;
    readFile(fullPathAndName);
}
bool DeCryptoControl::readFile(std::string inFullPathAndName)
{
    FILE* inFile = fopen(inFullPathAndName.c_str(),"rb");
    if(!inFile)
    {
        this->effective = READ_FILE_ERROR;
        return false;
    }
    fseek(inFile, 0, SEEK_END);
    this->encryptedLength = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);

    this->encrypted.clear();
    this->encrypted.reserve(this->encryptedLength);
    unsigned char buffer[1]={0};
    for(size_t i = 0;i<this->encryptedLength;i++)
    {
        fread(buffer,sizeof (unsigned char),1,inFile);
        this->encrypted.push_back(buffer[0]);
    }

    fclose(inFile);
    std::vector<unsigned char> headMessage(this->encrypted.begin()+this->encrypted.size()-1024,this->encrypted.end());
    for(int i = 0;i<1024;i++)
    {
        this->encrypted.pop_back();
    }
    verifyHeadMessage(headMessage);
    return true;
}

bool DeCryptoControl::verifyHeadMessage(std::vector<unsigned char> &headMessage)
{
    //然后512~512+128-1位为版本信息
    this->version = headMessage[512+128-1];
    //前512位为文件名
    for(size_t i = 0; i<512;i++)
    {
        this->fileName = std::string(headMessage.begin(),headMessage.begin()+512);
    }
    //然后512+128~512+128+64-1位为原文件的hash校验码
    std::vector<unsigned char> verifyEncryptoSHA256Code(headMessage.begin()+512+128,headMessage.begin()+512+128+64);
    std::string encryptedSHA256Code = getSHA256Code(this->encrypted.begin(),this->encrypted.end());
    for(int i = 0;i<64;i++)
    {
        if(encryptedSHA256Code[i] != verifyEncryptoSHA256Code[i])
        {
            this->effective = VERIFY_ENCRYPTO_ERROR;
            return false;
        }
    }
    return true;
}
bool DeCryptoControl::decrypt()
{
    if(this->effective!=NORMAL)
    {
        return false;
    }
    Aes256 aes(this->key);
    aes.decrypt(this->key,this->encrypted,this->plain);

    std::vector<unsigned char> plainSHA256Code(this->plain.begin()+this->plain.size()-64,this->plain.end());
    std::string verifyPlainSHA256Code = getSHA256Code(this->plain.begin(),this->plain.begin()+this->plain.size()-64);
    for(int i = 0;i<64;i++)
    {
        if(plainSHA256Code[i] != verifyPlainSHA256Code[i])
        {
            this->effective = VERIFY_PLAIN_ERROR;
            return false;
        }
    }
    for(int i = 0;i<64;i++)
    {
        this->plain.pop_back();
    }

    return true;
}

bool DeCryptoControl::isEffective()
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

size_t DeCryptoControl::getEffective()
{
    return this->effective;
}

bool DeCryptoControl::outFile(std::string outFullPath)
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
    FILE* outFile = fopen((outFullPath+this->fileName).c_str(),"wb");
    if(!outFile)
    {
        this->effective = READ_FILE_ERROR;
        return false;
    }
    unsigned char buffer[1]={0};

    for(auto it = this->plain.begin();it!=this->plain.end();it++)
    {
        buffer[0] = *it;
        fwrite(buffer,sizeof (unsigned char),1,outFile);
    }

    fclose(outFile);
    return true;
}
