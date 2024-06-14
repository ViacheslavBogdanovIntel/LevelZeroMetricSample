#include "NPUMetricStreamer.h"


NPUMetricStreamer::NPUMetricStreamer(NPUDevice* device, NPUMetricGroup* metric_group)
{
    m_device = device;
    m_metric_group = metric_group;

    // Create notification event
    ze_result_t status = ZE_RESULT_SUCCESS;
    ze_device_handle_t device_handle = m_device->GetHandle();
    status = zeEventPoolCreate(m_device->GetContext(), &m_event_pool_desc, 1, &device_handle, &m_event_pool);
    if (status == ZE_RESULT_SUCCESS)
    {
        m_event_desc.index = 0;
        m_event_desc.signal = ZE_EVENT_SCOPE_FLAG_HOST;
        m_event_desc.wait = ZE_EVENT_SCOPE_FLAG_HOST;
        zeEventCreate(m_event_pool, &m_event_desc, &m_notification_event);
    }

    // Open metric streamer
    m_metric_streamer_desc.samplingPeriod = 1000;
    m_metric_streamer_desc.notifyEveryNReports = 32768;
    zetMetricStreamerOpen(m_device->GetContext(), device_handle, metric_group->GetHandle(), &m_metric_streamer_desc, m_notification_event, &m_metric_streamer);
}

NPUMetricStreamer::~NPUMetricStreamer()
{
    if (m_metric_streamer)
        zetMetricStreamerClose(m_metric_streamer);

    if (m_notification_event)
        zeEventDestroy(m_notification_event);

    if (m_event_pool)
        zeEventPoolDestroy(m_event_pool);
}

bool NPUMetricStreamer::ReadData(std::vector<uint8_t>& raw_data)
{
    if (m_metric_streamer == nullptr)
        return false;

    ze_result_t status = ZE_RESULT_SUCCESS;
    size_t raw_size = 0;
    status = zetMetricStreamerReadData(m_metric_streamer, UINT32_MAX, &raw_size, nullptr);
    if (status != ZE_RESULT_SUCCESS)
        return false;
    if (raw_size == 0)
        return false;
    raw_data.resize(raw_size);
    status = zetMetricStreamerReadData(m_metric_streamer, UINT32_MAX, &raw_size, raw_data.data());
    return (status == ZE_RESULT_SUCCESS);
}
