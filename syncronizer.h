#ifndef CSYNCRONIZER_H
#define CSYNCRONIZER_H

#include <QtCore/QCoreApplication>
#include "qtservice.h"

namespace db_connector
{
    class CSyncronizer : public QtService<QCoreApplication>
    {
        public:
            CSyncronizer(int argc, char *argv[]);

        protected:
            void start();
            void stop();
    };
}

#endif // CSYNCRONIZER_H
