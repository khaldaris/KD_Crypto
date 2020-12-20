#ifndef DECRYPTOCONTROL_H
#define DECRYPTOCONTROL_H

#include <cryptocontrol.h>

class DeCryptoControl : public CryptoControl
{
public:
    DeCryptoControl(){};
    void process_data(const std::string& in_full_path,const std::string& out_full_path,const std::string& key);
};

#endif // DECRYPTOCONTROL_H
