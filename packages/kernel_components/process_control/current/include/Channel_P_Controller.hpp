// Copyright (C) 2003 Klaas Gadeyne <klaas.gadeyne@mech.kuleuven.ac.be>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  

#ifndef __CHANNEL_P_CONTROLLER_H__
#define __CHANNEL_P_CONTROLLER_H__

#include <control_kernel/DataServer.hpp>
#include <control_kernel/KernelInterfaces.hpp>
#include <control_kernel/PropertyExtension.hpp>
#include <control_kernel/BaseComponents.hpp>
#include <control_kernel/ExtensionComposition.hpp>
#include <corelib/PropertyComposition.hpp>

#pragma interface

namespace ORO_ControlKernel
{

    /**
     * The output of the Channel_P_Controller.
     */
    struct Channel_P_Controller_Output
        : ServedTypes< std::vector<double> > 
    {
        Channel_P_Controller_Output() {
            this->insert(std::make_pair(0, "ChannelValues"));
        }
    };

    struct CPC_Input
        : ServedTypes< std::vector<double> > 
    {
    };

    struct CPC_SetPoint
        : ServedTypes< std::vector<double> > 
    {
    };

  /**
   *  @brief A simple P controller which operates on signals.
   *  - In: Position Setpoints and Model
   *  - Out: Velocity sendpoints to send to the effector
   *  @ingroup kcomps kcomp_controller 
   */
  template <class Base = Controller<Expects<CPC_Input>, Expects<NoModel>, Expects<CPC_SetPoint>, Writes<Channel_P_Controller_Output>, MakeExtension<PropertyExtension, KernelBaseFunction>::CommonBase > >
  class Channel_P_Controller
    : public Base
  {
  protected:
      typedef std::vector<double> ChannelType;
      DataObjectInterface<ChannelType>* inp_dObj;
      DataObjectInterface<ChannelType>* outp_dObj;
      DataObjectInterface<ChannelType>* setp_dObj;
      ChannelType   _result;
      ChannelType   _refPos;
      ChannelType   _measPos;

      double _K;
      unsigned int _num_axes;

      ChannelType _xyerr;

      Property<double> _controller_gain;

  public:
      Channel_P_Controller(unsigned int _num_axes);
      virtual ~Channel_P_Controller();

      // Redefining virtual members
      virtual bool updateProperties(const PropertyBag& bag);
      virtual bool componentLoaded();
      virtual bool componentStartup();

      virtual void pull();
      virtual void calculate();
      virtual void push();

  };


#include "Channel_P_Controller.inc"

    extern template class Channel_P_Controller<>;

}

#endif // __N_AXES_P_CONTROLLER_H__
