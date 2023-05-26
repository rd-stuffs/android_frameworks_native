/*
 * Copyright 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <utils/threads.h>
#include <list>
#include <mutex>
#include <optional>

namespace android {

/** A thread-safe FIFO queue. */
template <class T>
class SyncQueue {
public:
    /** Retrieve and remove the oldest object. Returns std::nullopt if the queue is empty. */
    std::optional<T> pop() {
        std::scoped_lock lock(mLock);
        if (mQueue.empty()) {
            return {};
        }
        T t = std::move(mQueue.front());
        mQueue.erase(mQueue.begin());
        return t;
    };

    /** Add a new object to the queue. */
    template <class... Args>
    void push(Args&&... args) {
        std::scoped_lock lock(mLock);
        mQueue.emplace_back(args...);
    };

private:
    std::mutex mLock;
    std::list<T> mQueue GUARDED_BY(mLock);
};

} // namespace android
