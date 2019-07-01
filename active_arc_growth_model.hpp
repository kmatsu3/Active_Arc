#ifndef __GROWTH_MODEL_ACTIVE_ARC__
#define __GROWTH_MODEL_ACTIVE_ARC__
#include "active_arc_state.hpp"
#include "active_arc_io.hpp"
#include "active_arc_reconstruction.hpp"
#include <vector>
#include <memory>
#include <cmath>
//
class schedule
{
private: io_active_arc io_method;
private: double PI;
  //
public: std::string categoly;
public: std::string parameter;
  //  public: long int conponent;
public: std::string control_type;
public: std::string growth_type;
public: double start_value;
public: double incriment;
public: double wave_number;
public: double amplitude;
public: bool adaptation_on;
public: double adaptation_amplitude;
public: double adaptation_threshold;
public: double adaptation_width;
public: std::string adaptation_source;
public: double value;
  // for each segments
public: std::vector<double> values;
public: std::vector<double> incriments;
public: std::vector<double> adaptations;
  //
public: void initialize_values(
			       std::unique_ptr<state_active_arc_class> & state
			       );
public: void replace_values(
			    const std::vector<double> & input_values
			    );
public: void replace_incriments(
				const std::vector<double> & input_incriments
				);
public: void replace_adaptations( const long int & number_of_arcs);
public: void get_values(
			std ::vector<double> & output_values
			);
public: void get_incriments(
			    std ::vector<double> & output_incriments
			    );
public: void output_growth();
public: schedule();
};
//
class growth_model
{
private: double bit_parameter;
private: io_active_arc io_method;
  // operator
public: void incriment_growth(
			      schedule & input_schedule
			      );
  //
public: void inverse_growth(
			    schedule & input_schedule
			    );
  // 
public: void calculate_adaptation(
				  schedule & input_schedule,
				  const std::vector<double> & stress,
				  const std::vector<double> & volumes
				  );
public: void output_adaptation(
			       schedule & input_schedule,
			       const std::vector<double> & stress,
			       const std::vector<double> & volumes
			       );
  // constructor
  growth_model();
};
#endif // __GROWTH_MODEL_ACTIVE_ARC__
