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
    
        template<typename U = T> intrusive_ptr<const volatile U> shared_from_this() const volatile noexcept;
        template<typename U = T> intrusive_ptr<const U> shared_from_this() const noexcept;
        template<typename U = T> intrusive_ptr<volatile U> shared_from_this() volatile noexcept;
        template<typename U = T> intrusive_ptr<U> shared_from_this() noexcept;
    
        template<typename U = T> intrusive_weak_ptr<const volatile U> weak_from_this() const volatile;
        template<typename U = T> intrusive_weak_ptr<const U> weak_from_this() const;
        template<typename U = T> intrusive_weak_ptr<volatile U> weak_from_this() volatile;
        template<typename U = T> intrusive_weak_ptr<U> weak_from_this();
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
    
    template<typename T, typename ... Args> intrusive_ptr<T> make_intrusive(Args &&... args);
    
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

# `template<typename T, class D> class intrusive_base`

##### In general

* `T` and `D` shall be complete types.
* `intrusive_base<T, D>` shall be a public, non-ambiguous base of `T`.
* `D` shall satisfy the requirements of **DefaultConstructible**. None of the default constructor, the copy/move constructor, the copy/move assignment operator of `D` shall throw exceptions.

##### `constexpr intrusive_base() noexcept;`

* _Post-condition:_ `use_count() == 1`.

##### `constexpr intrusive_base(const intrusive_base & rhs) noexcept;`

* _Effects:_ `intrusive_base()`.

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

* _Effects:_ Let `pu` be the result of conversion from `this` to type `cv U *`. If `pu` is not null, increments the reference count of `pu`.
* _Returns:_ `intrusive_ptr<cv U>(pu)`.
* _Post-condition:_ If `pu` is not null, `use_count()` is one greater than the value before the call.

##### `template<typename U = T> intrusive_weak_ptr<const volatile U> weak_from_this() const volatile;`
##### `template<typename U = T> intrusive_weak_ptr<const U> weak_from_this() const;`
##### `template<typename U = T> intrusive_weak_ptr<volatile U> weak_from_this() volatile;`
##### `template<typename U = T> intrusive_weak_ptr<U> weak_from_this();`

* _Returns:_ `intrusive_weak_ptr<cv U>(shared_from_this())`, as if `shared_from_this()` had never returned a null pointer.
* _Throws:_ Any exceptions that might be thrown by `reserve_weak()`.
* _Post-condition:_ `weak_count()` is one greater than the value before the call.

# `template<typename T> class intrusive_ptr`

##### In general

* `T` shall be a complete type.
* Let `intrusive_base<T, D>` be a public, non-ambiguous base of `T`. `deleter_type` is the template parameter `D`. If no such base can be found, the program is ill-formed.

##### `constexpr intrusive_ptr(nullptr_t = nullptr) noexcept;`

* _Post-condition:_ `get() == nullptr` and `use_count() == 0` and `weak_count() == 0`.

##### `explicit constexpr intrusive_ptr(element_type * rhs) noexcept;`

* _Effects:_ If `rhs` is not null, increments the reference count of `*rhs`.
* _Post-condition:_ `get() == rhs`. If `rhs` is not null, `rhs->use_count()` is one greater than the value before the call.

##### `template<typename U, typename E> intrusive_ptr(unique_ptr<U, E> && rhs) noexcept;`

* _Remarks:_ This constructor shall not participate in overload resolution unless `U *` is implicitly convertible to `T *` and `E` is implicitly convertible to `D`.
* _Effects:_ `intrusive_ptr(rhs.release())`.

##### `template<typename U> intrusive_ptr(const intrusive_ptr<U> & rhs) noexcept;`

* _Remarks:_ This constructor shall not participate in overload resolution unless `U *` is implicitly convertible to `T *`.
* _Effects:_ If `rhs` is not null, increments the reference count of `*rhs`.
* _Post-condition:_ `get() == rhs.get()`and `use_count() == rhs.use_count()`. If `rhs.use_count()` is greater than zero before the call, `use_count()` is one greater than that value.

##### `template<typename U> intrusive_ptr(intrusive_ptr<U> && rhs) noexcept;`

* _Remarks:_ This constructor shall not participate in overload resolution unless `U *` is implicitly convertible to `T *`.
* _Post-condition:_ `get()` equals the value of `rhs.get()` before the call and `rhs.get() == nullptr`. `use_count()` equals the value of `rhs.use_count()` before the call and `rhs.use_count() == 0`.

##### `intrusive_ptr(const intrusive_ptr & rhs) noexcept;`

* _Effects:_ `intrusive_ptr(rhs.get())`

##### `intrusive_ptr(intrusive_ptr && rhs) noexcept;`

* _Effects:_ `intrusive_ptr(rhs.release())`

##### `intrusive_ptr & operator=(const intrusive_ptr & rhs) noexcept;`

* _Effects:_ `intrusive_ptr(rhs).swap(*this)`.

##### `intrusive_ptr & operator=(intrusive_ptr && rhs) noexcept;`

* _Effects:_ `reset()` followed by `rhs.swap(*this)`.

