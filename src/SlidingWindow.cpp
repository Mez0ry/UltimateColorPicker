#include "SlidingWindow.hpp"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <string>

SlidingWindow::SlidingWindow(int start, int end, int window_size)
    : m_StartIndex(start)
    , m_EndIndex(end)
    , m_WindowSize(window_size)
{}

int SlidingWindow::GetStartIndex() const
{
    return m_StartIndex;
}

void SlidingWindow::SetStartIndex(int new_start_index) {
    this->m_StartIndex = new_start_index;
}

int SlidingWindow::GetEndIndex() const
{
    return m_EndIndex;
}

void SlidingWindow::SetEndIndex(int new_end_index)
{
    m_EndIndex = new_end_index;
}

int SlidingWindow::GetWindowSize() const {
    return m_WindowSize;
}

bool SlidingWindow::SlideForwardByWindowSize(const int arr_size) {
    return SlideForwardByValue(m_WindowSize, arr_size);
}

bool SlidingWindow::SlideForwardByValue(const int value, const int arr_size) {
    if(std::min(m_EndIndex, arr_size) == arr_size){
        return false;
    }

    m_EndIndex = std::clamp((m_EndIndex + value), value, arr_size);
    m_StartIndex = std::clamp(m_StartIndex + value, 0, m_EndIndex - value);

    return true;
}

bool SlidingWindow::SlideBackwardByWindowSize(const int arr_size) {
    return SlideBackwardByValue(m_WindowSize, arr_size);
}

bool SlidingWindow::SlideBackwardByValue(const int value, const int arr_size) {
    if(std::max(m_EndIndex, m_WindowSize) == m_WindowSize){
        return false;
    }

    m_EndIndex = std::clamp(m_EndIndex - value, m_WindowSize, arr_size);
    m_StartIndex = std::clamp(m_StartIndex - value, 0, m_EndIndex - m_WindowSize);

    return true;
}

void SlidingWindow::ShrinkWindowSizeTo(IShrinkStrategy &shrink_strategy){
    shrink_strategy.Execute(this);
}

void ShrinkTowardsStart::Execute(SlidingWindow* const window) {
    constexpr int min_allowed_window_size = 2;
    constexpr int mask = min_allowed_window_size - 1;

    if((m_NewWindowSize & mask) == m_NewWindowSize){
        throw std::logic_error("can't shrink towards start, new window size must be more than " + std::to_string(m_NewWindowSize));
    }

    auto window_diff = std::abs(window->GetWindowSize() - m_NewWindowSize);
    auto end_index_diff = std::abs(window->GetEndIndex() - window->GetStartIndex());
    if (window_diff == end_index_diff) {
        throw std::logic_error("Difference values should differ");
    }

    auto new_end_index = window->GetEndIndex() - window_diff;
    window->SetEndIndex(new_end_index);
}

void ShrinkTowardsEnd::Execute(SlidingWindow* const window) {
    constexpr int min_allowed_window_size = 2;
    constexpr int mask = min_allowed_window_size - 1;

    if((m_NewWindowSize & mask) == m_NewWindowSize){
        throw std::logic_error("can't shrink towards end, new window size must be more than " + std::to_string(m_NewWindowSize));
    }

    auto window_diff = std::abs(window->GetWindowSize() - m_NewWindowSize);
    auto end_index_diff = std::abs(window->GetEndIndex() - window->GetStartIndex());
    if (window_diff == end_index_diff) {
        throw std::logic_error("window_diff and end_index_diff values should differ");
    }

    auto new_end_index = window->GetStartIndex() + window_diff;
    window->SetEndIndex(new_end_index);
}

void ShrinkTowardsCenter::Execute(SlidingWindow* const window) {
    constexpr int mask = 2;
    if((m_NewWindowSize & mask) == m_NewWindowSize){
        throw std::logic_error("can't shrink towards center, new window size must be more than " + std::to_string(m_NewWindowSize));
    }

    const auto old_size = window->GetWindowSize();

    if (m_NewWindowSize >= old_size) {
        throw std::logic_error("Can't shrink towards center when new window size equals or exceeds existing window size");
    }

    const auto shift = std::abs(old_size - m_NewWindowSize) / 2;

    const auto original_start = window->GetStartIndex();
    const auto original_end = window->GetEndIndex();
    window->SetStartIndex(original_start + shift);
    window->SetEndIndex(original_end - shift);

    if (window->GetStartIndex() > window->GetEndIndex()) {
        throw std::logic_error("Invalid window indices after shrinking: start index exceeded end index");
    }
}

