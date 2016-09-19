#ifndef CSTARTER_H
#define CSTARTER_H

#include <QString>
#include <QTimer>
#include <QThread>
#include <vector>

#include <QTextCodec>
#include <QFile>
#include <QtCore/QCoreApplication>
#include "qtservice.h"

#include "constansts.h"
#include "dbconnector.h"
#include "ccon_file.h"
#include "logger.h"

#include "commonfunction.h"

using namespace ns_logger;
using namespace db_connector;

const QString APPNAME = "TimeControlSyncService";
const QString ORGANIZATION = "Inside development";
const QString DESCRIPTION = "Syncronizing persons from 1C and creation access profiles in TimeControl";

class CStarter : public QtService<QCoreApplication>
{
    public:
        CStarter(int argc, char *argv[]);

    protected:
        void start();
        void stop();

    private:

        QTimer* l_profileCreation;
        QTimer* l_userSync;
};

#endif // CSTARTER_H
