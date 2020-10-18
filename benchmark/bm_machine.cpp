//
// Created by Jelle Spijker on 9/4/20.
//

#include <benchmark/benchmark.h>

#include <iostream>
#include <string_view>

#include <spdlog/spdlog.h>

#include "GHermeneus/Dialects/Marlin.h"

static const std::string simple_file{ "../resources/simple.gcode" };
static const std::string big_file{ "../resources/big.gcode" };
constexpr spdlog::level::level_enum log_level = spdlog::level::critical;

int64_t getNol(const std::string& filename)
{
    std::ifstream gcode_file(filename);
    std::string line;
    int64_t nol{ 0 };
    while (std::getline(gcode_file, line))
    {
        nol++;
    }
    return nol;
}

using namespace GHermeneus;

static void bmSimpleFileParallel(benchmark::State& state)
{
    spdlog::set_level(log_level);
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        state.ResumeTiming();
        std::ifstream gcode_file(simple_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(getNol(simple_file));
}

BENCHMARK(bmSimpleFileParallel);

static void bmSimpleFile(benchmark::State& state)
{
    spdlog::set_level(log_level);
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        UM3.setParallelExecution(false);
        state.ResumeTiming();
        std::ifstream gcode_file(simple_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(getNol(simple_file));
}

BENCHMARK(bmSimpleFile);

static void bmBigFileParallel(benchmark::State& state)
{
    spdlog::set_level(log_level);
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        state.ResumeTiming();
        std::ifstream gcode_file(big_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(getNol(big_file));
}

BENCHMARK(bmBigFileParallel);

static void bmBigFile(benchmark::State& state)
{
    spdlog::set_level(log_level);
    for (auto _ : state)
    {
        state.PauseTiming();
        auto UM3 = MarlinMachine();
        UM3.setParallelExecution(false);
        state.ResumeTiming();
        std::ifstream gcode_file(big_file);
        UM3 << gcode_file;
    }
    state.SetItemsProcessed(getNol(big_file));
}

BENCHMARK(bmBigFile);

BENCHMARK_MAIN();