##### `~intrusive_ptr();`

* _Effects:_ If `get()` is not null, decrements the reference count of `*get()`, and if the result is zero, deletes the object as follows:

        using base = intrusive_base<T, D>;
        auto d = move(get()->base::get_deleter());
        move(d)(get());

##### `element_type * get() const noexcept;`

* _Returns:_ The stored pointer.

##### `element_type * release() noexcept;`

* _Effects:_ Sets the stored pointer to `nullptr` without deleting any objects.
* _Returns:_ The value of `get()` before the call.
* _Post-condition:_ `get() == nullptr`.

##### `long use_count() const noexcept;`

* _Returns:_ If `get()` is null, `0`. Otherwise, `get()->intrusive_base<T, D>::use_count()`.

##### `long weak_count() const noexcept;`

* _Returns:_ If `get()` is null, `0`. Otherwise, `get()->intrusive_base<T, D>::weak_count()`.

##### `void reset(nullptr_t = nullptr) noexcept;`

* _Effects:_ `intrusive_ptr().swap(*this)`.

##### `void reset(element_type * rhs) noexcept;`

* _Effects:_ `intrusive_ptr(rhs).swap(*this)`.

##### `void swap(intrusive_ptr & rhs) noexcept;`

* _Post-condition:_ `get()` equals the value of `rhs.get()` before the call and `rhs.get()` equals the value of `get()` before the call. `use_count()` equals the value of `rhs.use_count()` before the call and `rhs.use_count()` equals the value of `use_count()` before the call.

##### `explicit constexpr operator bool() const noexcept;`

* _Returns:_ `get() != nullptr`.

##### `element_type & operator*() const;`

* _Pre-condition:_ `get()` shall not be null.
* _Returns:_ `*get()`.

##### `element_type * operator->() const;`

* _Pre-condition:_ `get()` shall not be null.
* _Returns:_ `get()`.

##### `template<typename T1, typename T2> bool operator==(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator==(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator==(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;`

* _Returns:_ `lhs.get() == rhs.get()`, `lhs.get() == rhs` and `lhs == rhs.get()`, respectively.

##### `template<typename T1, typename T2> bool operator!=(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator!=(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator!=(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;`

* _Returns:_ `lhs.get() != rhs.get()`, `lhs.get() != rhs` and `lhs != rhs.get()`, respectively.

##### `template<typename T1, typename T2> bool operator<(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator<(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator<(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;`

* _Returns:_ `lhs.get() < rhs.get()`, `lhs.get() < rhs` and `lhs < rhs.get()`, respectively.

##### `template<typename T1, typename T2> bool operator>(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator>(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator>(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;`

* _Returns:_ `lhs.get() > rhs.get()`, `lhs.get() > rhs` and `lhs > rhs.get()`, respectively.

##### `template<typename T1, typename T2> bool operator<=(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator<=(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator<=(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;`

* _Returns:_ `lhs.get() <= rhs.get()`, `lhs.get() <= rhs` and `lhs <= rhs.get()`, respectively.

##### `template<typename T1, typename T2> bool operator>=(const intrusive_ptr<T1> & lhs, const intrusive_ptr<T2> & rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator>=(const intrusive_ptr<T1> & lhs, T2 * rhs) noexcept;`
##### `template<typename T1, typename T2> bool operator>=(T1 * lhs, const intrusive_ptr<T2> & rhs) noexcept;`

* _Returns:_ `lhs.get() >= rhs.get()`, `lhs.get() >= rhs` and `lhs >= rhs.get()`, respectively.

##### `template<typename T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs) noexcept;`

* _Effects:_ `lhs.swap(rhs)`.

##### `template<typename T, typename ... Args> intrusive_ptr<T> make_intrusive(Args &&... args);`

* _Returns:_ `intrusive_ptr<T>(new T(forward<Args>(args)...))`.

##### `template<typename U, typename T> intrusive_ptr<U> static_pointer_cast(intrusive_ptr<T> src) noexcept;`

* _Effects:_ Let `u` be the result of `static_cast<U *>(src.get())`. Calls `src.release()`.
* _Returns:_ `intrusive_ptr<U>(u)`.

##### `template<typename U, typename T> intrusive_ptr<U> dynamic_pointer_cast(intrusive_ptr<T> src) noexcept;`

* _Effects:_ Let `u` be the result of `dynamic_cast<U *>(src.get())`. If `u` is not null, calls `src.release()`.
* _Returns:_ `intrusive_ptr<U>(u)`.

##### `template<typename U, typename T> intrusive_ptr<U> const_pointer_cast(intrusive_ptr<T> src) noexcept;`

* _Effects:_ Let `u` be the result of `const_cast<U *>(src.get())`. Calls `src.release()`.
* _Returns:_ `intrusive_ptr<U>(u)`.

# `template<typename T> class intrusive_weak_ptr`

##### In general

* `T` shall be a complete type.
* Let `intrusive_base<T, D>` be a public, non-ambiguous base of `T`. `deleter_type` is the template parameter `D`. If no such base can be found, the program is ill-formed.

