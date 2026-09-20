//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_MOVE_H
#define CONTAINERS_MOVE_H

namespace mct {
    template<typename T> struct remove_reference { using type = T; };
    template<typename T> struct remove_reference<T&> { using type = T; };
    template<typename T> struct remove_reference<T&&> { using type = T; };
    template<typename T> using remove_reference_t = typename mct::remove_reference<T>::type;

    template<typename T>
    constexpr remove_reference_t<T> move(T&& t) {
        return static_cast<remove_reference_t<T>&&>(t);
    }

    template<typename T> struct remove_pointer { using type = T; };
    template<typename T> struct remove_pointer<T*> { using type = T; };
    template<typename T> using remove_pointer_t = typename mct::remove_pointer<T>::type;

    template<typename T>
    T&& declval() noexcept;

    template<typename T>
    struct is_lvalue_reference {
        static constexpr bool value = false;
    };

    template<typename T>
    struct is_lvalue_reference<T&> {
        static constexpr bool value = true;
    };

    template<typename T>
    inline constexpr bool is_lvalue_reference_v = mct::is_lvalue_reference<T>::value;

    template<typename T>
    constexpr T&& forward(mct::remove_reference_t<T>& t) {
        return static_cast<T&&>(t);
    }

    template<typename T>
    constexpr T&& forward(mct::remove_reference_t<T>&& t) {
        static_assert(!is_lvalue_reference_v<T>);
        return static_cast<T&&>(t);
    }

    template <typename T>
    struct is_ptr {
        static constexpr bool value = false;
    };

    template <typename T>
    struct is_ptr<T*> {
        static constexpr bool value = true;
    };

    template <typename T>
    inline constexpr bool is_ptr_v = mct::is_ptr<T>::value;

    template <typename T>
    struct is_bool {
        static constexpr bool value = false;
    };

    template <>
    struct is_bool<bool> {
        static constexpr bool value = true;
    };

    template <typename T>
    inline constexpr bool is_bool_v = mct::is_bool<T>::value;

}
#endif //CONTAINERS_MOVE_H
