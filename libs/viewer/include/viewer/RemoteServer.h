/*
 * Copyright (C) 2021 The Android Open Source Project
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

#ifndef VIEWER_REMOTE_SERVER_H
#define VIEWER_REMOTE_SERVER_H

#include <stddef.h>
#include <mutex>

class CivetServer;

namespace filament {
namespace viewer {

class WebSocketHandler;

struct IncomingMessage {
    enum { BUFFER, COMMAND } type;
    char* label;
    char* buffer;
    size_t bufferByteCount;
    size_t fetchedByteCount;
    size_t messageIndex;
};

class RemoteServer {
public:
    RemoteServer(int port = 8082);
    ~RemoteServer();
    bool isValid() const { return mCivetServer; }
    IncomingMessage const* peekIncomingMessage() const;
    IncomingMessage const* acquireIncomingMessage();
    void releaseIncomingMessage(IncomingMessage* message);

private:
    void enqueueIncomingMessage(IncomingMessage* message);
    CivetServer* mCivetServer = nullptr;
    WebSocketHandler* mWebSocketHandler = nullptr;
    size_t mNextMessageIndex = 0;
    size_t mOldestMessageIndex = 0;
    static const size_t kMessageCapacity = 4;
    IncomingMessage* mIncomingMessages[kMessageCapacity] = {};
    mutable std::mutex mIncomingMessagesMutex;
    friend class WebSocketHandler;
};

} // namespace viewer
} // namespace filament

#endif // VIEWER_REMOTE_SERVER_H
