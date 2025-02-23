# Level Zero Metric Sample

This repository contains the sample code to get Level Zero metrics for GPU or NPU profiling.

This project uses oneAPI Level Zero library as a 3rdparty [oneAPI](https://github.com/oneapi-src/level-zero/) project.

See the [oneAPI specification](https://spec.oneapi.com/versions/latest/introduction.html) for more information about the oneAPI project.

See the [Level Zero specification](https://spec.oneapi.com/level-zero/latest/index.html) for more information about Level Zero.

See the [Level Zero specification repo](https://github.com/oneapi-src/level-zero-spec) for contributing to the specification.

# Building and Installing

Project is defined using [CMake](https://cmake.org/).

## Windows

```
mkdir _build
cd _build
cmake ..
```

## Linux

Not yet supported

# How to use

Run LevelZeroMetricSample.exe with the following parameters:

```
-device <type> : device to use: npu or gpu (default npu)
-set <metricSet> : metric set to use (default ShaveIL1Cache)
-csv <CsvFileName> : dump to csv file (default no)
-listMetrics <enable>: list available metrics only (default 0 = disabled)
-samples <n> : number of reports to read (default 5)
-help : display help message
```

Example command : 
```
LevelZeroMetricSample.exe -device npu -set NOC -csv dump.csv -listMetrics false -samples 20
```
