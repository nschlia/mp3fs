#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

/**
 * @brief The thread_pool class.
 */
class thread_pool
{
    typedef struct THREADINFO                       /**< Thread info structure */
    {
        void (*m_thread_func)(void *);              /**< Job function pointer */
        void *m_opaque;                             /**< Parameter for job function */
    } THREADINFO;

public:
    /**
     * @brief Construct a thread_pool object.
     * @param[in] num_threads - Optional: number of threads to create in pool. Defaults to Defaults to 4 x number of CPU cores.
     */
    explicit thread_pool(unsigned int num_threads = std::thread::hardware_concurrency() * 4);
    /**
     * @brief Object destructor. Ends all threads and cleans up resources.
     */
    virtual ~thread_pool();

    /**
     * @brief Initialise thread pool.
     * @param[in] num_threads - Optional: number of threads to create in pool. Defaults to Defaults to 4x number of CPU cores.
     */
    void            init(unsigned int num_threads = 0);
    /**
     * @brief Shut down the thread pool.
     * @param[in] silent - If true, no log messages will be issued.
     */
    void            tear_down(bool silent = false);
    /**
     * @brief Schedule a new thread from pool.
     * @param[in] thread_func - Thread function to start.
     * @param[in] opaque - Parameter passed to thread function.
     * @return Returns true if thread was successfully scheduled, fals if not.
     */
    bool            schedule_thread(void (*thread_func)(void *), void *opaque);
    /**
     * @brief Get number of currently running threads.
     * @return Returns number of currently running threads.
     */
    unsigned int    current_running() const;
    /**
     * @brief Get number of currently queued threads.
     * @return Returns number of currently queued threads.
     */
    unsigned int    current_queued();
    /**
     * @brief Get current pool size.
     * @return Return current pool size.
     */
    unsigned int    pool_size() const;

private:
    /**
     * @brief Start loop function.
     * @param[in] tp - Thread pool object of caller.
     */
    static void     loop_function_starter(thread_pool &tp);
    /**
     * @brief Start loop function
     */
    void            loop_function();

protected:
    std::vector<std::thread>    m_thread_pool;      /**< Thread pool */
    std::mutex                  m_queue_mutex;      /**< Mutex for critical section */
    std::condition_variable     m_queue_condition;  /**< Condition for critical section */
    std::queue<THREADINFO>      m_thread_queue;     /**< Thread queue parameters */
    volatile bool               m_queue_shutdown;   /**< If true all threads have been shut down */
    unsigned int                m_num_threads;      /**< Max. number of threads. Defaults to 4x number of CPU cores. */
    unsigned int                m_cur_threads;      /**< Current number of threads. */
    volatile unsigned int       m_threads_running;  /**< Currently running threads. */
};

#endif // THREAD_POOL_H
