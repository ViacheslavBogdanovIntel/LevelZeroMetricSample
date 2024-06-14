#pragma once
#include "common.h"
#include "NPUEngine.h"
#include "NPUMetricGroup.h"

class NPUMetricStreamer;

struct DeviceDescriptor
{
    ze_device_handle_t handle;
    ze_context_handle_t context;
    ze_device_properties_t props;
};

class NPUDevice
{
public:
    NPUDevice(DeviceDescriptor descriptor);
    virtual ~NPUDevice();

    std::string GetDeviceName() const { return m_device_properties.name; }
    ze_device_type_t GetType() const { return m_device_properties.type; }
    const std::vector<NPUEngine*>& GetEngines() const { return m_engines; }
    const std::vector<NPUMetricGroup*>& GetTimeBasedMetricGroups() const { return m_time_based_metric_groups; }
    const std::vector<NPUMetricGroup*>& GetEventBasedMetricGroups() const { return m_event_based_metric_groups; }

    ze_device_handle_t GetHandle() const { return m_device_handle; }
    ze_context_handle_t GetContext() const { return m_device_context; }

    bool ActivateMetricGroup(const std::string& metric_group_name);
    bool ActivateMetricGroup(NPUMetricGroup* group);
    bool DeactivateMetricGroup();

    NPUMetricStreamer* CreateMetricStreamer(NPUMetricGroup* group);

private:
    bool InitEngines();
    bool InitMetrics();

    bool AddMetricGroup(
        std::vector<NPUMetricGroup*>& target_group, 
        zet_metric_group_handle_t metric_group_handle, 
        zet_metric_group_sampling_type_flags_t sampling_type);

private:
    ze_device_handle_t      m_device_handle;
    ze_context_handle_t     m_device_context;
    ze_device_properties_t  m_device_properties;
    std::vector<NPUEngine*> m_engines;

    std::vector<NPUMetricGroup*> m_time_based_metric_groups;
    std::vector<NPUMetricGroup*> m_event_based_metric_groups;
};
