#include <benchmark/benchmark.h>

#include <cstddef>
#include <cstdint>
#include <random>
#include <string_view>
#include <unordered_set>

struct Tuple {
  uint64_t id;
  uint64_t timestamp;
  std::string name;
};

constexpr size_t LEFT_SIDE_TUPLES = 1000;
constexpr size_t RIGHT_SIDE_TUPLES = 100;
constexpr size_t STRING_LENGTH = 100;
constexpr double JOIN_PARTNER_PROBABILITY = 0.5;

size_t count_tuples_with_join_partners(std::vector<Tuple> left_side, std::vector<Tuple> right_side) {
  size_t left_tuples_with_partner = 0;

  std::unordered_set<std::string> right_names;
  right_names.reserve(right_side.size());
  for (Tuple tup : right_side) {
    right_names.emplace(tup.name);
  }

  for (Tuple tup : left_side) {
    if (right_names.contains(tup.name)) {
      ++left_tuples_with_partner;
    }
  }

  return left_tuples_with_partner;
}

/******************************************************************************
 * BENCHMARKING CODE
 */

constexpr std::string_view ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

template <typename T>
uint64_t int_between_0_and(T&& rng, uint64_t max) {
  return rng() % max;
}

static void BM_Join(benchmark::State& state) {
  std::vector<Tuple> left_tuples(LEFT_SIDE_TUPLES);
  std::vector<Tuple> right_tuples(RIGHT_SIDE_TUPLES);

  std::mt19937_64 rng{std::random_device{}()};

  for (auto& tup : left_tuples) {
    tup.id = rng();
    tup.timestamp = rng();

    tup.name.resize(STRING_LENGTH);
    std::generate(tup.name.begin(), tup.name.end(),
                  [&]() { return ALPHABET[int_between_0_and(rng, ALPHABET.size())]; });
  }

  std::bernoulli_distribution join_partner_distribution(JOIN_PARTNER_PROBABILITY);

  for (auto& tup : right_tuples) {
    tup.id = rng();
    tup.timestamp = rng();

    tup.name.resize(STRING_LENGTH);
    if (join_partner_distribution(rng)) {
      tup.name = left_tuples[int_between_0_and(rng, left_tuples.size())].name;
    } else {
      std::generate(tup.name.begin(), tup.name.end(), [&]() {
        auto index = rng() % ALPHABET.size();
        return ALPHABET[index];
      });
    }
  }

  for (auto _ : state) {
    auto result = count_tuples_with_join_partners(left_tuples, right_tuples);
    benchmark::DoNotOptimize(result);
  }

  state.counters["PerTuple"] =
      benchmark::Counter(static_cast<double>(state.iterations() * (left_tuples.size() + right_tuples.size())),
                         benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
}
BENCHMARK(BM_Join);

BENCHMARK_MAIN();
