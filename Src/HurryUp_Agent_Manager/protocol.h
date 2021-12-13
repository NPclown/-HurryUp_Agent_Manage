#pragma once
#include <cppcore.h>

enum PROTOCOL {
    AGENT_INIT = 100,
    AGENT_START,
    AGENT_STOP,
    AGENT_UPDATE,
    AGENT_STATUS,
    ERROR = 500
};

// 메시지 전송에 필요한 메인 구조
struct ST_PACKET_INFO : public core::IFormatterObject
{
    int protocol;
    std::tstring data;

    ST_PACKET_INFO(void)
    {}
    ST_PACKET_INFO(int _protocol, std::tstring _data)
        : protocol(_protocol), data(_data)
    {}

    void OnSync(core::IFormatter& formatter)
    {
        formatter
            + core::sPair(TEXT("protocol"), protocol)
            + core::sPair(TEXT("data"), data)
            ;
    }
};

struct ST_ENV_INFO : public core::IFormatterObject
{
    std::tstring serverIp;
    std::tstring serverPort;
    std::tstring serverFilePort;
    std::tstring serialNumber;
    std::tstring environment;

    ST_ENV_INFO(void)
    {}
    ST_ENV_INFO(std::tstring _serverIp, std::tstring _serverPort, std::tstring _serverFilePort, std::tstring _serialNumber, std::tstring _environment)
        : serverIp(_serverIp), serverPort(_serverPort), serverFilePort(_serverFilePort), serialNumber(_serialNumber), environment(_environment)
    {}

    void OnSync(core::IFormatter& formatter)
    {
        formatter
            + core::sPair(TEXT("server_ip"), serverIp)
            + core::sPair(TEXT("server_port"), serverPort)
            + core::sPair(TEXT("server_file_port"), serverFilePort)
            + core::sPair(TEXT("serial_number"), serialNumber)
            + core::sPair(TEXT("environment"), environment)
            ;
    }
};