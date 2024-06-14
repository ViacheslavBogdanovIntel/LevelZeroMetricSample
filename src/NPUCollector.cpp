#include "NPUCollector.h"


NPUCollector::NPUCollector(ze_init_flag_t init_flag)
{
    ze_result_t result;
    result = zeInit(init_flag);
    if (result != ZE_RESULT_SUCCESS)
    {
        return;
    }

    // Get Device_list
    uint32_t driver_count = 0;
    result = zeDriverGet(&driver_count, nullptr);
    if (result != ZE_RESULT_SUCCESS)
    {
        return;
    }

    std::vector<ze_driver_handle_t> drivers(driver_count);
    result = zeDriverGet(&driver_count, drivers.data());
    if (result != ZE_RESULT_SUCCESS)
    {
        return;
    }

    for (const auto driver : drivers)
    {
        auto found_devices = FindAllDevices(driver);
        std::move(found_devices.begin(), found_devices.end(), std::back_inserter(m_device_handle_list));
    }

    // initiate device objects with the device_handles
    for (size_t i = 0; i < m_device_handle_list.size(); i++)
    {
        m_devices.push_back(new NPUDevice(m_device_handle_list[i]));
    }
}

NPUCollector::~NPUCollector()
{
    for (auto d : m_devices)
    {
        delete d;
    }
    m_devices.clear();
}

DeviceContextList NPUCollector::FindAllDevices(ze_driver_handle_t driver_handle)
{
    uint32_t deviceCount = 0;
    zeDeviceGet(driver_handle, &deviceCount, nullptr);
    std::vector<ze_device_handle_t> devices(deviceCount);
    zeDeviceGet(driver_handle, &deviceCount, devices.data());

    m_driver_properties.stype = ZE_STRUCTURE_TYPE_DRIVER_PROPERTIES;
    zeDriverGetProperties(driver_handle, &m_driver_properties);
    zeDriverGetApiVersion(driver_handle, &m_version);

    // for each device, find the first one matching the type
    DeviceContextList found;
    for (uint32_t device = 0; device < deviceCount; ++device)
    {
        auto                   device_handle = devices[device];
        ze_device_properties_t device_properties = {};
        device_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES;
        if (zeDeviceGetProperties(device_handle, &device_properties) == ZE_RESULT_SUCCESS)
        {
            // Create context
            ze_context_handle_t context;
            ze_context_desc_t context_desc = { ZE_STRUCTURE_TYPE_CONTEXT_DESC, nullptr, 0 };
            zeContextCreate(driver_handle, &context_desc, &context);
            found.push_back({ device_handle, context, device_properties });
        }
    }
    return found;
}
