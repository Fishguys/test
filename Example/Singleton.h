#include <mutex>
#include <iostream>

template <typename T>
class Singleton
{
public:
    static T& Instance()
    {
        if (m_pInstance == NULL)
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            if (m_pInstance == NULL)
            {
                m_pInstance = new T();
                atexit(Singleton::Destroy);
            }
            return *m_pInstance;
        }
        return *m_pInstance;
    }

protected:
    Singleton(void) {
        std::cout << "Singleton base constructor called!" << std::endl;
    }
    virtual ~Singleton(void) {
        std::cout << "Singleton base destructor called!" << std::endl;
    }

private:
    Singleton(const Singleton& rhs) = delete;
    Singleton& operator = (const Singleton& rhs) = delete;

    static void Destroy()
    {
        if (m_pInstance != NULL)
            delete m_pInstance;
        m_pInstance = NULL;
    }
private:
    static T* volatile m_pInstance;
    static std::mutex m_mutex;
};

template <typename T>
T* volatile Singleton<T>::m_pInstance = NULL;

template <typename T>
std::mutex Singleton<T>::m_mutex;