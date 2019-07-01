#include "active_arc_state.hpp"
/*
  Operation
 */
//
void state_active_arc_class::initialize_stress()
{
  tangent_stress.clear();
  normal_stress.clear();
  shear_stress.clear();
  bending_stress.clear();
  shearbending_stress.clear();
  effective_elongation_stress.clear();
  effective_ripening_stress.clear();
  effective_shear_stress.clear();
  stress_for_plot.clear();
  action_point_for_plot.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      tangent_stress.push_back(0.0);
      normal_stress.push_back(0.0);
      shear_stress.push_back(0.0);
      bending_stress.push_back(0.0);
      shearbending_stress.push_back(0.0);
      effective_elongation_stress.push_back(0.0);
      effective_ripening_stress.push_back(0.0);
      effective_shear_stress.push_back(0.0);
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  stress_for_plot.push_back(0.0);
	  action_point_for_plot.push_back(0.0);
	}
    };
};
//
void state_active_arc_class::calculate_stress()
{
  calculate_tangent_stress();
  calculate_normal_stress();
  calculate_shear_stress();
  calculate_bending_stress();
  calculate_shearbending_stress();
  calculate_effective_stress();
};
//
void state_active_arc_class::calculate_tangent_stress()
{
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    {
      tangent_stress[arc_index]
	= 
	effective_tangent_strain[arc_index]
	* bulk_modulus[arc_index]
	+
	effective_bending_strain[arc_index]
	* tangent_bending_modulus_first_power[arc_index];
    };
};
//
void state_active_arc_class::calculate_normal_stress()
{
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    {
      normal_stress[arc_index]
	= 
	 effective_normal_strain[arc_index] 
	* bulk_modulus[arc_index]
	+
	 effective_shearbending_strain[arc_index]
	* tangent_bending_modulus_first_power[arc_index];
    };
};
//
void state_active_arc_class::calculate_shear_stress()
{
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    {
      shear_stress[arc_index]
	= 
	2.0
	* effective_shear_strain[arc_index]
	* bulk_modulus[arc_index];
    };
};
//
void state_active_arc_class::calculate_bending_stress()
{
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    { 
      bending_stress[arc_index]
       = 
	effective_tangent_strain[arc_index] 
	* tangent_bending_modulus_first_power[arc_index]
	+
	effective_bending_strain[arc_index]
	* tangent_bending_modulus_second_power[arc_index];
    };
};
//
void state_active_arc_class::calculate_shearbending_stress()
{
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    { 
      shearbending_stress[arc_index]
	= 
	effective_normal_strain[arc_index]
	* tangent_bending_modulus_first_power[arc_index]
	+
	effective_shearbending_strain[arc_index]
	* tangent_bending_modulus_second_power[arc_index];
    };
};
//
void state_active_arc_class::calculate_effective_stress()
{
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    { 
      effective_elongation_stress[arc_index]
	=
	tangent_stress[arc_index]
	+
	0.5*shear[arc_index]
	*shear_stress[arc_index];
      effective_ripening_stress[arc_index]
	=
	normal_stress[arc_index]
	+
	0.5*shear[arc_index]
	*shear_stress[arc_index]
	+
	arc_curvature_reference[arc_index]
	*arc_length_reference[arc_index]
	*bending_stress[arc_index];
    };
  if(shear_constraint_switch)
    {
      for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
	{ 
	  effective_shear_stress[arc_index]
	    =
	    shear[arc_index]*
	    normal_stress[arc_index]
	    +
	    0.5*(1.0+elongation[arc_index]+ripening[arc_index])
	    *shear_stress[arc_index]
	    +shear_constraint_stress[arc_index];
	};
    }
  else
    {
      for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
	{ 
	  effective_shear_stress[arc_index]
	    =
	    shear[arc_index]*
	    normal_stress[arc_index]
	    +
	    0.5*(1.0+elongation[arc_index]+ripening[arc_index])
	    *shear_stress[arc_index];
	};
    };
};
//
void state_active_arc_class::stress_get()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      stress.tangent_stress[arc_index]=tangent_stress[arc_index];
      stress.normal_stress[arc_index]=normal_stress[arc_index];
      stress.shear_stress[arc_index]=shear_stress[arc_index];
      stress.bending_stress[arc_index]=bending_stress[arc_index];
      stress.shearbending_stress[arc_index]=shearbending_stress[arc_index];
    };
};
//
void state_active_arc_class::output_stress(
					   const long int & time_index,
					   const int & sweep_step
					   )
{
  state_debug_io debug_stress(
			      "stress_output",
			      0,
			      number_of_arcs,
			      8
			      );
  debug_stress.set_debug_data(
			     "tangent",
			     tangent_stress,
			     0,
			     1
			     );
  debug_stress.set_debug_data(
			     "normal",
			     normal_stress,
			     1,
			     1
			     );
  debug_stress.set_debug_data(
			     "shear",
			     shear_stress,
			     2,
			     1
			     );
  debug_stress.set_debug_data(
			     "bending",
			     bending_stress,
			     3,
			     1
			     );
  debug_stress.set_debug_data(
			     "shearbending",
			     shearbending_stress,
			     4,
			     1
			     );
  debug_stress.set_debug_data(
			     "effective_elongation_stress",
			     effective_elongation_stress,
			     5,
			     1
			     );
  debug_stress.set_debug_data(
			     "effective_ripening_stress",
			     effective_ripening_stress,
			     6,
			     1
			     );
  debug_stress.set_debug_data(
			     "effective_shear_stress",
			     effective_shear_stress,
			     7,
			     1
			     );
  debug_stress.output_debug_data(
				0,
				time_index,
				sweep_step
				);
};
//
void state_active_arc_class::local_stress_plot(
					       const model_parameters_active_arc_class & model,
					       const long int & time_index,
					       const int & sweep_step,
					       const std::string & format
					       )
{
  gnuplot_driver gplot("local_stress_plot");
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
      // for upper stress:
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  for(
	      int direction_index=0;
	      direction_index<space_dimension;
	      direction_index++
	      )
	    {
	      stress_for_plot[arc_index*space_dimension+direction_index]
		=normal_stress[arc_index]
		*normal_vectors[arc_index*space_dimension+direction_index];
	      action_point_for_plot[arc_index*space_dimension+direction_index]
		=centers[arc_index*space_dimension+direction_index]
		+arc_width_reference[arc_index]
		*normal_vectors[arc_index*space_dimension+direction_index];
	    };
	};
      //
      gplot.output_vectors_on_loading_file(
					   action_point_for_plot,
					   stress_for_plot,
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
      // for lower stress:
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  for(
	      int direction_index=0;
	      direction_index<space_dimension;
	      direction_index++
	      )
	    {
	      stress_for_plot[arc_index*space_dimension+direction_index]
		=-normal_stress[arc_index]
		*normal_vectors[arc_index*space_dimension+direction_index];
	      action_point_for_plot[arc_index*space_dimension+direction_index]
		=centers[arc_index*space_dimension+direction_index]
		-arc_width_reference[arc_index]
		*normal_vectors[arc_index*space_dimension+direction_index];
	    };
	};
      //
      gplot.output_vectors_on_loading_file(
					   centers,
					   normal_vectors,
					   number_of_arcs,
					   "on",
					   "3",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "off"
					   );
      // for left stress:
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  for(
	      int direction_index=0;
	      direction_index<space_dimension;
	      direction_index++
	      )
	    {
	      stress_for_plot[arc_index*space_dimension+direction_index]
		=tangent_stress[arc_index]
		*tangent_vectors[arc_index*space_dimension+direction_index];
	      action_point_for_plot[arc_index*space_dimension+direction_index]
		=centers[arc_index*space_dimension+direction_index]
		+arc_length_reference[arc_index]
		*tangent_vectors[arc_index*space_dimension+direction_index];
	    };
	};
      //
      gplot.output_vectors_on_loading_file(
					   action_point_for_plot,
					   stress_for_plot,
					   number_of_arcs,
					   "on",
					   "4",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "on"
					   );
      // for right stress:
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  for(
	      int direction_index=0;
	      direction_index<space_dimension;
	      direction_index++
	      )
	    {
	      stress_for_plot[arc_index*space_dimension+direction_index]
		=-tangent_stress[arc_index]
		*tangent_vectors[arc_index*space_dimension+direction_index];
	      action_point_for_plot[arc_index*space_dimension+direction_index]
		=centers[arc_index*space_dimension+direction_index]
		-arc_length_reference[arc_index]
		*tangent_vectors[arc_index*space_dimension+direction_index];
	    };
	};
      //
      gplot.output_vectors_on_loading_file(
					   action_point_for_plot,
					   stress_for_plot,
					   number_of_arcs,
					   "on",
					   "4",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "on"
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
