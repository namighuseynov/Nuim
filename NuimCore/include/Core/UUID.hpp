#pragma once
#include "Base.hpp"

#include <random>
#include <atomic>

namespace Nuim {

    class UUID {
    public:
        UUID() : m_value(Generate()) {}
        explicit UUID(U64 v) : m_value(v) {}

        U64 Value() const { return m_value; }

        bool operator==(const UUID& other) const { return m_value == other.m_value; }
        bool operator!=(const UUID& other) const { return m_value != other.m_value; }
        bool operator<(const UUID& other) const { return m_value < other.m_value; }

    private:
        static U64 Generate() {
            static thread_local std::mt19937_64 rng{ Seed() };
            static std::uniform_int_distribution<U64> dist;

            U64 v = dist(rng);

            if (v == 0) v = 1;
            return v;
        }

        static U64 Seed() {
            std::random_device rd;
            U64 a = (U64(rd()) << 32) ^ U64(rd());
            static std::atomic<U64> counter{ 1 };
            return a ^ (counter.fetch_add(1) * 0x9E3779B97F4A7C15ull);
        }

    private:
        U64 m_value = 0;
    };

} 
