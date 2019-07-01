#include "active_arc_state.hpp"
/*
  Operation
 */

//
void state_active_arc_class::cg_relaxation(
					   model_parameters_active_arc_class & model,
					   const int & sweep_step
					   )
{
  // initializationo for displacements
  std::string message;
  local_geometry();
  calculate_displacement("debug");
  //
  std::string initializer_real_1d_array;
  generate_initializer(initializer_real_1d_array, "solution");
  alglib::real_1d_array solution = initializer_real_1d_array.c_str();
  generate_initializer(initializer_real_1d_array, "gradient");
  alglib::real_1d_array function_gradient = initializer_real_1d_array.c_str();
  double termination_point_sg = 0.0;
  double termination_point_sf = 0.0;
  double termination_point_sx = 0.0;
  //
  alglib::ae_int_t maximum_number_of_iteration = 2000;
  {
    alglib::mincgstate cg_state;
    alglib::mincgreport cg_report;
    alglib::mincgcreate(
			solution, 
			cg_state
			);
    alglib::mincgsetcond(
			 cg_state,
			 termination_point_sg,
			 termination_point_sf,
			 termination_point_sx,
			 maximum_number_of_iteration
			 );
    {
      //***********************************
      //optimization_loop (mincgoptimize)
      //***********************************
      time_index=0;
      alglib_impl::ae_state _alglib_env_state;;
      alglib_impl::ae_state_init(&_alglib_env_state);
      try
	{
	  while( alglib_impl::mincgiteration(cg_state.c_ptr(), &_alglib_env_state) )
        {
            if( cg_state.needfg )
            {
                driver_cg_alglib(
				 model,
				 cg_state.x, 
				 cg_state.f, 
				 cg_state.g,
				 NULL,
				 time_index,
				 sweep_step
				 );
                continue;
            }
            if( cg_state.xupdated )
	      {
                continue;
            }
            throw alglib::ap_error("ALGLIB: error in 'mincgoptimize' (some derivatives were not provided?)");
        }
	  alglib_impl::ae_state_clear(&_alglib_env_state);
	}
      catch(alglib_impl::ae_error_type)
	{
	  throw alglib::ap_error(_alglib_env_state.error_msg);
	};
    };
    //*************************************
    //  Loop out
    //*************************************
    alglib::mincgresults(
			 cg_state,
			 solution,
			 cg_report
			 );
    message = "///// Termination Report of alglib is " + io_method.int_to_string(cg_report.terminationtype);
    io_method.standard_output(message);
    message = "///// Iteration Report of alglib is " + io_method.int_to_string(cg_report.iterationscount);
    io_method.standard_output(message);
    pull_solution(solution);
    finalization(sweep_step,"cg",model);
  };
};
//
void state_active_arc_class::generate_initializer(
						  std::string & initializer_real_1d_array,
						  const std::string & array_type
						  )
{
  if(array_type=="solution")
    {
      std::vector<double>::iterator component_index = coordinates.begin();
      std::vector<double>::iterator component2_index = angles.begin();
      initializer_real_1d_array 
	= '['
	+ io_method.double_to_string(*component_index);
      component_index++;
      initializer_real_1d_array 
	+= ','
	+ io_method.double_to_string(*component_index);
      component_index++;
      initializer_real_1d_array 
	+= ','
	+ io_method.double_to_string(*component2_index);
      component2_index++;
      while(component2_index != angles.end())
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(*component_index);
	  component_index++;
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(*component_index);
	  component_index++;
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(*component2_index);
	  component2_index++;
	}
      component2_index = normal_modulation.begin();
      for(
	  component_index = ripening.begin();
	  component_index != ripening.end();
	  component_index++
	  )
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(*component_index)
	    + ','
	    + io_method.double_to_string(*component2_index);
	  component2_index++;
	};
      // for elastic constraint;
      if(inner_type=="volume"||inner_type=="volume_radius")
	{
	}
      else if
	(
	 inner_type=="elastic"
	 )
	{
	  component2_index = center_of_coordinates.begin();
	  while(
		component2_index
		!=
		center_of_coordinates.end()
		)
	    {
	      initializer_real_1d_array
		+="," + io_method.double_to_string(*component2_index);
	      component2_index++;
	    };
	}
      else
	{
	  io_method.error_output(
				 "active_arc_cg",
				 "generate_initializer",
				 "irregal inner type at solution initializer!"
				 );
	};
      //
      initializer_real_1d_array 
	+= ']';
    }
  else if(array_type=="gradient")
    {
      std::vector<double>::iterator component_index = gradient.begin();
      initializer_real_1d_array 
	= '['
	+ io_method.double_to_string(*component_index);
      component_index++;
      while(component_index != gradient.end())
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(*component_index);
	  component_index++;
	}
      initializer_real_1d_array 
	+= ']';
    }
  else if(array_type=="upper bound")
    {
      std::vector<double>::iterator component_index = coordinates.begin();
      initializer_real_1d_array 
	= '['
	+ io_method.double_to_string(*component_index);
      component_index++;
      while(component_index != coordinates.end())
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(reference_box_max*cg_upper_margin);
	  component_index++;
	}
      for(component_index = angles.begin();component_index != angles.end();component_index++)
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(3.141592653589793);
	};
      for(component_index = arc_width_reference.begin();component_index != arc_width_reference.end();component_index++)
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string((*component_index)*cg_upper_margin)
	    + ','
	    + "+inf";
	};
      // for elastic constraint; 
      if(inner_type=="volume"||inner_type=="volume_radius")
	{
	}
      else if(
	 inner_type=="elastic"
	 )
	{
	  component_index = center_of_coordinates.begin();
	  while(
		component_index
		!= 
		center_of_coordinates.end()
		)
	    {
	      initializer_real_1d_array
		+=", +inf";
	      component_index++;
	    };
	}
      else
	{
	  io_method.error_output(
				 "active_arc_cg",
				 "generate_initializer",
				 "irregal inner type at solution initializer!"
				 );
	};
      //
      initializer_real_1d_array 
	+= ']';
    }
  else if(array_type=="lower bound")
    {
      std::vector<double>::iterator component_index = coordinates.begin();
      initializer_real_1d_array 
	= '['
	+ io_method.double_to_string(*component_index);
      component_index++;
      while( component_index != coordinates.end())
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(reference_box_min*cg_lower_margin);
	  component_index++;
	}
      for(component_index = angles.begin();component_index != angles.end();component_index++)
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string(-3.141592653589793);
	};
      for(component_index = arc_width_reference.begin();component_index != arc_width_reference.end();component_index++)
	{
	  initializer_real_1d_array 
	    += ','
	    + io_method.double_to_string((*component_index)*(-1.0))
	    + ','
	    + "-inf";
	};
      // for elastic constraint; 
      if(inner_type=="volume"||inner_type=="volume_radius")
	{
	}
      else if(
	      inner_type=="elastic"
	      )
	{
	  component_index = center_of_coordinates.begin();
	  while(component_index!=center_of_coordinates.end())
	    {
	      initializer_real_1d_array
		+=", -inf";
	      component_index++;
	    };
	}
      else
	{
	  io_method.error_output(
				 "active_arc_cg",
				 "generate_initializer",
				 "irregal inner type at solution initializer!"
				 );
	};
      //
      initializer_real_1d_array 
	+= ']';
    };
  //io_method.standard_output("initializer: "+ initializer_real_1d_array);
};
//
void state_active_arc_class::driver_cg_alglib(
					      model_parameters_active_arc_class & model,
					      const alglib::real_1d_array & solution,
					      double & optimization_function,
					      alglib::real_1d_array & function_gradient,
					      void *tmporary_ptr,
					      long int & time_index,
					      const int & sweep_step
					      )
{
  time_index++;
  io_method.standard_output(
			   "///////////Iter =" 
			   + io_method.longint_to_string(time_index)
			   + " ///////////");
  pull_solution(solution);
  //
  local_geometry();
  calculate_displacement("");
  calculate_strain();
  calculate_constraint();
  calculate_stress();
  calculate_elastic_energy();
  calculate_elastic_gradient();
  calculate_inner();
  //
  summarize_energy_gradient(
			    optimization_function,
			    function_gradient
			    );
  //
  io_method.standard_output("energy = " + io_method.double_to_string(optimization_function));
};
//
void state_active_arc_class::pull_solution(
					   const alglib::real_1d_array & solution
					   )
{
  for(int direction_index = 0; direction_index < space_dimension; direction_index++ )
    {
      for(long int joint_index = 0; joint_index < number_of_joints; joint_index++)
	{
	coordinates[joint_index*space_dimension+direction_index]
	  = solution[joint_index*number_of_joint_components+direction_index];
	};
    };
  for(long int joint_index = 0; joint_index < number_of_joints; joint_index++)
    {
      angles[joint_index]
	= solution[joint_index*number_of_joint_components+space_dimension];
    };
  for(long int arc_index = 0; arc_index < number_of_joints; arc_index++)
    {
      ripening[arc_index] 
	=solution[internal_head_pointer+arc_index*number_of_internal_components];
      normal_modulation[arc_index]
	=solution[internal_head_pointer+arc_index*number_of_internal_components+1];
    };
  if(inner_type=="volume"||inner_type=="volume_radius")
    {
    }
  else if(inner_type=="elastic")
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  center_of_coordinates[direction_index]
	    =solution[constraint_head_pointer+direction_index];
	}
    }
  else
    {
      io_method.error_output(
			     "active_arc_cg",
			     "pull_solution",
			     "irregal inner type!"
			     );
    };
}