##### `constexpr intrusive_weak_ptr(nullptr_t = nullptr) noexcept;`

* _Post-condition:_ `expired()` and `weak_count() == 0`.

##### `explicit intrusive_weak_ptr(element_type * rhs);`

* _Effects:_ If `rhs` is not null, increments the weak reference count of `*rhs`.
* _Throws:_ Any exceptions that might be thrown by `rhs->reserve_weak()`.
* _Post-condition:_ `lock().get() == rhs`. If `rhs` is not null, `rhs->weak_count()` is one greater than the value before the call.

##### `intrusive_weak_ptr(const intrusive_ptr<T> & rhs);`

* _Effects:_ `intrusive_weak_ptr(rhs.get())`.

##### `template<typename U> intrusive_weak_ptr(const intrusive_weak_ptr<U> & rhs) noexcept;`

* _Remarks:_ This constructor shall not participate in overload resolution unless `U *` is implicitly convertible to `T *`.
* _Effects:_ If `rhs.weak_count()` is greater than zero, increments the weak reference count of the object refered by `rhs`, as if it had never expired.
* _Post-condition:_ `weak_count() == rhs.weak_count()`. If `rhs.weak_count()` is greater than zero before the call, `weak_count()` is one greater than that value.

##### `template<typename U> intrusive_weak_ptr(intrusive_weak_ptr<U> && rhs) noexcept;`

* _Remarks:_ This constructor shall not participate in overload resolution unless `U *` is implicitly convertible to `T *`.
* _Post-condition:_ `weak_count()` equals the value of `rhs.weak_count()` before the call and `rhs.weak_count() == 0`.

##### `intrusive_weak_ptr(const intrusive_weak_ptr & rhs) noexcept;`

* _Effects:_ `intrusive_weak_ptr(rhs.lock().get())`, as if the object referred by `rhs` had never expired, if any.

##### `intrusive_weak_ptr(intrusive_weak_ptr && rhs) noexcept;`

* _Effects:_ `intrusive_weak_ptr(rhs.lock().get())` then `rhs.reset()`, as if the object referred by `rhs` had never expired, if any.

##### `intrusive_weak_ptr & operator=(const intrusive_weak_ptr & rhs) noexcept;`

* _Effects:_ `intrusive_weak_ptr(rhs).swap(*this)`.

##### `intrusive_weak_ptr & operator=(intrusive_weak_ptr && rhs) noexcept;`

* _Effects:_ `reset()` followed by `rhs.swap(*this)`.

##### `~intrusive_weak_ptr();`

* _Effects:_ Let `pt` be a pointer to the object that `*this` is referring as if that object had never expired, or a null pointer if no such object exists. If `pt` is not null, decrements the weak reference count of `*pt`.

##### `bool expired() const noexcept;`

* _Effects:_ Let `pt` be a pointer to the object that `*this` is referring as if that object had never expired, or a null pointer if no such object exists.
* _Returns:_ If `pt` is null, `true`. Otherwise, `pt->use_count() > 0`.

##### `long weak_count() const noexcept;`

* _Effects:_ Let `pt` be a pointer to the object that `*this` is referring as if that object had never expired, or a null pointer if no such object exists.
* _Returns:_ If `pt` is null, `0`. Otherwise, `pt->weak_count()`.

##### `template<typename U = T> intrusive_ptr<U> lock() const noexcept;`

* _Effects:_ Atomically executes: Let `pt` be a pointer to the object that `*this` is referring as if that object had never expired, or a null pointer if no such object exists. Let `pu` be the result of conversion from `pu` to type `U *`. If `pu` is not null, increments the reference count of `pu`.
* _Returns:_ `intrusive_ptr<U>(pu)`.
* _Post-condition:_ If `pu` is not null, `use_count()` is one greater than `pt->intrusive_base<T, D>::use_count()` before the call.

##### `void reset(nullptr_t = nullptr) noexcept;`

* _Effects:_ `intrusive_weak_ptr().swap(*this)`.

##### `void reset(element_type * rhs);`

* _Effects:_ `intrusive_weak_ptr(rhs).swap(*this)`.

##### `void swap(intrusive_weak_ptr & rhs) noexcept;`

* _Post-condition:_ `lock()` equals the value of `rhs.lock()` before the call and `rhs.lock()` equals the value of `lock()` before the call. `weak_count()` equals the value of `rhs.weak_count()` before the call and `rhs.weak_count()` equals the value of `weak_count()` before the call.

##### `template<typename T> bool operator==(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;`
##### `template<typename T> bool operator!=(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;`
##### `template<typename T> bool operator< (const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;`
##### `template<typename T> bool operator> (const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;`
##### `template<typename T> bool operator<=(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;`
##### `template<typename T> bool operator>=(const intrusive_weak_ptr<T> & lhs, const intrusive_weak_ptr<T> & rhs) const noexcept;`

* _Remarks:_ These functions define an otherwise unspecified strick weak ordering of `intrusive_weak_ptr`, enabling use of `intrusive_weak_ptr` in associative containers.

##### `template<typename T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs) noexcept;`

* _Effects:_ `lhs.swap(rhs)`.
