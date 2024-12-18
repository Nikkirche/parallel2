#pragma once
#include <vector>
#include <parlay/primitives.h>


using Node = std::vector<std::uint32_t>;

using Graph = std::vector<Node>;

inline uint32_t bfs_seq(const Graph &graph, const uint32_t start, const uint32_t end) {
    auto dest = std::vector(graph.size(), -1);
    dest[start] = 0;
    std::deque<uint32_t> q;
    q.push_front(start);
    while (!q.empty()) {
        const auto u = q.back();
        q.pop_back();
        for (auto v: graph[u]) {
            if (dest[v] == -1) {
                dest[v] = dest[u] + 1;
                q.push_front(v);
            }
        }
    }
    return dest[end];
}


inline uint32_t bfs_par(const Graph &graph, const uint32_t start, const uint32_t end) {
    auto front = std::vector<uint32_t>(1, start);
    auto dest = std::vector<std::atomic_uint32_t>(graph.size());
    parlay::for_each(dest, [](auto &a) { a.store(-1); });
    dest[start].store(0);
    uint32_t iter = 0;
    while (!front.empty()) {
        auto mapped = parlay::map(front, [&](auto &a) { return graph[a].size(); });
        auto ind_size = parlay::scan_inplace(mapped);
        if (ind_size > 500 * 500 ) {
            auto front_new = parlay::sequence<uint32_t>(ind_size, -1);

            parlay::parallel_for(0, front.size(), [&](auto j) {
                auto &node = graph[front[j]];
                for (auto i = 0; i < node.size(); i++) {
                    uint32_t tmp = -1;
                    if (dest[node[i]].compare_exchange_strong(tmp, iter + 1)) {
                        front_new[mapped[j] + i] = node[i];
                    }
                }
            });
            if( front.size() > front_new.size()) {
                auto s  = parlay::filter_into_uninitialized(front_new, front, [](auto a) { return a != -1; });
                front.resize(s);
            }
            else {
                front = std::move(parlay::filter(front_new, [](auto a) { return a != -1; }).to_vector());
            }
        } else {
            auto front_new = std::vector<uint32_t>{};
            for (unsigned int j: front) {
                auto &node = graph[j];
                for (unsigned int i: node) {
                    uint32_t tmp = -1;
                    if (dest[i].compare_exchange_strong(tmp, iter + 1)) {
                        front_new.push_back(i);
                    }
                }
            }
            front = std::move(front_new);
        }
        iter++;
    }
    return dest[end].load();
}
