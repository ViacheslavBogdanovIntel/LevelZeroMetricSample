#pragma once
#include "common.h"


class NPUEngine
{
public:
    NPUEngine(zes_engine_handle_t engine_handle);
    double GetUtilization();
    std::string GetEngineType() const { return std::to_string(m_type); }

private:
    zes_engine_group_t  m_type = ZES_ENGINE_GROUP_FORCE_UINT32;
    zes_engine_handle_t m_engine_handle = nullptr;
    zes_engine_stats_t  m_engine_statsT0 = {0, 0};
    zes_engine_stats_t  m_engine_statsT1 = {0, 0};
};
