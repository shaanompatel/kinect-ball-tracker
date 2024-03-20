#ifndef MY_MUTEX
#define MY_MUTEX

#include <Mutex>
#include <libfreenect.hpp>

class myMutex {
public:
    myMutex() {
        pthread_mutex_init(&m_mutex, NULL);
    }
    void lock() {
        pthread_mutex_lock(&m_mutex);
    }
    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};

#endif // MUTEX