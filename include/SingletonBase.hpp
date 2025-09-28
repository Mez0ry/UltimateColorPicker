#ifndef SINGLETONBASE_H
#define SINGLETONBASE_H
#include <mutex>

class QWidget;
class ColorCombo;

#include <QObject>
#include <QMutex>

template<typename T>
class SingletonBase{
public:

    static T* GetInstance() noexcept {
        QMutexLocker locker(&m_mutex);
        std::call_once(m_InitFlag, [&](){
            m_instance = new T();
        });
        return m_instance;
    }

    static void Destroy() noexcept {
        QMutexLocker locker(&m_mutex);
        delete m_instance;
        m_instance = nullptr;
    }
private:
    inline static T* m_instance = nullptr;
    inline static QMutex m_mutex;
    inline static std::once_flag m_InitFlag;
};

#endif // SINGLETONBASE_H
