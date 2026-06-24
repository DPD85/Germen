#pragma once

#include <gvc.h>

namespace Diagramma
{
    struct Vec2: public ImVec2
    {
        constexpr Vec2()
        {
            x = 0.0f;
            y = 0.0f;
        }

        constexpr Vec2(float _x, float _y)
        {
            x = _x;
            y = _y;
        }

        constexpr Vec2(double _x, double _y)
        {
            x = static_cast<float>(_x);
            y = static_cast<float>(_y);
        }

        constexpr Vec2(const ImVec2 &valore): ImVec2(valore) {}

        constexpr Vec2(const pointf &valore)
        {
            x = static_cast<float>(valore.x);
            y = static_cast<float>(valore.y);
        }

        constexpr Vec2 operator+(const Vec2 &valore) const
        {
            return { x + valore.x, y + valore.y };
        }

        constexpr Vec2 &operator+=(const Vec2 &valore)
        {
            x += valore.x;
            y += valore.y;

            return *this;
        }

        constexpr Vec2 operator-(const Vec2 &valore) const
        {
            return { x - valore.x, y - valore.y };
        }

        constexpr Vec2 &operator-=(const Vec2 &valore)
        {
            x -= valore.x;
            y -= valore.y;

            return *this;
        }

        constexpr Vec2 operator*(float valore) const
        {
            return { x * valore, y * valore };
        }

        constexpr Vec2 &operator*=(float valore)
        {
            x *= valore;
            y *= valore;

            return *this;
        }

        constexpr Vec2 operator/(float valore) const
        {
            return { x / valore, y / valore };
        }

        constexpr Vec2 &operator/=(float valore)
        {
            x /= valore;
            y /= valore;

            return *this;
        }

        float Length() const
        {
            return std::sqrtf(x * x + y * y);
        }

        bool Normalize()
        {
            const float lunghezza = Length();
            if (lunghezza < 1e-4f) return false;
            *this /= lunghezza;

            return true;
        }
    };

    static constexpr Vec2 operator+(const pointf &a, const Vec2 &b)
    {
        return { a.x + b.x, a.y + b.y };
    }

    static constexpr Vec2 operator-(const pointf &a, const Vec2 &b)
    {
        return { a.x - b.x, a.y - b.y };
    }

    static constexpr Vec2 operator*(const pointf &a, const Vec2 &b)
    {
        return { a.x * b.x, a.y * b.y };
    }

    static constexpr Vec2 operator/(const pointf &a, const Vec2 &b)
    {
        return { a.x / b.x, a.y / b.y };
    }
}
