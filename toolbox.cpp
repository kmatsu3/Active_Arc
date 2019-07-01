#include "toolbox.hpp"
  /*=================================
    Methods
   =================================*/
  /*:::::::::::::::::::::::::::::::::
    Vector analysis
   :::::::::::::::::::::::::::::::::*/
// Vectir norm
double toolbox::norm(
		     const std::vector<double> & array
		     )
  const{
  return pow(std::inner_product(array.begin(),array.end(),array.begin(),0.0),0.5);
};
int toolbox::check_finite_vector(
				 const std::vector<double> & array
				 )
  const{
  int return_value;
  //if(std::accumulate(array.begin(),array.end(),0.0,add_absolute_double())>bit_parameter)
  if(std::inner_product(array.begin(),array.end(),array.begin(),0.0)>bit_parameter)
    {
      return_value=true_return;
    }
  else
    {
      return_value=false_return;
    };
  return return_value;
};
// Vector normalization
std::vector<double> toolbox::normalized_vector(
					       const std::vector<double> & array
					       )
  const {
  std::vector<double> return_vector;
  std::vector<double>::const_iterator index=array.begin();
  double array_norm=pow(std::inner_product(array.begin(),array.end(),array.begin(),0.0),0.5);
  if(fabs(array_norm)<bit_parameter)
    {
      while(index!=array.end())
	{
	  return_vector.push_back(0.0);
	  index++;
	};
    }
  else
    {
      while(index!=array.end())
	{
	  return_vector.push_back(*index/array_norm);
	  index++;
	};
    };
  //
  return return_vector;
};
//
  // Vector normalization
std::string toolbox::normalize(
			       std::vector<double> & array,
			       const double & normalized_value
			       )
const {
    std::string return_message="Unexpected error";
  if(array.empty())
    {
      return_message= "Zero element";
    }else{
    double array_norm;
    array_norm=pow(std::inner_product(array.begin(),array.end(),array.begin(),0.0),0.5);
    if(fabs(array_norm)<bit_parameter)
      {
	return_message= "Zero vector";
      }else{
      std::vector<double> work_vector;
      std::vector<double>::iterator index=array.begin();
      array_norm=array_norm/normalized_value;
      while(index!=array.end())
	{
	  work_vector.push_back(*index/array_norm);
	  index++;
	};
      copy(work_vector.begin(),work_vector.end(),array.begin());
      return_message= "Successfully end";
    };
  };
  return return_message;
};
//
double toolbox::normalized_product(
				   const std::vector<double> vector_1,
				   const std::vector<double> vector_2
				   ) 
  const{
  double norm_1=pow(std::accumulate(vector_1.begin(),vector_1.end(),0.0,add_square_double()),0.5);
  double norm_2=pow(std::accumulate(vector_2.begin(),vector_2.end(),0.0,add_square_double()),0.5);
  double product_12  =std::inner_product(vector_1.begin(),vector_1.end(),vector_2.begin(),0.0);
  double return_value=product_12/norm_1/norm_2;
  return return_value;
};				 
//
int toolbox::comparison_double(
			       const double & double_1,
			       const double & double_2
			       )
