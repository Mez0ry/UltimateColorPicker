#ifndef SLIDINGWINDOW_HPP
#define SLIDINGWINDOW_HPP
#include <cstddef>
#include <iterator>

template<typename ValueType>
class SlidingWindowIterator;

class SlidingWindow;

class IShrinkStrategy{
protected:
    int m_NewWindowSize;
    int GetNewSize() const {return m_NewWindowSize;}
public:
    IShrinkStrategy(const int new_size) : m_NewWindowSize(new_size) {}
    virtual ~IShrinkStrategy() {}
    virtual void Execute(SlidingWindow* const window) = 0;
};

class ShrinkTowardsStart : public IShrinkStrategy{
public:
    ShrinkTowardsStart() = delete;
    ShrinkTowardsStart(const int new_window_size) : IShrinkStrategy(new_window_size){}
    ~ShrinkTowardsStart() override = default;

    void Execute(SlidingWindow* const window) override;
};

class ShrinkTowardsEnd : public IShrinkStrategy{
public:
    ShrinkTowardsEnd() = delete;
    ShrinkTowardsEnd(const int new_window_size) : IShrinkStrategy(new_window_size){}
    ~ShrinkTowardsEnd() override = default;

    void Execute(SlidingWindow* const window) override;
};

class ShrinkTowardsCenter : public IShrinkStrategy{
public:
    ShrinkTowardsCenter() = delete;
    ShrinkTowardsCenter(const int new_window_size) : IShrinkStrategy(new_window_size){}
    ~ShrinkTowardsCenter() override = default;

    void Execute(SlidingWindow* const window) override;
};

class SlidingWindow{
private:
    int m_StartIndex{0};
    int m_EndIndex{0};
    int m_WindowSize{0};
public:
    SlidingWindow() = delete;
    SlidingWindow(int start, int end, int window_size);
    ~SlidingWindow() = default;

    int GetStartIndex() const;
    void SetStartIndex(int new_start_index);

    int GetEndIndex() const;
    void SetEndIndex(int new_end_index);

    int GetWindowSize() const;

    /**
     * @return true if window moved forward and false otherwise
    */
    bool SlideForwardByWindowSize(const int arr_size);

    /**
     * @return true if window moved forward and false otherwise
    */
    bool SlideForwardByValue(const int value, const int arr_size);

    /**
    * @return true if window moved backward by window_size and false otherwise
    */
    bool SlideBackwardByWindowSize(const int arr_size);

    /**
    * @return true if window moved backward by value and false otherwise
    */
    bool SlideBackwardByValue(const int value, const int arr_size);

    /**
     * @brief Shrinks window to new_window_size. To expand window size you muse use function ExpandWindowSizeTo
     * @param new_window_size must be less than inner window_size
     */
    void ShrinkWindowSizeTo(IShrinkStrategy& shrink_strategy);

    template<typename ValueType = int>
    SlidingWindowIterator<ValueType> begin()
    {
        return SlidingWindowIterator<ValueType>(this, m_StartIndex);
    }

    template<typename ValueType = int>
    SlidingWindowIterator<ValueType> end()
    {
        return SlidingWindowIterator<ValueType>(this, m_EndIndex);
    }

    template<typename ValueType = int>
    SlidingWindowIterator<ValueType> end(const int arr_size)
    {
        return SlidingWindowIterator<ValueType>(this, std::min(arr_size,m_EndIndex));
    }

    template<typename ValueType = int>
    SlidingWindowIterator<ValueType> begin(ValueType* arr_data) const
    {
        return SlidingWindowIterator<ValueType>(this, arr_data, m_StartIndex);
    }

    template<typename ValueType = int>
    SlidingWindowIterator<ValueType> end(ValueType* arr_data) const
    {
        return SlidingWindowIterator<ValueType>(this, arr_data, m_EndIndex + 1);
    }
};

template<typename ValueType>
class SlidingWindowIterator {
private:
    SlidingWindow* window_;
    int current_position_;
    ValueType* array_data_{nullptr};

public:
    using difference_type = std::ptrdiff_t;
    using value_type = ValueType;
    using pointer = ValueType*;
    using reference = ValueType&;
    using iterator_category = std::forward_iterator_tag;

    SlidingWindowIterator(SlidingWindow* win, int pos) noexcept
        : window_(win), current_position_(pos) {}


    SlidingWindowIterator(SlidingWindow* win, ValueType* arr, int pos) noexcept
        : window_(win), current_position_(pos), array_data_(arr) {}

    reference operator*() {
        if(!array_data_){
            return current_position_;
        }
        return *(array_data_ + current_position_);
    }

    pointer operator->() const {
        return &(operator*());
    }

    SlidingWindowIterator& operator++() {
        if (current_position_ < window_->GetEndIndex()) {
            ++current_position_;
        }
        return *this;
    }

    SlidingWindowIterator operator++(int) {
        SlidingWindowIterator temp(*this);
        ++(*this);
        return temp;
    }

    bool operator==(const SlidingWindowIterator& other) const {
        return this->window_ == other.window_ && this->current_position_ == other.current_position_ && this->array_data_ == other.array_data_;
    }

    bool operator!=(const SlidingWindowIterator& other) const {
        return !(*this == other);
    }
};
#endif // SLIDINGWINDOW_HPP
