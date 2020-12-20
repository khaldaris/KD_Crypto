#include <decryptocontrol.h>

std::string get_file_name(std::vector<unsigned char>& head_message)
{
    std::string file_name;
    for(size_t i = 0; i<512;i++)
    {
        file_name = std::string(head_message.begin(),head_message.begin()+512);
    }
    return file_name;
}

void DeCryptoControl::process_data(const std::string &in_full_path, const std::string &out_full_path, const std::string &key)
{
    this->key = std::vector<unsigned char>(key.begin(),key.end());
    this->key.resize(32,'0');

    in_file(in_full_path,this->encrypted);

    std::vector<unsigned char> hash256_string_encrypted_vec(this->encrypted.end()-64,this->encrypted.end());
    std::string hash256_string_encryptoed = create_hash256_string(this->encrypted.begin(),this->encrypted.end()-64);
    for(int i = 0;i<64;i++)
    {
        if(hash256_string_encrypted_vec[i] != hash256_string_encryptoed[i])
        {
            throw VerifyEncryptoError();
        }
    }

    this->encrypted.erase(this->encrypted.end()-64, this->encrypted.end());

    this->encrypted.erase(this->encrypted.begin(), this->encrypted.begin() + 64);

    Aes256 aes(this->key);
    aes.decrypt(this->key,this->encrypted,this->plain);

    std::string file_name;
    file_name = std::string(this->plain.end()-1024,this->plain.end());

    this->plain.erase(this->plain.end()-1024,this->plain.end());

    std::vector<unsigned char> plain_hash256(this->plain.end()-64,this->plain.end());
    std::string hash256_string = create_hash256_string(this->plain.begin(),this->plain.end()-64);
    for(int i = 0;i<64;i++)
    {
        if(plain_hash256[i] != hash256_string[i])
        {
            throw VerifyPlainError();
        }
    }

    this->plain.erase(this->plain.end()-64,this->plain.end());

    file_name = out_full_path + file_name;

    out_file(file_name,this->plain);
}
