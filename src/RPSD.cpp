#include "RPSD.h"
#include <iostream>

using namespace rpsd;

RPSD::RPSD()
{
    m_Val = ROCK;
}

RPSD::~RPSD()
{
}

move RPSD::play(move sel)
{
    mtx.lock();
    if (m_Val == ROCK && sel == PAPER)
    {
        m_Val = PAPER;
    }
    else if (m_Val == PAPER && sel == SCISS)
    {
        m_Val = SCISS;
    }
    else if (m_Val == SCISS && sel == ROCK)
    {
        m_Val = ROCK;
    }
    mtx.unlock();
    return read();
}

move RPSD::read()
{
    return m_Val;
}