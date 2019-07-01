#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::initialize_strain()
{
  tangent_strain.clear();
  effective_tangent_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      tangent_strain.push_back(0.0);
      effective_tangent_strain.push_back(0.0);
    };
  normal_strain.clear();
  effective_normal_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      normal_strain.push_back(0.0);
      effective_normal_strain.push_back(0.0);
    };
  shear_strain.clear();
  effective_shear_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      shear_strain.push_back(0.0);
      effective_shear_strain.push_back(0.0);
    };
  bending_strain.clear();
  effective_bending_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      bending_strain.push_back(0.0);
      effective_bending_strain.push_back(0.0);
    };
  shearbending_strain.clear();
  effective_shearbending_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      shearbending_strain.push_back(0.0);
      effective_shearbending_strain.push_back(0.0);
    };
  tangent_strain.clear();
  effective_tangent_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      tangent_strain.push_back(0.0);
      effective_tangent_strain.push_back(0.0);
    };
  normal_strain.clear();
  effective_normal_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      normal_strain.push_back(0.0);
      effective_normal_strain.push_back(0.0);
    };
  shear_strain.clear();
  effective_shear_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      shear_strain.push_back(0.0);
      effective_shear_strain.push_back(0.0);
    };
  bending_strain.clear();
  effective_bending_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      bending_strain.push_back(0.0);
      effective_bending_strain.push_back(0.0);
    };
  shearbending_strain.clear();
  effective_shearbending_strain.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      shearbending_strain.push_back(0.0);
      effective_shearbending_strain.push_back(0.0);
    };
};
//
//
void state_active_arc_class::calculate_strain()
{
  calculate_tangent_strain();
  calculate_normal_strain();
  calculate_shear_strain();
  calculate_bending_strain();
  calculate_shearbending_strain();
  calculate_effective_strain();
  // After calculation of strains
  //calculate_shearbending_strain_from_other_strain();
};
//
void state_active_arc_class::initialize_diagonlizar()
{
  eigenvalues_plus.clear();
  eigenvalues_minus.clear();
  for(int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  eigenvalues_plus.push_back(0.0);
	  eigenvalues_minus.push_back(0.0);
	};
    };
  eigenvectors_plus.clear();
  eigenvectors_minus.clear();
  crossvectors_plus.clear();
  crossvectors_minus.clear();
  for(int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  eigenvectors_plus.push_back(0.0);
	  eigenvectors_plus.push_back(0.0);
	  eigenvectors_minus.push_back(0.0);
	  eigenvectors_minus.push_back(0.0);
	  crossvectors_plus.push_back(0.0);
	  crossvectors_plus.push_back(0.0);
	  crossvectors_minus.push_back(0.0);
	  crossvectors_minus.push_back(0.0);
	};
    };
  symmetric_matrix.clear();
  for(int arc_index=0; arc_index<number_of_arcs;arc_index++)
    {
      for(int low_index=0;low_index<space_dimension;low_index++)
	{
	  for(int column_index=low_index;column_index<space_dimension;column_index++)
	    {
	      symmetric_matrix.push_back(0.0);
	    };
	};
    };
};
//
void state_active_arc_class::calculate_tangent_strain()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      tangent_strain[arc_index]=elongation[arc_index];
    };
};
//
void state_active_arc_class::calculate_normal_strain()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      normal_strain[arc_index]
	=ripening[arc_index]
	+shear[arc_index]*shear[arc_index]/2.0;
    };
};
//
void state_active_arc_class::calculate_shear_strain()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      shear_strain[arc_index]=shear[arc_index]*(1.0+ripening[arc_index]+elongation[arc_index])/2.0;
    };
};
//
void state_active_arc_class::calculate_bending_strain()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      bending_strain[arc_index]
	=(arc_curvature_reference[arc_index]
	  *arc_length_reference[arc_index]*ripening[arc_index]
	  -curvature_releasing[arc_index]);
    };
};
//
void state_active_arc_class::calculate_shearbending_strain()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      //     shearbending_strain[arc_index]=ripening[arc_index]/2.0;
      shearbending_strain[arc_index]
	=normal_modulation[arc_index];
    }
  
};
//
void state_active_arc_class::calculate_effective_strain()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      effective_tangent_strain[arc_index]
	=tangent_strain[arc_index]
	+arc_poisson_ratio_reference[arc_index]
	*normal_strain[arc_index];
      effective_normal_strain[arc_index]
	=normal_strain[arc_index]
	+arc_poisson_ratio_reference[arc_index]
	*tangent_strain[arc_index];
      effective_shear_strain[arc_index]
	=(1.0-arc_poisson_ratio_reference[arc_index])
	*shear_strain[arc_index];
      effective_bending_strain[arc_index]
	=bending_strain[arc_index]
	+arc_poisson_ratio_reference[arc_index]
	*shearbending_strain[arc_index];
      effective_shearbending_strain[arc_index]
	=shearbending_strain[arc_index]
	+arc_poisson_ratio_reference[arc_index]
	*bending_strain[arc_index];
    };
};
//
void state_active_arc_class::precalculate_normal_strain_constant()
{
  for(long int arc_index =0; arc_index<number_of_arcs; arc_index++)
    { 
      coefficient_normal_strain_normalizer[arc_index]
	=(1.0+shear[arc_index])*(tangent_bending_modulus[arc_index]-bulk_modulus[arc_index]);
      coefficient_tangent_to_normal_strain[arc_index]
	=-(
	  bulk_modulus[arc_index]*arc_poisson_ratio_reference[arc_index]
	  *(1.0+shear[arc_index])
	  + tangent_bending_modulus[arc_index]
	  * (
	     1.0 - arc_poisson_ratio_reference[arc_index]
	     - arc_poisson_ratio_reference[arc_index]*arc_poisson_ratio_reference[arc_index]
	     - arc_poisson_ratio_reference[arc_index]*shear[arc_index]
	     )
	  );
      coefficient_bending_to_normal_strain[arc_index]
	=-tangent_bending_modulus[arc_index]
	*(1.0-arc_poisson_ratio_reference[arc_index]*arc_poisson_ratio_reference[arc_index]);
      coefficient_shear_to_normal_strain[arc_index]
	=-bulk_modulus[arc_index]*shear[arc_index]*(1.0-arc_poisson_ratio_reference[arc_index]);
    }; 
};
//
  /*
void state_active_arc_class::calculate_shearbending_strain_from_other_strain()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      //     shearbending_strain[arc_index]=ripening[arc_index]/2.0;
      shearbending_strain[arc_index]
	=-normal_strain[arc_index]
	-arc_poisson_ratio_reference[arc_index]
	*(tangent_strain[arc_index]+bending_strain[arc_index]);
    };
};
 */
