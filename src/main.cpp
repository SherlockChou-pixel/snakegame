#include "Snake.h"
#include "EpollServer.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <ncurses.h>

int main(int argc, char* argv[])
{
    EpollServer server(8888);
    return 0;
}
