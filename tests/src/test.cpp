#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/osx/iokit_hid_device.hpp>

TEST_CASE("iokit_hid_device") {
  pqrs::osx::iokit_hid_device hid_device(nullptr);

  REQUIRE(!hid_device.get_device());
  REQUIRE(hid_device.find_number_property(CFSTR(kIOHIDVendorIDKey)) == std::nullopt);
  REQUIRE(hid_device.find_string_property(CFSTR(kIOHIDManufacturerKey)) == std::nullopt);
}
