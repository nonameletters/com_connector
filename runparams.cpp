#include "runparams.h"

CRunParams::CRunParams(int id, QString description, void_f exec_f, int argc, char *argv[]) :
    m_id(id), m_description(description), f_function(exec_f), m_argc(argc), m_argv(argv)
{

}

// ---------- ---------- ---------- ---------- ---------- ----------
void CRunParams::exec(CRunParams p)
{
    p.f_function();
}

// ---------- ---------- ---------- ---------- ---------- ----------
void CRunParams::execIfParam(CRunParams p)
{
    if (p.m_argc < 2)
    {
        CRunParams::print(p);
    }
    else if (p.m_id == stoi((p.m_argv)[1]))
    {
        p.f_function();
    }
}

