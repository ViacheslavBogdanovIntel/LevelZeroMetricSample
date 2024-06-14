#pragma once
#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "NPUMetricGroup.h"

class CsvDumper
{
public:
    CsvDumper(const std::string& filename);
    virtual ~CsvDumper();

    void WriteHeader(NPUMetricGroup* metric_group);
    void WriteMetricValues(std::vector<zet_typed_value_t> calculated_metrics);

private:
    std::ofstream m_ofs;
    NPUMetricGroup* m_metric_group = nullptr;
};
