// Header for toolbox class
#ifndef __TOOLBOX__
#define __TOOLBOX__
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <list>
class toolbox{
  /*=================================
    Definitions of general paramerers
   =================================*/
private:double bit_parameter;
private:int true_return;
private:int false_return;
private:int error_return;
  /*=================================
    Methods
   =================================*/
  /*:::::::::::::::::::::::::::::::::
    Vector analysis
   :::::::::::::::::::::::::::::::::*/
  // Vector normalization
public:double norm(
		   const std::vector<double> & array
		   ) const;
  //
public:int check_finite_vector(
			       const std::vector<double> & array
			       ) const;
  //
public:std::vector<double> normalized_vector(
					     const std::vector<double> & array
					     ) const;
  //
public:std::string normalize(
			     std::vector<double> &array,
			     const double & normalized_value
			     ) const;
  //
public:double normalized_product(
				 const std::vector<double> vector_1,
				 const std::vector<double> vector_2
				 ) const;
  //      
  /*:::::::::::::::::::::::::::::::::
    Comparison
    :::::::::::::::::::::::::::::::::*/
public:int comparison_double(
			     const double & double_1,
			     const double & double_2
			     ) const;
  //
public:int comparison_double_vector(
				    const std::vector<double> & double_vector_1,
				    const std::vector<double> & double_vector_2
				    ) const;
  //
public:int existence_list_double(
				 const std::list<double> & double_list,
				 const double & double_check
				 ) const;
public:int finite_abs_check(
			    const double & double_check
			    ) const;
public:int get_value(
		     const std::string & code
		     ) const;
  //
public:std::string symmetry_check_double(
					 const std::vector< std::vector <double> > & data
					 ) const ;
  //
public:std::vector<double> calculate_vector_multiplesum(
							const std::vector<double> & data,
							const long long int & size,
							const long long int & rank
							) const;
  //
private:class add_square_double{
public: double operator()(const double summation, const double value)
  {
    return summation + value * value;
  };
};
  //
private:class add_absolute_double{
public: double operator()(const double summation, const double value)
  {
    return summation + fabs(value);
  };
};
  /*:::::::::::::::::::::::::::::::::
    Criteria construction
    :::::::::::::::::::::::::::::::::*/
public:double extend_little(
			    const double & candidate
			    ) const;
  /*:::::::::::::::::::::::::::::::::
    list operation
    :::::::::::::::::::::::::::::::::*/
public:double get_element_of_double_list(
					 const std::list<double> & double_list,
					 const long long int & index
					 )const;
  /*:::::::::::::::::::::::::::::::::
    search operation
    :::::::::::::::::::::::::::::::::*/
  /*=================================
    Constructor & Destractor
   =================================*/
public: toolbox();
};
#endif // __TOOLBOX__