const {
  int return_value;
  if((double_1>double_2-bit_parameter)&&(double_1<double_2+bit_parameter))
    {
      return_value = true_return;
    } else
    {
      return_value = false_return;
    };
  return return_value;
};
//
int toolbox::comparison_double_vector(
				      const std::vector<double> & double_vector_1,
				      const std::vector<double> & double_vector_2
				      ) 
  const{
  int return_value=true_return;
  if(double_vector_1.size()!=double_vector_2.size())
    {
      return_value=false_return;
    }
  else
    {
      int component_index;
      for(component_index=0;component_index<(int)double_vector_1.size();component_index++)
	{
	  if(
	     fabs(
		  double_vector_1[component_index]
		  -
		  double_vector_2[component_index]
		  )
	     > 
	     bit_parameter
	     )
	    {
	      return_value = false_return;
	    };
	};
    };
  return return_value;
};
//
int toolbox::existence_list_double(
				   const std::list<double> & double_list,
				   const double & double_check
				   )
  const {
  int return_value=0;
  std::list<double>::const_iterator double_iterator=double_list.begin();
  while(double_iterator!=double_list.end())
    {
      if(comparison_double(double_check,*double_iterator)==1) return_value=1;
      double_iterator++;
    };
  return return_value;
};
//
double toolbox::extend_little(
			      const double & candidate
			      )
  const {
  return candidate + bit_parameter;
};
//
int toolbox::finite_abs_check(
			      const double & double_check
			      ) 
  const{
  int return_value=false_return;
  if(fabs(double_check)>bit_parameter) return_value = true_return;
  return return_value;
};
//
int toolbox::get_value(
		       const std::string & code
		       )
  const {
  int return_value=error_return;
  if(code=="true")
    {
      return_value=true_return;
    }
  else if(code=="false")
    {
      return_value=false_return;
    }
  else
    {
    return_value=error_return;
    };
  return return_value;
};
//
std::string toolbox::symmetry_check_double(
					   const std::vector< std::vector <double> > &  data
					   )
  const {
  std::string return_message="symmetric";
  long int row_dimension=(long int)data.size();
  std::vector<long int> column_dimensions(row_dimension,0);
  long int column_dimension=(long int)data[0].size();
  long int row_index;
  for (row_index=0;row_index<row_dimension;row_index++)
    {
      column_dimensions[row_index]=(long int)data[row_index].size();
      if(column_dimensions[row_index]!=column_dimension)
	{
	  return_message = "non-uniform column";
	}
    };
  long int column_index;
  for(row_index=0;row_index<row_dimension;row_index++)
    {
      for(column_index=0;column_index<column_dimension;column_index++)
	{
	  if(comparison_double(data[row_index][column_index],data[column_index][row_index])==get_value("false"))
	    {
	      return_message = "non-symmetric";
	    }
	};
    };
  return return_message;
};
//
double toolbox::get_element_of_double_list(
					   const std::list<double> & double_list,
					   const long long int & index
					   )
  const{
  double return_value=-1.0;
  std::list<double>::const_iterator double_iterator=double_list.begin();
  if((long long int)double_list.size()<index||index<0)
    {
      std::ofstream ofs( "toolbox_error.txt", std::ios::out | std::ios::app  );
	ofs << "out of bound (tool_box::get_element_of_double_list)" << std::endl;
        ofs.close();
    }
  long long int counter=0;
  while((double_iterator!=double_list.end())&&(counter<=index))
    {
      if(counter==index) return_value=*double_iterator;
      double_iterator++;
      counter++;
    };
  return return_value;
};
//
std::vector<double> toolbox::calculate_vector_multiplesum(
							  const std::vector<double> & data,
							  const long long int & size,
							  const long long int & rank
							  ) 
  const{
  long long int component_index;
  long long int vector_index;
  long long int dimension=(long long int)data.size()/size;
  std::vector<double> return_value(size,0.0);
  std::vector<double> work_vector(size,0.0);
  if(rank==1)
    {
      for(component_index=0;component_index<size;component_index++)
	{
	  for(vector_index=0;vector_index<dimension;vector_index++)
	    {
	      work_vector[vector_index]=data[vector_index*size+component_index];
	    };
	  return_value[component_index]=std::accumulate(work_vector.begin(),work_vector.end(),0.0);					
	};
    }
  else if(rank==2)
    {
      for(component_index=0;component_index<size;component_index++)
	{
	  for(vector_index=0;vector_index<dimension;vector_index++)
	    {
	      work_vector[vector_index]=data[vector_index*size+component_index];
	    };
	  return_value[component_index]=std::inner_product(work_vector.begin(),work_vector.end(),work_vector.begin(),0.0);				  
	};
    }
  else
    {
      std::ofstream ofs( "toolbox_error.txt", std::ios::out | std::ios::app  );
	ofs << "error of range (tool_box::calculate_vector_multiplesum)" << std::endl;
        ofs.close();
    }
  return return_value;
};
  /*=================================
    Constructor & Destractor
   =================================*/
toolbox::toolbox()
{
  bit_parameter=0.000001;
  true_return=1;
  false_return=0;
  error_return=-1;
};
