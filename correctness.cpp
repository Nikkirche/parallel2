#include "bfs.h"
#include <gtest/gtest.h>

static uint32_t size1d = 10;

static Graph generate1d() {
    auto gr = std::vector<Node>(size1d);
    for (uint32_t i = 0; i < size1d; i++) {
        if (i < size1d - 1) {
            gr[i].push_back(i + 1);
        }
        if (i > 0) {
            gr[i].push_back(i - 1);
        }
    }
    return gr;
}

static uint32_t size1_2d = 5000;
static uint32_t size2_2d = 5000;

static Graph generate2d() {
    auto gr = std::vector<Node>(size1_2d * size2_2d);
    for (uint32_t i = 0; i < size1_2d; i++) {
        for (uint32_t j = 0; j < size2_2d; j++) {
            const auto idx = (i * size2_2d) + j;
            if ( i < size1_2d - 1) {
                gr[idx].push_back(idx + size2_2d);
            }
            if ( i > 0) {
                gr[idx].push_back(idx - size2_2d);
            }
            if (j < size2_2d - 1) {
                gr[idx].push_back(idx + 1);
            }
            if (j > 0) {
                gr[idx].push_back(idx - 1);
            }
        }
    }

    return gr;
}

TEST(seq_correct, 1d) {
    auto gr = generate1d();
    ASSERT_EQ(bfs_seq(gr, 0, size1d - 1), size1d - 1);
}

TEST(seq_correct, 2d) {
    auto gr = generate2d();
    ASSERT_EQ(bfs_seq(gr, 0, gr.size() - 1), size1_2d + size2_2d - 2);
}

TEST(par_correct, 1d) {
    auto gr = generate1d();
    ASSERT_EQ(bfs_par(gr, 0, size1d - 1), size1d - 1);
}

TEST(par_correct, 2d) {
    auto gr = generate2d();
    ASSERT_EQ(bfs_par(gr, 0, gr.size() - 1), size1_2d + size2_2d - 2);
}