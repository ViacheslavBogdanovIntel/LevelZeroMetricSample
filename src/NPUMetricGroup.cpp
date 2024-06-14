#include "NPUMetricGroup.h"


NPUMetricGroup::NPUMetricGroup(
    zet_metric_group_handle_t metric_group_handle,
    zet_metric_group_properties_t metric_group_properties,
    zet_metric_group_sampling_type_flags_t sampling_type)
{
    m_metric_group_handle = metric_group_handle;
    m_metric_group_properties = metric_group_properties;
    m_sampling_type = sampling_type;

    m_name = m_metric_group_properties.name;
    m_description = m_metric_group_properties.description;

    uint32_t metric_count = m_metric_group_properties.metricCount;
    std::vector<zet_metric_handle_t> metrics(metric_count);
    if (zetMetricGet(m_metric_group_handle, &metric_count, metrics.data()) == ZE_RESULT_SUCCESS)
    {
        for (auto m : metrics)
        {
            m_metric_descs.push_back(new NPUMetric(m));
        }
    }
}

NPUMetricGroup::~NPUMetricGroup()
{
    for (auto m : m_metric_descs)
    {
        delete m;
    }
    m_metric_descs.clear();
}

bool NPUMetricGroup::CalculateMetrics(const std::vector<uint8_t>& raw_data, std::vector<zet_typed_value_t>& calculated_metrics)
{
    uint32_t calculated_size = 0;
    ze_result_t status = ZE_RESULT_SUCCESS;
    status = zetMetricGroupCalculateMetricValues(m_metric_group_handle, ZET_METRIC_GROUP_CALCULATION_TYPE_METRIC_VALUES, raw_data.size(),
        raw_data.data(), &calculated_size, nullptr);

    if (status != ZE_RESULT_SUCCESS)
        return false;

    calculated_metrics.resize(calculated_size);
    status = zetMetricGroupCalculateMetricValues(m_metric_group_handle, ZET_METRIC_GROUP_CALCULATION_TYPE_METRIC_VALUES, raw_data.size(),
        raw_data.data(), &calculated_size, calculated_metrics.data());
    return (status == ZE_RESULT_SUCCESS);
}
