// $Id: nAxisGeneratorCartesianPosForce.hpp,v 1.1.1.1 2003/12/02 20:32:06 kgadeyne Exp $
// Copyright (C) 2003 Klaas Gadeyne <klaas.gadeyne@mech.kuleuven.ac.be>
//                    Wim Meeussen  <wim.meeussen@mech.kuleuven.ac.be>
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


#include "kernel_components/nAxesSensorForcesensor.hpp"
#include <assert.h>


namespace ORO_ControlKernel
{

  using namespace ORO_ControlKernel;
  using namespace ORO_DeviceInterface;
  using namespace ORO_Execution;


  nAxesSensorForcesensor::nAxesSensorForcesensor(unsigned int num_axes,
						 std::vector<AxisInterface*> axes,
						 SensorInterface<ORO_Geometry::Wrench>* forcesensor,
						 ORO_KinDyn::KinematicsComponent* kin,
						 const Frame& MP_FS,
						 std::string name)
    : nAxesSensorForcesensor_typedef(name),
      _num_axes(num_axes), 
      _axes(axes),
      _position_sensors(num_axes),
      _forcesensor(forcesensor),
      _kinematics(kin),
      _position_joint(num_axes),
      _MP_FS(MP_FS)
  {
    assert(_axes.size() == num_axes);

    // get position sensors from axes
    for (unsigned int i=0; i<_num_axes; i++){
      _position_sensors[i] = _axes[i]->getSensor("Position");
      assert( _position_sensors[i] != NULL );
    }
  }


  nAxesSensorForcesensor::~nAxesSensorForcesensor(){};
  

  void nAxesSensorForcesensor::pull()
  {
    // copy values from position sensors to local variable
    for (unsigned int i=0; i<_num_axes; i++)
      _position_joint[i] = _position_sensors[i]->readSensor();

    // copy values from force sensor to local variable
    _forcesensor->readSensor(_force);
  }


  void nAxesSensorForcesensor::calculate()
  {
    // forwarard kinematics
    ORO_CoreLib::Double6D temp;
    for (unsigned int i=0; i<_num_axes; i++)
      temp[i] = _position_joint[i];
    _kinematics->positionForward(temp, _world_MP );

    // calculate average measurement if requested
    if (_is_measuring){
      if (_counter_measurements < _num_measurements){
	_average_measurement += ( _force/_num_measurements );
	_counter_measurements++;
      }
      else
	_is_measuring = false;
    }
  }


  
  void nAxesSensorForcesensor::push()      
  {
    _position_out_DOI->Set(_world_MP * _MP_FS);
    _force_out_DOI->Set(_force);
  }



  bool nAxesSensorForcesensor::componentLoaded()
  {
    // get interface to Input data types
    if (!Sensor< Writes<nAxesSensorForcesensorInput_pos_force> >::Input::dObj()->Get("ForcesensorPosition", _position_out_DOI) ||
	!Sensor< Writes<nAxesSensorForcesensorInput_pos_force> >::Input::dObj()->Get("ForcesensorForce", _force_out_DOI)){
      cerr << "nAxesSensorForcesensor::componentLoaded() DataObjectInterface not found" << endl;
      return false;
    }

    // set empty values
    Frame  _temp_frame;
    Wrench _temp_wrench;
    _position_out_DOI->Set(_temp_frame);
    _force_out_DOI->Set(_temp_wrench);

    return true;
  }


  bool nAxesSensorForcesensor::componentStartup()
  {
    _is_measuring = false;
    return true;
  }
  

  CommandFactoryInterface* nAxesSensorForcesensor::createCommandFactory()
  {
    TemplateCommandFactory<nAxesSensorForcesensor>* my_commandFactory = newCommandFactory( this );
    my_commandFactory->add( "averageMeasure", command( &nAxesSensorForcesensor::startMeasuring,
						       &nAxesSensorForcesensor::finishedMeasuring,
						       "calculate the average force during num_meas samples",
						       "num_meas", "Number of measurements to take"));
    return my_commandFactory;
  }


  MethodFactoryInterface* nAxesSensorForcesensor::createMethodFactory()
  {
    TemplateMethodFactory<nAxesSensorForcesensor>* my_methodFactory = newMethodFactory( this );
    my_methodFactory->add( "finishedMeasuring", method( &nAxesSensorForcesensor::finishedMeasuring, "Arrived at new position" ));

    return my_methodFactory;
  }



  bool nAxesSensorForcesensor::startMeasuring(int num_meas)
  {
    if (_is_measuring)
      return false;
    else{
      _num_measurements = num_meas;
      _counter_measurements = 0;
      SetToZero(_average_measurement);
      _is_measuring = true;
      return true;
    }
  }

  
  bool nAxesSensorForcesensor::finishedMeasuring() const
  {
    return !_is_measuring;
  }
  

  Wrench nAxesSensorForcesensor::getMeasurement()
  {
    return _average_measurement;
  }
  


} // end namespace ORO_nAxesControlKernel
