#pragma once

#define H_SINGLETON_DECLARE(T)  \
    friend class HSingleton<T>; \
    T() = default;

template <typename T>
class HSingleton
{
private:
    HSingleton(const Singleton&)            = delete;
    HSingleton(Singleton&&)                 = delete;
    HSingleton& operator=(const Singleton&) = delete;
    HSingleton& operator=(Singleton&&)      = delete;

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
