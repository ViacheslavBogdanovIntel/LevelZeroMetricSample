#include "NPUDevice.h"
#include "NPUMetricStreamer.h"


NPUDevice::NPUDevice(DeviceDescriptor descriptor)
{
    m_device_handle = descriptor.handle;
    m_device_context = descriptor.context;
    m_device_properties = descriptor.props;

    InitEngines();
    InitMetrics();
}

NPUDevice::~NPUDevice()
{
    for (auto sm : m_time_based_metric_groups)
    {
        delete sm;
    }
    m_time_based_metric_groups.clear();

    for (auto em : m_event_based_metric_groups)
    {
        delete em;
    }
    m_event_based_metric_groups.clear();

    for (auto e : m_engines)
    {
        delete e;
    }
    m_engines.clear();
}

bool NPUDevice::InitEngines()
{
    ze_result_t status;
    uint32_t    engine_group_count = 0;
    status = zesDeviceEnumEngineGroups(m_device_handle, &engine_group_count, nullptr);
    m_engines.reserve(engine_group_count);
    std::vector<zes_engine_handle_t> engine_handles(engine_group_count);
    status = zesDeviceEnumEngineGroups(m_device_handle, &engine_group_count, engine_handles.data());
    if (status != ZE_RESULT_SUCCESS)
    {
        return false;
    }

    for (const auto engine_handle : engine_handles)
    {
        m_engines.push_back(new NPUEngine(engine_handle));
    }
    return true;
}

bool NPUDevice::InitMetrics()
{
    ze_result_t status;
    uint32_t metric_group_count = 0;
    status = zetMetricGroupGet(m_device_handle, &metric_group_count, nullptr);
    if (status != ZE_RESULT_SUCCESS || metric_group_count == 0)
    {
        return false;
    }

    std::vector<zet_metric_group_handle_t> metric_groups(metric_group_count);
    status = zetMetricGroupGet(m_device_handle, &metric_group_count, metric_groups.data());
    if (status == ZE_RESULT_SUCCESS)
    {
        for (auto mg : metric_groups)
        {
            AddMetricGroup(m_time_based_metric_groups, mg, ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_TIME_BASED);
            AddMetricGroup(m_event_based_metric_groups, mg, ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_EVENT_BASED);
        }
        return true;
    }
    return false;
}

bool NPUDevice::AddMetricGroup(
    std::vector<NPUMetricGroup*>& target_group,
    zet_metric_group_handle_t metric_group_handle,
    zet_metric_group_sampling_type_flags_t sampling_type)
{
    ze_result_t status;
    zet_metric_group_properties_t	metric_group_properties{};
    metric_group_properties.stype = ZET_STRUCTURE_TYPE_METRIC_GROUP_PROPERTIES;
    status = zetMetricGroupGetProperties(metric_group_handle, &metric_group_properties);
    if (status == ZE_RESULT_SUCCESS)
    {
        // Check whether the obtained metric group supports the desired sampling type
        if ((metric_group_properties.samplingType & sampling_type) == sampling_type)
        {
            target_group.push_back(new NPUMetricGroup(metric_group_handle, metric_group_properties, sampling_type));
            return true;
        }
    }
    return false;
}

bool NPUDevice::ActivateMetricGroup(const std::string& metric_group_name)
{
    for (auto m : m_time_based_metric_groups)
    {
        if (m->GetName() == metric_group_name)
        {
            return ActivateMetricGroup(m);
        }
    }
    return false;
}

bool NPUDevice::ActivateMetricGroup(NPUMetricGroup* group)
{
    if (group == nullptr)
        return false;
    zet_metric_group_handle_t h = group->GetHandle();
    ze_result_t status = zetContextActivateMetricGroups(m_device_context, m_device_handle, 1, &h);
    return (status == ZE_RESULT_SUCCESS);
}

bool NPUDevice::DeactivateMetricGroup()
{
    ze_result_t status = zetContextActivateMetricGroups(m_device_context, m_device_handle, 0, nullptr);
    return (status == ZE_RESULT_SUCCESS);
}

NPUMetricStreamer* NPUDevice::CreateMetricStreamer(NPUMetricGroup* group)
{
    return new NPUMetricStreamer(this, group);
}
