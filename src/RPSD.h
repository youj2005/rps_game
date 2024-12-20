#ifndef RPSD_H
#define RPSD_H
#include <mutex>

enum move : uint8_t{
    ROCK = 1,
    PAPER = 2,
    SCISS = 3
};

namespace rpsd
{
    class RPSD
    {
        public:
            RPSD();
            ~RPSD();
            move play(move sel);
            move read();
        private:
            std::mutex mtx;
            move m_Val;
    };
}

#endif