#pragma once
#include "common.h"

class NPUMetric
{
public:
    NPUMetric(zet_metric_handle_t metric_handle);

    const std::string& GetName() const { return m_name; }
    const std::string& GetDescription() const { return m_description; }
    const std::string& GetUnits() const { return m_units; }

private:
    zet_metric_handle_t     m_metric_handle;
    zet_metric_properties_t m_metric_props;

    std::string             m_name;
    std::string             m_description;
    std::string             m_units;
};
