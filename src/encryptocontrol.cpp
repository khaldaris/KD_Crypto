#include <encryptocontrol.h>

std::string get_file_name(std::string full_path)
{
    return full_path.substr(full_path.find_last_of('/')+1);
}

void EnCryptoControl::process_data(const std::string& in_full_path, const std::string& out_full_path, const std::string& key)
{
    this->key = std::vector<unsigned char>(key.begin(),key.end());
    this->key.resize(32,'0');

    in_file(in_full_path,this->plain);

    std::string hash256_string = create_hash256_string(this->plain.begin(), this->plain.end());
    this->plain.insert(this->plain.end(),hash256_string.begin(),hash256_string.end());

    std::string file_name = get_file_name(in_full_path);
    std::vector<unsigned char> file_name_vec(file_name.begin(),file_name.end());
    file_name_vec.resize(1024,'\0');

    this->plain.insert(this->plain.end(),file_name_vec.begin(),file_name_vec.end());

    Aes256 aes(this->key);
    aes.encrypt(this->key,this->plain,this->encrypted);

    this->encrypted.insert(this->encrypted.begin(),FILE_MESSAGE_STRING.begin(),FILE_MESSAGE_STRING.end());

    std::string hash256_string_encrypted = create_hash256_string(this->encrypted.begin(), this->encrypted.end());
    this->encrypted.insert(this->encrypted.end(),hash256_string_encrypted.begin(),hash256_string_encrypted.end());

    out_file(out_full_path,this->encrypted);
}
