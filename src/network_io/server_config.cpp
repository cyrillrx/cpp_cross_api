#include "server_config.h"

namespace network_io
{
    const std::string server_config::KEY_PORT = "port";
    const std::string server_config::KEY_POOL_SIZE = "pool_size";
    const std::string server_config::KEY_FILE_MANAGER_ROOT = "root";

    const unsigned short server_config::DEFAULT_PORT = 8082;
    const int server_config::DEFAULT_POOL_SIZE = 3;

    server_config::server_config(const std::string& configFile) : properties(configFile)
    {
        port_ = get_int(KEY_PORT, DEFAULT_PORT);
        pool_size_ = get_int(KEY_POOL_SIZE, DEFAULT_POOL_SIZE);
        root_ = get_string(KEY_FILE_MANAGER_ROOT);
    }

    server_config::~server_config()
    {
        set_int(KEY_PORT, port_);
        set_int(KEY_POOL_SIZE, pool_size_);
        set_string(KEY_FILE_MANAGER_ROOT, root_);
        save_properties();
    }

    unsigned short server_config::port() const { return port_; }

    int server_config::pool_size() const { return pool_size_; }

    std::string server_config::root() const { return root_; }
}