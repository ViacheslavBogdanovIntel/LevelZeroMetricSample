#include "NPUEngine.h"

NPUEngine::NPUEngine(zes_engine_handle_t engine_handle)
{
    m_engine_handle = engine_handle;
    zes_engine_properties_t props;
    ze_result_t status;
    status = zesEngineGetProperties(engine_handle, &props);
    if (status == ZE_RESULT_SUCCESS)
        m_type = props.type;
}

double NPUEngine::GetUtilization()
{
    double utilization = 0.0;
    ze_result_t         status;
    status = zesEngineGetActivity(m_engine_handle, &m_engine_statsT1);
    if (status != ZE_RESULT_SUCCESS)
    {
        return -1.0;
    }
    if (m_engine_statsT0.timestamp != 0)
    {
        utilization = 100 *
            (static_cast<double>(m_engine_statsT1.activeTime) - static_cast<double>(m_engine_statsT0.activeTime)) /
            (static_cast<double>(m_engine_statsT1.timestamp) - static_cast<double>(m_engine_statsT0.timestamp));
    }
    m_engine_statsT0 = m_engine_statsT1;
    return utilization;
}
