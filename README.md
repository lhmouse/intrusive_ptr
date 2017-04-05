# Proposal for std::intrusive_ptr

## Synopsis

    #include <memory>
    
    namespace std {
    
    template<typename T, typename D = default_delete<T>>
    class intrusive_base;
    
    template<typename T>
    class intrusive_ptr;
    template<typename T>
    class intrusive_weak_ptr;
    
    template<typename T, class D>
    class intrusive_base {
    public:
        constexpr intrusive_base() noexcept;
        constexpr intrusive_base(const intrusive_base & rhs) noexcept;
        intrusive_base & operator=(const intrusive_base & rhs) noexcept;
        ~intrusive_base();
    
    public:
        const deleter_type & get_deleter() const noexcept;
        deleter_type & get_deleter() noexcept;
    
        bool unique() const volatile noexcept;
        long use_count() const volatile noexcept;
        long weak_count() const noexcept;
        void reserve_weak() const volatile;
    
        template<typename U = T>
        intrusive_ptr<const volatile U> shared_from_this() const volatile noexcept;
        template<typename U = T>
        intrusive_ptr<const U> shared_from_this() const noexcept;
        template<typename U = T>
        intrusive_ptr<volatile U> shared_from_this() volatile noexcept;
        template<typename U = T>
        intrusive_ptr<U> shared_from_this() noexcept;
    
        template<typename U = T>
        intrusive_weak_ptr<const volatile U> weak_from_this() const volatile;
        template<typename U = T>
        intrusive_weak_ptr<const U> weak_from_this() const;
        template<typename U = T>
        intrusive_weak_ptr<volatile U> weak_from_this() volatile;
        template<typename U = T>
        intrusive_weak_ptr<U> weak_from_this();
    };
    
    template<typename T>
    class intrusive_ptr {
    public:
        using pointer      = T *;
        using element_type = T;
        using deleter_type = /* see below */;
    
        constexpr intrusive_ptr(nullptr_t = nullptr) noexcept;
        explicit constexpr intrusive_ptr(element_type * rhs) noexcept;
        template<typename U, typename E> intrusive_ptr(unique_ptr<U, E> && rhs) noexcept;
        template<typename U> intrusive_ptr(const intrusive_ptr<U> & rhs) noexcept;
        template<typename U> intrusive_ptr(intrusive_ptr<U> && rhs) noexcept;
        intrusive_ptr(const intrusive_ptr & rhs) noexcept;
        intrusive_ptr(intrusive_ptr && rhs) noexcept;
        intrusive_ptr & operator=(const intrusive_ptr & rhs) noexcept;
        intrusive_ptr & operator=(intrusive_ptr && rhs) noexcept;
        ~intrusive_ptr();
    
        element_type * get() const noexcept;
        element_type * release() noexcept;
        long use_count() const noexcept;
        long weak_count() const noexcept;
    
        void reset(nullptr_t = nullptr) noexcept;
        void reset(element_type * rhs) noexcept;
    
        void swap(intrusive_ptr & rhs) noexcept;
    
        explicit constexpr operator bool() const noexcept;
    
        element_type & operator*() const;
        element_type * operator->() const;
    };
    
    template<typename T1, typename T2> bool operator==(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;
    template<typename T1, typename T2> bool operator==(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;
    template<typename T1, typename T2> bool operator==(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;
    
    template<typename T1, typename T2> bool operator!=(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;
    template<typename T1, typename T2> bool operator!=(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;
    template<typename T1, typename T2> bool operator!=(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;
    
    template<typename T1, typename T2> bool operator< (const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;
    template<typename T1, typename T2> bool operator< (const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;
    template<typename T1, typename T2> bool operator< (T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;
    
    template<typename T1, typename T2> bool operator> (const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;
    template<typename T1, typename T2> bool operator> (const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;
    template<typename T1, typename T2> bool operator> (T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;
    
    template<typename T1, typename T2> bool operator<=(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;
    template<typename T1, typename T2> bool operator<=(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;
    template<typename T1, typename T2> bool operator<=(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;
    
    template<typename T1, typename T2> bool operator>=(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;
    template<typename T1, typename T2> bool operator>=(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;
    template<typename T1, typename T2> bool operator>=(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;
    
    template<typename T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs) noexcept;
    
    template<typename T, typename ... Args>
    intrusive_ptr<T> make_intrusive(Args &&... args);
    
    template<typename U, typename T> intrusive_ptr<U> static_pointer_cast(intrusive_ptr<T> src) noexcept;
    template<typename U, typename T> intrusive_ptr<U> dynamic_pointer_cast(intrusive_ptr<T> src) noexcept;
    template<typename U, typename T> intrusive_ptr<U> const_pointer_cast(intrusive_ptr<T> src) noexcept;
    
    template<typename T>
    class intrusive_weak_ptr {
    public:
        using pointer      = typename intrusive_ptr<T>::pointer;
        using element_type = typename intrusive_ptr<T>::element_type;
        using deleter_type = typename intrusive_ptr<T>::deleter_type;
    
        constexpr intrusive_weak_ptr(nullptr_t = nullptr) noexcept;
        explicit intrusive_weak_ptr(element_type * rhs);
        intrusive_weak_ptr(const intrusive_ptr<T> & rhs);
        template<typename U> intrusive_weak_ptr(const intrusive_weak_ptr<U> & rhs) noexcept;
        template<typename U> intrusive_weak_ptr(intrusive_weak_ptr<U> && rhs) noexcept;
        intrusive_weak_ptr(const intrusive_weak_ptr & rhs) noexcept;
        intrusive_weak_ptr(intrusive_weak_ptr && rhs) noexcept;
        intrusive_weak_ptr & operator=(const intrusive_weak_ptr & rhs) noexcept;
        intrusive_weak_ptr & operator=(intrusive_weak_ptr && rhs) noexcept;
        ~intrusive_weak_ptr();
    
        bool expired() const noexcept;
        long weak_count() const noexcept;
        template<typename U = T> intrusive_ptr<U> lock() const noexcept;
    
        void reset(nullptr_t = nullptr) noexcept;
        void reset(element_type * rhs);
    
        void swap(intrusive_weak_ptr & rhs) noexcept;
    };
    
    template<typename T> bool operator==(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;
    
    template<typename T> bool operator!=(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;
    
    template<typename T> bool operator< (const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;
    
    template<typename T> bool operator> (const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;
    
    template<typename T> bool operator<=(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;
    
    template<typename T> bool operator>=(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;
    
    template<typename T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs) noexcept;
    
    }

# Requirements of `intrusive_base`

##### `constexpr intrusive_base() noexcept;`

* _Post-condition:_ `use_count() == 1`.

##### `constexpr intrusive_base(const intrusive_base & rhs) noexcept;`

* _Effects:_ The same as `intrusive_base::intrusive_base()`.

##### `intrusive_base & operator=(const intrusive_base & rhs) noexcept;`

* _Effects:_ No operation.
* _Returns:_ `*this`.

##### `~intrusive_base();`

* _Effects:_ If `use_count() > 1` calls `terminate()`.

##### `const deleter_type & get_deleter() const noexcept;`
##### `deleter_type & get_deleter() noexcept;`

* _Returns:_ A reference to the stored deleter.

##### `bool unique() const volatile noexcept;`

* _Returns:_ `use_count() == 1`.

##### `long use_count() const volatile noexcept;`

* _Returns:_ The reference count of this object.

##### `long weak_count() const volatile noexcept;`

* _Returns:_ The weak reference count of this object.

##### `void reserve_weak() const volatile;`

* _Effects:_ Satisifies any allocation requests for resources that would be required if an `intrusive_weak_ptr` was to be constructed. Construction of `intrusive_weak_ptr` referring this object shall not throw any exceptions hereafter.
* _Throws:_ `bad_alloc`, or an implementation-defined exception when a resource other than memory could not be obtained.

##### `template<typename U = T> intrusive_ptr<const volatile U> shared_from_this() const volatile noexcept;`
##### `template<typename U = T> intrusive_ptr<const U> shared_from_this() const noexcept;`
##### `template<typename U = T> intrusive_ptr<volatile U> shared_from_this() volatile noexcept;`
##### `template<typename U = T> intrusive_ptr<U> shared_from_this() noexcept;`

* _Effects:_ Converts `this` to `cv U *`, and if the result is not null, increments the reference count of this object.
* _Returns:_ `intrusive_ptr<cv U>(u)`, where `u` is the conversion above.
* _Post-condition:_ `use_count()` is one greater than the value before the call.

##### `template<typename U = T> intrusive_weak_ptr<const volatile U> weak_from_this() const volatile;`
##### `template<typename U = T> intrusive_weak_ptr<const U> weak_from_this() const;`
##### `template<typename U = T> intrusive_weak_ptr<volatile U> weak_from_this() volatile;`
##### `template<typename U = T> intrusive_weak_ptr<U> weak_from_this();`

* _Returns:_ `intrusive_weak_ptr<cv U>(shared_from_this())`.
* _Throws:_ Any exception that could be thrown by `reserve_weak()`.
* _Post-condition:_ `weak_count()` is one greater than the value before the call.

