#pragma once

#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

namespace blazeLogger
{
    
    template<typename T>
    class ThreadSafeQueue {
    public:

        ThreadSafeQueue() noexcept =  default;
        ~ThreadSafeQueue() noexcept = default;

        // Disable copy
        ThreadSafeQueue(const ThreadSafeQueue&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

        // Allow move
        ThreadSafeQueue(ThreadSafeQueue&&) = default;
        ThreadSafeQueue& operator=(ThreadSafeQueue&&) = default;

        // Push item into queue
        void push(T item) noexcept {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                queue_.push(std::move(item));
            }
            cv_.notify_one();
        }

        // Blocking pop
        bool pop(T& out) noexcept {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] { return !queue_.empty() || is_shutdown_.load(); });

            if (is_shutdown_.load() && queue_.empty())
                return false;

            out = std::move(queue_.front());
            queue_.pop();
            return true;
        }

        // Graceful shutdown
        void shutdown() noexcept {
            is_shutdown_.store(true);
            cv_.notify_all();
        }

        bool empty() const noexcept {
            std::scoped_lock lock(mutex_);
            return queue_.empty();
        }

    private:
        mutable std::mutex mutex_;
        std::condition_variable cv_;
        std::queue<T> queue_;
        std::atomic<bool> is_shutdown_{false};
    };

} // namespace blazeLogger