//
void state_active_arc_class::strain_plot(
					 model_parameters_active_arc_class & model,
					 const long int & time_index,
					 const int & sweep_step,
					 const std::string & format
					 )
{
  //
  if(format=="gnuplot")
    {
      gnuplot_driver gplot("strain_plot");
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
      {
	std::string message;
		  message = io_method.longint_to_string(time_index)
		    + ","
		    + io_method.int_to_string(sweep_step)  ;
		  io_method.standard_output(message);
      };
      gplot.make_plot_file(
			   0,
			   "png",
			   "splot",
			   time_index,
			   sweep_step
			   );
      set_symmetric_matrix2("strain");
      diagonalize2();
      calculate_cross_vectors();
      arc_cross_plot(
		     gplot,
		     time_index,
		     sweep_step,
		     format
		     );
      //
      gnuplot_driver gplot_sub("bending_plot");
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
	  gplot_sub.set_axis(
			     direction_index,
			     min*plot_margin,
			     max*plot_margin,
			     (double)(std::max((int)((double)(max-min)/4.0),1)),
			     direction_label
			     );
	};
      gplot_sub.make_plot_file(
			       0,
			       "png",
			       "splot",
			       time_index,
			       sweep_step
			       );
      set_symmetric_matrix2("bending");
      diagonalize2();
      calculate_cross_vectors();
      arc_cross_plot(
		     gplot_sub,
		     time_index,
		     sweep_step,
		     format
		     );
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
inline void state_active_arc_class::arc_cross_plot(
						   gnuplot_driver & gplot,
						   const long int & time_index,
						   const int & sweep_step,
						   const std::string & format
						   )
{
  //
  double arc_radius;
  for(long int arc_index=0; arc_index<number_of_arcs-1; arc_index++)
    {
      if(
	 arc_curvature_reference[arc_index]
	 *arc_curvature_reference[arc_index]
	 >
	 bit_parameter
	 )
	{
	  arc_radius=1.0/arc_curvature_reference[arc_index];
	}
      else
	{
	  arc_radius=0.0;
	};
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  centers[space_dimension*arc_index+direction_index]
	    =(  coordinates[(arc_index+1)*space_dimension+direction_index]
		+ coordinates[(arc_index+0)*space_dimension+direction_index])/2.0
	    +(1.0-cos(arc_length_reference[arc_index]*arc_curvature_reference[arc_index]/2.0))
	    *arc_radius
	    *normal_vectors[arc_index*space_dimension+direction_index];
	  /*
	      if(std::isnan(centers[space_dimension*arc_index+direction_index]))
		{
		  std::string message;
		  message = io_method.longint_to_string(arc_index);
		    + ","
		    + io_method.double_to_string(symmetric_matrix[arc_index*3+1])
		    + ","
		    + io_method.double_to_string(symmetric_matrix[arc_index*3+2])
		    + ","
		    + io_method.double_to_string(symmetric_matrix[arc_index*3+3]);
		  io_method.standard_output(message);
		  };
	  */
	};
    };
  if(boundary_condition=="close")
    {
      if(
	 arc_curvature_reference[number_of_arcs-1]
	 *arc_curvature_reference[number_of_arcs-1]
	 >
	 bit_parameter
	 )
	{
	  arc_radius=1.0/arc_curvature_reference[number_of_arcs-1];
	}
      else
	{
	  arc_radius=0.0;
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
  gplot.output_vectors_on_loading_file(
				       centers,
				       crossvectors_plus,
				       number_of_arcs,
				       "on",
				       "3",
				       0,
				       time_index,
				       sweep_step,
				       space_dimension,
				       strain_width,
				       "on"
				       );
  gplot.output_vectors_on_loading_file(
				       centers,
				       crossvectors_minus,
				       number_of_arcs,
				       "on",
				       "4",
				       0,
				       time_index,
				       sweep_step,
				       space_dimension,
				       strain_width,
				       "off"
				       );
};
//
void state_active_arc_class::set_symmetric_matrix2(
						   const std::string & matrix_flag
						   )
{
  if(matrix_flag=="strain")
    {
      for(
	  int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  symmetric_matrix[arc_index*3+1]
	    =tangent_strain[arc_index];
	  symmetric_matrix[arc_index*3+2]
	    =shear_strain[arc_index];
	  symmetric_matrix[arc_index*3+3]
	    =normal_strain[arc_index];
	  /*
		{
		  std::string message;
		  message = io_method.longint_to_string(arc_index)
		    + ","
		    + io_method.double_to_string(symmetric_matrix[arc_index*3+1])
		    + ","
		    + io_method.double_to_string(symmetric_matrix[arc_index*3+2])
		    + ","
		    + io_method.double_to_string(symmetric_matrix[arc_index*3+3]);
		  io_method.standard_output(message);
		  };
	  */
	};
    }
  else if(matrix_flag=="bending")
    {
      for(
	  int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  symmetric_matrix[arc_index*3+1]
	    =bending_strain[arc_index];
	  symmetric_matrix[arc_index*3+2]
	    =0.0;
	  symmetric_matrix[arc_index*3+3]
	    =shearbending_strain[arc_index];
	};
    };
  //
};
//
void state_active_arc_class::calculate_cross_vectors()
{
  for(
      int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  crossvectors_plus[arc_index*2+direction_index]
	    = (
	       eigenvectors_plus[arc_index*2+1]
	       * tangent_vectors[arc_index*space_dimension+direction_index]
	       + eigenvectors_plus[arc_index*2+2]
	       * normal_vectors[arc_index*space_dimension+direction_index]
	       )
	    * eigenvalues_plus[arc_index]; 
	  crossvectors_minus[arc_index*2+direction_index]
	    = ( 
	       eigenvectors_minus[arc_index*2+1]
	       * tangent_vectors[arc_index*space_dimension+direction_index]
	       + eigenvectors_minus[arc_index*2+2]
	       * normal_vectors[arc_index*space_dimension+direction_index]
		)
	    * eigenvalues_minus[arc_index]; 
	};
    };
};
//
void state_active_arc_class::diagonalize2(
					  )
{
      for(
	  int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  if(std::abs(symmetric_matrix[arc_index*3+2])<bit_parameter)
	    {
	      if(
		 symmetric_matrix[arc_index*3+1]
		 >
		 symmetric_matrix[arc_index*3+3]
		 )
		{
		  eigenvalues_plus[arc_index]
		    =symmetric_matrix[arc_index*3+1];
		  eigenvalues_minus[arc_index]
		    =symmetric_matrix[arc_index*3+3];
		  eigenvectors_plus[arc_index*2+1]=1.0;
		  eigenvectors_plus[arc_index*2+2]=0.0;
		  eigenvectors_minus[arc_index*2+1]=0.0;
		  eigenvectors_minus[arc_index*2+2]=1.0;
		}
	      else
		{
		  eigenvalues_plus[arc_index]
		    =symmetric_matrix[arc_index*3+3];
		  eigenvalues_minus[arc_index]
		    =symmetric_matrix[arc_index*3+1];
		  eigenvectors_plus[arc_index*2+1]=0.0;
		  eigenvectors_plus[arc_index*2+2]=1.0;
		  eigenvectors_minus[arc_index*2+1]=1.0;
		  eigenvectors_minus[arc_index*2+2]=0.0;
		};
	    }
	  else if( 
		  std::abs(
			   std::pow(
				    symmetric_matrix[arc_index*3+1]
				    -
				    symmetric_matrix[arc_index*3+3],
				    2
				    )
			   +4.0
			   *std::pow(
				     symmetric_matrix[arc_index*3+2],
				     2
				     )
			   ) < bit_parameter
		   ) 
	    {
		  eigenvalues_plus[arc_index]
		    = (symmetric_matrix[arc_index*3+1]
		       + symmetric_matrix[arc_index*3+3])*0.5;
		  eigenvalues_minus[arc_index]
		    = eigenvalues_plus[arc_index];
		  eigenvectors_plus[arc_index*2+1]=1.0;
		  eigenvectors_plus[arc_index*2+2]=0.0;
		  eigenvectors_minus[arc_index*2+1]=0.0;
		  eigenvectors_minus[arc_index*2+2]=1.0;
	    }
	  else if( 
		  std::pow(
			   symmetric_matrix[arc_index*3+1]
			   -
			   symmetric_matrix[arc_index*3+3],
			   2
			   )
		  +4.0
		  *std::pow(
			    symmetric_matrix[arc_index*3+2],
			    2
			    ) < 0.0
		   ) 
	    {
	      eigenvalues_plus[arc_index]
		= 0.0;
	      eigenvalues_minus[arc_index]
		= 0.0;
	      eigenvectors_plus[arc_index*2+1]=1.0;
	      eigenvectors_plus[arc_index*2+2]=0.0;
	      eigenvectors_minus[arc_index*2+1]=0.0;
	      eigenvectors_minus[arc_index*2+2]=1.0;
	    }
	  else
	    {
	      work_vectors[arc_index*2+1]
		= symmetric_matrix[arc_index*3+1]
		- symmetric_matrix[arc_index*3+3];
	      work_vectors[arc_index*2+2]
		=std::sqrt(
			   pow(
			       work_vectors[arc_index*2+1],
			       2
			       )
			   +4.0
			   *std::pow(
				     symmetric_matrix[arc_index*3+2],
				     2
				     )
			   );
		  eigenvalues_plus[arc_index]
		    = (
		       symmetric_matrix[arc_index*3+1]
		       + symmetric_matrix[arc_index*3+3]
		       + work_vectors[arc_index*2+2]
		       )*0.5;
		  // minus component calculation
		  eigenvalues_minus[arc_index]
		    = (
		       symmetric_matrix[arc_index*3+1]
		       + symmetric_matrix[arc_index*3+3]
		       - work_vectors[arc_index*2+2]
		       )*0.5;
		  // plus component calculation
		  eigenvectors_plus[arc_index*2+1]
		    = 0.5
		    *(
		       work_vectors[arc_index*2+1]
		       + work_vectors[arc_index*2+2]
		      )/symmetric_matrix[arc_index*3+2];
		  eigenvectors_minus[arc_index*2+1]
		    = 0.5
		    *(
		       work_vectors[arc_index*2+1]
		       - work_vectors[arc_index*2+2]
		      )/symmetric_matrix[arc_index*3+2];
		  // plus eigenvector normalizing 
		  work_vectors[arc_index*2+1]
		    = std::sqrt(
				std::pow(eigenvectors_plus[arc_index*2+1],2)
				+ 1.0
				);
		   eigenvectors_plus[arc_index*2+1]
		    = eigenvectors_plus[arc_index*2+1]
		     / work_vectors[arc_index*2+1];
		   eigenvectors_plus[arc_index*2+2]
		    = 1.0
		     / work_vectors[arc_index*2+1];
		   // minus eigenvector normalizing
		  work_vectors[arc_index*2+2]
		    = std::sqrt(
				std::pow(eigenvectors_minus[arc_index*2+1],2)
				+ 1.0
				);
		   eigenvectors_minus[arc_index*2+1]
		    = eigenvectors_minus[arc_index*2+1]
		     / work_vectors[arc_index*2+2];
		   eigenvectors_minus[arc_index*2+2]
		    = 1.0
		     / work_vectors[arc_index*2+2];
	    };
	  /*
	  {
	    std::string message;
	    message = io_method.int_to_string(arc_index)
	    + "/"
	      + io_method.int_to_string(number_of_arcs)
	    + ","
	      + io_method.double_to_string(eigenvectors_minus[arc_index*2+1])
	      + ","
	      + io_method.double_to_string(eigenvectors_minus[arc_index*2+2])
	      + ","
	      + io_method.double_to_string(eigenvectors_plus[arc_index*2+1])
	      + ","
	      + io_method.double_to_string(eigenvectors_plus[arc_index*2+2]);
	    io_method.standard_output(message);
	  };
	  */
	};
};
//
void state_active_arc_class::output_strain(
					   const long int & time_index,
					   const int & sweep_step
					   )
{
  state_debug_io debug_strain(
			      "strain_output",
			      0,
			      number_of_arcs,
			      10
			      );
  debug_strain.set_debug_data(
			     "tangent",
			     tangent_strain,
			     0,
			     1
			     );
  debug_strain.set_debug_data(
			     "normal",
			     normal_strain,
			     1,
			     1
			     );
  debug_strain.set_debug_data(
			     "shear",
			     shear_strain,
			     2,
			     1
			     );
  debug_strain.set_debug_data(
			     "bending",
			     bending_strain,
			     3,
			     1
			     );
  debug_strain.set_debug_data(
			     "shearbending",
			     shearbending_strain,
			     4,
			     1
			     );
  debug_strain.set_debug_data(
			     "effective tangent strain",
			     effective_tangent_strain,
			     5,
			     1
			     );
  debug_strain.set_debug_data(
			     "effective normal strain",
			     effective_normal_strain,
			     6,
			     1
			     );
  debug_strain.set_debug_data(
			     "effective shear strain",
			     effective_shear_strain,
			     7,
			     1
			     );
  debug_strain.set_debug_data(
			     "effective bending strain",
			     effective_bending_strain,
			     8,
			     1
			     );
  debug_strain.set_debug_data(
			     "effective shearbending strain",
			     effective_shearbending_strain,
			     9,
			     1
			     );
  debug_strain.output_debug_data(
				0,
				time_index,
				sweep_step
				);
};

