#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>

using namespace std;


namespace ns_logger
{
    class CLogger
    {
        public:
            static CLogger* getInstance();
            void log(const char* mess);
            void close();
            ~CLogger();

        private:
            CLogger();

        private:
            static CLogger* m_instance;
            ofstream m_log;
    };
}
#endif // LOGGER_H
