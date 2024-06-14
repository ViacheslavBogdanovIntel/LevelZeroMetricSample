#include "NPUMetric.h"


NPUMetric::NPUMetric(zet_metric_handle_t metric_handle)
{
    m_metric_handle = metric_handle;

    zetMetricGetProperties(metric_handle, &m_metric_props);
    m_name = m_metric_props.name;
    m_description = m_metric_props.description;
    m_units = m_metric_props.resultUnits;
}
