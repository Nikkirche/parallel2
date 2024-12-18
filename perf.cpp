#include <algorithm>
#include <iostream>
#include <benchmark/benchmark.h>

#include "bfs.h"

static uint32_t size1_2d = 5000;
static uint32_t size2_2d = 5000;

static uint32_t size1_3d = 500;
static uint32_t size2_3d = 500;
static uint32_t size3_3d = 500;

static Graph generate3d() {
    auto gr = std::vector<Node>(size1_3d * size2_3d * size3_3d);
    for (uint32_t i = 0; i < size1_3d; i++) {
        for (uint32_t j = 0; j < size2_3d; j++) {
            for (uint32_t k = 0; k < size3_3d; k++) {
                const auto idx = (size2_3d * size3_3d) * k + (i * size2_3d) + j;
                if (k < size3_3d - 1) {
                    gr[idx].push_back(idx + size2_3d * size3_3d);
                }
                if (k > 0) {
                    gr[idx].push_back(idx - size2_3d * size3_3d);
                }
                if (i < size1_3d - 1) {
                    gr[idx].push_back(idx + size2_3d);
                }
                if (i > 0) {
                    gr[idx].push_back(idx - size2_3d);
                }
                if (j < size2_3d - 1) {
                    gr[idx].push_back(idx + 1);
                }
                if (j > 0) {
                    gr[idx].push_back(idx - 1);
                }
            }
        }
    }

    return gr;
}

static void sequential(benchmark::State &state) {
    auto gr = generate3d();
    for (auto _: state) {
        bfs_seq(gr, 0, gr.size() - 1);
    }
}

static void parallel(benchmark::State &state) {
    auto gr = generate3d();
    for (auto _: state) {
        bfs_par(gr, 0, gr.size() - 1);
    }
}

BENCHMARK(sequential)->Iterations(5)->MeasureProcessCPUTime()->UseRealTime();

BENCHMARK(parallel)->Iterations(5)->MeasureProcessCPUTime()->UseRealTime();
