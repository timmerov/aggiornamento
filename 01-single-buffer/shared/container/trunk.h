/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.
trunk interface.

anyone can put a string into or copy a string from the trunk.
*/

#pragma once

namespace agm {
    namespace single_buffer {
        const int kMaxSize = 100;
    }

    namespace master {
        void init() throw();
        void exit() throw();
    }

    namespace trunk {
        // overwrites whatever was in the trunk.
        void putString(const char *buffer) throw();

        // copies the string from the trunk.
        // the string in the trunk is preserved.
        void getString(char *buffer, int size) throw();

        // template divines size of getString buffer
        template <int size>
        void getString(
           char (&buffer)[size]
        ) throw() {
            getString(buffer, size);
        }
    }
}
