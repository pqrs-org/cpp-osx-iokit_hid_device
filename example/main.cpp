#include <IOKit/hid/IOHIDElement.h>
#include <IOKit/hid/IOHIDUsageTables.h>
#include <csignal>
#include <pqrs/osx/iokit_hid_device.hpp>
#include <pqrs/osx/iokit_hid_manager.hpp>

namespace {
auto global_wait = pqrs::make_thread_wait();
}

int main(void) {
  pqrs::dispatcher::extra::initialize_shared_dispatcher();

  std::signal(SIGINT, [](int) {
    global_wait->notify();
  });

  std::vector<pqrs::cf_ptr<CFDictionaryRef>> matching_dictionaries{
      pqrs::osx::iokit_hid_manager::make_matching_dictionary(
          pqrs::osx::iokit_hid_usage_page_generic_desktop,
          pqrs::osx::iokit_hid_usage_generic_desktop_keyboard),
  };

  auto hid_manager = std::make_unique<pqrs::osx::iokit_hid_manager>(pqrs::dispatcher::extra::get_shared_dispatcher(),
                                                                    matching_dictionaries);

  hid_manager->device_matched.connect([](auto&& registry_entry_id, auto&& device_ptr) {
    if (device_ptr) {
      auto hid_device = pqrs::osx::iokit_hid_device(*device_ptr);
      std::cout << "device_matched registry_entry_id:" << registry_entry_id << std::endl;
      if (auto manufacturer = hid_device.find_manufacturer()) {
        std::cout << "  manufacturer:" << *manufacturer << std::endl;
      }
      if (auto product = hid_device.find_product()) {
        std::cout << "  product:" << *product << std::endl;
      }
      if (auto serial_number = hid_device.find_serial_number()) {
        std::cout << "  serial_number:" << *serial_number << std::endl;
      }
      if (auto vendor_id = hid_device.find_vendor_id()) {
        std::cout << "  vendor_id:" << *vendor_id << std::endl;
      }
      if (auto product_id = hid_device.find_product_id()) {
        std::cout << "  product_id:" << *product_id << std::endl;
      }

#if 0
      std::cout << "  ";
      for (const auto& e : hid_device.make_elements()) {
        std::cout << IOHIDElementGetUsagePage(*e) << "," << IOHIDElementGetUsage(*e)
                  << " (" << IOHIDElementGetLogicalMin(*e) << " - " << IOHIDElementGetLogicalMax(*e) << "), ";
      }
      std::cout << std::endl;
#endif
    }
  });

  hid_manager->error_occurred.connect([](auto&& message, auto&& iokit_return) {
    std::cerr << "error_occurred " << message << " " << iokit_return << std::endl;
  });

  hid_manager->async_start();

  // ============================================================

  global_wait->wait_notice();

  // ============================================================

  hid_manager = nullptr;

  pqrs::dispatcher::extra::terminate_shared_dispatcher();

  std::cout << "finished" << std::endl;

  return 0;
}
