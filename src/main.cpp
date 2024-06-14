#include <windows.h> //for Sleep()
#include "NPUCollector.h"
#include "NPUMetricStreamer.h"


int main(int argc, char* argv[]) 
{
	bool enumerate_metrics = true;
	bool use_npu = false;

	ze_init_flag_t collector_init_flag;
	ze_device_type_t device_type;

	if (use_npu)
	{
		collector_init_flag = ZE_INIT_FLAG_VPU_ONLY;
		device_type = ZE_DEVICE_TYPE_VPU;
	}
	else
	{
		collector_init_flag = ZE_INIT_FLAG_GPU_ONLY;
		device_type = ZE_DEVICE_TYPE_GPU;
	}

	NPUCollector* collector = new NPUCollector(collector_init_flag);

	auto devices = collector->GetDevices();
	NPUDevice* selected_device = nullptr;
	for (auto device : devices)
	{
		if (device->GetType() == device_type)
			selected_device = device;

		printf("Device: %s\n", device->GetDeviceName().c_str());
		if (enumerate_metrics)
		{
			printf("\n\tTime based groups:\n");
			auto time_based_groups = device->GetTimeBasedMetricGroups();
			for (auto group : time_based_groups)
			{
				printf("\t\tName: %s\n", group->GetName().c_str());
				printf("\t\tDescription: %s\n", group->GetDescription().c_str());
				printf("\t\tMetrics:\n");
				auto metrics = group->GetMetricDescs();
				for (auto m : metrics)
				{
					printf("\t\t\tName: %s\n", m->GetName().c_str());
					printf("\t\t\tDescription: %s\n", m->GetDescription().c_str());
					printf("\t\t\tUnits: %s\n", m->GetUnits().c_str());
				}
			}

			printf("\n\tEvent based groups:\n");
			auto event_based_groups = device->GetEventBasedMetricGroups();
			for (auto group : event_based_groups)
			{
				printf("\t\tName: %s\n", group->GetName().c_str());
				printf("\t\tDescription: %s\n", group->GetDescription().c_str());
				printf("\t\tMetrics:\n");
				auto metrics = group->GetMetricDescs();
				for (auto m : metrics)
				{
					printf("\t\t\tName: %s\n", m->GetName().c_str());
					printf("\t\t\tDescription: %s\n", m->GetDescription().c_str());
					printf("\t\t\tUnits: %s\n", m->GetUnits().c_str());
				}
			}
		}
	}

	if (selected_device == nullptr)
	{
		printf("No requested device found.\n");
		return -1;
	}

	NPUMetricGroup* selected_group = nullptr;
	const auto& time_based_groups = selected_device->GetTimeBasedMetricGroups();
	for (auto group : time_based_groups)
	{
		if (group && group->GetHandle())
		{
			selected_group = group;
			printf("Selecting metric group: %s\n", selected_group->GetName().c_str());
			break;
		}
	}

	if (selected_group == nullptr)
	{
		printf("No suitable metric group found.\n");
		return -1;
	}

	if (!selected_device->ActivateMetricGroup(selected_group))
	{
		printf("Cannot activate metric group.\n");
		return -1;
	}

	NPUMetricStreamer* streamer = selected_device->CreateMetricStreamer(selected_group);
	if (streamer == nullptr)
	{
		printf("Cannot create metric streamer.\n");
		return -1;
	}

	int interval = 1000; //ms
	for (int i = 0; i < 5; ++i)
	{
		printf("Tick(ms):%10llu\n", GetTickCount64());
		std::vector<uint8_t> raw_data;
		if (streamer->ReadData(raw_data))
		{
			std::vector<zet_typed_value_t> calculated_metrics;
			if (selected_group->CalculateMetrics(raw_data, calculated_metrics))
			{
				for (auto m : calculated_metrics)
				{
					printf("\t%llu,\n", m.value.ui64);
				}
			}
			else
			{
				printf("Calculate raw data failed.\n");
			}
		}
		else
		{
			printf("Read data failed.\n");
		}

        Sleep(interval);
	}

	selected_device->DeactivateMetricGroup();

	delete streamer;
	delete collector;

	return 0;
}
