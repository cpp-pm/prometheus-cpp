#include <chrono>

#include <benchmark/benchmark.h>
#include "lib/registry.h"

#include "benchmark_helpers.h"

static void BM_Registry_CreateFamily(benchmark::State& state) {
  using prometheus::Registry;
  using prometheus::Counter;
  Registry registry{{}};

  while (state.KeepRunning()) registry.AddCounter("benchmark counter", "", {});
}
BENCHMARK(BM_Registry_CreateFamily);

static void BM_Registry_CreateCounter(benchmark::State& state) {
  using prometheus::Registry;
  using prometheus::Counter;
  Registry registry{GenerateRandomLabels(10)};
  auto counterFamily =
      registry.AddCounter("benchmark counter", "", GenerateRandomLabels(10));

  while (state.KeepRunning()) {
    auto labels = GenerateRandomLabels(state.range(0));

    auto start = std::chrono::high_resolution_clock::now();
    counterFamily->Add(labels);
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
  }
}
BENCHMARK(BM_Registry_CreateCounter)->Range(0, 4096);