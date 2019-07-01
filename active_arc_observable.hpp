#ifndef __OBSERVABLE_ACTIVE_ARC__
#define __OBSERVABLE_ACTIVE_ARC__
#include "active_arc_io.hpp"
class active_arc_observable{
public: std::string observable;
public: std::vector<double> values;
private: io_active_arc io_method;
public: void output_observable(const long int & number);
public: void check_observable();
public: void initialize_observable(
				   const std::string & input_observable,
				   const long int & number_of_values
				   );
public: void push(
		  const double & value
		  );
};
//
class control_observables{
public: std::vector<active_arc_observable> observables;
public: control_observables();
};
#endif // __STATE_ACTIVE_ARC__
