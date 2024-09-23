#include "PreComplier.h"

namespace TemplateFeature
{
    using namespace std;

    template<typename T0>
    void magic(T0 value) {
        std::cout << "type: " << typeid(value).name() << ", value: " << value << std::endl;
    }

    template<typename T, typename... Ts>
    void magic(T value, Ts... args) {
        std::cout << sizeof...(args) << std::endl;
        std::cout << "type: " << typeid(value).name() << ", value: " << value << std::endl;
        magic(args...);
    }

    template<typename T0, typename... T>
    void printf2(T0 t0, T... t) {
        std::cout << t0 << std::endl;
        if constexpr (sizeof...(t) > 0) printf2(t...);
    }

    template<typename T, typename... Ts>
    auto printf3(T value, Ts... args) {
        std::cout << value << std::endl;
        (void)std::initializer_list<T>{([&args] {
            std::cout << args << std::endl;
            }(), value)...};
    }

    using callback = std::function<void()>;

    enum class new_enum : unsigned int {
        value1 = 1,
        value2 = 2,
        value3 = 3,
        value4 = 4
    };

    // std::underlying_type是一个模板类，在 C++ 中用于获取枚举类型的底层整数类型
    template<typename T>
    std::ostream& operator<<(
        typename std::enable_if<std::is_enum<T>::value,
        std::ostream>::type& stream, const T& e)
    {
        return stream << static_cast<typename std::underlying_type<T>::type>(e);
    }

    template<typename E>
    bool operator==(E e, typename std::underlying_type<E>::type i) {
        return static_cast<typename std::underlying_type<E>::type>(e) == i;
    }

    template<typename E>
    bool operator==(typename std::underlying_type<E>::type i, E e) {
        return e == i;
    }

    void TestEnum()
    {
        if (new_enum::value3 == 5) {
            // 会输出 true
            std::cout << "new_enum::value3 == new_enum::value4" << std::endl;
        }
    }

    // 结构化绑定（structured binding）
    template <typename Key, typename Value, typename F>
    void update(std::map<Key, Value>& m, F foo) {
        for (auto& [k, v] : m) {
            m[k] = static_cast<Value>(foo(k));
        }
    }

    void StructBindingTest() {
        std::map<std::string, long long int> m{
            {"a", 1},
            {"b", 2},
            {"c", 3}
        };
        update(m, [](std::string key) {
            return std::hash<std::string>{}(key);
            });
        for (const auto& [key, value] : m)
            std::cout << key << ":" << value << std::endl;
    }

    // 折叠表达式
    template<typename... Args>
    void printArgs(Args... args) {
        auto test = std::ref(((std::cout << args << " "), ...));
        test.get() << std::endl;
    }
}