#include <boost/ut.hpp>
#include <pqrs/osx/iokit_hid_device.hpp>

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "iokit_hid_device"_test = [] {
    pqrs::osx::iokit_hid_device hid_device(nullptr);

    expect(!hid_device.get_device());
    expect(!hid_device.get_service());
    expect(!hid_device.get_registry_entry());
    expect(!hid_device.conforms_to(pqrs::hid::usage_page::generic_desktop,
                                   pqrs::hid::usage::generic_desktop::keyboard));
    expect(hid_device.find_int64_property(CFSTR(kIOHIDVendorIDKey)) == std::nullopt);
    expect(hid_device.find_string_property(CFSTR(kIOHIDManufacturerKey)) == std::nullopt);
    expect(hid_device.find_max_input_report_size() == std::nullopt);
    expect(hid_device.find_vendor_id() == std::nullopt);
    expect(hid_device.find_product_id() == std::nullopt);
    expect(hid_device.find_country_code() == std::nullopt);
    expect(hid_device.find_location_id() == std::nullopt);
    expect(hid_device.find_manufacturer() == std::nullopt);
    expect(hid_device.find_product() == std::nullopt);
    expect(hid_device.find_serial_number() == std::nullopt);
    expect(hid_device.find_transport() == std::nullopt);
    expect(hid_device.find_device_address() == std::nullopt);
    expect(hid_device.make_elements().empty());
    expect(!hid_device.make_queue(1024));
  };

  return 0;
}
