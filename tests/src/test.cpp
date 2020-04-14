#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/osx/iokit_hid_device.hpp>

TEST_CASE("iokit_hid_device") {
  pqrs::osx::iokit_hid_device hid_device(nullptr);

  REQUIRE(!hid_device.get_device());
  REQUIRE(!hid_device.conforms_to(pqrs::osx::iokit_hid_usage_page::generic_desktop,
                                  pqrs::osx::iokit_hid_usage::generic_desktop::keyboard));
  REQUIRE(hid_device.find_int64_property(CFSTR(kIOHIDVendorIDKey)) == std::nullopt);
  REQUIRE(hid_device.find_string_property(CFSTR(kIOHIDManufacturerKey)) == std::nullopt);
  REQUIRE(hid_device.make_elements().empty());
  REQUIRE(!hid_device.make_queue(1024));
}
