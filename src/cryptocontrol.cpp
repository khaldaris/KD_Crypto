#include "cryptocontrol.h"
#include <exception>

CryptoControl::CryptoControl():key({}),plain({}),encrypted({}){}

void CryptoControl::in_file(const std::string& in_path, const std::string& file_name, std::vector<unsigned char>& result_vec)
{
    auto str_in_path = std::string(in_path);
    auto str_file_name = std::string(file_name);
    in_file(format_string_to_full_path(str_in_path,str_file_name),result_vec);
}

void CryptoControl::in_file(const std::string& in_full_path, std::vector<unsigned char>& result_vec)
{
    FILE* file = fopen(in_full_path.c_str(),"rb");
    size_t file_length = 0;
    if(!file)
    {
        throw ReadFileError();
    }
#ifdef WIN32
    _fseeki64(file, 0, SEEK_END);
    file_length = _ftelli64(file);
    _fseeki64(file, 0, SEEK_SET);
#else
    fseeko64(file, 0, SEEK_END);
    file_length = ftello64(file);
    fseeko64(file, 0, SEEK_SET);
#endif
    result_vec.reserve(file_length);
    unsigned char buffer[1];
    for(size_t i = 0;i<file_length;i++)
    {
        fread(buffer,sizeof (unsigned char),1,file);
        result_vec.push_back(buffer[0]);
    }
    fclose(file);
}

void CryptoControl::out_file(const std::string &out_path, const std::string &file_name, std::vector<unsigned char> &result_vec)
{
    auto str_out_path = std::string(out_path);
    auto str_file_name = std::string(file_name);
    out_file(format_string_to_full_path(str_out_path,str_file_name),result_vec);
}

void CryptoControl::out_file(const std::string &out_full_path, std::vector<unsigned char> &result_vec)
{
    FILE* outFile = fopen(out_full_path.c_str(),"wb");
    if(!outFile)
    {
        throw WriteFileError();
    }

    unsigned char buffer[1];
    for(auto it = result_vec.begin();it!=result_vec.end();it++)
    {
        buffer[0] = *it;
        fwrite(buffer,sizeof (unsigned char),1,outFile);
    }

    fclose(outFile);
}

std::string &CryptoControl::format_string_to_full_path(std::string &path, const std::string &file_name)
{
    if('/' == path[-1]||'\\' == path[-1])
    {
        path += file_name;
        return path;
    }
    path += std::string("/");
    path += file_name;
    return path;
}

CryptoControl::~CryptoControl(){}

int CryptoControl::is_decrypto_file(const std::string& file_name)
{
    FILE* file = fopen(file_name.c_str(),"rb");
    size_t file_length = 0;
    if(!file)
    {
        throw ReadFileError();
    }
#ifdef WIN32
    _fseeki64(file, 0, SEEK_END);
    file_length = _ftelli64(file);
    _fseeki64(file, 0, SEEK_SET);
#else
    fseeko64(file, 0, SEEK_END);
    file_length = ftello64(file);
    fseeko64(file, 0, SEEK_SET);
#endif
    std::string headMessage;
    unsigned char buffer[64] = {'\0'};
    for(size_t i = 0;i<64;i++)
    {
        fread(&(buffer[i]),sizeof (unsigned char),1,file);
        if(buffer[i]!=FILE_MESSAGE_ARRAY[i])
        {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}
