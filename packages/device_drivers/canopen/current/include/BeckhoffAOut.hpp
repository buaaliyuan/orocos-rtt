#ifndef BECKHOFFAOUT_HPP
#define BECKHOFFAOUT_HPP

#include <device_interface/AnalogOutInterface.hpp>

namespace Beckhoff
{
    using ORO_DeviceInterface::AnalogOutInterface;
    /**
     * A BeckHoff terminal for 4 analog outputs.
     */
    class BeckhoffAOut
        : AnalogOutInterface<unsigned int>
    {
    public:
        BeckhoffAOut()
        {
            channels[0] = 0;
            channels[1] = 0;
            channels[2] = 0;
            channels[3] = 0;
        }

        virtual void rangeSet( unsigned int chan, unsigned int range)
        {}

	    virtual void arefSet(unsigned int chan,  unsigned int aref)
        {}

        virtual void write( unsigned int chan, unsigned int value )
        {
            if (chan <0 || chan >3 ) return;
            channels[chan] = value;
        }    

        virtual unsigned int binaryRange() const { return 0xFFFF; }

        virtual unsigned int binaryZero()  const { return 0x0; }

        virtual unsigned int binaryLowest() const  { return 0x8001;}

        virtual unsigned int binaryHighest() const { return 0x7FFF; }

        virtual double lowest(unsigned int chan) const { return -10.0; }

        virtual double highest(unsigned int chan) const { return 10.0; }

        // in bits per MU
        virtual double resolution(unsigned int chan) const 
        { return binaryRange() / (lowest(chan ) + highest(chan)); }
             
        /**
         * Returns the value of a channel <chan>.
         */
        unsigned int value(unsigned int chan)
        {
            if (chan <0 || chan >3 ) return ~0;
            return channels[chan];
        }

        virtual unsigned int nbOfChannels() const { return 4; }
                    
        void addTerminal(unsigned int nr, unsigned int channels) 
        {}
    protected:
        unsigned int channels[4];
    };






}


#endif
