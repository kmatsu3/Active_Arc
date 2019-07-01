#ifndef __MODEL_PARAMETERS_ACTIVE_ARC__
#define __MODEL_PARAMETERS_ACTIVE_ARC__
#include "active_arc_io.hpp"
#include <string>
#include <vector>
class model_parameters_active_arc_class
{
  /*
    Definitions of elastic model parameters
   */
private:int space_dimension;
private:long int number_of_arcs;
private:double arc_length;
private:double arc_width;
private:double arc_curvature;
private:double poisson_ratio;
private:double young_modulus;
private:double angle_penalty_energy;
private:bool bending_energy_on;
private:bool coupling_energy_on;
private:double uniform_growth_ratio;
private:double normal_gradient_growth;
private:double tangent_gradient_growth;
private:std::string boundary_condition;
private:std::string initial_state;
private:double md_time_difference;
private:double md_termination_number_of_iteration;
private:bool md_on;
private:bool cg_on;
private:long int shape_plot_frequency; 
private:long int shape_plot_max; 
private:long int shape_plot_base; 
  //private:const static double PI = 3.141592653589793238;
private: double PI;
  /*
    Definitians of inner material parameter
   */
private:double inner_radius;
private:double inner_control_volume;
private:double inner_material_Young_modulus;
private:double inner_material_Poisson_ratio;
private:double inner_height;
private:double inner_width;
private:std::string inner_type;
  /*
    Definitians of constraints
   */
private:bool shear_constraint_on;
private:double shear_constraint_modulus;
private:double shear_constraint_power;
  /*
    Definitians of initial state
   */
private:double initial_frequency;
private:double initial_amplitude;
  /*
    Definitons of local values
   */
private:io_active_arc io_method;
// system_dimensions[space_dimension] should be corrected for any space dimension.
  /*
    Function declaration
   */
public: void input_model();
private: int show_model_parameters();
private: int input_model_parameters();
private: void show_inner_material_parameters();
private: void input_inner_material_parameters();
private: void input_md_parameter();
private: void show_md_parameters();
private: void input_cg_parameter();
private: void show_cg_parameters();
private: void input_initialstate_parameter();
private: void show_initialstate_parameters();
private: void input_constraint_parameter();
private: void show_constraint_parameters();
private: void input_io_parameter();
private: void show_io_parameters();
public: long int get_number_of_arcs ();
public: void change_number_of_arcs( 
				   long int & input_number_of_arcs
				    );
public: int get_space_dimension();
public: double get_arc_length();
public: double get_arc_width();
public: double get_arc_curvature();
public: double get_poisson_ratio();
public: double get_young_modulus();
public: double get_angle_penalty_energy();
public: bool check_bending_energy_on();
public: bool check_coupling_energy_on();
public: double get_uniform_growth_ratio();
public: double get_normal_gradient_growth();
public: double get_tangent_gradient_growth();
public: std::string get_boundary_condition();
public: std::string get_initial_state();
public: double get_control_volume();
public: double get_inner_material_Young_modulus();
public: double get_inner_material_Poisson_ratio();
public: double get_inner_height();
public: double get_inner_width();
public: std::string get_inner_type();
public: bool check_shear_constraint_on();
public: double get_shear_constraint_modulus();
public: double get_shear_constraint_power();
public: double get_md_time_difference();
public: double get_md_termination_number_of_iteration();
public: double get_initial_frequency();
public: double get_initial_amplitude();
public: bool check_md_on();
public: bool check_cg_on();
public: long int get_shape_plot_frequency();
public: long int get_shape_plot_max();
public: long int get_shape_plot_base();
  //private:
  //  void check_model_parameter(std::string model_parameter);
  /*
    Constructer declaration
  */
public:
  model_parameters_active_arc_class();
  /*
    Local variables declaration
  */
};
#endif // __MODEL_PARAMETERS_ACTIVE_ARC__
