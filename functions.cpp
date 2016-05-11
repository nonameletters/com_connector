#include "functions.h"

namespace nsComConnector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    QString getPrefixById(QString id)
    {
        QString l_prefix;

        if (id.compare("266") == 0)
        {
            l_prefix = "";
        }
        else if (id.compare("267") == 0)
        {
            l_prefix = "ДЭН";
        }
        else if (id.compare("268") == 0)
        {
            l_prefix = "ТД";
        }
        else if (id.compare("269") == 0)
        {
            l_prefix = "Э";
        }
        else if (id.compare("270") == 0)
        {
            l_prefix = "ФБ";
        }
        else
        {
            l_prefix = "";
        }

        return l_prefix;
    }
}
