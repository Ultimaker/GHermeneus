//
// Created by Jelle Spijker on 9/4/20.
//

#include <benchmark/benchmark.h>

static void BM_placeholder_test(benchmark::State& state) {
    // init test
    for (auto _ : state) {
        // time code
    }
}
BENCHMARK(BM_placeholder_test);

BENCHMARK_MAIN();