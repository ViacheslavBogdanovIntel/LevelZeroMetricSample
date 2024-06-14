#pragma once
#include "common.h"
#include "NPUDevice.h"

using DeviceContextList = std::vector<DeviceDescriptor>;

class NPUCollector
{
public:
    NPUCollector(ze_init_flag_t init_flag);
    virtual ~NPUCollector();
    const std::vector<NPUDevice*> GetDevices() const { return m_devices; }

private:
    DeviceContextList FindAllDevices(ze_driver_handle_t pDriver);

private:
    std::vector<NPUDevice*> m_devices;
    DeviceContextList       m_device_handle_list;
    ze_driver_properties_t  m_driver_properties{};
    ze_api_version_t        m_version{};
};
