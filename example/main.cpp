#include <IOKit/hid/IOHIDElement.h>
#include <IOKit/hid/IOHIDUsageTables.h>
#include <csignal>
#include <pqrs/osx/iokit_hid_device.hpp>
#include <pqrs/osx/iokit_hid_manager.hpp>

namespace {
auto global_wait = pqrs::make_thread_wait();
}

int main(void) {
  std::signal(SIGINT, [](int) {
    global_wait->notify();
  });

  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  std::vector<pqrs::cf::cf_ptr<CFDictionaryRef>> matching_dictionaries{
      pqrs::osx::iokit_hid_manager::make_matching_dictionary(
          pqrs::osx::iokit_hid_usage_page_generic_desktop,
          pqrs::osx::iokit_hid_usage_generic_desktop_keyboard),
  };

  auto hid_manager = std::make_unique<pqrs::osx::iokit_hid_manager>(dispatcher,
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
      if (auto location_id = hid_device.find_location_id()) {
        std::cout << "  location_id:" << *location_id << std::endl;
      }
      if (auto country_code = hid_device.find_country_code()) {
        std::cout << "  country_code:" << *country_code << std::endl;
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

  hid_manager->device_terminated.connect([](auto&& registry_entry_id) {
    std::cout << "device_terminated registry_entry_id:" << registry_entry_id << std::endl;
  });

  hid_manager->error_occurred.connect([](auto&& message, auto&& kern_return) {
    std::cerr << "error_occurred " << message << " " << kern_return << std::endl;
  });

  hid_manager->async_start();

  // ============================================================

  global_wait->wait_notice();

  // ============================================================

  hid_manager = nullptr;

  dispatcher->terminate();
  dispatcher = nullptr;

  std::cout << "finished" << std::endl;

  return 0;
}
