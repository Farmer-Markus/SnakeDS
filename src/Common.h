#pragma once
#include <cstdint>


template<typename T>
struct Vec2D
{
    T X;
    T Y;

    bool operator>(const Vec2D<T>& other) const
    {
        return X > other.X || Y > other.Y;
    }

    bool operator>(const int val) const
    {
        return X > val || Y > val;
    }

    bool operator<(const Vec2D<T>& other) const
    {
        return X < other.X || Y < other.Y;
    }

    bool operator<(const int val) const
    {
        return X < val || Y < val;
    }

    bool operator>=(const Vec2D<T>& other) const
    {
        return X >= other.X || Y >= other.Y;
    }

    bool operator>=(const int val) const
    {
        return X >= val || Y >= val;
    }

    bool operator<=(const Vec2D<T>& other) const
    {
        return X <= other.X || Y <= other.Y;
    }

    bool operator<=(const int val) const
    {
        return X <= val || Y <= val;
    }

    bool operator==(const Vec2D<T>& other) const
    {
        return X == other.X && Y == other.Y;
    }

    bool operator!=(const Vec2D<T>& other) const
    {
        return X != other.X || Y != other.Y;
    }
};

enum class Direction : uint8_t
{
    N = (1 << 0), // 0001
    O = (1 << 1), // 0010
    S = (1 << 2), // 0100
    W = (1 << 3)  // 1000
};
