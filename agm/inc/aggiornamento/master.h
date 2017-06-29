/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
interface to the master thread.
tell the master thread to stop all threads.
**/

#pragma once

namespace agm {
    namespace master {
        bool isDone() noexcept;
        void setDone() noexcept;
        void waitDone() noexcept;
    }
}
