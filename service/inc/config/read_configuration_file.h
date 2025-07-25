#ifndef READ_CONFIGURATION_FILE_H
#define READ_CONFIGURATION_FILE_H

#include <string>

namespace zinpass::config{

class ReadConfigurationFile{
    public:
        ReadConfigurationFile();
        ~ReadConfigurationFile();

        void read_config_dbpath_from_file(); // 从配置文件中读取数据库配置
        std::string get_db_path();

    private:
        std::string database_path;    // 数据库路径
};

}

#endif