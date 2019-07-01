#include "active_arc_state.hpp"
//
void state_active_arc_class::initialize_reference(
						  model_parameters_active_arc_class & model
						  )
{
  // dimensitons of arcs
  arc_length_reference.clear();
  arc_width_reference.clear();
  arc_curvature_reference.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      arc_length_reference.push_back(model.get_arc_length());
      arc_width_reference.push_back(model.get_arc_width());
      arc_curvature_reference.push_back(model.get_arc_curvature());
    };
  // elastic parameters of arcs
  arc_young_modulus_reference.clear();
  arc_poisson_ratio_reference.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      arc_young_modulus_reference.push_back(model.get_young_modulus());
      arc_poisson_ratio_reference.push_back(model.get_poisson_ratio());
    };
};						  
//
void state_active_arc_class::initialize_reference_parameters(
							     const std::string & mode_flag
							     )
{
  if(mode_flag=="uniform_initialize")
    {
      for(long int arc_index=0; arc_index< number_of_arcs; arc_index++)
	{
	  arc_length_reference[arc_index] = arc_length;
	  arc_width_reference[arc_index] = arc_width;
	  arc_curvature_reference[arc_index] = arc_curvature;
	  arc_young_modulus_reference[arc_index] = young_modulus;
	  arc_poisson_ratio_reference[arc_index] = poisson_ratio;
	};
      //
      local_geometry();
      reference_plot(
		     0,
		     0,
		     "gnuplot"
		     );
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "reference_set",
			     "Error:undefined format!"
			     );
    };
  push_reference_box();
  calculate_modulus();
};
//
void state_active_arc_class::push_reference_box()
{     
      reference_box_max=*std::max_element(coordinates.begin(), coordinates.end());
      reference_box_min=*std::min_element(coordinates.begin(), coordinates.end());
};
//
void state_active_arc_class::reference_plot(
					    const long int & time_index,
 					    const int & sweep_step,
					    const std::string & format
					    )
{
  if(format=="gnuplot")
    {
      gnuplot_driver gplot("reference_plot");
      std::string direction_label;
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
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  reference_arc_plot(
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
inline void state_active_arc_class::reference_arc_plot(
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
  double Radial_length;
  if(
     arc_curvature_reference[arc_index]
     *arc_curvature_reference[arc_index]
     >bit_parameter
     )
    {
      arc_radius=1.0/arc_curvature_reference[arc_index];
      Radial_length
	= arc_radius
	- arc_width_reference[arc_index]/2.0;
      difference_angle
	=arc_length_reference[arc_index]
	*arc_curvature_reference[arc_index]/4.0;
    }
  else
    {
      difference_angle=2.0*PI/(double)number_of_arcs;
      arc_radius=arc_length_reference[arc_index]/difference_angle;
      Radial_length=arc_radius- arc_width_reference[arc_index]/2.0;
      difference_angle=difference_angle/4.0;
    };
  double start_angle=-2.0*difference_angle;
  double Angular_length;
  double normal_length;
  double tangent_length;
  //
  if(
     (boundary_condition=="close")
     &&
     (arc_index==number_of_arcs-1)
     )
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  centers[(number_of_arcs-1)*space_dimension+direction_index]
	    =(  coordinates[                                    direction_index]
	      + coordinates[(number_of_arcs -1)*space_dimension+direction_index])/2.0
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
  //
  for(int point_index=0; point_index<number_of_radial_points; point_index++)
    {
      Angular_length = start_angle + difference_angle * double(point_index);
      normal_length = Radial_length * cos(Angular_length) - arc_radius;
      tangent_length = Radial_length * sin(Angular_length);
      for(int direction_index=0; direction_index<space_dimension; direction_index++)
	{
	   reference_coordinates[point_index*space_dimension+direction_index]
	     = normal_length * normal_vectors[arc_index*space_dimension+direction_index]
	     + tangent_length * tangent_vectors[arc_index*space_dimension+direction_index]
	     + centers[space_dimension*arc_index+direction_index];
	};
      //      std::string message= "debug 0: "
      //	+ io_method.double_to_string(Angular_length)
      //	+ ", "
      //	+ io_method.double_to_string(tangent_length);
      //      io_method.standard_output(message);
    };
  //
  difference_angle=-difference_angle;
  Radial_length = arc_radius + arc_width_reference[arc_index]/2.0;
  start_angle=-2.0*difference_angle;
  for(int point_index=number_of_radial_points; point_index<number_of_reference_coordinates; point_index++)
    {
      Angular_length = start_angle + difference_angle * double(point_index-number_of_radial_points);
      normal_length = Radial_length * cos(Angular_length) - arc_radius;
      tangent_length = Radial_length * sin(Angular_length);
      for(int direction_index=0; direction_index<space_dimension; direction_index++)
	{
	   reference_coordinates[point_index*space_dimension+direction_index]
	     = normal_length * normal_vectors[arc_index*space_dimension+direction_index]
	     + tangent_length * tangent_vectors[arc_index*space_dimension+direction_index]
	     + centers[space_dimension*arc_index+direction_index];
	};
      //      std::string message= "debug 1: "
      //	+ io_method.double_to_string(Angular_length)
      //	+ ", "
      //	+ io_method.double_to_string(tangent_length);
      // io_method.standard_output(message);
    };
  //
  for(int point_index=0; point_index<number_of_reference_coordinates-1; point_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  arrows[point_index*space_dimension+direction_index]
	    =reference_coordinates[(point_index+1)*space_dimension+direction_index]
	    -reference_coordinates[(point_index+0)*space_dimension+direction_index];
	};
    };
  for(int direction_index=0;direction_index<space_dimension;direction_index++)
    {
      arrows[(number_of_reference_coordinates-1)*space_dimension+direction_index]
	=reference_coordinates[                                                   +direction_index]
	-reference_coordinates[(number_of_reference_coordinates-1)*space_dimension+direction_index];
    };
  //
  gplot.output_vectors_on_loading_file(
				       reference_coordinates,
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
};
//
void state_active_arc_class::output_reference(
					      const long int & time_index,
					      const int & sweep_step
					      )
{
    state_debug_io debug_configuration(
				       "reference_output",
				       0,
				       number_of_arcs,
				       5
				       );
    debug_configuration.set_debug_data(
				       "length",
				       arc_length_reference,
				       0,
				       1
				       );
    debug_configuration.set_debug_data(
				       "width",
				       arc_width_reference,
				       1,
				       1
				       );
    debug_configuration.set_debug_data(
				       "curvature",
				       arc_curvature_reference,
				       2,
				       1
				       );
    debug_configuration.set_debug_data(
				       "young_modulus",
				       arc_young_modulus_reference,
				       3,
				       1
				       );
    debug_configuration.set_debug_data(
				       "poisson_ratio",
				       arc_poisson_ratio_reference,
				       4,
				       1
				       );
  debug_configuration.output_debug_data(
					0,
					time_index,
					sweep_step
					);
};
