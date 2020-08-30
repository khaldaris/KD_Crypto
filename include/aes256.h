#ifndef AES256_CPP
#define AES256_CPP

#include <vector>

#define BLOCK_SIZE 16

class Aes256 {

    public:
        Aes256(const std::vector<unsigned char>& key);
        ~Aes256();

        static std::vector<unsigned char>::size_type encrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& plain, std::vector<unsigned char>& encrypted);
        static std::vector<unsigned char>::size_type encrypt(const std::vector<unsigned char>& key, const unsigned char* plain, const std::vector<unsigned char>::size_type plain_length, std::vector<unsigned char>& encrypted);
        static std::vector<unsigned char>::size_type decrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& encrypted, std::vector<unsigned char>& plain);
        static std::vector<unsigned char>::size_type decrypt(const std::vector<unsigned char>& key, const unsigned char* encrypted, const std::vector<unsigned char>::size_type encrypted_length, std::vector<unsigned char>& plain);

        std::vector<unsigned char>::size_type encrypt_start(const std::vector<unsigned char>::size_type plain_length, std::vector<unsigned char>& encrypted);
        std::vector<unsigned char>::size_type encrypt_continue(const std::vector<unsigned char>& plain, std::vector<unsigned char>& encrypted);
        std::vector<unsigned char>::size_type encrypt_continue(const unsigned char* plain, const std::vector<unsigned char>::size_type plain_length, std::vector<unsigned char>& encrypted);
        std::vector<unsigned char>::size_type encrypt_end(std::vector<unsigned char>& encrypted);

        std::vector<unsigned char>::size_type decrypt_start(const std::vector<unsigned char>::size_type encrypted_length);
        std::vector<unsigned char>::size_type decrypt_continue(const std::vector<unsigned char>& encrypted, std::vector<unsigned char>& plain);
        std::vector<unsigned char>::size_type decrypt_continue(const unsigned char* encrypted, const std::vector<unsigned char>::size_type encrypted_length, std::vector<unsigned char>& plain);
        std::vector<unsigned char>::size_type decrypt_end(std::vector<unsigned char>& plain);

    private:
        std::vector<unsigned char>            m_key;
        std::vector<unsigned char>            m_salt;
        std::vector<unsigned char>            m_rkey;

        unsigned char        m_buffer[3 * BLOCK_SIZE];
        unsigned char        m_buffer_pos;
        std::vector<unsigned char>::size_type m_remainingLength;

        bool                 m_decryptInitialized;

        void check_and_encrypt_buffer(std::vector<unsigned char>& encrypted);
        void check_and_decrypt_buffer(std::vector<unsigned char>& plain);

        void encrypt(unsigned char *buffer);
        void decrypt(unsigned char *buffer);

        void expand_enc_key(unsigned char *rc);
        void expand_dec_key(unsigned char *rc);

        void sub_bytes(unsigned char *buffer);
        void sub_bytes_inv(unsigned char *buffer);

        void copy_key();

        void add_round_key(unsigned char *buffer, const unsigned char round);

        void shift_rows(unsigned char *buffer);
        void shift_rows_inv(unsigned char *buffer);

        void mix_columns(unsigned char *buffer);
        void mix_columns_inv(unsigned char *buffer);
};

#endif /* AES256_CPP */
