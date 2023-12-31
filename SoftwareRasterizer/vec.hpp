#pragma once

#include <cmath>
#include <initializer_list>
#include <cassert>

#define KOMVUX_METHOD(method_name, index)   \
    T& method_name() {                      \
        return arr[index];                  \
    }                                       \
    T method_name() const {                 \
        return arr[index];                  \
    }

#define KOMVUX_GENERATE_MATH_OPERATORS(size, op)                \
    vec##size<T> operator op (const vec##size<T>& v) const {    \
        vec##size<T> result(*this);                             \
        for (int i = 0; i < size; i++)                          \
            result[i] op##= v[i];                               \
        return result;                                          \
    }                                                           \
    vec##size<T> operator op (T scalar) const {                 \
        vec##size<T> result(*this);                             \
        for (int i = 0; i < size; i++)                          \
            result[i] op##= scalar;                             \
        return result;                                          \
    }                                                           \
    vec##size<T>& operator op##= (const vec##size<T>& v) {      \
        for (int i = 0; i < size; i++)                          \
            (*this)[i] op##= v[i];                              \
        return *this;                                           \
    }                                                           \
    vec##size<T>& operator op##= (T scalar) {                   \
        for (int i = 0; i < size; i++)                          \
            (*this)[i] op##= scalar;                            \
        return *this;                                           \
    }

#define KOMVUX_VEC_BEGIN(size)                                      \
    template <typename T> class vec##size {                         \
    private:                                                        \
        T arr[size] = {0};                                          \
    public:                                                         \
        template <typename... I> vec##size(I... init) :             \
            arr{ init... } {}                                       \
                                                                    \
        KOMVUX_GENERATE_MATH_OPERATORS(size, +);                    \
        KOMVUX_GENERATE_MATH_OPERATORS(size, -);                    \
        KOMVUX_GENERATE_MATH_OPERATORS(size, *);                    \
        KOMVUX_GENERATE_MATH_OPERATORS(size, /);                    \
                                                                    \
        vec##size<T> normalize() const {                            \
            return *this / length();                                \
        }                                                           \
        T length() const {                                          \
            T accumulated = 0;                                      \
            for (int i = 0; i < size; i++) {                        \
                accumulated += (*this)[i] * (*this)[i];             \
            }                                                       \
            return std::sqrt(accumulated);                          \
        }                                                           \
        T& operator [] (int i) {                                    \
            assert(i >= 0 && i < size && "Index outside bounds.");  \
            return arr[i];                                          \
        }                                                           \
        T operator [] (int i) const {                               \
            assert(i >= 0 && i < size && "Index outside bounds.");  \
            return arr[i];                                          \
        }

#define KOMVUX_VEC_END }

namespace komvux {
    class mat4;

    // vec2

    KOMVUX_VEC_BEGIN(2);
        KOMVUX_METHOD(x, 0);
        KOMVUX_METHOD(y, 1);
        KOMVUX_METHOD(u, 0);
        KOMVUX_METHOD(v, 1);
    KOMVUX_VEC_END;

    using vec2f = vec2<float>;
    using vec2d = vec2<double>;
    using vec2i = vec2<int>;

    // vec3

    KOMVUX_VEC_BEGIN(3);
        KOMVUX_METHOD(x, 0);
        KOMVUX_METHOD(y, 1);
        KOMVUX_METHOD(z, 2);
        KOMVUX_METHOD(r, 0);
        KOMVUX_METHOD(g, 1);
        KOMVUX_METHOD(b, 2);
        KOMVUX_METHOD(u, 0);
        KOMVUX_METHOD(v, 1);
        KOMVUX_METHOD(w, 2);

        vec3<T> cross(const vec3<T>& v) const {
            vec3<T> result;
            result[0] = y() * v.z() - z() * v.y();
            result[1] = z() * v.x() - x() * v.z();
            result[2] = x() * v.y() - y() * v.x();
            return result;
        }

        T dot(const vec3<T>& v) const {
            return x() * v.x() + y() * v.y() + z() * v.z();
        }
    KOMVUX_VEC_END;

    using vec3f = vec3<float>;
    using vec3d = vec3<double>;
    using vec3i = vec3<int>;

    // vec4

    KOMVUX_VEC_BEGIN(4);
        KOMVUX_METHOD(x, 0);
        KOMVUX_METHOD(y, 1);
        KOMVUX_METHOD(z, 2);
        KOMVUX_METHOD(w, 3);
        KOMVUX_METHOD(r, 0);
        KOMVUX_METHOD(g, 1);
        KOMVUX_METHOD(b, 2);
        KOMVUX_METHOD(a, 3);

        vec4<T> operator * (const mat4& m) const {
            vec4<T> result;
            for (int y = 0; y < 4; y++) {
                float i = 0;
                for (int x = 0; x < 4; x++) {
                    i += m[x][y] * (*this)[x];
                }
                result[y] = i;
            }
            return result;
        }

        vec4<T>& operator *= (const mat4& m) {
            return (*this = *this * m);
        }
    KOMVUX_VEC_END;

    using vec4f = vec4<float>;
    using vec4d = vec4<double>;
    using vec4i = vec4<int>;
}

#undef KOMVUX_VEC_END
#undef KOMVUX_VEC_BEGIN
#undef KOMVUX_GENERATE_MATH_OPERATORS
#undef KOMVUX_METHOD