#include <cstdarg>

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

#include "mock_linux_socket.h"
#include "mock_linux_ioctl.h"

#include <can_interface_lib/can.h>

MockLinuxSocket c_mock_linux_socket;
MockLinuxIoctl c_mock_linux_ioctl;

extern "C"
{
    int socket(int domain, int type, int protocol)
    {
        return c_mock_linux_socket.socket(domain, type, protocol);
    }

    int bind(int fd, const struct sockaddr *addr, socklen_t len)
    {
        return c_mock_linux_socket.bind(fd, addr, len);
    }

    int close(int fd)
    {
        return c_mock_linux_socket.close(fd);
    }

    int ioctl(int fd, unsigned long int request, ...)
    {
        va_list var_arg_list;
        va_start(var_arg_list, request);
        ifreq *interface_index_pointer = va_arg(var_arg_list, ifreq *);
        va_end(var_arg_list);
        return c_mock_linux_ioctl.ioctl(fd, request, interface_index_pointer);
    }
};

SCENARIO("can test")
{
    using trompeloeil::_;
    using trompeloeil::eq;
    constexpr int MOCK_SOCKET{1};
    constexpr int MOCK_CAN_INTERFACE_INDEX{0};
    constexpr int RESULT_SUCCESS{0};
    constexpr int RESULT_FAILURE{-1};

    GIVEN("A CAN object")
    {
        auto can = can_interface_lib::makeCanInterface();

        WHEN("disconnect is called before being connected")
        {
            const auto success = can->disconnect();
            THEN("it fails")
            {
                CHECK_FALSE(success);
            }
        }

        WHEN("send is called before being connected")
        {
            const auto can_frame = can_interface_lib::makeCanFrame();
            const auto success = can->send(can_frame);
            THEN("it fails")
            {
                CHECK_FALSE(success);
            }
        }

        WHEN("receive is called before being connected")
        {
            auto can_frame = can_interface_lib::makeCanFrame();
            const auto success = can->receive(can_frame);
            THEN("it fails")
            {
                CHECK_FALSE(success);
            }
        }

        WHEN("connect is called")
        {
            REQUIRE_CALL(c_mock_linux_socket, socket(eq(PF_CAN), eq(SOCK_RAW), eq(CAN_RAW))).RETURN(MOCK_SOCKET);
            REQUIRE_CALL(c_mock_linux_ioctl, ioctl(eq(MOCK_SOCKET), eq(SIOCGIFINDEX), _)).LR_SIDE_EFFECT(_3->ifr_ifindex = MOCK_CAN_INTERFACE_INDEX).RETURN(RESULT_SUCCESS);
            REQUIRE_CALL(c_mock_linux_socket, bind(eq(MOCK_SOCKET), _, sizeof(sockaddr_can))).RETURN(RESULT_SUCCESS);

            const auto connect_success = can->connect("can0");
            THEN("it suceeds")
            {
                CHECK(connect_success);
                AND_WHEN("disconnect is called")
                {
                    REQUIRE_CALL(c_mock_linux_socket, close(eq(MOCK_SOCKET))).RETURN(RESULT_SUCCESS);

                    const auto disconnect_success = can->disconnect();
                    THEN("it suceeds")
                    {
                        CHECK(disconnect_success);
                    }
                }
            }
        }
    }
}