#ifndef CRYPTOCONTROL_H
#define CRYPTOCONTROL_H
#include <sys/stat.h>
#include <picosha2.h>
#include <aes256.h>
#include <exceptions.h>

const std::string FILE_MESSAGE_STRING = std::string("6A756E68656E677A686964616F70696E6768656E677A68696A69616E00000001");
const unsigned char FILE_MESSAGE_ARRAY[65] = "6A756E68656E677A686964616F70696E6768656E677A68696A69616E00000001";

class CryptoControl
{
public:
    CryptoControl();
    virtual void process_data(const std::string& in_full_path,const std::string& out_full_path,const std::string& key) = 0;
    virtual ~CryptoControl();
    static int is_decrypto_file(const std::string& file_name);
protected:
    void in_file(const std::string& in_path, const std::string& file_name,std::vector<unsigned char>& result_vec);
    void in_file(const std::string& in_full_path,std::vector<unsigned char>& result_vec);
    void out_file(const std::string& out_path, const std::string& file_name,std::vector<unsigned char>& result_vec);
    void out_file(const std::string& out_full_path,std::vector<unsigned char>& result_vec);
    std::string& format_string_to_full_path(std::string& path, const std::string& file_name);
    template <typename RaIter>
    std::string create_hash256_string (RaIter first, RaIter last)
    {
        picosha2::hash256_one_by_one hasher;
        hasher.init();
        hasher.process(first,last);
        hasher.finish();
        std::string hex_str;
        picosha2::get_hash_hex_string(hasher, hex_str);
        return hex_str;
    }
protected:
    std::vector<unsigned char> key;
    std::vector<unsigned char> plain;
    std::vector<unsigned char> encrypted;
};

#endif // CRYPTOCONTROL_H
