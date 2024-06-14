#include "CsvDumper.h"


CsvDumper::CsvDumper(const std::string& filename)
{
    if (filename.empty())
        return;

    m_ofs.open(filename, std::ios_base::out);
}

CsvDumper::~CsvDumper()
{
    m_ofs.close();
}

void CsvDumper::WriteHeader(NPUMetricGroup* metric_group)
{
    if (metric_group == nullptr || m_ofs.bad())
        return;

    m_metric_group = metric_group;
    auto current_metric_list = m_metric_group->GetMetricDescs();
    for (auto m : current_metric_list)
    {
        m_ofs << m->GetName() << ",";
    }
    m_ofs << std::endl;
    m_ofs.flush();
}

void CsvDumper::WriteMetricValues(std::vector<zet_typed_value_t> calculated_metrics)
{
    if (m_metric_group == nullptr || m_ofs.bad())
        return;

    auto current_metric_list = m_metric_group->GetMetricDescs();
    if (calculated_metrics.size() % current_metric_list.size() == 0)
    {
        size_t metric_id = 0;
        for (auto m : calculated_metrics)
        {
            if (metric_id != 0 && (metric_id % current_metric_list.size() == 0))
                m_ofs << std::endl;
            m_ofs << NPUMetric::FormatValue(m) << std::endl;
            metric_id++;
        }
        m_ofs << std::endl;
        m_ofs.flush();
    }
}
