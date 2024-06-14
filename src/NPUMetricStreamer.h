#pragma once
#include "common.h"
#include "NPUDevice.h"

class NPUMetricStreamer
{
public:
    NPUMetricStreamer(NPUDevice* device, NPUMetricGroup* metric_group);
    virtual ~NPUMetricStreamer();

    bool ReadData(std::vector<uint8_t>& raw_data);

private:
    NPUMetricGroup*                 m_metric_group = nullptr;
    NPUDevice*                      m_device = nullptr;
    ze_event_handle_t               m_notification_event = nullptr;
    ze_event_pool_handle_t          m_event_pool = nullptr;
    ze_event_pool_desc_t            m_event_pool_desc = { ZE_STRUCTURE_TYPE_EVENT_POOL_DESC, nullptr, ZE_EVENT_POOL_FLAG_HOST_VISIBLE, 1 };
    ze_event_desc_t                 m_event_desc = { ZE_STRUCTURE_TYPE_EVENT_DESC, nullptr };
    zet_metric_streamer_handle_t    m_metric_streamer = nullptr;
    zet_metric_streamer_desc_t      m_metric_streamer_desc = { ZET_STRUCTURE_TYPE_METRIC_STREAMER_DESC };
};
