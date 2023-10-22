#include <atomic>

class SpinLock
{
  private:
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
  public:
    void lock()
    {
        while(m_lock.test_and_set(/*std::memory_order_acquire*/))
        { /* Spin */ }
    }

    void unlock()
    {
        m_lock.clear(/*std::memory_order_release*/);
    }
};
