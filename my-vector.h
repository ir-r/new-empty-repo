#pragma once
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

template <class T>

class my_vector {
  private:
    T* data_ptr_ = nullptr;
    std::size_t actual_size_ = 0;
    std::size_t capacity_ = 0;

    void reallocate(std::size_t new_cap) {
        T* vec_new = nullptr;
        if (new_cap != 0) {
            vec_new = new T[new_cap];
        }
        for (std::size_t i = 0; i < actual_size_; ++i) {
            vec_new[i] = std::move(data_ptr_[i]);
        }
        delete[] data_ptr_;
        data_ptr_ = vec_new;
        capacity_ = new_cap;
    }

    void grow() {
        std::size_t cap_new;
        if (capacity_) {
            cap_new = capacity_ * 2;
        } else {
            cap_new = 1;
        }
        reallocate(cap_new);
    }

  public:
    std::size_t size() const noexcept {
        return actual_size_;
    }

    std::size_t capacity() const noexcept {
        return capacity_;
    }

    my_vector() noexcept = default;

    explicit my_vector(std::size_t sz, const T& val = T()) {
        if (sz != 0) {
            data_ptr_ = new T[sz];
        } else {
            data_ptr_ = nullptr;
        }
        actual_size_ = sz;
        capacity_ = sz;
        for (std::size_t i = 0; i < actual_size_; ++i) {
            data_ptr_[i] = val;
        }
    }

    my_vector(std::initializer_list<T> inp_list) {
        std::size_t sz = inp_list.size();
        if (sz != 0) {
            data_ptr_ = new T[sz];
        } else {
            data_ptr_ = nullptr;
        }
        actual_size_ = sz;
        capacity_ = sz;
        std::size_t i = 0;
        for (const auto& x : inp_list) {
            data_ptr_[i++] = x;
        }
    }

    my_vector(my_vector&& vector2) noexcept
        : data_ptr_(vector2.data_ptr_), actual_size_(vector2.actual_size_),
          capacity_(vector2.capacity_) {
        vector2.data_ptr_ = nullptr;
        vector2.actual_size_ = 0;
        vector2.capacity_ = 0;
    }

    ~my_vector() {
        delete[] data_ptr_;
    }

    my_vector& operator=(const my_vector& vector2) {
        if (this == &vector2) {
            return *this;
        }

        if (vector2.actual_size_ > capacity_) {
            T* new_data;
            if (vector2.actual_size_ != 0) {
                new_data = new T[vector2.actual_size_];
            } else {
                new_data = nullptr;
            }
            delete[] data_ptr_;
            data_ptr_ = new_data;
            capacity_ = vector2.actual_size_;
        }

        actual_size_ = vector2.actual_size_;
        for (std::size_t i = 0; i < actual_size_; ++i) {
            data_ptr_[i] = vector2.data_ptr_[i];
        }
        return *this;
    }

    my_vector& operator=(my_vector&& vector2) noexcept {
        if (this == &vector2) {
            return *this;
        }
        delete[] data_ptr_;
        actual_size_ = vector2.actual_size_;
        data_ptr_ = vector2.data_ptr_;
        capacity_ = vector2.capacity_;
        vector2.actual_size_ = 0;
        vector2.capacity_ = 0;
        vector2.data_ptr_ = nullptr;

        return *this;
    }

    T& operator[](std::size_t i) {
        return data_ptr_[i];
    }

    const T& operator[](std::size_t i) const {
        return data_ptr_[i];
    }

    T& at(std::size_t i) {
        if (i >= actual_size_) {
            throw std::out_of_range("my_vector::at");
        }
        return data_ptr_[i];
    }

    const T& at(std::size_t i) const {
        if (i >= actual_size_) {
            throw std::out_of_range("my_vector::at");
        }
        return data_ptr_[i];
    }

    T& front() {
        return data_ptr_[0];
    }

    const T& front() const {
        return data_ptr_[0];
    }

    T& back() {
        return data_ptr_[actual_size_ - 1];
    }

    const T& back() const {
        return data_ptr_[actual_size_ - 1];
    }

    T* data() noexcept {
        return data_ptr_;
    }

    const T* data() const noexcept {
        return data_ptr_;
    }

    T* begin() noexcept {
        return data_ptr_;
    }

    const T* begin() const noexcept {
        return data_ptr_;
    }

    T* end() noexcept {
        return data_ptr_ + actual_size_;
    }

    const T* end() const noexcept {
        return data_ptr_ + actual_size_;
    }

    bool empty() const noexcept {
        return actual_size_ == 0;
    }

    void pop_back() {
        --actual_size_;
    }

    void push_back(const T& a) {
        if (actual_size_ == capacity_) {
            grow();
        }
        data_ptr_[actual_size_++] = a;
    }

    void push_back(T&& a) {
        if (actual_size_ == capacity_) {
            grow();
        }
        data_ptr_[actual_size_++] = std::move(a);
    }

    void clear() noexcept {
        actual_size_ = 0;
    }

    template <class... Args>
    T& emplace_back(Args&&... args) {
        if (actual_size_ == capacity_) {
            grow();
        }
        data_ptr_[actual_size_] = T(std::forward<Args>(args)...);
        return data_ptr_[actual_size_++];
    }

    void swap(my_vector& another_vector) noexcept {
        std::swap(data_ptr_, another_vector.data_ptr_);
        std::swap(actual_size_, another_vector.actual_size_);
        std::swap(capacity_, another_vector.capacity_);
    }

    void resize(std::size_t sz, const T& val = T()) {
        if (sz <= actual_size_) {
            actual_size_ = sz;
        } else {
            if (sz > capacity_) {
                reallocate(sz);
            }
            for (; actual_size_ < sz; ++actual_size_) {
                data_ptr_[actual_size_] = val;
            }
        }
    }

    void reserve(std::size_t new_cap) {
        if (capacity_ >= new_cap) {
            return;
        }
        reallocate(new_cap);
    }
};