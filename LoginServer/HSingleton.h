#pragma once

#define H_SINGLETON_DECLARE(T)  \
    friend class HSingleton<T>; \
    T() = default;

template <typename T>
class HSingleton
{
private:
    HSingleton(const HSingleton&)            = delete;
    HSingleton(HSingleton&&)                 = delete;
    HSingleton& operator=(const HSingleton&) = delete;
    HSingleton& operator=(HSingleton&&)      = delete;

protected:
    HSingleton() = default;

public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

    ~HSingleton() {}
};
