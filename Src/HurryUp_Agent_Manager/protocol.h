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

struct ST_MODULE_INFO : public core::IFormatterObject
{
    std::tstring moduleIp;
    std::tstring serialNumber;

    ST_MODULE_INFO(void)
    {}
    ST_MODULE_INFO(std::tstring _moduleIp, std::tstring _serialNumber)
        : moduleIp(_moduleIp), serialNumber(_serialNumber)
    {}

    void OnSync(core::IFormatter& formatter)
    {
        formatter
            + core::sPair(TEXT("module_ip"), moduleIp)
            + core::sPair(TEXT("serial_number"), serialNumber)
            ;
    }
};

struct ST_ENV_INFO : public core::IFormatterObject
{
    std::tstring serverIp;
    std::tstring serverPort;
    std::tstring serialNumber;
    std::tstring environment;
    std::vector<ST_MODULE_INFO> modules;

    ST_ENV_INFO(void)
    {}
    ST_ENV_INFO(std::tstring _serverIp, std::tstring _serverPort, std::tstring _serialNumber, std::tstring _environment, std::vector<ST_MODULE_INFO> _modules)
        : serverIp(_serverIp), serverPort(_serverPort), serialNumber(_serialNumber), environment(_environment), modules(_modules)
    {}

    void OnSync(core::IFormatter& formatter)
    {
        formatter
            + core::sPair(TEXT("server_ip"), serverIp)
            + core::sPair(TEXT("server_port"), serverPort)
            + core::sPair(TEXT("serial_number"), serialNumber)
            + core::sPair(TEXT("environment"), environment)
            + core::sPair(TEXT("modules"), modules)
            ;
    }
};