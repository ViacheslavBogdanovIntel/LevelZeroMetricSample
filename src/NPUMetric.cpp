#include "NPUMetric.h"

NPUMetric::NPUMetric(zet_metric_handle_t metric_handle)
{
    m_metric_handle = metric_handle;

    zetMetricGetProperties(metric_handle, &m_metric_props);
    m_name = m_metric_props.name;
    m_description = m_metric_props.description;
    m_units = m_metric_props.resultUnits;
}

std::string NPUMetric::FormatValue(zet_typed_value_t value)
{
    switch (value.type)
    {
    case ZET_VALUE_TYPE_UINT32:
        return std::to_string(value.value.ui32);
    case ZET_VALUE_TYPE_UINT64:
        return std::to_string(value.value.ui64);
    case ZET_VALUE_TYPE_FLOAT32:
        return std::to_string(value.value.fp32);
    case ZET_VALUE_TYPE_FLOAT64:
        return std::to_string(value.value.fp64);
    case ZET_VALUE_TYPE_BOOL8:
        return std::to_string(value.value.b8);
    case ZET_VALUE_TYPE_UINT8:
        return std::to_string(value.value.ui32);
    case ZET_VALUE_TYPE_UINT16:
        return std::to_string(value.value.ui32);
    }
    return "???";
}
