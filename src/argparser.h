#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

template <typename T>
struct InputParam
{
    InputParam(const char* optionText, T value)
        : OptionText(optionText)
        , Value(value)
    { }

    const char* OptionText;
    T           Value;
};

struct Settings
{
    Settings()
        : DeviceType("-device", "npu")
        , MetricSetName("-set", "ShaveIL1Cache")
        , ListAvailableMetrics("-listMetrics", true)
        , SampleCount("-samples", 5)
    { }

    InputParam<const char*> DeviceType;
    InputParam<const char*> MetricSetName;
    InputParam<bool>        ListAvailableMetrics;
    InputParam<uint32_t>    SampleCount;
};

bool ParseInputArguments(int argc, char* argv[], Settings* settings)
{
    auto parseBoolParameter = [](const char* stringValue) { return atoi(stringValue) != 0; };
    auto parseStringParameter = [](const char* stringValue) { return stringValue; };
    auto parseUintParameter = [](const char* stringValue) { return (uint32_t)atoi(stringValue); };
    auto parseIntParameter = [](const char* stringValue) { return atoi(stringValue); };

    if (!settings)
        return false;

    if (argc)
    {
        // Start from 1 to skip application name
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], settings->DeviceType.OptionText) == 0)
            {
                settings->DeviceType.Value = parseStringParameter(argv[++i]);
            }
            else if (strcmp(argv[i], settings->MetricSetName.OptionText) == 0)
            {
                settings->MetricSetName.Value = parseStringParameter(argv[++i]);
            }
            else if (strcmp(argv[i], settings->ListAvailableMetrics.OptionText) == 0)
            {
                settings->ListAvailableMetrics.Value = parseBoolParameter(argv[++i]);
            }
            else if (strcmp(argv[i], settings->SampleCount.OptionText) == 0)
            {
                settings->SampleCount.Value = parseUintParameter(argv[++i]);
            }
            else if (strcmp(argv[i], "-help") == 0)
            {
                char helpMessageBuffer[8192]; // sufficiently large buffer
                sprintf_s(helpMessageBuffer, "Command line args (optional):\n" \
                    "\t-device <type> : device to use: npu or gpu (default npu)\n" \
                    "\t-set <metricSet> : metric set to use (default ShaveIL1Cache)\n" \
                    "\t-listMetrics : list available metrics only (default true)\n" \
                    "\t-samples <n> : number of reports to read (default 5)\n" \
                    "\t-help : display help message\n" \
                    "Example command : LevelZeroMetricSample.exe -device npu -set NOC -listMetrics false -samples 20\n" \
                );
                printf("%s", helpMessageBuffer);
                return false;
            }
        }
    }

    return true;
}
