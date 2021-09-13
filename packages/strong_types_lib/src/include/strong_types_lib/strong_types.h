#ifndef STRONG_TYPES_LIB_STRONG_TYPES_H
#define STRONG_TYPES_LIB_STRONG_TYPES_H

namespace strong_types
{
    template <typename T, typename Parameter>
    class StrongType
    {
    public:
        explicit StrongType(T const &value) : value_(value) {}
        T &get() { return value_; }
        T const &get() const { return value_; }

    private:
        T value_;
    };

} // namespace strong_types

#endif // STRONG_TYPES_LIB_STRONG_TYPES_H