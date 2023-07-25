#ifndef EBS_UTILITY_H
#define EBS_UTILITY_H

#include <vector>
#include <string>
#include <set>
#include <boost/asio.hpp>
#include <memory>
#include <QString>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

extern asio::io_context g_ioc;

struct BroadcastInfo
{
    std::string ebm_id;
    std::vector<std::string> play_address;
    uint32_t start_time;
    uint32_t end_time;

    bool operator<(const BroadcastInfo& other) const
    {
        return ebm_id < other.ebm_id;
    }
};

struct Device
{
    std::string device_id;
    std::string resource_id;

    std::set<BroadcastInfo> broadcastInfos;

    std::shared_ptr<tcp::socket> dSocket; //附录D
    std::vector<uint8_t> dSocketReadBuffer;
    asio::deadline_timer dHeartBeatTimer{g_ioc};
    asio::deadline_timer eHeartBeatTimer{g_ioc};
    asio::deadline_timer ePrivate0x1Timer{g_ioc};
};

extern std::vector<std::shared_ptr<Device>> g_all_devices;
extern std::mutex g_all_devices_mutex;

bool AddDevice( std::shared_ptr<Device> device );
bool ReadFromExcel( const QString& file);

void StartDHeartBeat(std::shared_ptr<Device> device);
void StartEHeartBeat(std::shared_ptr<Device> device);
void StartEPri_0x1(std::shared_ptr<Device> device);

void StartReadMsg(std::shared_ptr<Device> device);
void DoReadMsg(std::shared_ptr<std::vector<uint8_t>> read_buf, std::shared_ptr<Device> device);
void parseIns(std::shared_ptr<Device> device);

struct EMsg;
void SendEMsg(std::shared_ptr<Device> device, EMsg& msg);
void ResendEMsg(tcp::endpoint ep, std::shared_ptr<std::vector<uint8_t>> buffer, int count = 1);

#endif // EBS_UTILITY_H
