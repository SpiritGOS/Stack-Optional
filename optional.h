<<<<<<< HEAD
﻿#pragma once
#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);

    Optional& operator=(const T& value);
    Optional& operator=(T&& rhs);
    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs);

    ~Optional();

    template<typename... Args>
    void Emplace(Args&&... args);

    bool HasValue() const;

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value();
    const T& Value() const;

    void Reset();

    void swap(Optional& other);

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* value_ = nullptr;
};

template<typename T>
inline Optional<T>::Optional(const T& value)
{
    value_ = new (&data_[0]) T(value);
    is_initialized_ = true;
}

template<typename T>
inline Optional<T>::Optional(T&& value)
{
    value_ = new (&data_[0]) T(std::move(value));
    is_initialized_ = true;
}

template<typename T>
inline Optional<T>::Optional(const Optional& other)
{
    if (other.is_initialized_) {
        Optional<T> tmp;
        tmp.is_initialized_ = other.is_initialized_;
        tmp.value_ = new (&data_[0]) T(*other.value_);
        swap(tmp);
    }
}

template<typename T>
inline Optional<T>::Optional(Optional&& other)
{
    if (other.is_initialized_) {
        Optional<T> tmp;
        tmp.is_initialized_ = other.is_initialized_;
        tmp.value_ = new (&data_[0]) T(std::move(*other.value_));
        swap(tmp);
    }
}

template<typename T>
inline Optional<T>& Optional<T>::operator=(const T& value)
{
    if (!is_initialized_) {
        value_ = new (&data_[0]) T(value);
        is_initialized_ = true;
    }
    else *value_ = value;

    return *this;
}

template<typename T>
inline Optional<T>& Optional<T>::operator=(T&& rhs)
{
    if (!is_initialized_) {
        value_ = new (&data_[0]) T(std::move(rhs));
        is_initialized_ = true;
    }
    else *value_ = std::move(rhs);

    return *this;
}

template<typename T>
inline Optional<T>& Optional<T>::operator=(const Optional& rhs)
{
    if (!is_initialized_) {
        if (rhs.is_initialized_) {
            value_ = new (&data_[0]) T(rhs.Value());
            is_initialized_ = rhs.is_initialized_;
        }
    }
    else if (rhs.is_initialized_) {
        *value_ = rhs.Value();
    }
    else Reset();

    return *this;
}

template<typename T>
inline Optional<T>& Optional<T>::operator=(Optional&& rhs)
{
    if (!is_initialized_) {
        if (rhs.is_initialized_) {
            value_ = new (&data_[0]) T(std::move(rhs.Value()));
            is_initialized_ = rhs.is_initialized_;
        }
    }
    else if (rhs.is_initialized_) {
        *value_ = std::move(rhs.Value());
    }
    else Reset();

    return *this;
}

template<typename T>
inline Optional<T>::~Optional()
{
    Reset();
}

template<typename T>
inline bool Optional<T>::HasValue() const
{
    return is_initialized_;
}

template<typename T>
inline T& Optional<T>::operator*()
{
    return *value_;
}

template<typename T>
inline const T& Optional<T>::operator*() const
{
    return *value_;
}

template<typename T>
inline T* Optional<T>::operator->()
{
    return value_;
}

template<typename T>
inline const T* Optional<T>::operator->() const
{
    return value_;
}

template<typename T>
inline T& Optional<T>::Value()
{
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return *value_;
}

template<typename T>
inline const T& Optional<T>::Value() const
{
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return *value_;
}

template<typename T>
inline void Optional<T>::Reset()
{
    if (is_initialized_) {
        value_->~T();
        value_ = nullptr;
        is_initialized_ = false;
    }
}

template<typename T>
inline void Optional<T>::swap(Optional& other)
{
    auto init = is_initialized_;
    is_initialized_ = other.is_initialized_;
    other.is_initialized_ = init;

    auto val = value_;
    value_ = other.value_;
    other.value_ = val;
}

template<typename T>
template<typename ...Args>
inline void Optional<T>::Emplace(Args && ...args)
{
    if (is_initialized_) {
        Reset();
    }
    value_ = new (&data_[0]) T(std::forward<Args>(args)...);
    is_initialized_ = true;
}
=======
﻿#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);

    Optional& operator=(const T& value);
    Optional& operator=(T&& rhs);
    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs);

    ~Optional();

    bool HasValue() const;

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value();
    const T& Value() const;

    void Reset();

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
};
>>>>>>> 7be885c44619747b4645d9f6966ecab511baa0fe
