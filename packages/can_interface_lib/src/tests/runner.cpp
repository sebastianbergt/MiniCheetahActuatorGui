#include <can_interface_lib/can.h>
#include <iostream>

int main()
{
    const auto names = can_interface_lib::getCanInterfaceNames();
    for (const auto &name : names)
    {
        std::cout << name << "\n";
    }
    return EXIT_SUCCESS;
}