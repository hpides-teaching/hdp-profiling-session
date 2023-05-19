#include <benchmark/benchmark.h>

#include <cstddef>
#include <cstdint>
#include <random>
#include <span>
#include <string_view>
#include <unordered_set>

constexpr size_t LEFT_SIDE_TUPLES = 1'000'000;
constexpr size_t RIGHT_SIDE_TUPLES = 100'000;
constexpr size_t STRING_LENGTH = 20;
constexpr double JOIN_PARTNER_PROBABILITY = 0.5;

size_t count_tuples_with_join_partners(std::vector<std::string> left_side, std::vector<std::string> right_side) {
  size_t right_tuples_with_partner = 0;

  std::unordered_set<std::string> left_names;
  left_names.reserve(left_side.size());
  for (auto value : left_side) {
    left_names.emplace(value);
  }

  for (auto value : right_side) {
    if (left_names.contains(value)) {
      ++right_tuples_with_partner;
    }
  }

  return right_tuples_with_partner;
}

/******************************************************************************
 * BENCHMARKING CODE
 */

constexpr std::string_view ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

static std::vector<std::string> left_tuples(LEFT_SIDE_TUPLES);
static std::vector<std::string> right_tuples(RIGHT_SIDE_TUPLES);

template <typename T>
uint64_t int_between_0_and(T&& rng, uint64_t max) {
  return rng() % max;
}

void generate_data() {
  std::mt19937_64 rng{std::random_device{}()};

  for (auto& value : left_tuples) {
    value.resize(STRING_LENGTH);
    std::generate(value.begin(), value.end(), [&]() { return ALPHABET[int_between_0_and(rng, ALPHABET.size())]; });
  }

  std::bernoulli_distribution join_partner_distribution(JOIN_PARTNER_PROBABILITY);

  for (auto& value : right_tuples) {
    value.resize(STRING_LENGTH);
    if (join_partner_distribution(rng)) {
      value = left_tuples[int_between_0_and(rng, left_tuples.size())];
    } else {
      std::generate(value.begin(), value.end(), [&]() { return ALPHABET[int_between_0_and(rng, ALPHABET.size())]; });
    }
  }
}

void BM_Join(benchmark::State& state) {
  for (auto _ : state) {
    auto result = count_tuples_with_join_partners(left_tuples, right_tuples);
    benchmark::DoNotOptimize(result);
  }

  state.counters["PerTuple"] =
      benchmark::Counter(static_cast<double>(state.iterations() * (left_tuples.size() + right_tuples.size())),
                         benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
}
BENCHMARK(BM_Join)->Iterations(10);

int main(int argc, char** argv) {
  generate_data();

  char arg0_default[] = "benchmark";
  char* args_default = arg0_default;
  if (!argv) {
    argc = 1;
    argv = &args_default;
  }
  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
}
