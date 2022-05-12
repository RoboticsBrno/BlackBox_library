#pragma once

#include <SmartLeds.h>
#include <any>
#include <array>
#include <functional>

#include "library/BlackBox_CircularInteger.hpp"

class BaseDisplay {
public:
    class Iterator {
    private:
        Rgb* m_element;

        const Rgb* const m_begin;
        const Rgb* const m_end;

        const ssize_t m_offset;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Rgb;
        using pointer = Rgb*;
        using reference = Rgb&;

        Iterator();
        Iterator(const Iterator&);
        ~Iterator();

        Iterator& operator=(const Iterator&);

        friend bool operator==(const Iterator& a, const Iterator& b);
        friend bool operator!=(const Iterator& a, const Iterator& b);
        bool operator<(const Iterator&) const;
        bool operator>(const Iterator&) const;
        bool operator<=(const Iterator&) const;
        bool operator>=(const Iterator&) const;

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);

        Iterator& operator+=(size_type);
        Iterator operator+(size_type) const;
        friend Iterator operator+(size_type, const Iterator&);
        Iterator& operator-=(size_type);
        Iterator operator-(size_type) const;
        difference_type operator-(Iterator) const;

        reference operator*() const;
        pointer operator->();
    };

    class CIterator {
    private:
        const Rgb* m_element;

        const Rgb* const m_begin;
        const Rgb* const m_end;

        const ssize_t m_offset;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const Rgb;
        using pointer = const Rgb*;
        using reference = const Rgb&;

        CIterator();
        CIterator(const CIterator&);
        CIterator(const Iterator&);
        ~CIterator();

        CIterator& operator=(const CIterator&);

        friend bool operator==(const CIterator& a, const CIterator& b);
        friend bool operator!=(const CIterator& a, const CIterator& b);
        bool operator<(const CIterator&) const;
        bool operator>(const CIterator&) const;
        bool operator<=(const CIterator&) const;
        bool operator>=(const CIterator&) const;

        CIterator& operator++();
        CIterator operator++(int);
        CIterator& operator--();
        CIterator operator--(int);

        CIterator& operator+=(size_type);
        CIterator operator+(size_type) const;
        friend CIterator operator+(size_type, const CIterator&);
        CIterator& operator-=(size_type);
        CIterator operator-(size_type) const;
        difference_type operator-(CIterator) const;

        reference operator*() const;
        pointer operator->();
        reference operator[](ssize_t);
    };

    using RIterator = std::reverse_iterator<Iterator>;
    using RCIterator = std::reverse_iterator<CIterator>;

    virtual Rgb& operator[](ssize_t index) = 0;
    virtual const Rgb& operator[](ssize_t index) const = 0;
    virtual Rgb& at(ssize_t index) = 0;
    virtual const Rgb& at(ssize_t index) const = 0;

    virtual void colorize(std::function<void(Rgb&, ssize_t, std::any&)>, std::any&) = 0;
    virtual void fill(const Rgb&) = 0;

    virtual ssize_t size() = 0;

    virtual Iterator begin() = 0;
    virtual CIterator begin() const = 0;
    virtual CIterator cbegin() const = 0;

    virtual Iterator end() = 0;
    virtual CIterator end() const = 0;
    virtual CIterator cend() const = 0;

    virtual RIterator rbegin() = 0;
    virtual RCIterator rbegin() const = 0;
    virtual RCIterator rcbegin() const = 0;

    virtual RIterator rend() = 0;
    virtual RCIterator rend() const = 0;
    virtual RCIterator rcend() const = 0;
};

class Display : public BaseDisplay {
private:
    std::array<Rgb> m_buffer;

public:
    virtual const Rgb& operator[](ssize_t index) const override;
    virtual Rgb& operator[](ssize_t index) override;
    virtual void colorize(std::function<void(Rgb&, ssize_t, std::any&)>, std::any&) override;
    virtual void fill(const Rgb&) override;
    virtual ssize_t size() override;
    auto test = m_buffer.begin();
};
