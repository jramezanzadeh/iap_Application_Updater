#include "helpertfunc.h"

HelpertFunc::HelpertFunc()
{

}

QByteArray HelpertFunc::calcMac(QByteArray data)
{
    uint8_t mk[8]  = { 0xAC, 0x16, 0x9A, 0x3B, 0xE7, 0xFC, 0xC2, 0xEC };
    uint8_t mk1[8]  = { 0xD8, 0x5F, 0x17, 0x28, 0xF7, 0x7F, 0x8D, 0xCA };
    uint8_t mac[8] = { 0xA1, 0xBF, 0xC2, 0x89, 0xEB, 0x77, 0x00, 0xAD};

    for (int i = 0; i < data.size(); i++) {
        mac[0] = mk[0] * data[i] + mk[1] * mac[3] + mk[7] * 3 + (mac[0] ^ data[i]);
        mac[1] = mk[1] * data[i] + mk[2] * mac[2] + mk[6] * 5 + (mac[1] ^ data[i]);
        mac[2] = mk[2] * data[i] + mk[3] * mac[1] + mk[5] * 7 + (mac[2] ^ data[i]);
        mac[3] = mk[3] * data[i] + mk[0] * mac[0] + mk[4] * 11 +(mac[3] ^ data[i]);
        mac[4] = mk1[0] * data[i] + mk1[1] * mac[7] + mk1[7] * 13 + (mac[4] ^ data[i]);
        mac[5] = mk1[1] * data[i] + mk1[2] * mac[6] + mk1[6] * 17 + (mac[5] ^ data[i]);
        mac[6] = mk1[2] * data[i] + mk1[3] * mac[5] + mk1[5] * 19 + (mac[6] ^ data[i]);
        mac[7] = mk1[3] * data[i] + mk1[0] * mac[4] + mk1[4] * 23 +(mac[7] ^ data[i]);
    }
    QByteArray outMac(reinterpret_cast<char *>(mac));
    return outMac;
}
