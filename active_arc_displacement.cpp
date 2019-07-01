#include "active_arc_state.hpp"
//
void state_active_arc_class::initialize_displacements(
						      )
{
  // displacements
  elongation.clear();
  ripening.clear();
  shear.clear();
  curvature_releasing.clear();
  normal_modulation.clear();
  // supplemental displacements
  normal_angles.clear();
  local_curvature.clear();
  reference_angles.clear();
  average_angles.clear();
  difference_angles.clear();
  //
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      elongation.push_back(0.0);
      ripening.push_back(0.0);
      shear.push_back(0.0);
      curvature_releasing.push_back(0.0);
      normal_modulation.push_back(0.0);
      //
      normal_angles.push_back(0.0);
      local_curvature.push_back(0.0);
      average_angles.push_back(0.0);
      difference_angles.push_back(0.0);
    };
  //
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  reference_angles.push_back(0.0);
	};
    };
};
//
void state_active_arc_class::calculate_displacement(const std::string & flag)
{
  if(flag=="initialization")
    {
      calculate_normal_angles();
      calculate_reference_angles();
      calculate_average_angles();
      calculate_difference_angles();
      calculate_local_curvature();
      //
      calculate_curvature_releasing();
      calculate_elongation();
      calculate_ripening();
      calculate_shear();
      calculate_normal_modulation();  
    }
  else if(flag=="debug")
    {
      //io_method.standard_output("!!! debug initialization of displacements!!!");
      calculate_normal_angles();
      calculate_reference_angles();
      calculate_average_angles();
      calculate_difference_angles();
      calculate_local_curvature();
      //
      calculate_curvature_releasing();
      calculate_elongation();
      calculate_ripening();
      calculate_shear();
      zero_initialization_normal_modulation();  
    }
  else
    {
      //copy_variable_to_joint();
      //
      calculate_normal_angles();
      calculate_reference_angles();
      calculate_average_angles();
      calculate_difference_angles();
      calculate_local_curvature();
      //
      //copy_variable_to_arc();
      //
      calculate_curvature_releasing();
      calculate_elongation();
      //      calculate_ripening();
      calculate_shear();
      //      calculate_normal_modulation(); 
    };
};
//
void state_active_arc_class::calculate_normal_angles()
{
    /*
  std::string message="debug in normal_angles:";
  io_method.standard_output(message);
   */
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      normal_angles[arc_index]
	=atan2(
	       normal_vectors[arc_index*space_dimension+1],
	       normal_vectors[arc_index*space_dimension  ]
	       );
            /*
      message=
	io_method.longint_to_string(arc_index)+ " : "+
	io_method.double_to_string(normal_angles[arc_index]);
	io_method.standard_output(message);
	*/
    };
};
//
void state_active_arc_class::calculate_reference_angles()
{
   /* debug
  std::string message="debug in reference_angles:";
  io_method.standard_output(message);
     */
  if(boundary_condition=="close")
    {
      for(long int arc_index=0; arc_index<number_of_arcs-1; arc_index++)
	{
	  for (int joint_index=0; joint_index<number_of_joints_for_arc; joint_index++)
	    {
	      reference_angles[arc_index*number_of_joints_for_arc+joint_index]
		= angles[arc_index+joint_index]
		- normal_angles[arc_index];
	    }
	};
      reference_angles[(number_of_arcs-1)*number_of_joints_for_arc]
	= angles[number_of_arcs-1]
	- normal_angles[number_of_arcs-1];
      reference_angles[(number_of_arcs-1)*number_of_joints_for_arc+1]
	= angles[0]
	- normal_angles[number_of_arcs-1];
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  for (int joint_index=0; joint_index<number_of_joints_for_arc; joint_index++)
	    {
	      reference_angles[arc_index*number_of_joints_for_arc+joint_index]
		= angles[arc_index+joint_index]
		- normal_angles[arc_index];
	    }
	};
    };
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      for (int joint_index=0; joint_index<number_of_joints_for_arc; joint_index++)
	{
	  if(
	     reference_angles[
			      arc_index*number_of_joints_for_arc
			      +joint_index
			      ]
	     >PI
	     )
	    {
	      reference_angles[
			       arc_index*number_of_joints_for_arc
			       +joint_index
			       ]
		/*
		=2.0*PI
		-reference_angles[
				  arc_index*number_of_joints_for_arc
				  +joint_index
				  ];
		*/
		=-2.0*PI
		+reference_angles[
				  arc_index*number_of_joints_for_arc
				  +joint_index
				  ];
	    }
	  else if(
		  reference_angles[
				   arc_index*number_of_joints_for_arc
				   +joint_index]
		  <-PI
		  )
	    {
	      reference_angles[arc_index*number_of_joints_for_arc+joint_index]
		=2.0*PI
		+reference_angles[
				  arc_index*number_of_joints_for_arc
				  +joint_index
				  ];
	    }
	}
                 /* debug
      message=
	io_method.longint_to_string(arc_index)+ " : "+
	io_method.double_to_string(angles[arc_index])+ " , "+
	io_method.double_to_string(normal_angles[arc_index])+ " , "+
	io_method.double_to_string(angles[arc_index+1])+ " , "+
	io_method.double_to_string(reference_angles[arc_index*number_of_joints_for_arc])+ " , "+
	io_method.double_to_string(reference_angles[arc_index*number_of_joints_for_arc+1])+ " , ";
	io_method.standard_output(message);
       	    */
    };
};
//
void state_active_arc_class::calculate_average_angles()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      average_angles[arc_index]
	= (
	     reference_angles[arc_index*number_of_joints_for_arc+1]
	   + reference_angles[arc_index*number_of_joints_for_arc  ]
	   )/2.0;
    };
};
//
void state_active_arc_class::calculate_difference_angles()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      // difference_angle is defined positive along anti-clockwise rotation.
      difference_angles[arc_index]
	= -(
	     reference_angles[arc_index*number_of_joints_for_arc+1]
	   - reference_angles[arc_index*number_of_joints_for_arc  ]
	   );
    };
};
//
void state_active_arc_class::calculate_local_curvature()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      local_curvature[arc_index]
	=2.0/end_to_end_length[arc_index]*sin(difference_angles[arc_index]*0.5);
    };
};
//
void state_active_arc_class::calculate_curvature_releasing()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      curvature_releasing[arc_index]
	/* old definition
	=1.0-arc_curvature_reference[arc_index]/local_curvature[arc_index];
	*/
	=difference_angles[arc_index] 
	- arc_curvature_reference[arc_index]*arc_length_reference[arc_index];
    };
};
//
void state_active_arc_class::calculate_elongation()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      elongation[arc_index]
	= 1.0 - arc_length_reference[arc_index]/end_to_end_length[arc_index]
	*boost::math::sinc_pi(difference_angles[arc_index]/2.0);
	/*
	=end_to_end_length[arc_index]/arc_length_reference[arc_index]
	/boost::math::sinc_pi(difference_angles[arc_index]/2.0)-1.0;
	*/
    };
};
//
void state_active_arc_class::calculate_ripening()
{
  if(boundary_condition=="close")
    {
      for(long int arc_index=0; arc_index<number_of_arcs-1; arc_index++)
	{
	  ripening[arc_index]
	    =(
	      thickness[arc_index+1]
	      + thickness[arc_index]
	      )/2.0/arc_width_reference[arc_index] - 1.0;
	};
      ripening[number_of_arcs-1]
	=(
	  thickness[number_of_arcs-1]
	  + thickness[0]
	  )/2.0/arc_width_reference[number_of_arcs-1] -1.0;
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  ripening[arc_index]
	    =(
	      thickness[arc_index+1]
	      + thickness[arc_index]
	      )/2.0 - arc_width_reference[arc_index];
	};
    };
};
//
void state_active_arc_class::calculate_shear()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      shear[arc_index]
	=average_angles[arc_index];
    };
};
//
void state_active_arc_class::calculate_normal_modulation()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      normal_modulation[arc_index]
	= arc_poisson_ratio_reference[arc_index]
	* (  
	   elongation[arc_index] 
	  +curvature_releasing[arc_index]
	     )
	- (1.0 - arc_poisson_ratio_reference[arc_index])
	* ripening[arc_index]
	- shear[arc_index]*shear[arc_index]/2.0;
    };
};
//
void state_active_arc_class::zero_initialization_normal_modulation()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      normal_modulation[arc_index] = 0.0;
    };
};
//
/*
void state_active_arc_class::calculate_curvature_releasing_from_tangent()
{
  if(boundary_condition!="close")
    {
      curvature_releasing[0] =
	1.0
	-std::sqrt(
		   (
		    1.0
		    -std::inner_product(
					tangent_vectors.begin()+(number_of_arcs-1)*space_dimension,
					tangent_vectors.begin()+(number_of_arcs  )*space_dimension,
					tangent_vectors.begin()+(               1)*space_dimension,
					0.0
					)
		    )
		   /2.0
		   )
	/arc_curvature_reference[0]
	/
	(
	 arc_length_reference[0]
	 +0.5*(
	       arc_length_reference[0+1]
	       +arc_length_reference[number_of_arcs-1]
	       )
	 );
      //
      curvature_releasing[number_of_arcs-1] =
	1.0
	-std::sqrt(
		   (
		    1.0
		    -std::inner_product(
					tangent_vectors.begin()+(number_of_arcs-2)*space_dimension,
					tangent_vectors.begin()+(number_of_arcs  )*space_dimension,
					tangent_vectors.begin()+(number_of_arcs-2)*space_dimension,
					0.0
					)
		    )
		   /2.0
		   )
	/arc_curvature_reference[0]
	/
	(
	 arc_length_reference[0]
	 +0.5*(
	       arc_length_reference[0+1]
	       +arc_length_reference[number_of_arcs-1]
	       )
	 );
    }
  else
    {
      curvature_releasing[0] = 0.0;
      curvature_releasing[number_of_arcs-1] = 0.0;
    };
  for(long int arc_index = 1; arc_index < number_of_arcs-1; arc_index++)
    {
      curvature_releasing[arc_index] =
	1.0
	-std::sqrt(
		   (
		    1.0
		    -std::inner_product(
					tangent_vectors.begin()+(arc_index-1)*space_dimension,
					tangent_vectors.begin()+(arc_index  )*space_dimension,
					tangent_vectors.begin()+(arc_index+1)*space_dimension,
					0.0
					)
		    )
		   /2.0
		   )
	/arc_curvature_reference[arc_index]
	/
	(
	 arc_length_reference[arc_index]
	 +0.5*(
	       arc_length_reference[arc_index+1]
	       +arc_length_reference[arc_index-1]
	       )
	 );
    };
};
*/
//
void state_active_arc_class::shape_plot(
					const long int & time_index,
					const int & sweep_step,
					const std::string & format
					)
{
  if(format=="gnuplot")
    {
      gnuplot_driver gplot("shape_plot");
      std::string direction_label;
      local_geometry();
      calculate_displacement("");
      //
      output_configuration(time_index,sweep_step);
      output_reference(time_index,sweep_step);
      double max=*std::max_element(coordinates.begin(), coordinates.end());
      double min=*std::min_element(coordinates.begin(), coordinates.end());
      //
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  if(direction_index==0)
	    {
	      direction_label="x";
	    } 
	  else if(direction_index==1)
	    {
	      direction_label="y";
	    }
	  else
	    {
	      direction_label="z";
	    }
	  gplot.set_axis(
			 direction_index,
			 min*plot_margin,
			 max*plot_margin,
			 (double)(std::max((int)((double)(max-min)/4.0),1)),
			 direction_label
			 );
	};
      gplot.make_plot_file(
			   0,
			   "png",
			   "splot",
			   time_index,
			   sweep_step
			   );
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  arc_plot(
		   gplot,
		   time_index,
		   sweep_step,
		   format,
		   arc_index
		   );
	};
    }
  else
    {
      io_method.error_output(
			     "state_active_arc_class",
			     "referene_plot",
			     "Error:undefined format!"
			     );
    };  
};
//
inline void state_active_arc_class::arc_plot(
					     gnuplot_driver & gplot,
					     const long int & time_index,
					     const int & sweep_step,
					     const std::string & format,
					     const long int & arc_index
					     )
{
  int number_of_radial_points=number_of_reference_coordinates/2;
  double difference_angle;
  double arc_radius;
  if(
     arc_curvature_reference[arc_index]
     *arc_curvature_reference[arc_index]
     >bit_parameter
     )
    {
    arc_radius=1.0/arc_curvature_reference[arc_index];
    difference_angle
    =arc_length_reference[arc_index]
    *arc_curvature_reference[arc_index]/4.0;
    }
  else
    {
    difference_angle=difference_angles[arc_index]/4.0;
    arc_radius
    = (elongation[arc_index]+1.0)
    *arc_length_reference[arc_index]
    /difference_angles[arc_index];
    };
  double start_angle
    =-2.0*difference_angle;
  //
  if(
     (boundary_condition=="close")
     &&
     (arc_index==number_of_arcs-1)
     )
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  centers[
		  (number_of_arcs-1)*space_dimension
		  +direction_index
		  ]
	    =
	    (  
	     coordinates[direction_index]
	     + coordinates[
			   (number_of_arcs -1)*space_dimension
			   +direction_index
			   ]
	       )/2.0
	    +(1.0-cos(start_angle))*arc_radius
	    *normal_vectors[arc_index*space_dimension+direction_index];
	};
    }
  else
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  centers[space_dimension*arc_index+direction_index]
	    =(  coordinates[(arc_index+1)*space_dimension+direction_index]
		+ coordinates[(arc_index+0)*space_dimension+direction_index])/2.0
	    +(1.0-cos(start_angle))*arc_radius
	    *normal_vectors[arc_index*space_dimension+direction_index];
	};
    };
  /*
  std::string message="debug 1:"
	+ io_method.longint_to_string(arc_index)
    + "/,"
	+ io_method.double_to_string(centers[space_dimension*arc_index])
	+ ", "
	+ io_method.double_to_string(centers[space_dimension*arc_index+1]);
  io_method.standard_output(message);
  */
  //
  double reference_normal_length = -arc_width_reference[arc_index]/2.0;
  double start_tangent = -arc_length_reference[arc_index]/2.0;
  double tangent_tics = arc_length_reference[arc_index]/4.0;
  double reference_tangent_length;
  double normal_local_displacement;
  //
  for(int point_index=0; point_index<number_of_radial_points; point_index++)
    {
      //
      reference_tangent_length=start_tangent+tangent_tics*point_index;
      normal_local_displacement
	=reference_normal_length
	*(
	  1.0
	  +ripening[arc_index]
	  +normal_modulation[arc_index]*reference_normal_length
	  *arc_curvature_reference[arc_index]*arc_curvature_reference[arc_index]
	  /0.5
	  );
      //
      for(int direction_index=0; direction_index<space_dimension; direction_index++)
	{
	  shape_coordinates[point_index*space_dimension+direction_index]
	    = (
	       normal_local_displacement
	       * cos(
		     local_curvature[arc_index]*reference_tangent_length
		     +shear[arc_index]
		     )
	       + (1.0 + elongation[arc_index])*reference_tangent_length
	       * csinc(
		       local_curvature[arc_index]*reference_tangent_length
		       // this is probabliy a mistake from note
		       //		       +shear[arc_index]
		       )
	       )
	    * normal_vectors[arc_index*space_dimension+direction_index]
	    + (
	       normal_local_displacement
	       *sin(local_curvature[arc_index]*reference_tangent_length+shear[arc_index])
	       + (1.0 + elongation[arc_index])*reference_tangent_length
	       * boost::math::sinc_pi(
				      local_curvature[arc_index]*reference_tangent_length
		       // this is probabliy a mistake from note
		       //		      +shear[arc_index]
				      )
	       )
	    * tangent_vectors[arc_index*space_dimension+direction_index]
	    + centers[space_dimension*arc_index+direction_index];
	};
    };
  //
  reference_normal_length = arc_width_reference[arc_index]/2.0;
  start_tangent = arc_length_reference[arc_index]/2.0;
  tangent_tics = -arc_length_reference[arc_index]/4.0;
  //
  for(
      int point_index=number_of_radial_points; 
      point_index<number_of_reference_coordinates; 
      point_index++
      )
    {
      //
      reference_tangent_length=start_tangent+tangent_tics*(point_index-number_of_radial_points);
      normal_local_displacement
	=reference_normal_length
	*(
	  1.0
	  +ripening[arc_index]
	  +normal_modulation[arc_index]*reference_normal_length
	  *arc_curvature_reference[arc_index]*arc_curvature_reference[arc_index]
	  /0.5
	  );
      //
      for(int direction_index=0; direction_index<space_dimension; direction_index++)
	{
	  shape_coordinates[point_index*space_dimension+direction_index]
	    = (
	       normal_local_displacement
	       * cos(
		     local_curvature[arc_index]*reference_tangent_length
		     +shear[arc_index]
		     )
	       + (1.0 + elongation[arc_index])*reference_tangent_length
	       * csinc(
		       local_curvature[arc_index]*reference_tangent_length
		       // this is probabliy a mistake from note
		       //+shear[arc_index]
		       )
	       )
	    * normal_vectors[arc_index*space_dimension+direction_index]
	    + (
	       normal_local_displacement*sin(
					     local_curvature[arc_index]*reference_tangent_length
					     +shear[arc_index]
					     )
	       + (1.0 + elongation[arc_index])*reference_tangent_length
	       * boost::math::sinc_pi(
				      local_curvature[arc_index]*reference_tangent_length
		       // this is probabliy a mistake from note
		       //		      +shear[arc_index]
				      )
	       )
	    * tangent_vectors[arc_index*space_dimension+direction_index]
	    + centers[space_dimension*arc_index+direction_index];
	  /*// debug should be deleted
	  io_method.standard_output("debug in plot: point["+io_method.int_to_string(point_index)+ "]");
	  io_method.standard_output("debug in plot: direction["+io_method.int_to_string(direction_index)+ "]");
	  io_method.standard_output("debug in plot: normal_mod["+io_method.double_to_string(normal_modulation[arc_index])+ "]");
	  io_method.standard_output("debug in plot: ripening["+io_method.double_to_string(ripening[arc_index])+ "]");
	  io_method.standard_output("debug in plot: curvature["+io_method.double_to_string(local_curvature[arc_index])+ "]");
	  io_method.standard_output("debug in plot: elongation["+io_method.double_to_string(elongation[arc_index])+ "]");
	  io_method.standard_output("debug in plot: shear["+io_method.double_to_string(shear[arc_index])+ "]");
	  io_method.standard_output("debug in plot: tangent_vectors["+io_method.double_to_string(tangent_vectors[arc_index*space_dimension*direction_index])+ "]");
	  io_method.standard_output("debug in plot: normal_vectors["+io_method.double_to_string(normal_vectors[arc_index*space_dimension*direction_index])+ "]");
	  io_method.standard_output("debug in plot: centers["+io_method.double_to_string(centers[arc_index*space_dimension*direction_index])+ "]");
	  io_method.standard_output("debug in plot: end_to_end_length["+io_method.double_to_string(end_to_end_length[arc_index])+ "]");
	  io_method.standard_output("debug in plot: difference_angles["+io_method.double_to_string(difference_angles[arc_index])+ "]");
	  //end_to_end_length[arc_index], difference_angles[arc_index]*/
	};
    };
  //
  for(int point_index=0; point_index<number_of_reference_coordinates-1; point_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  arrows[point_index*space_dimension+direction_index]
	    =shape_coordinates[(point_index+1)*space_dimension+direction_index]
	    -shape_coordinates[(point_index+0)*space_dimension+direction_index];
	};
    };
  for(int direction_index=0;direction_index<space_dimension;direction_index++)
    {
      arrows[(number_of_reference_coordinates-1)*space_dimension+direction_index]
	=shape_coordinates[                                                   +direction_index]
	-shape_coordinates[(number_of_reference_coordinates-1)*space_dimension+direction_index];
    };
  //
  gplot.output_vectors_on_loading_file(
				       shape_coordinates,
				       arrows,
				       number_of_reference_coordinates,
				       "off",
				       io_method.longint_to_string(arc_index%4),
				       0,
				       time_index,
				       sweep_step,
				       space_dimension,
				       1.0,
				       "off"
				       );
  //
};
//
inline double state_active_arc_class::csinc(const double & input_double)
{
  if(input_double<bit_parameter)
    {
      return -input_double/2.0;
    }
  else
    {
      return (cos(input_double)-1.0)/input_double;
    };
};
//
void state_active_arc_class::output_displacements(
						  const long int & time_index,
						  const int & sweep_step
						  )
{
  std::string message;
  static long int plane_index = 0;
  std::string file_header="displacements";
  std::string file=
    file_header
    + io_method.longint_to_format_string(plane_index,"%03d") 
    + "_"
    + io_method.longint_to_format_string(time_index,"%04d")  
    + "_"
    + io_method.int_to_format_string(sweep_step,"%04d") 
    +".dat";
  push_displacements_for_output();
  message = "# ";
  for(
      int observable_index = 0;
      observable_index<number_of_displacements
	+number_of_displacements_supplements;
      observable_index++
      )
    {
      message
	+=labels_for_output[observable_index] + "/";
    };
  io_method.output_message(message,file);
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      message = io_method.longint_to_string(arc_index) + " ";
      for(
	  int observable_index = 0;
	  observable_index<number_of_displacements
	+number_of_displacements_supplements;
	  observable_index++
	  )
	{
	  message
	    += io_method.double_to_string(
					  observables_for_output[
								 arc_index
								 *(number_of_displacements
								   +number_of_displacements_supplements)
								 +observable_index
								 ]
					  ) + " ";
	};
      io_method.output_message(message,file);
    };
  message=" ";
  io_method.output_message(message,file);
  io_method.output_message(message,file);
};
//
void state_active_arc_class::push_displacements_for_output()
{
  labels_for_output[0]="elongation";
  labels_for_output[1]="ripening";
  labels_for_output[2]="shear";
  labels_for_output[3]="curvature_releasing";
  labels_for_output[4]="normal_modulation";
  labels_for_output[5]="end_to_end_lengyh";
  labels_for_output[6]="local_curvature";
  labels_for_output[7]="difference_angle";
  //  labels_for_output[8]="-r*dsinc(DA/2)/2l";
  // labels_for_output[9]="R*cos(DA/2)/r";
  /*
  std::string message="debug in push:";
  io_method.standard_output(message);
  */
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+0]=elongation[arc_index];
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+1]=ripening[arc_index];
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+2]=shear[arc_index];
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+3]=curvature_releasing[arc_index];
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+4]=normal_modulation[arc_index];
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+5]=end_to_end_length[arc_index];
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+6]=local_curvature[arc_index];
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+7]=difference_angles[arc_index];
      /* Since dsinc function is inline, it should not be called here
      observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+8]=-end_to_end_length[arc_index]*dsinc(difference_angles[arc_index]*0.5)*0.5/arc_length_reference[arc_index];
      */
      /*
      if(arc_curvature_reference[arc_index]*arc_curvature_reference[arc_index]>bit_parameter){
	observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+9]
	  =-cos(difference_angles[arc_index]*0.5)/end_to_end_length[arc_index]/arc_curvature_reference[arc_index];
      }
      else
	{
	  observables_for_output[arc_index*(number_of_displacements+number_of_displacements_supplements)+9]=0.0; 
	};
      */
      /*
      message="";
      message=io_method.double_to_string(elongation[arc_index]) + " "
	+io_method.double_to_string(ripening[arc_index]) + " "
	+io_method.double_to_string(shear[arc_index]) + " "
	+io_method.double_to_string(curvature_releasing[arc_index]) + " "
	+io_method.double_to_string(normal_modulation[arc_index]);
      io_method.standard_output(message);
      */
    };
};
//
void state_active_arc_class::copy_variable_to_joint()
{
  for(int direction_index = 0; direction_index < space_dimension; direction_index++)
    {
      for(long int joint_index = 0; joint_index <  number_of_joints; joint_index++)
	{
	  coordinates[joint_index*space_dimension+direction_index]
	    =variable[joint_index*number_of_joint_components+direction_index];
	};
    };
  //
  for(long int joint_index = 0; joint_index <  number_of_joints; joint_index++)
    {
      angles[joint_index]
	=variable[joint_index*number_of_joint_components+space_dimension];
    };
};
//
void state_active_arc_class::copy_variable_to_arc()
{
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      ripening[arc_index]
	=variable[internal_head_pointer+arc_index*number_of_internal_components  ];
      normal_modulation[arc_index]
	=variable[internal_head_pointer+arc_index*number_of_internal_components+1];
    };
};
//
void state_active_arc_class::variable_initialization()
{
  for(int direction_index = 0; direction_index < space_dimension; direction_index++)
    {
      for(long int joint_index = 0; joint_index <  number_of_joints; joint_index++)
	{
	  variable[joint_index*number_of_joint_components+direction_index]
	    =coordinates[joint_index*space_dimension+direction_index];
	};
    };
  //
  for(long int joint_index = 0; joint_index <  number_of_joints; joint_index++)
    {
      variable[joint_index*number_of_joint_components+space_dimension] 
	= angles[joint_index];
    };
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      variable[internal_head_pointer+arc_index*number_of_internal_components  ]
	= ripening[arc_index];
      variable[internal_head_pointer+arc_index*number_of_internal_components+1]
	= normal_modulation[arc_index];
    };
};
//
void state_active_arc_class::output_arc_angles(
					       const long int & time_index,
					       const int & sweep_step
					       )
{
  state_debug_io debug_arc_angle(
				 "arc_angle_output",
				 0,
				 number_of_arcs,
				 5
				 );
  debug_arc_angle.set_debug_data(
			     "reference_angle",
			     reference_angles,
			     0,
			     2
			     );
  debug_arc_angle.set_debug_data(
			     "average_angle",
			     average_angles,
			     2,
			     1
			     );
  debug_arc_angle.set_debug_data(
			     "difference_angle",
			     difference_angles,
			     3,
			     1
			     );
  debug_arc_angle.set_debug_data(
			     "normal_angle",
			     normal_angles,
			     4,
			     1
			     );
  debug_arc_angle.output_debug_data(
				0,
				time_index,
				sweep_step
				);
};
