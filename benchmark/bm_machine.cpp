//
// Created by Jelle Spijker on 9/4/20.
//

#include <benchmark/benchmark.h>

#include <iostream>
#include <string_view>

#include "GHermeneus/GHermeneus.h"

static const std::string simple_file{ "../resources/simple.gcode" };
static const std::string big_file{ "../resources/big.gcode" };

size_t get_nol(const std::string& filename)
{
    std::ifstream gcode_file(filename);
    std::string line;
    size_t nol{ 0 };
    while (std::getline(gcode_file, line))
    {
        nol++;
    }
    return nol;
}

using namespace GHermeneus::Dialects::Marlin;

static void BM_simple_file_parallel(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        state.ResumeTiming();
        std::ifstream gcode_file(simple_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(get_nol(simple_file));
}

BENCHMARK(BM_simple_file_parallel);

static void BM_simple_file(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        UM3.setParallelExecution(false);
        state.ResumeTiming();
        std::ifstream gcode_file(simple_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(get_nol(simple_file));
}

BENCHMARK(BM_simple_file);

static void BM_big_file_parallel(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        state.ResumeTiming();
        std::ifstream gcode_file(big_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(get_nol(big_file));
}

BENCHMARK(BM_big_file_parallel);

static void BM_big_file(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        UM3.setParallelExecution(false);
        state.ResumeTiming();
        std::ifstream gcode_file(big_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(get_nol(big_file));
}

BENCHMARK(BM_big_file);

BENCHMARK_MAIN();
