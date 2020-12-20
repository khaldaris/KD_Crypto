#ifndef ENCRYPTOCONTROL_H
#define ENCRYPTOCONTROL_H

#include <cryptocontrol.h>

class EnCryptoControl : public CryptoControl
{
public:
    EnCryptoControl(){};
    void process_data(const std::string& in_full_path, const std::string& out_full_path, const std::string& key);
};

#endif // ENCRYPTOCONTROL_H
