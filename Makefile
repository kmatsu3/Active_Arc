System_source  = active_arc_io.cpp           \
                 active_arc_gnuplot.cpp      \
                 active_arc_interface.cpp    \
                 active_arc_growth_model.cpp \
                 active_arc_schedule.cpp     \
                 active_arc_observable.cpp   \
                 toolbox.cpp
Alglib_include = -I/usr/local/include/alglib 
Modules_source = active_arc_definition.cpp   \
                 active_arc_state.cpp
Noheader_source= active_arc_md.cpp             \
                 active_arc_cg.cpp             \
                 active_arc_initstate.cpp      \
                 active_arc_reconstruction.cpp \
                 active_arc_constraint.cpp     \
                 active_arc_innerelasticity.cpp\
                 active_arc_gradient.cpp     \
                 active_arc_geometry.cpp     \
                 active_arc_displacement.cpp \
                 active_arc_reference.cpp    \
                 active_arc_modulus.cpp      \
                 active_arc_strain.cpp       \
                 active_arc_stress.cpp       \
                 active_arc_energy.cpp       \
                 active_arc_innervolume.cpp  \
                 active_arc_configuration.cpp\
                 active_arc_measurement.cpp  \
                 alglibinternal.cpp          \
                 alglibmisc.cpp              \
                 ap.cpp                      \
                 dataanalysis.cpp            \
                 diffequations.cpp           \
                 fasttransforms.cpp          \
                 integration.cpp             \
                 interpolation.cpp           \
                 linalg.cpp                  \
                 optimization.cpp            \
                 solvers.cpp                 \
                 specialfunctions.cpp        \
                 statistics.cpp
Main_source    = active_arc_main.cpp        
Source = $(System_source)                    \
         $(Modules_source)                   \
         $(Noheader_source)                  \
         $(Main_source)
Objects = $(Source:%.cpp=%.o)
System_headers = $(System_source:%.cpp=%.hpp) 
Includes = $(Alglib_include)                 \
           -I/usr/local/include              \
           -I/opt/local/include
Headers = $(System_source:%.cpp=%.hpp)       \
          $(Modules_source:%.cpp=%.hpp)      \
          $(Files_Include)
Dependences=$(System_source:%.cpp=%.d)       \
            $(Modules_source:%.cpp=%.d)      \
            $(Noheader_source:%.cpp=%.d)
Program=actarc
Cc = /opt/intel/bin/icc
Debugs= -g -debug -traceback
Options= -c -static -MMD -MP
ifeq ($(CC),icc_fast)
Cc = /opt/intel/bin/icc
Debugs= 
Options=  -O3 -static -ipo -no-prec-div -c -MMD -MP
endif
ifeq ($(CC),icc_gprof)
Cc = /opt/intel/bin/icc
Debugs= 
Options= -g  -pg -c -MMD -MP
endif
ifeq ($(CC),icc_profgen)
Cc = /opt/intel/bin/icc
Debugs= -O3 -xHOST -O3 -ipo -no-prec-div -prof_gen
Options= -c -MMD -MP 
endif
ifeq ($(CC),icc_profuse)
Cc = /opt/intel/bin/icc
Debugs= -O3 -prof_use
Options= -c -static -MMD -MP
endif
ifeq ($(CC),gcc)
Cc = g++
Debugs= -g -ggdb -Wall -W -pg
Options= -c -static -MMD -MP
endif
ifeq ($(CC),gcc_prof)
Cc = g++
Debugs= -pg
Options= -O0 -c -static -MMD
endif
ifeq ($(CC),gcc_prof5)
Cc = g++
Debugs= -pg
Options= -O5 -c -static -MMD 
endif
ifeq ($(CC),gcc_fast)
Cc = g++
Debugs=
Options= -O5 -c -static -MMD
endif
ifeq ($(CC),clang)
Cc = gcc 
Debugs=  -Wall  
Options= -c -static -MMD -MP
endif
LIBs= -lgfortran                   \
      $(Includes)                  \
      -lalglib                     \
      -L/usr/local/lib/libblas.a   \
      -L/usr/local/lib/liblapack.a \
      -L/usr/local/lib/            \
      -L/usr/local/lib/libalglib.a
-include $(Includes) $(Dependences)
.SUFFIXES:.cpp .o
.cpp.o:
	$(Cc) $(Options) $(LIBs) $(Debugs) $< -o $@ 
actarc: $(Objects) $(Headers)
	$(Cc) $(LIBs) $(Objects) $(Debugs)  -v -o $@
active_arc_main.o:active_arc_main.cpp
active_arc_reconstruction.o:active_arc_reconstruction.cpp
active_arc_initstate.o:active_arc_initstate.cpp
active_arc_schedule.o:active_arc_schedule.cpp
active_arc_md.o:active_arc_md.cpp
active_arc_cg.o:active_arc_cg.cpp
active_arc_constraint.o:active_arc_constraint.cpp
active_arc_gradient.o:active_arc_gradient.cpp
active_arc_energy.o:active_arc_energy.cpp
active_arc_innerelasticity.o:active_arc_innerelasticity.cpp	
active_arc_innervolume.o:active_arc_innervolume.cpp
active_arc_strain.o:active_arc_stress.cpp
active_arc_strain.o:active_arc_strain.cpp
active_arc_modulus.o:active_arc_modulus.cpp
active_arc_reference.o:active_arc_reference.cpp
active_arc_displacement.o:active_arc_displacement.cpp
active_arc_geometry.o:active_arc_geometry.cpp
active_arc_configuration.o:active_arc_configuration.cpp
active_arc_growth_model.o:active_arc_growth_model.cpp
active_arc_measurement.o:active_arc_measurement.cpp
active_arc_state.o:active_arc_state.cpp
active_arc_observable.o:active_arc_observable.cpp
active_arc_definition.o:active_arc_definition.cpp
active_arc_reference_io.o:active_arc_interface.cpp
active_arc_gnuplot.o:active_arc_gnuplot.cpp
active_arc_io.o:active_arc_io.cpp
toolbox.o:toolbox.cpp
alglibinternal.o:alglibinternal.cpp
alglibmisc.o:alglibmisc.cpp
ap.o:ap.cpp 
dataanalysis.o:dataanalysis.cpp
diffequations.o:diffequations.cpp
fasttransforms.o:fasttransforms.cpp
integration.o:integration.cpp
interpolation.o:interpolation.cpp
linalg.o:linalg.cpp
optimization.o:optimization.cpp
solvers.o:solvers.cpp
specialfunctions.o:specialfunctions.cpp
statistics.o:statistics.cpp
clean:
	rm -f $(Dependences) $(Objects) $(Program) *~
