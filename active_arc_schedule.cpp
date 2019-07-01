#include "active_arc_schedule.hpp"
void schedule_active_arc::input_schedule(
					 std::unique_ptr<state_active_arc_class> & state
					 )
{
  schedule work_schedule;
  std::string structure_item;
  number_of_schedule_termination=io_method.get_input_longint(
							     "model_input",
							     "schedule.number_of_schedule_termination"
							     );
  number_of_scheduled_parameters=io_method.get_input_longint(
							     "model_input",
							     "schedule.number_of_parameters"
							     );
  schedules.clear();
  //
  for(
      int parameter_index=0;
      parameter_index<number_of_scheduled_parameters;
      parameter_index++
      )
    {
      structure_item=set_structure_type_item(
					     parameter_index,
					     "categoly"
					     );
      work_schedule.categoly=io_method.get_input_string(
							"model_input",
							structure_item
							);
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "parameter"
					     );
      work_schedule.parameter=io_method.get_input_string(
							 "model_input",
							 structure_item
							 );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "control_type"
					     );
      work_schedule.control_type=io_method.get_input_string(
							    "model_input",
							    structure_item
							    );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "growth_type"
					     );
      work_schedule.growth_type=io_method.get_input_string(
							    "model_input",
							    structure_item
							    );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "start_value"
					     );
      work_schedule.start_value=io_method.get_input_double(
							   "model_input",
							   structure_item
							   );
      //
      work_schedule.value=work_schedule.start_value;
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "incriment"
					     );
      work_schedule.incriment=io_method.get_input_double(
							 "model_input",
							 structure_item
							 );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "wave_number"
					     );
      work_schedule.wave_number=io_method.get_input_double(
							   "model_input",
							   structure_item
							   );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "amplitude"
					     );
      work_schedule.amplitude=io_method.get_input_double(
							 "model_input",
							 structure_item
							 );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "adaptation.switch"
					     );
      if(
	 io_method.get_input_string(
				    "model_input",
				    structure_item
				    )=="off"
	 )
	{
	  work_schedule.adaptation_on=false;
	}
      else
	{
	  work_schedule.adaptation_on=true;
	};
    //
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "adaptation.amplitude"
					     );
      work_schedule.adaptation_amplitude=io_method.get_input_double(
								    "model_input",
								    structure_item
								    );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "adaptation.threshold"
					     );
      work_schedule.adaptation_threshold=io_method.get_input_double(
								    "model_input",
								    structure_item
								    );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "adaptation.width"
					     );
      work_schedule.adaptation_width=io_method.get_input_double(
								"model_input",
								structure_item
								);
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "adaptation.threshold"
					     );
      work_schedule.adaptation_threshold=io_method.get_input_double(
								    "model_input",
								    structure_item
								    );
      //
      structure_item=set_structure_type_item(
					     parameter_index,
					     "adaptation.source"
					     );
      work_schedule.adaptation_source=io_method.get_input_string(
								 "model_input",
								 structure_item
								 );
      //
      work_schedule.initialize_values(state);
      schedules.push_back(work_schedule);
    };
  output_schedule();
};
void schedule_active_arc::output_schedule()
{
  std::string output_text;
  io_method.standard_output("=== Schedule parameters started  ===");
  for(
      int parameter_index=0;
      parameter_index<number_of_scheduled_parameters;
      parameter_index++
      )
    {
      output_text 
	= "*Parameter[" + io_method.int_to_string(parameter_index)+"]: "
	+ "Categoly = " + schedules[parameter_index].categoly
	+ "/Parameter = "  + schedules[parameter_index].parameter
	+ "/Control_type = "  + schedules[parameter_index].control_type;
      io_method.standard_output(output_text);
      output_text 
	= "Start =" + io_method.double_to_string(schedules[parameter_index].start_value)+": "
	+ "Incriment = " + io_method.double_to_string(schedules[parameter_index].incriment) + ":"
	+ "Wave number = "  + io_method.double_to_string(schedules[parameter_index].wave_number) + ":"
	+ "Amplitude = "  + io_method.double_to_string(schedules[parameter_index].amplitude)  + ":";
	if(schedules[parameter_index].adaptation_on)
	  {
	    output_text+= "Adaptation amplitude = on" ":";
	  }
	else
	  {
	    output_text+= "Adaptation amplitude = off" ":";
	  }
	output_text 
	+= "Adaptation amplitude ="  + io_method.double_to_string(schedules[parameter_index].adaptation_amplitude) + ":"
	+ "Adaptation threshold ="  + io_method.double_to_string(schedules[parameter_index].adaptation_threshold)+ ":";
	+ "Adaptation width ="  + io_method.double_to_string(schedules[parameter_index].adaptation_width);
      io_method.standard_output(output_text);
    };
  io_method.standard_output("=== Schedule parameters finished ===");
};
//
void schedule_active_arc::update(
				 std::unique_ptr<state_active_arc_class> & state
				 )
{
  stress_adaptation(state);
  for(
      int parameter_index=0; 
      parameter_index<number_of_scheduled_parameters;
      parameter_index++
      )
    {
      if(schedules[parameter_index].control_type=="incriment")
	{
	  growth.incriment_growth(schedules[parameter_index]);
	}
      else if(schedules[parameter_index].control_type=="inverse")
	{
	  growth.inverse_growth(schedules[parameter_index]);
	}
      else
	{
	  io_method.error_output(
				 "schedule_active_arc",
				 "update",
				 "irregal parameter indication!"
				 );
	}
    };
};
//
void schedule_active_arc::stress_adaptation(
					    std::unique_ptr<state_active_arc_class> & state
					    )
{
  state->stress_get();
  state->inner_stress_get();
  state->reference_get();
  volume_calculation(
		     state->reference.length,
		     state->reference.width
		     );
  inner_volume_calculation(
			   state->reference.inner_height,
			   state->reference.inner_width
			   );
  for(
      int parameter_index=0; 
      parameter_index<number_of_scheduled_parameters;
      parameter_index++
      )
    {
      if(
	 schedules[parameter_index].adaptation_source=="arc.length"
	 //	 (schedules[parameter_index].categoly=="arc")
	 //	 &&
	 //	 (schedules[parameter_index].parameter=="length")
	 )
	{
	  growth.calculate_adaptation(
				      schedules[parameter_index],
				      state->stress.tangent_stress,
				      volumes
				      );
	}
      else if
	(
	 schedules[parameter_index].adaptation_source=="arc.curvature"
	 //	 (schedules[parameter_index].categoly=="arc")
	 //	 &&
	 //	 (schedules[parameter_index].parameter=="curvature")
	 )
	{
	  growth.calculate_adaptation(
				      schedules[parameter_index],
				      state->stress.bending_stress,
				      volumes
				      );
	}
      else if
	(
	 schedules[parameter_index].adaptation_source=="arc.width"
	 //	 (schedules[parameter_index].categoly=="arc")
	 //	 &&
	 //	 (schedules[parameter_index].parameter=="width")
	 )
	{
	  growth.calculate_adaptation(
				      schedules[parameter_index],
				      state->stress.normal_stress,
				      volumes
				      );
	}
      else if
	(
	 schedules[parameter_index].adaptation_source=="inner.volume"
	 //	 (schedules[parameter_index].categoly=="inner")
	 //	 &&
	 //	 (schedules[parameter_index].parameter=="volume")
	 )
	{
	  growth.calculate_adaptation(
				      schedules[parameter_index],
				      state->inner_stress.dummy,
				      inner_volumes
				      );
	}
      else if
	(
	 schedules[parameter_index].adaptation_source=="inner.width"
	 //	 (schedules[parameter_index].categoly=="inner")
	 //	 &&
	 //	 (schedules[parameter_index].parameter=="width")
	 )
	{
	  growth.calculate_adaptation(
				      schedules[parameter_index],
				      state->inner_stress.inner_elongation_stress,
				      inner_volumes
				      );
	}
      else if
	(
	 schedules[parameter_index].adaptation_source=="inner.height"
	 //	 (schedules[parameter_index].categoly=="inner")
	 //	 &&
	 //	 (schedules[parameter_index].parameter=="height")
	 )
	{
	  growth.calculate_adaptation(
				      schedules[parameter_index],
				      state->inner_stress.inner_ripening_stress,
				      inner_volumes
				      );
	}
      else if
	(
	 schedules[parameter_index].adaptation_source=="inner.shear"
	 //	 (schedules[parameter_index].categoly=="inner")
	 //	 &&
	 //	 (schedules[parameter_index].parameter=="shear")
	 )
	{
	  growth.calculate_adaptation(
				      schedules[parameter_index],
				      state->inner_stress.inner_shear_stress,
				      inner_volumes
				      );
	}
      else
	{
	  io_method.error_output(
				 "schedule_active_arc",
				 "stress_adaptation",
				 "irregal parameter indication!"
				 );
	};
    };
};
//
void schedule_active_arc::replace(
				  schedule & input_schedule,
				  std::vector<double> & output_values
				  )
{
  long int counter = 0;
  if(input_schedule.values.size()==output_values.size())
    {
      for(
	  auto index=output_values.begin();
	  index != output_values.end();
	  index ++
	  )
	{
	  (*index) = input_schedule.values[counter];
	  counter++;
	};
      output_parameter_change(
			      true,
			      output_values,
			      input_schedule
			      );
    }
  else
    {
      io_method.error_output(
			     "schedule_active_arc",
			     "replace",
			     "array size inconsistency for"
			     + input_schedule.categoly
			     + " : "
			     + input_schedule.parameter
			     + " ! "
			     );
    }
};
//
void schedule_active_arc::volume_calculation(
					      std::vector<double> & length,
					      std::vector<double> & width
					      )
{
  long int number_of_arcs = length.size();
  if((long int)volumes.size()!=number_of_arcs)
    {
      volumes.resize(number_of_arcs);
    };
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      volumes[arc_index]=length[arc_index]*width[arc_index];
    };
};									 
//
void schedule_active_arc::inner_volume_calculation(
						    std::vector<double> & height,
						    std::vector<double> & width
						    )
{
  long int number_of_arcs = height.size();
  if((long int)inner_volumes.size()!=number_of_arcs)
    {
      inner_volumes.resize(number_of_arcs);
    };
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      inner_volumes[arc_index]=height[arc_index]*width[arc_index];
    };
};		
//
void schedule_active_arc::reference_replace(
					    std::unique_ptr<state_active_arc_class> & state
					    )
{
  state->reference_get();
  for(
      int parameter_index=0; 
      parameter_index<number_of_scheduled_parameters;
      parameter_index++
      )
    {
      if(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="length")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.length
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="curvature")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.curvature
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="width")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.width
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="young_modulus")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.young_modulus
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="poisson_ratio")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.poisson_ratio
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="volume")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.inner_control_volume
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="width")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.inner_width
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="height")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.inner_height
		  );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="shear")
	 )
	{
	  replace(
		  schedules[parameter_index],
		  state->reference.inner_shear
		  );
	}
      else
	{
	  io_method.error_output(
				 "schedule_active_arc",
				 "set_schedule",
				 "irregal parameter indication!"
				 );
	};
    };
  state->reference_set();
};
//
void schedule_active_arc::set_schedule(
				       std::unique_ptr<state_active_arc_class> & state,
				       const long int & iteration_index
				       )
{
  state->reference_get();
  state->stress_get();
  state->inner_stress_get();
  for(
      int parameter_index=0; 
      parameter_index<number_of_scheduled_parameters;
      parameter_index++
      )
    {
      if(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="length")
	 )
	{
	  set_values(
		     state->reference.length,
		     state,
		     state->stress.tangent_stress,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="curvature")
	 )
	{
	  set_values(
		     state->reference.curvature,
		     state,
		     state->stress.bending_stress,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="width")
	 )
	{
	  set_values(
		     state->reference.width,
		     state,
		     state->stress.normal_stress,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="young_modulus")
	 )
	{
	  set_values(
		     state->reference.young_modulus,
		     state,
		     state->stress.dummy,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="poisson_ratio")
	 )
	{
	  set_values(
		     state->reference.young_modulus,
		     state,
		     state->stress.dummy,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="arc")
	 &&
	 (schedules[parameter_index].parameter=="young_modulus")
	 )
	{
	  set_values(
		     state->reference.young_modulus,
		     state,
		     state->stress.dummy,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="volume")
	 )
	{
	  set_values(
		     state->reference.inner_control_volume,
		     state,
		     state->inner_stress.dummy,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="width")
	 )
	{
	  set_values(
		     state->reference.inner_width,
		     state,
		     state->inner_stress.inner_elongation_stress,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="height")
	 )
	{
	  set_values(
		     state->reference.inner_height,
		     state,
		     state->inner_stress.inner_ripening_stress,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else if
	(
	 (schedules[parameter_index].categoly=="inner")
	 &&
	 (schedules[parameter_index].parameter=="shear")
	 )
	{
	  set_values(
		     state->reference.inner_shear,
		     state,
		     state->inner_stress.inner_shear_stress,
		     schedules[parameter_index],
		     iteration_index
		     );
	}
      else
	{
	  io_method.error_output(
				 "schedule_active_arc",
				 "set_schedule",
				 "irregal parameter indication!"
				 );
	};
    };
  state->reference_set();
};
//
double schedule_active_arc::calculate_base(
					   const schedule & input_schedule,
					   const long int & iteration_index
					   )
//
{
  return ((double)iteration_index)*input_schedule.incriment+input_schedule.start_value;
};
//
std::string schedule_active_arc::set_structure_type_item(
							 const int & parameter_index,
							 const std::string & child
							 )
{
  return io_method.generate_structure(
				      "schedule.parameter",
				      parameter_index,
				      child
				      );
};
//
void schedule_active_arc::set_values(
				     std:: vector<double> & vector_values,
				     std::unique_ptr<state_active_arc_class> & state,
				     const std::vector<double> & adaptation,
				     schedule & input_schedule,
				     const long int & iteration_index
				     )
{
  output_parameter_change(
			  true,
			  vector_values,
			  input_schedule
			  );
  calculate_adaptation_ratios(
			      adaptation,
			      input_schedule
			      );
  set_growth_ratios(
		    vector_values,
		    state,
		    input_schedule,
		    iteration_index
		    );
  if(input_schedule.growth_type=="constant")
    {
      constant_growth(
		      vector_values,
		      state,
		      input_schedule,
		      iteration_index
		      );
    }
  else if(input_schedule.growth_type=="dynamic")
    {
      dynamic_growth(
		     vector_values,
		     state,
		     input_schedule,
		     iteration_index
		     );
    }
  else
    {
      io_method.error_output(
			     "active_arc_schedule",
			     "set_values",
			     "undefined operation"
			     );
    };
  output_parameter_change(
			  false,
			  vector_values,
			  input_schedule
			  );
};
//
void schedule_active_arc::constant_growth(
					  std::vector<double> & vector_values,
					  std::unique_ptr<state_active_arc_class> & state,
					  schedule & input_schedule,
					  const long int & iteration_index
					  )
{
  std::vector<double>::iterator index=vector_values.begin();
  long int counter=0;
  double position=0.0;
  if(input_schedule.control_type=="incriment")
    {
      while(index!=vector_values.end())
	{
	  position=((double)counter)/((double)vector_values.size());
	  *index=
	    input_schedule.start_value
	    *(1.0+growth_ratios[counter]*((double)iteration_index));
	  input_schedule.value=*index;
	  index++;
	  counter++;
	};
    }
  else if(
	  input_schedule.control_type=="inverse"
	  &&
	  input_schedule.parameter=="curvature"
	  )
    {
      while(index!=vector_values.end())
	{
	  position=((double)counter)/((double)vector_values.size());
	  *index=
	    input_schedule.start_value
	    /(1.0+growth_ratios[counter]*((double)iteration_index));
	  input_schedule.value=*index;
	  index++;
	  counter++;
	};
    }
  else if(
	  input_schedule.control_type=="interior_constriction"
	  &&
	  input_schedule.parameter=="curvature"
	  )
    {
      while(index!=vector_values.end())
	{
	  *index=
	    2.0/state->reference.width[counter]
	    *(
	      1.0-strain_ratio(
			       -0.5*state->reference.width[counter]
			       *input_schedule.start_value,
			       growth_ratios[counter]
			       *((double)iteration_index)
			       )
	      );
	  input_schedule.value=*index;
	  index++;
	  counter++;
	};
    }
  else if(
	  input_schedule.control_type=="consistent_length"
	  &&
	  input_schedule.parameter=="curvature"
	  )
    {
      while(index!=vector_values.end())
	{
	  position=((double)counter)/((double)vector_values.size());
	  *index=
	    input_schedule.start_value
	    /(1.0+growth_ratios[counter]*((double)iteration_index));
	  input_schedule.value=*index;
	  index++;
	  counter++;
	};
    }
  else
    {
      io_method.error_output(
			     "active_arc_schedule",
			     "constant_growth",
			     "undefined operation"
			     );
    };
};
//
void schedule_active_arc::dynamic_growth(
					 std::vector<double> & vector_values,
					 std::unique_ptr<state_active_arc_class> & state,
					 schedule & input_schedule,
					 const long int & iteration_index
					 )
{
  std::vector<double>::iterator index=vector_values.begin();
  long int counter=0;
  double position=0.0;
  if(input_schedule.control_type=="incriment")
    {
      while(index!=vector_values.end())
	{
	  position=((double)counter)/((double)vector_values.size());
	  input_schedule.value
	    =input_schedule.value
	    +growth_ratios[counter]*adaptation_ratios[counter];
	  *index=input_schedule.value;
	  index++;
	  counter++;
	};
    }
  else if(
	  input_schedule.control_type=="consistent_length"
	  &&
	  input_schedule.parameter=="curvature"
	  )
    {
      while(index!=vector_values.end())
	{
	  *index=
	    input_schedule.start_value
	    /(1.0+growth_ratios[counter])+adaptation_ratios[counter];
	  index++;
	  counter++;
	};
    }
  else
    {
      io_method.error_output(
			     "active_arc_schedule",
			     "time_depending_growth",
			     "undefined operation"
			     );
    };
};
//
void schedule_active_arc::output_parameter_change(
						  const bool & previous_on,
						  const std:: vector<double> & vector_values,
						  const schedule & input_schedule
						  )
{
  if(previous_on)
    {
      io_method.standard_output("::: Change in"+ input_schedule.parameter + " :::");
      for(
	  int position_index=0;
	  position_index<std::min((int)vector_values.size(),3);
	  position_index++
	  )
	{
	  io_method.standard_output("::: Previous value ["+ io_method.longint_to_string(position_index)+ " ] = " + io_method.double_to_string(vector_values[position_index]) );
	};
    }
  else
    {
      for(
	  int position_index=0;
	  position_index<std::min((int)vector_values.size(),3);
	  position_index++
	  )
	{
	  io_method.standard_output("::: Changed value ["+ io_method.longint_to_string(position_index)+ " ] = " + io_method.double_to_string(vector_values[position_index]) );
	};
    };
};
//
inline double schedule_active_arc::strain_ratio(
						const double & denominator,
						const double & diviser
						)
{
  return (1.0+denominator)/(1.0+diviser);
};
//
bool schedule_active_arc::check_termination(
					    const int & iteration_index
					    )
{
  io_method.standard_output("**** Check next parameter ***");
  if(iteration_index<number_of_schedule_termination)
    {
      io_method.standard_output(
				"::: Starting of Process Iter = " 
				+ io_method.longint_to_string(iteration_index)
				+ " :::"
				);
    }
  else
    {
      io_method.standard_output(
				"::: Iteration terminated at Iter = " 
				+ io_method.longint_to_string(iteration_index-1)
				+ " :::"
				);
    }
  return iteration_index<number_of_schedule_termination;
};
//
void schedule_active_arc::set_value(
				    double & value,
				    const schedule & input_schedule,
				    const long int & iteration_index
				    )
{
  value=
    calculate_base(input_schedule,iteration_index);
};
//
void schedule_active_arc::set_growth_ratios(
					    std:: vector<double> & vector_values,
					    std::unique_ptr<state_active_arc_class> & state,
					    const schedule & input_schedule,
					    const long int & iteration_index
					    )
{
  if(input_schedule.control_type=="consistent_length")
    {
       consistent_growth_ratios(
				vector_values,
				state,
				input_schedule,
				iteration_index
				);
    }
  else
    {
      calculate_growth_ratios(
			      vector_values,
			      input_schedule,
			      iteration_index
			      );
    }
};
//
void schedule_active_arc::calculate_growth_ratios(
						  const std::vector<double> & vector_values,
						  const schedule & input_schedule,
						  const long int & iteration_index
						  )
{
  double position;
  if(growth_ratios.size()!=vector_values.size())
    {
      growth_ratios.clear();
      for(
	  long int position_index=0;
	  position_index<(long int)vector_values.size();
	  position_index++
	  )
	{
	  position=((double)position_index)/((double)vector_values.size());
	  growth_ratios.push_back(
				  input_schedule.incriment
				  *(1.0
				    +input_schedule.amplitude
				    *cos(input_schedule.wave_number*2.0*PI*position)
				    )
				  );
	};
    }
  else
    {
      for(
	  long int position_index=0;
	  position_index<(long int)vector_values.size();
	  position_index++
	  )
	{
	  position=((double)position_index)/((double)vector_values.size());
	  growth_ratios[position_index]
	    =input_schedule.incriment
	    *(1.0
	      +input_schedule.amplitude
	      *cos(input_schedule.wave_number*2.0*PI*position)
	      );
	};
    };
};
//
void schedule_active_arc::calculate_adaptation_ratios(
						      const std::vector<double> & adaptation,
						      const schedule & input_schedule
						      )
{
  //
  if(adaptation_ratios.size()!=adaptation.size())
    {
      adaptation_ratios.clear();
      if(input_schedule.adaptation_on)
	{
	  if(input_schedule.parameter=="curvature")
	    {
	      for(
		  long int arc_index=0;
		  arc_index<(long int)adaptation.size();
		  arc_index++
		  )
		{
		  adaptation_ratios.push_back(
					      tanh(
						   adaptation[arc_index]
						   /
						   input_schedule.adaptation_threshold
						   )
					      *input_schedule.adaptation_amplitude
					      );
		};
	    }
	  else
	    {
	      for(
		  long int arc_index=0;
		  arc_index<(long int)adaptation.size();
		  arc_index++
		  )
		{
		  adaptation_ratios.push_back(
					      (1.0
					       +tanh(
						     adaptation[arc_index]
						     /
						     input_schedule.adaptation_threshold
						     )
					       )*0.5
					      *input_schedule.adaptation_amplitude
					      );
		};
	    };
	}
      else
	{
	  adaptation_ratios.push_back(1.0);
	};
    }
  else
    {     
      if(input_schedule.adaptation_on)
	{ 
	  if(input_schedule.parameter=="curvature")
	    {
	      for(
		  long int arc_index=0;
		  arc_index<(long int)adaptation.size();
		  arc_index++
		  )
		{
		  adaptation_ratios[arc_index]
		    =tanh(
			  adaptation[arc_index]
			  /
			  input_schedule.adaptation_threshold
			  )
		    *input_schedule.adaptation_amplitude;
		};
	    }
	  else
	    {
	      for(
		  long int arc_index=0;
		  arc_index<(long int)adaptation.size();
		  arc_index++
		  )
		{
		  adaptation_ratios[arc_index]
		    =(1.0
		      +tanh(
			    adaptation[arc_index]
			    /
			    input_schedule.adaptation_threshold
			    )
		      )*0.5
		    *input_schedule.adaptation_amplitude;
		};
	    };
	}
      else
	{
	  for(
	      long int arc_index=0;
	      arc_index<(long int)adaptation.size();
	      arc_index++
	      )
	    {
	      adaptation_ratios[arc_index]=1.0;
	    };
	};
    };
};
//
void schedule_active_arc::consistent_growth_ratios(
						   std:: vector<double> & vector_values,
						   std::unique_ptr<state_active_arc_class> & state,
						   const schedule & input_schedule,
						   const long int & iteration_index
						   )
{
  //
  if(
     input_schedule.parameter=="curvature"
     &&
     input_schedule.control_type=="consistent_length"
     )
    {
      int pointer=-1;
      for(
	  int parameter_index=0;
	  parameter_index<number_of_scheduled_parameters;
	  parameter_index++
	  )
	{
	  if(
	     schedules[parameter_index].parameter=="length"
	     &&
	     schedules[parameter_index].categoly=="arc"
	     )
	    {
	      pointer=parameter_index;
	    };
	};
      if(pointer==-1) io_method.error_output(
					     "active_arc_schedule",
					     "curcature_constraint_growth",
					     "no input schedule of arc length!"
					     );
      set_values(
		 state->reference.dummy,
		 state,
		 state->stress.tangent_stress,
		 schedules[pointer],
		 iteration_index
		 );
    }
  else
    {
      io_method.error_output(
			     "active_arc_schedule",
			     "curcature_constraint_growth",
			     "no input schedule of arc length!"
			     );
    };
};
//
void schedule_active_arc::check_consistency_values_and_incriments(
								  const schedule & input_schedule
								  )
{
  if(input_schedule.incriments.size()!=input_schedule.values.size())
    {
      io_method.error_output(
			     "active_arc_schedule",
			     "check_consistency_values_and_incriments",
			     "inconsistency in array size of schedule"
			     );
    };
};
//
void schedule_active_arc::reconstruction_schedule_work(
						       const long int & number_of_new_arcs
						       )
{
  long int new_arc_index=0;
  if((long int) work_values.size()!=number_of_new_arcs)
    {
      work_values.clear();
      for(long int arc_index=0;
	  arc_index<(long int)division_on.size();
	  arc_index++)
	{
	  if(division_on[arc_index])
	    {
	      work_values.push_back(0.0);
	      new_arc_index++;
	      work_values.push_back(0.0);
	      new_arc_index++;
	    }
	  else
	    {
	      work_values.push_back(0.0);
	      new_arc_index++;
	    };
	};
    }
};
//
void schedule_active_arc::reconstruction_schedule_length(
							 const long int & number_of_new_arcs,
							 schedule & input_schedule,
							 const double & value_division,
							 const double & incriment_division
							 )
{
  //
  //
  long int new_arc_index=0;
  if((long int)work_values.size()==number_of_new_arcs)
    {
      for(long int arc_index=0;
	  arc_index<(long int)input_schedule.values.size();
	  arc_index++)
	{
	  if(division_on[arc_index])
	    {
	      work_values[new_arc_index]=input_schedule.values[arc_index]/value_division;
	      new_arc_index++;
	      work_values[new_arc_index]=input_schedule.values[arc_index]/value_division;
	      new_arc_index++;
	    }
	  else
	    {
	      work_values[new_arc_index]=input_schedule.values[arc_index];
	      new_arc_index++;
	    };
	};
    }
  else
    {
      io_method.error_output(
			     "active_arc_schedule",
			     "reconstruction_schedule_length",
			     "too short size of work array!"
			     );
    };
  input_schedule.replace_values(
				work_values
				);

  //
  //
  new_arc_index=0;
  for(long int arc_index=0;
      arc_index<(long int)input_schedule.incriments.size();
      arc_index++)
    {
      if(division_on[arc_index])
	{
	  work_values[new_arc_index]=input_schedule.incriments[arc_index]/incriment_division;
	  new_arc_index++;
	  work_values[new_arc_index]=input_schedule.incriments[arc_index]/incriment_division;
	  new_arc_index++;
	}
      else
	{
	  work_values[new_arc_index]=input_schedule.incriments[arc_index];
	  new_arc_index++;
	};
    };
  //
  input_schedule.replace_incriments(
				    work_values
				    );
  //
  input_schedule.replace_adaptations((long int)work_values.size());
  //
  input_schedule.output_growth();
  //
};
//
void schedule_active_arc::reconstruction(
					 model_parameters_active_arc_class & model,
					 model_reconstructor & reconstructor
					 )
{
  set_division_on(
		  reconstructor
		  );
  long int number_of_new_arcs= model.get_number_of_arcs();
  reconstruction_schedule_work(
			       number_of_new_arcs
			       );
  volumes.resize(number_of_new_arcs);
  inner_volumes.resize(number_of_new_arcs);
  for(
      int parameter_index=0;
      parameter_index<number_of_scheduled_parameters;
      parameter_index++
      )
    {
      if(
	 (
	  (
	   schedules[parameter_index].categoly=="arc"
	   )
	  &&
	  (
	   schedules[parameter_index].parameter=="length"
	   )
	  )
	 ||
	 (
	  (
	   schedules[parameter_index].categoly=="inner"
	   )
	  &&
	  (
	   schedules[parameter_index].parameter=="width"
	   )
	  )
	 )
	{
	  check_consistency_values_and_incriments(
						  schedules[parameter_index]
						  );
	  reconstruction_schedule_length(
					 number_of_new_arcs,
					 schedules[parameter_index],
					 2.0,
					 2.0
					 );
	}
      else if
	(
	 (
	  schedules[parameter_index].categoly=="arc"
	  )
	 &&
	 (
	  schedules[parameter_index].parameter=="curvature"
	  )
	 )
	{
	  check_consistency_values_and_incriments(
						  schedules[parameter_index]
						  );
	  reconstruction_schedule_length(
					 number_of_new_arcs,
					 schedules[parameter_index],
					 1.0,
					 1.0
					 );
	}
      else
	{
	  check_consistency_values_and_incriments(
						  schedules[parameter_index]
						  );
	  reconstruction_schedule_length(
					 number_of_new_arcs,
					 schedules[parameter_index],
					 1.0,
					 1.0
					 );
	};
    };
};
//
void schedule_active_arc::set_division_on(
					  model_reconstructor & reconstructor
					  )
{
  division_on.clear();
  for(long int index=0;
      index<(long int)reconstructor.get_number_of_old_arcs();
      index++)
    {
      division_on.push_back(0.0);
    };
  reconstructor.get_division_on(division_on);
};
//
/* constructor*/
schedule_active_arc::schedule_active_arc()
{
   PI=3.1415926535897931;
};
