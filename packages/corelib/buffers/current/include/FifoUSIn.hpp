 
#ifndef FIFOUSIN_HPP
#define FIFOUSIN_HPP

#include "os/fosi.h"

#include <cstdio>
#include <fcntl.h>

#include "ReadInterface.hpp"

namespace ORO_CoreLib
{

    /**
     * A FifoUSIn is meant for reading from /dev/... fifos from userspace.
     * one can write to a fifo by using FifoUSOut from userspace or 
     * FifoRTOut in kernel space. You have to create the fifo first in
     * kernel space to be able to specify the fifos size.
     */
    class FifoUSIn 
        : public ReadInterface
    {

        public:

            /**
             * Wrap an input fifo around an already created rtosf
             * (associated with fifo-number <fifoNr>).
             * When this object is deleted, it won't destroy the fifo.
             */
            FifoUSIn( unsigned int fNr );

            /**
             * Destroy (wrapper) fifo.
             */
            ~FifoUSIn();

            /**
             * Read blocking from the fifo.
             */
            int read( char* buf, size_t length );

            /**
             * Return the fifo number.
             */
            unsigned int fifoNr() const;

        protected:
            // forbidden
            FifoUSIn();

        private:
        unsigned int okr_len;
        unsigned int fNr;
        FILE* fifo;
    };

} // namespace

#endif 
