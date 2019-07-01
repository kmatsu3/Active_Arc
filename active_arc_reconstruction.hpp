#ifndef __RECONSTRUCTOR_ACTIVE_ARC__
#define __RECONSTRUCTOR_ACTIVE_ARC__
#include "active_arc_io.hpp"
#include "active_arc_gnuplot.hpp"
#include "active_arc_definition.hpp"
#include "active_arc_interface.hpp"
#include <vector>
 /*
Class
 */
class model_reconstructor
{
private: io_active_arc io_method;
private: int space_dimension;
private: long int old_number_of_arcs;
private: long int old_number_of_joints;
private: std::string inner_type;
private: double PI;
public: long int new_number_of_arcs;
public: long int new_number_of_joints;
  //public:reference_io old_reference;
public:reference_io new_reference;
  //public:configuration_io old_configuration;
public:configuration_io new_configuration;
public:std::vector<bool> old_division_on;
  // operator
  //public: void reconstruction();
  //
public: long int get_number_of_old_arcs();
  //
public: void set_division_on(const std::vector<bool> & input_division_on);
  //
public: void get_division_on(std::vector<bool> & output_division_on);
  //
private: void reference_reconstruction(
				       reference_io & old_reference,
				       std::vector<bool> & division_on
				       );
private: void configuration_reconstruction(
					   configuration_io & old_configuration,
					   std::vector<bool> & division_on
					   );
  /*
public: void old_construction(
			      long int & input_number_of_arcs,
			      long int & input_number_of_joints,
			      int & space_dimension,
			      std::vector<bool> & input_division_on
			      );
  */
public: void new_construction(
			      long int & input_number_of_arcs,
			      long int & input_number_of_joints,
			      int & space_dimension,
			      std::vector<bool> & input_division_on,
			      reference_io & old_reference,
			      configuration_io & old_configuration
			      );
public: void get_construction(
			      reference_io & reference,
			      configuration_io & configuration
			      );
  // constructor
public: model_reconstructor(
			    model_parameters_active_arc_class & model
			    );
};
#endif //__RECONSTRUCTOR_ACTIVE_ARC__
