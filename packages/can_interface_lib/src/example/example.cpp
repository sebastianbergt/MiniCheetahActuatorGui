#include <can_interface_lib/can.h>
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  namespace cil = ::can_interface_lib;

  auto can = cil::makeCanInterface();
  can->connect("vcan0");

  auto sent_frame = cil::CanFrame{};
  sent_frame.id = cil::CanId(0x123);
  sent_frame.data =
      cil::CanData{{0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x00, 0x00}};
  sent_frame.bytes_used = cil::BytesUsed(4U);
  can->send(sent_frame);

  can->filter(cil::CanId(0x123), cil::CanMask{0xFFF});

  // wait for a frame to be received with the filtered CanId 0x123
  auto receive_frame = cil::makeCanFrame();
  while (!can->receive(receive_frame)) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::cout << "received frame with bytes:\n";
  for (const auto &byte : receive_frame.data) {
    std::cout << std::hex << byte << " ";
  }
  std::cout << "\n";

  return EXIT_SUCCESS;
}
