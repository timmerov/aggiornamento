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
        bool isDone() throw();
        void setDone() throw();
        void waitDone() throw();
    }
}
