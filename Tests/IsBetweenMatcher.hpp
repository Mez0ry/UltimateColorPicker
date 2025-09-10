#ifndef ISBETWEENMATCHER_HPP
#define ISBETWEENMATCHER_HPP
#include <catch2/matchers/catch_matchers.hpp>
#include <string>
#include <sstream>

template <typename T>
class IsBetweenMatcher : public Catch::Matchers::MatcherBase<T> {
    T m_begin, m_end;
public:
    IsBetweenMatcher(T begin, T end) : m_begin(begin), m_end(end) {}

    bool match(T const& in) const override {
        return in >= m_begin && in <= m_end;
    }

    std::string describe() const override {
        std::ostringstream ss;
        ss << "is between " << m_begin << " and " << m_end;
        return ss.str();
    }
};

template <typename T>
IsBetweenMatcher<T> IsBetween(T begin, T end) {
    return { begin, end };
}

#endif // ISBETWEENMATCHER_HPP
