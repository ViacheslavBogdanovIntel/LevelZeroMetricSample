#pragma once
#include "common.h"
#include "NPUMetric.h"


class NPUMetricGroup
{
public:
    NPUMetricGroup(
        zet_metric_group_handle_t metric_group_handle,
        zet_metric_group_properties_t metric_group_properties,
        zet_metric_group_sampling_type_flags_t sampling_type);
    virtual ~NPUMetricGroup();
	const std::string& GetName() const { return m_name; }
	const std::string& GetDescription() const { return m_description; }
	std::vector<NPUMetric*> GetMetricDescs() const { return m_metric_descs; }
	zet_metric_group_handle_t GetHandle() const { return m_metric_group_handle; }

	bool CalculateMetrics(const std::vector<uint8_t>& raw_data, std::vector<zet_typed_value_t>& calculated_metrics);

private:
	zet_metric_group_handle_t		m_metric_group_handle;
	zet_metric_group_properties_t	m_metric_group_properties{};
	zet_metric_group_sampling_type_flags_t m_sampling_type;

	std::string						m_name;
	std::string						m_description;

	std::vector<NPUMetric*>			m_metric_descs;
};
