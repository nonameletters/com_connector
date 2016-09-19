#ifndef CRUNPARAMS_H
#define CRUNPARAMS_H

#include <functional>
#include <QString>
#include <iostream>

using namespace std;

typedef function<void()> void_f;

class CRunParams
{
    public:
        CRunParams(int id, QString description, void_f exec_f, int argc, char *argv[]);
        int     m_id;
        QString m_description;
        void_f  f_function;
        int     m_argc;
        char**  m_argv;

        static void exec(CRunParams p);
        static void execIfParam(CRunParams p);
        static void print(CRunParams p) {cout << p.m_id << " - " << p.m_description.toStdString() << endl;}

};

#endif // CRUNPARAMS_H
