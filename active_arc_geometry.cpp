#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::initialize_local_geometry()
{
  end_to_end_length.clear();
  tangent_vectors.clear();
  normal_vectors.clear();
  tangent_vectors.clear();
  normal_vectors.clear();
  //
  relative_coordinates.clear();
  //
  center_to_center_length.clear();
  center_of_coordinates.clear();
  center_coordinates.clear();
  center_tangent_vectors.clear();
  center_normal_vectors.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      end_to_end_length.push_back(0.0);
      center_to_center_length.push_back(0.0);
    };
  for(
      int direction_index=0;
      direction_index<space_dimension;
      direction_index++
      )
    {
      center_of_coordinates.push_back(0.0);
    };
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  relative_coordinates.push_back(0.0);
	  tangent_vectors.push_back(0.0);
	  normal_vectors.push_back(0.0);
	  center_coordinates.push_back(0.0);
	  center_tangent_vectors.push_back(0.0);
	  center_normal_vectors.push_back(0.0);
	};
    };
};
//
void state_active_arc_class::local_geometry()
{
  // calculation of relatice coordinates
  if(boundary_condition!="close")
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      relative_coordinates[ arc_index   *space_dimension+direction_index]
		= coordinates[(arc_index+1)*space_dimension+direction_index]
		- coordinates[ arc_index   *space_dimension+direction_index];
	    };
	};
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs-1; arc_index++)
	{
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      relative_coordinates[ arc_index   *space_dimension+direction_index]
		= coordinates[(arc_index+1)*space_dimension+direction_index]
		- coordinates[ arc_index   *space_dimension+direction_index];
	    };
	};
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  relative_coordinates[(number_of_arcs-1)*space_dimension+direction_index]
	    = coordinates[                                   direction_index]
	    - coordinates[(number_of_arcs-1)*space_dimension+direction_index];
	};
    };
  // calculation of end to end distance
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      end_to_end_length[arc_index]
	= std::sqrt(
		   std::inner_product(
				      relative_coordinates.begin()+ arc_index   *space_dimension,
				      relative_coordinates.begin()+(arc_index+1)*space_dimension,
				      relative_coordinates.begin()+ arc_index   *space_dimension,
				      0.0
				      )
		    );
    };
  // calculation of tangential vector
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  tangent_vectors[arc_index*space_dimension+direction_index]
	    =relative_coordinates[arc_index*space_dimension+direction_index]
	    / end_to_end_length[arc_index];
	};
    };
  // calculation of normal vector
  if(space_dimension==2)
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  normal_vectors[arc_index*space_dimension  ]
	    =-tangent_vectors[arc_index*space_dimension+1];
	  normal_vectors[arc_index*space_dimension+1]
	    = tangent_vectors[arc_index*space_dimension  ];
	};
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "local_geometry",
			     "Error:higher dimension!"
			     );
    };
};
//
void state_active_arc_class::initialize_center_of_coordinates()
{
  for(
      int direction_index=0;
      direction_index < space_dimension;
      direction_index++
      )
    {
      center_of_coordinates[direction_index]=0.0;
    }
  //
  for(
      int direction_index=0;
      direction_index < space_dimension;
      direction_index++
      )
    {
      for(
	  long int joint_index = 0;
	  joint_index < number_of_joints;
	  joint_index ++
	  )
	{
	  center_of_coordinates[direction_index]
	    +=coordinates[joint_index*space_dimension+direction_index];
	};
    };
  //
  for(
      int direction_index=0;
      direction_index < space_dimension;
      direction_index++
      )
    {
      center_of_coordinates[direction_index]
	=center_of_coordinates[direction_index]/((double)number_of_joints);
    }
};
//
void state_active_arc_class::translate_center_to_zero()
{
  for(
      long int joint_index=0;
      joint_index < number_of_joints;
      joint_index ++
      )
    {
      for(
	  int direction_index=0;
	  direction_index < space_dimension;
	  direction_index++
	  )
	{
	  coordinates[joint_index*space_dimension+direction_index]
	    -=center_coordinates[direction_index];
	};
    };
  for(
      int direction_index=0;
      direction_index < space_dimension;
      direction_index++
      )
    {
      center_coordinates[direction_index]=0.0;
    };
};
//
void state_active_arc_class::local_center_geometry()
{
  // calculate_center_of_coordinates();
  // calculation of relatice coordinates
  if(boundary_condition!="close")
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      center_coordinates[ arc_index   *space_dimension+direction_index]
		= (  
		   coordinates[(arc_index+1)*space_dimension+direction_index]
		   + coordinates[ arc_index   *space_dimension+direction_index]
		     )
		*0.5
		- center_of_coordinates[direction_index];
	    };
	};
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs-1; arc_index++)
	{
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      center_coordinates[ arc_index   *space_dimension+direction_index]
		= ( 
		   coordinates[(arc_index+1)*space_dimension+direction_index]
		   + coordinates[ arc_index   *space_dimension+direction_index]
		    )
		*0.5
		- center_of_coordinates[direction_index];
	    };
	};
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  center_coordinates[(number_of_arcs-1)*space_dimension+direction_index]
	    = (
	       coordinates[                                   direction_index]
	       + coordinates[(number_of_arcs-1)*space_dimension+direction_index]
	       )
	    *0.5
	    - center_of_coordinates[direction_index];
	};
    };
  // calculation of end to end distance
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      center_to_center_length[arc_index]
	= std::sqrt(
		   std::inner_product(
				      center_coordinates.begin()+ arc_index   *space_dimension,
				      center_coordinates.begin()+(arc_index+1)*space_dimension,
				      center_coordinates.begin()+ arc_index   *space_dimension,
				      0.0
				      )
		    );
    };
  // calculation of tangential vector
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  center_normal_vectors[arc_index*space_dimension+direction_index]
	    =center_coordinates[arc_index*space_dimension+direction_index]
	    / center_to_center_length[arc_index];
	};
    };
  // calculation of normal vector
  if(space_dimension==2)
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  center_tangent_vectors[arc_index*space_dimension  ]
	    = center_normal_vectors[arc_index*space_dimension+1];
	  center_tangent_vectors[arc_index*space_dimension+1]
	    =-center_normal_vectors[arc_index*space_dimension  ];
	};
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "local_center_geometry",
			     "Error:higher dimension!"
			     );
    };
};
//
void state_active_arc_class::local_geometry_plot(
						 const model_parameters_active_arc_class & model,
						 const long int & time_index,
						 const int & sweep_step,
						 const std::string & format
						 )
{
  gnuplot_driver gplot("local_geometry_plot");
  std::string direction_label;
  double arc_radius;
  if(format=="gnuplot")
    {
      for(long int arc_index=0; arc_index<number_of_arcs-1; arc_index++)
	{
	  if(
	     arc_curvature_reference[arc_index]
	     *arc_curvature_reference[arc_index]
	     >bit_parameter)
	    {
	      arc_radius
		=1.0/arc_curvature_reference[arc_index];
	    }
	  else
	    {
	      arc_radius = 0.0;
	     };
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      centers[space_dimension*arc_index+direction_index]
		=(  coordinates[(arc_index+1)*space_dimension+direction_index]
		  + coordinates[(arc_index+0)*space_dimension+direction_index])/2.0
		+(
		  1.0-cos(
			  arc_length_reference[arc_index]
			  *arc_curvature_reference[arc_index]/2.0
			  )
		  )
		*arc_radius
		*normal_vectors[arc_index*space_dimension+direction_index];
	    };
	};
      if(boundary_condition=="close")
	{
	  if(
	     arc_curvature_reference[number_of_arcs -1]
	     *arc_curvature_reference[number_of_arcs -1]
	     >bit_parameter)
	    {
	      arc_radius
		=1.0/arc_curvature_reference[number_of_arcs -1];
	    }
	  else
	    {
	      arc_radius = 0.0;
	     };
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      centers[(number_of_arcs-1)*space_dimension+direction_index]
		=(  coordinates[                                    direction_index]
		  + coordinates[(number_of_arcs -1)*space_dimension+direction_index])/2.0
		+(1.0-cos(arc_length_reference[number_of_arcs -1]*arc_curvature_reference[number_of_arcs -1]/2.0))
		*arc_radius
		*normal_vectors[(number_of_arcs -1)*space_dimension+direction_index];
	    };
	};
      //
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
      gplot.output_vectors_on_loading_file(
					   centers,
					   tangent_vectors,
					   number_of_arcs,
					   "on",
					   "3",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "on"
					   );
      gplot.output_vectors_on_loading_file(
					   centers,
					   normal_vectors,
					   number_of_arcs,
					   "on",
					   "4",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "off"
					   );
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "plot",
			     "Error:undefined format!"
			     );
    };
};
//
/*
void state_active_arc_class::local_geometry_plot(
						 const model_parameters_active_arc_class & model,
						 const long int & time_index,
						 const int & sweep_step,
						 const std::string & format
						 )
{
  gnuplot_driver gplot("local_geometry_plot");
  std::string direction_label;
  double arc_radius;
  if(format=="gnuplot")
    {
      for(long int arc_index=0; arc_index<number_of_arcs-1; arc_index++)
	{
	  if(
	     arc_curvature_reference[arc_index]
	     *arc_curvature_reference[arc_index]
	     >bit_parameter)
	    {
	      arc_radius
		=1.0/arc_curvature_reference[arc_index];
	    }
	  else
	    {
	      arc_radius = 0.0;
	     };
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      centers[space_dimension*arc_index+direction_index]
		=(  coordinates[(arc_index+1)*space_dimension+direction_index]
		  + coordinates[(arc_index+0)*space_dimension+direction_index])/2.0
		+(
		  1.0-cos(
			  arc_length_reference[arc_index]
			  *arc_curvature_reference[arc_index]/2.0
			  )
		  )
		*arc_radius
		*normal_vectors[arc_index*space_dimension+direction_index];
	    };
	};
      if(boundary_condition=="close")
	{
	  if(
	     arc_curvature_reference[number_of_arcs -1]
	     *arc_curvature_reference[number_of_arcs -1]
	     >bit_parameter)
	    {
	      arc_radius
		=1.0/arc_curvature_reference[number_of_arcs -1];
	    }
	  else
	    {
	      arc_radius = 0.0;
	     };
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      centers[(number_of_arcs-1)*space_dimension+direction_index]
		=(  coordinates[                                    direction_index]
		  + coordinates[(number_of_arcs -1)*space_dimension+direction_index])/2.0
		+(1.0-cos(arc_length_reference[number_of_arcs -1]*arc_curvature_reference[number_of_arcs -1]/2.0))
		*arc_radius
		*normal_vectors[(number_of_arcs -1)*space_dimension+direction_index];
	    };
	};
      //
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
      gplot.output_vectors_on_loading_file(
					   centers,
					   tangent_vectors,
					   number_of_arcs,
					   "on",
					   "3",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "on"
					   );
      gplot.output_vectors_on_loading_file(
					   centers,
					   normal_vectors,
					   number_of_arcs,
					   "on",
					   "4",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "off"
					   );
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "plot",
			     "Error:undefined format!"
			     );
    };
};
*/
 //
void state_active_arc_class::local_center_geometry_plot(
							const model_parameters_active_arc_class & model,
							const long int & time_index,
							const int & sweep_step,
							const std::string & format
							)
{
  gnuplot_driver gplot("local_center_geometry_plot");
  std::string direction_label;
  if(format=="gnuplot")
    {
      //
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
      gplot.output_vectors_on_loading_file(
					   center_coordinates,
					   center_tangent_vectors,
					   number_of_arcs,
					   "on",
					   "3",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "on"
					   );
      gplot.output_vectors_on_loading_file(
					   center_coordinates,
					   center_normal_vectors,
					   number_of_arcs,
					   "on",
					   "4",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "off"
					   );
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "plot",
			     "Error:undefined format!"
			     );
    };
};


