#ifndef __STATE_ACTIVE_ARC__
#define __STATE_ACTIVE_ARC__
#include "active_arc_io.hpp"
#include "active_arc_gnuplot.hpp"
#include "active_arc_definition.hpp"
#include "active_arc_interface.hpp"
#include "active_arc_reconstruction.hpp"
#include "active_arc_observable.hpp"
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <boost/math/special_functions/sinc.hpp>
#include "stdafx.h"
#include "interpolation.h"
#include "optimization.h"
class state_active_arc_class
{
  /*
    Definitions of model parameters
   */
private:int space_dimension;
private:long int number_of_arcs;
private:long int number_of_joints;
private:int number_of_joint_components;
private:int number_of_internal_components;
private:long int internal_head_pointer;
private:long int constraint_head_pointer;
private:double arc_length;
private:double arc_width;
private:double arc_curvature;
private:std::string boundary_condition;
private:double poisson_ratio;
private:double young_modulus;
private:double angle_penalty_energy;
private:double PI;
private:double bit_parameter;
private:double cg_upper_margin;
private:double cg_lower_margin;
  /* 
     Definitions of initial state
   */
private:double initial_frequency;
private:double initial_amplitude;
  /* 
     Definitions of local arc values
   */
private:std::vector<double> arc_length_reference;
private:std::vector<double> arc_width_reference;
private:std::vector<double> arc_curvature_reference;
private:std::vector<double> arc_young_modulus_reference;
private:std::vector<double> arc_poisson_ratio_reference;
private:int number_of_joints_for_arc; 
  /*
    Definitons of local joints values
   */
private:std::vector<double> coordinates;
private:std::vector<double> angles;
private:std::vector<double> thickness;
  /*
    Definitions of local coordinate
   */
private:std::vector<double> end_to_end_length;
private:std::vector<double> relative_coordinates;
private:std::vector<double> tangent_vectors;
private:std::vector<double> normal_vectors;
private:std::vector<double> center_to_center_length;
private:std::vector<double> center_of_coordinates;
private:std::vector<double> center_coordinates;
private:std::vector<double> center_tangent_vectors;
private:std::vector<double> center_normal_vectors;
  /*
    Definitions of local displacement fields
   */
private:std::vector<double> elongation;
private:std::vector<double> ripening;
private:std::vector<double> shear;
private:std::vector<double> curvature_releasing;
private:std::vector<double> normal_modulation;
  /*
    Definitions of data for calculating local displacement fields
   */
private:std::vector<double> normal_angles;
private:std::vector<double> reference_angles;
private:std::vector<double> average_angles;
private:std::vector<double> difference_angles;
private:std::vector<double> local_curvature;
private:std::vector<double> shape_coordinates;
  /*
    Definitions of strain tensor
   */
private:std::vector<double> tangent_strain;
private:std::vector<double> normal_strain;
private:std::vector<double> shear_strain;
private:std::vector<double> bending_strain;
private:std::vector<double> shearbending_strain;
  /*
    Definitions of effective strain tensor
   */
private:std::vector<double> effective_tangent_strain;
private:std::vector<double> effective_normal_strain;
private:std::vector<double> effective_shear_strain;
private:std::vector<double> effective_bending_strain;
private:std::vector<double> effective_shearbending_strain;
  /*
    Definition of modulus
   */
private:std::vector<double> bulk_modulus;
private:std::vector<double> shear_modulus;
private:std::vector<double> normal_bending_modulus;
private:std::vector<double> tangent_bending_modulus;
private:std::vector<double> tangent_bending_modulus_first_power;
private:std::vector<double> tangent_bending_modulus_second_power;
private:std::vector<double> inner_bulk_modulus;
private:bool bending_modulus_switch;
private:bool coupling_modulus_switch;
  /*
    Definition of stress
   */
private:std::vector<double> tangent_stress;
private:std::vector<double> normal_stress;
private:std::vector<double> shear_stress;
private:std::vector<double> bending_stress;
private:std::vector<double> shearbending_stress;
public: stress_io stress;
  /*
    Definition of effective_stress
   */
private:std::vector<double> effective_elongation_stress;
private:std::vector<double> effective_ripening_stress;
private:std::vector<double> effective_shear_stress;
  /*
    Definition of constraint_modulus
   */
private:bool shear_constraint_switch;
private:double shear_constraint_modulus;
private:double shear_constraint_power;
  /*
    Definition of constraint_energy
   */
private:double constraint_shear_energy;
  /*
    Definition of constraint_stress
   */
private:std::vector<double> shear_constraint_stress;
private:std::vector<double> shear_cos_minus_second_power;
  /*
    Definitions of energy
   */
private:std::vector<double> arc_elastic_energy;
private:double total_elastic_energy;
private:double total_energy;
  /*
    Definitions of gradients
   */
private:std::vector<double> Normal_force;
private:std::vector<double> Tangent_force;
private:std::vector<double> Symmetric_Torque;
private:std::vector<double> Antisymmetric_Torque;
private:std::vector<double> Normal_Pullingforce;
private:std::vector<double> Normal_Nonlinearforce;
private:std::vector<double> elastic_force;
  //
private:std::vector<double> variable;
private:std::vector<double> gradient;
private:double reference_box_max;
private:double reference_box_min;
  // for output of gradients
private:std::vector<double> force_for_arc_minus;
private:std::vector<double> force_for_arc_plus;
private:std::vector<double> force_for_arc_plot;
private:std::vector<double> moment_for_arc_minus;
private:std::vector<double> moment_for_arc_plus;
private:std::vector<double> moment_for_arc_plot;
private:std::vector<double> edge_base_point_plus;
private:std::vector<double> edge_base_point_minus;
private:std::vector<double> edge_head_point_plus;
private:std::vector<double> edge_head_point_minus;
  //
  /*
    interior system
   */
  //for inner parameters
private:std::vector<double> inner_volume;
private:std::vector<double> inner_control_volume;
private:std::vector<double> inner_material_Young_modulus;
private:std::vector<double> inner_material_Poisson_ratio;
private:double inner_volume_energy;
private:std::vector<double> inner_volume_force;
private:std::vector<double> inner_volume_force_for_plot;
  // for inner elasticity
private:std::string inner_type;
private:std::vector<double> inner_height;
private:std::vector<double> inner_width;
  // for elastic variables
  // deformation
private:std::vector<double> inner_elongation;
private:std::vector<double> inner_ripening;
private:std::vector<double> inner_shear;
  // stress
private:std::vector<double> inner_elongation_stress;
private:std::vector<double> inner_ripening_stress;
private:std::vector<double> inner_shear_stress;
public: inner_stress_io inner_stress;
  //
private:double inner_elastic_energy;
private:std::vector<double> inner_elastic_force;
private:std::vector<double> center_elastic_force;
private:std::vector<double> inner_elongation_force;
private:std::vector<double> inner_ripening_force;
private:std::vector<double> inner_normal_torque;
private:std::vector<double> inner_centertangent_torque;
private:std::vector<double> inner_force_for_plot;
private:std::vector<double> center_force_for_plot;
private:std::vector<double> stress_for_plot;
private:std::vector<double> action_point_for_plot;
  //
  /*
    MD relaxation
   */
private:long int termination_number_of_iteration;
private:double time_difference;
  /*
    Plot information
   */
private:long int shape_plot_max;
private:long int shape_plot_frequency;
private:long int shape_plot_base;
  /*
    reconstruction of state
   */
private:std::vector<bool> division_on; 
  /* 
     temporary memory for calculation of local displacements
   */
private:std::vector<double> work_vectors;
  /*
    Private Instance 
   */
private:std::string message;
private:io_active_arc io_method;
private:std::string configuration_plot;
private:double plot_margin;
private:std::vector<double> arrows;
private:std::vector<double> centers;
private:long int time_index;
private:int sweep_step;
  /*
    Operations
   */
public:void initialize(
		       model_parameters_active_arc_class & model,
		       const std::string & criterion
		       );
public: long int get_number_of_arcs();
private:void initialize_configuration(
				      const std::string & criterion
				      );
private:void angle_correction();
private:double construction_base_check(
				       const std::string & base
				       );
private:void curvature_based_construction();
private:void polar_compressed_construction(
					   const std::string & base,
					   const double & perturbation,
					   const double & frequency
					   );
private:void elongation_construction(
				     const std::string & base,
				     const double & perturbation
				     );
private:void ripening_construction(
				   const std::string & base,
				   const double & perturbation
				   );
private:void shear_construction(
				const std::string & base,
				const double & perturbation
				);
private:void bending_construction(
				  const std::string & base,
				  const double & perturbation
				  );
private:void wavy_construction(
			       const std::string & base,
			       const double & perturbation,
			       const double & frequency
			       );
private:void length_based_construction();
private:void output_configuration(
				  const long int & time_index,
				  const int & sweep_step
				  );
  // in reference file
public:void initialize_reference_parameters(
					    const std::string & mode_flag
					    );
private:void output_reference(
			      const long int & time_index,
			      const int & sweep_step
			      );
  // initialize reference
public:void initialize_reference(
				 model_parameters_active_arc_class & model
				 );
  // reference_plot
private:void push_reference_box();
public:void reference_plot(
			   const long int & time_index,
			   const int & sweep_step,
			   const std::string & format
			   );
private:inline void reference_arc_plot(
				       gnuplot_driver & gplot,
				       const long int & time_index,
				       const int & sweep_step,
				       const std::string & format,
				       const long int & arc_index
				       );
private:std::vector<double> reference_coordinates;
private:std::vector<double> reference_line_vectors;
private:int number_of_reference_coordinates;
  // calculation of geometric variables & plotter
  // in geometry file
public:void initialize_local_geometry();
public:void local_geometry();
public:void initialize_center_of_coordinates();
public:void translate_center_to_zero();
public:void local_center_geometry();
public:void local_geometry_plot(
				const model_parameters_active_arc_class & model,
				const long int & time_index,
				const int & sweep_step,
				const std::string & format
				);
public:void local_center_geometry_plot(
				       const model_parameters_active_arc_class & model,
				       const long int & time_index,
				       const int & sweep_step,
				       const std::string & format
				       );
  // calculation of displacement
  // in displacement file
public:void initialize_displacements();
public:void calculate_displacement(const std::string & flag);
  //
private:void calculate_normal_angles();
private:void calculate_reference_angles();
private:void calculate_average_angles();
private:void calculate_difference_angles();
private:void calculate_local_curvature();
  //
private:void calculate_curvature_releasing();
private:void calculate_elongation();
private:void calculate_ripening();
private:void calculate_shear();
private:void calculate_normal_modulation();
private:void zero_initialization_normal_modulation();
public:void shape_plot(
		       const long int & time_index,
		       const int & sweep_step,
		       const std::string & format
		       );
private:inline void arc_plot(
			     gnuplot_driver & gplot,
			     const long int & time_index,
			     const int & sweep_step,
			     const std::string & format,
			     const long int & arc_index
			     );
  //
public:void copy_variable_to_joint();
public:void copy_variable_to_arc();
public:void variable_initialization();
  //
private:void output_displacements(
				  const long int & time_index,
				  const int & sweep_step
				  );
private:void output_arc_angles(
			       const long int & time_index,
			       const int & sweep_step
			       );
private:void push_displacements_for_output();
  //
public:void initialize_strain();
public:void calculate_strain();
private:void calculate_tangent_strain();
private:void calculate_normal_strain();
private:void calculate_shear_strain();
private:void calculate_bending_strain();
private:void calculate_shearbending_strain();
  //
public:void calculate_effective_strain();
  //
  /*
public:void calculate_shearbending_strain_from_other_strain();
public:void calculate_normal_strain_from_other_strain();
  */
public:void precalculate_normal_strain_constant();
public:void output_strain(
			  const long int & time_index,
			  const int & sweep_step
			  );
  // work memory for precalculation
private:std::vector<double> coefficient_tangent_to_normal_strain;
private:std::vector<double> coefficient_bending_to_normal_strain;
private:std::vector<double> coefficient_shear_to_normal_strain;
private:std::vector<double> coefficient_normal_strain_normalizer;
  //
public:void initialize_modulus();
public:void calculate_modulus();
private:void calculate_bulk_modulus();
private:void calculate_shear_modulus();
private:void calculate_normal_bending_modulus();
private:void calculate_tangent_bending_modulus();
private:void calculate_tangent_bending_modulus_first_power();
private:void calculate_tangent_bending_modulus_second_power();
private:void calculate_inner_control_volume();
private:void calculate_inner_bulk_modulus();
public:void output_modulus();
  /*
    strain plot
   */
private:double strain_width;
public:void strain_plot(
			model_parameters_active_arc_class & model,
			const long int & time_index,
			const int & sweep_step,
			const std::string & format
			);
private: inline void arc_cross_plot(
				    gnuplot_driver & gplot,
				    const long int & time_index,
				    const int & sweep_step,
				    const std::string & format
				    );
  /*
    Lapack diagonalization work memory for plot
   */
public:void initialize_diagonlizar();
public:void set_symmetric_matrix2(
				  const std::string & matrix_flag
				  );
public:void diagonalize2();
public:void calculate_cross_vectors();
private:std::vector<double> eigenvalues_plus;
private:std::vector<double> eigenvalues_minus;
private:std::vector<double> eigenvectors_plus;
private:std::vector<double> eigenvectors_minus;
private:std::vector<double> crossvectors_plus;
private:std::vector<double> crossvectors_minus;
private:std::vector<double> symmetric_matrix;
  //
private:int number_of_displacements;
private:int number_of_displacements_supplements;
private:std::vector<std::string> labels_for_output;
private:std::vector<double> observables_for_output;
  //
private:inline double csinc(const double & input_double);
  //
public:void calculate_curvature_releasing_from_tangent();
  //
public:void update(
		   model_parameters_active_arc_class & model
		   );
  //
public:bool iteration_check(
			    const long int & iteration_index,
			    const long int & iteration_max,
			    const long int & times,
			    const long int & iteration_base,
			    long int & plot_index
			    );
  //
public:void plot(
		 const long int & time_index,
		 const int & sweep_step,
		 const std::string & format
		 );
  /*
    energy calculation
   */
public:void calculate_elastic_energy();
private:inline double calculate_strech_energy(const long int & arc_index);
private:inline double calculate_bending_energy(const long int & arc_index); 
private:inline double calculate_coupling_energy(const long int & arc_index); 
private:double energy_penalty();
  //
  //
  /*
    stress calculation
   */
public:void initialize_stress();
public:void calculate_stress();
private:void calculate_tangent_stress();
private:void calculate_normal_stress();
private:void calculate_shear_stress();
private:void calculate_bending_stress();
private:void calculate_shearbending_stress();
private:void calculate_effective_stress();
public:void output_stress(
			  const long int & time_index,
			  const int & sweep_step
			  );
public:void local_stress_plot(
			      const model_parameters_active_arc_class & model,
			      const long int & time_index,
			      const int & sweep_step,
			      const std::string & format
			      );
public:void stress_get();
  /*
    constraint calculation
   */
public:void initialize_constraint_stress();
public:void calculate_shear_cos_minus_second_power();
public:void calculate_constraint();
public:void calculate_constraint_energy();
public:void calculate_constraint_stress();
public:void calculate_constraint_shear_stress();
public:void calculate_constraint_shear_energy();
  /*
    strain gradient calculation
   */
  /*
public:void calculate_matrix_for_stress_transformation();
public:void calculate_matrix_from_stress_to_force()
  */
private: inline double dsinc(const double & data);
public:void initialize_elastic_force();
public:void calculate_preforce();
public:void calculate_elastic_force();
public:void calculate_force_for_arcs();
public:void calculate_moment_for_arcs();
public:void show_gradient(
			  const long int & time_index
			  );
public:void local_force_plot(
			     const model_parameters_active_arc_class & model,
			     const long int & time_index,
			     const int & sweep_step,
			     const std::string & format
			     );
public:void local_moment_plot(
			      const model_parameters_active_arc_class & model,
			      const long int & time_index,
			      const int & sweep_step,
			      const std::string & format
			      );
public:void output_forces(
			  const long int & time_index,
			  const int & sweep_step
			  );
public:void output_moments(
			   const long int & time_index,
			   const int & sweep_step
			   );
public:void output_preforces(
			     const long int & time_index,
			     const int & sweep_step
			     );
public:void local_elastic_force_plot_initialization();
public:void calculate_elastic_gradient();
public:void cg_relaxation(
			  model_parameters_active_arc_class & model,
			  const int & sweep_step
			  );
private:void driver_cg_alglib(
			      model_parameters_active_arc_class & model,
			      const alglib::real_1d_array & solution,
			      double & optimization_gradient,
			      alglib::real_1d_array & function_gradient,
			      void *temporary_ptr,
			      long int & time_index,
			      const int & sweep_step
			      );
private:void summarize_energy_gradient(
				       double & optimization_function,
				       alglib::real_1d_array & function_gradient
				       );
private:void finalization(
			  const int & sweep_step,
			  const std::string & label,
			  const model_parameters_active_arc_class & model
			  );
private:void pull_solution(
			   const alglib::real_1d_array & solution
			   );
private:void generate_initializer(
				  std::string & initializer_real_1d_array,
				  const std::string & array_type
				  );
  /*
    inner volume
   */
private:void initialize_inner_volume_force(
					   model_parameters_active_arc_class & model
					   );
private:void calculate_innervolumic();
private:void calculate_inner_volume();
private:void calculate_inner_volume_energy();
private:void calculate_inner_volume_force();
private:void add_inner_volume_force();
private:void local_inner_force_plot(
				    const model_parameters_active_arc_class & model,
				    const long int & time_index,
				    const int & sweep_step,
				    const std::string & format
				    );
private:void output_inner_values(
				 const long int & time_index,
				 const int & sweep_step
				 );
  /*
    inner elasticity
   */
private:void initialize_inner_elasticity(
					 model_parameters_active_arc_class & model
					 );
private:void inner_elasticity_plot(
				   const model_parameters_active_arc_class & model,
				   const long int & time_index,
				   const int & sweep_step,
				   const std::string & format
				   );
private:void initialize_inner_geometry(
				       model_parameters_active_arc_class & model
				       );
private:void initialize_inner_deformation();
private:void initialize_inner_stress();
private:void initialize_inner_force();
private:void calculate_inner_deformation();
  //
public:void inner_stress_get(); 
  //
private:void output_inner_deformation(
				      const long int & time_index,
				      const int & sweep_step
				      );
private:void calculate_inner_stress();
private:void output_inner_stress(
				 const long int & time_index,
				 const int & sweep_step
				 );
private:void calculate_inner_preforce();
private:void output_inner_preforce(
				   const long int & time_index,
				   const int & sweep_step
				   );
private:void calculate_inner_energy();
private:void calculate_inner_elastic_force();
private:void calculate_inner_elasticity();
private:void calculate_inner();
private:void output_inner_force(
				const long int & time_index,
				const int & sweep_step
				);
private:void output_inner_force_plot(
				     const model_parameters_active_arc_class & model,
				     const long int & time_index,
				     const int & sweep_step,
				     const std::string & format
				     );
  /*
    MD calculation
   */
public:void md_relaxation(
			  model_parameters_active_arc_class & model,
			  const int & sweep_index
			  );
private:void md_initialize(
			   model_parameters_active_arc_class & model
			   );
private:void md_main();
private:void md_update();
  /*
    work memory for strain gradient
   */
private:std::vector<double> coefficient_tangent_stress_to_tangent_force;
private:std::vector<double> coefficient_bending_stress_to_tangent_force;
  /*
    Interface class instance
  */
public:reference_io reference;
public:void reference_set();
public:void reference_get();
public:configuration_io configuration;
public:void configuration_set();
public:void configuration_get();
  /*
    output observables
   */
public:void initialize_observables(
				   control_observables & observable_data
				   );
public:void calculate_observables(
				  control_observables & observable_data
				  );
public:void calculate_max_stress(
				 control_observables & observable_data
				 );
public:void calculate_cortical_length(
				      control_observables & observable_data
				      );
public:void output_observables(
			       control_observables & observable_data,
			       const long int & number
			       );
  /*
    Debug IO Class
   */
class state_debug_io
{
  /*
    Private instance for debug
  */
private:io_active_arc io_method;
private:std::string file_header;
private:int number_of_comments;
private:long int number_of_debug_data;
private:int number_of_debug_data_components;
private:std::vector<std::string> comments;
private:std::vector<std::string> labels_for_debug_data;
private:std::vector<double> debug_data_for_output;
  /*
    output
   */
public:void output_debug_data(
			      const long int & plane_index,
			      const long int & time_index,
			      const int & sweep_step
			      );
  /*
    setter
   */
public:void set_debug_data(
			   const std::string & label,
			   const std::vector<double> & input_data,
			   const int & label_header,
			   const int & number_of_components
			   );
  //
public:void set_debug_comment(
			      const std::string & comment,
			      const int & comment_index
			      );
  /*
  constructor
  */
public:state_debug_io(
		      const std::string & file_header,
		      const int & number_of_comments,
		      const long int & number_of_debug_data,
		      const int & number_of_debug_data_components
		      );
  };
  /*
    Constructer declaration
  */
  /*
    Constructor inline functions
   */
private:void set_model_parameters_in_construction(
						  model_parameters_active_arc_class & model
						  );
private:void set_constants();
private:void calculate_dimensions();
  /*
    redivition
   */
public:void reconstructor_initialize();
public:bool reconstruction_check( model_reconstructor & reconstructor);
public:void reconstruction(
			   model_reconstructor & reconstructor,
			   const long int & sweep_step
			   );
  //
public:void get_division_on(
			    std::vector<bool> & output_division_on
			    );
  //
private:bool reconstruction_on;
public: bool nan_check(const long long int & error_code);
  /*
    Main of constractor
  */
public:state_active_arc_class(
			      model_parameters_active_arc_class & model
			      );
  /*
    Local variables declaration
  */
};
#endif // __STATE_ACTIVE_ARC__
