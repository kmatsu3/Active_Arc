#ifndef __SCHEDULE_ACTIVE_ARC__
#define __SCHEDULE_ACTIVE_ARC__
#include "active_arc_definition.hpp"
#include "active_arc_state.hpp"
#include "active_arc_reconstruction.hpp"
#include "active_arc_growth_model.hpp"
#include "active_arc_interface.hpp"
#include <memory>
#include <vector>
class schedule_active_arc
{
private: int number_of_scheduled_parameters;
private: long int number_of_schedule_termination;
private: growth_model growth;
private: std::vector<bool> division_on;
private: std::vector<double> work_values;
private: std::vector<double> volumes;
private: std::vector<double> inner_volumes;
private: growth_io growth_interface;
private: double PI;
private: std::vector<schedule> schedules;
private: std::vector<double> growth_ratios;
private: std::vector<double> adaptation_ratios;
private: io_active_arc io_method;
  //
public: void input_schedule(
			    std::unique_ptr<state_active_arc_class> & state
			    );
public: void output_schedule();
public: void update(
		    std::unique_ptr<state_active_arc_class> & state
		    );
public: void stress_adaptation(
			       std::unique_ptr<state_active_arc_class> & state
			       );
public: void volume_calculation(
				std::vector<double> & length,
				std::vector<double> & width
				);
public: void inner_volume_calculation(
				      std::vector<double> & height,
				      std::vector<double> & width
				      );
private: void replace(
		      schedule & input_schedule,
		      std::vector<double> & output_values
		      );
public: void reference_replace(
			       std::unique_ptr<state_active_arc_class> & state
			       );
private:void check_consistency_values_and_incriments(
						     const schedule & input_schedule
						     );
private: void reconstruction_schedule_work(
					   const long int & number_of_new_arcs
					   );
private:void reconstruction_schedule_length(
					    const long int & number_of_new_arcs,
					    schedule & input_schedule,
					    const double & value_division,
					    const double & incriments_division
					    );
public: void reconstruction(
			    model_parameters_active_arc_class & model,
			    model_reconstructor & reconstructor
			    );
private: void set_division_on(
			      model_reconstructor & reconstructor
			      );
public: void set_schedule(
			  std::unique_ptr<state_active_arc_class> & state,
			  const long int & iterationo_index
			  );
private: double calculate_base(
			       const schedule & input_schedule,
			       const long int & iteration_index
			       );
private: void set_growth_ratios(
				std:: vector<double> & vector_values,
				std::unique_ptr<state_active_arc_class> & state,
				const schedule & input_schedule,
				const long int & iteration_index
				);
private: void calculate_growth_ratios(
				      const std::vector<double> & vector_values,
				      const schedule & input_schedule,
				      const long int & iteration_index
				      );
  //
private: void consistent_growth_ratios(
				       std:: vector<double> & vector_values,
				       std::unique_ptr<state_active_arc_class> & state,
				       const schedule & input_schedule,
				       const long int & iteration_index
				       );
  //
private: void calculate_adaptation_ratios(
					  const std::vector<double> & adaptation,
					  const schedule & input_schedule
					  );
private: void set_values(
			 std::vector<double> & vector_values,
			 std::unique_ptr<state_active_arc_class> & state,
			 const std::vector<double> & adaptation,
			 schedule & input_schedule,
			 const long int & iteration_index
			 );
private: void constant_growth(
			      std::vector<double> & vector_values,
			      std::unique_ptr<state_active_arc_class> & state,
			      schedule & input_schedule,
			      const long int & iteration_index
			      );
  //
private:void dynamic_growth(
			    std::vector<double> & vector_values,
			    std::unique_ptr<state_active_arc_class> & state,
			    schedule & input_schedule,
			    const long int & iteration_index
			    );
  //
private: void output_parameter_change(
				      const bool & previous_on,
				      const std:: vector<double> & vector_values,
				      const schedule & input_schedule
				      );
public: bool check_termination(const int & iteration_index);
  //
private: inline double strain_ratio(
				    const double & denominator,
				    const double & diviser
				    );
  //
private: void set_value(
			double & vector_values,
			const schedule & input_schedule,
			const long int & iteration_index
			);
  //
private:std::string set_structure_type_item(
					    const int & parameter_index,
					    const std::string & child
					    );
  //
  /* constructor*/
public:schedule_active_arc();
  //
};
#endif // __STATE_ACTIVE_ARC__
