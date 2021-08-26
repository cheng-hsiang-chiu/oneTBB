//
// Example program that reads a file of decimal integers in text format,
// increments each and changes each to its square.
//
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>


#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>

#include "oneapi/tbb/parallel_pipeline.h"
#include "oneapi/tbb/tick_count.h"
#include "oneapi/tbb/tbb_allocator.h"
#include "oneapi/tbb/global_control.h"

#include "common/utility/utility.hpp"
#include "common/utility/get_default_num_threads.hpp"

extern void generate_if_needed(const char*);

std::string InputFileName  = "./input.txt";
//std::string OutputFileName = "./output.txt";

std::ifstream inputfile (InputFileName.c_str());



class MyInputFunc {
public:
  MyInputFunc() {}
    
  ~MyInputFunc(){}
    
  float operator()(oneapi::tbb::flow_control& fc) const;
};

float MyInputFunc::operator()(oneapi::tbb::flow_control& fc) const {
  float retval = 0.0;
  std::string line;
    
  if (getline(inputfile, line)) {
    retval = 1 + std::stoi(line);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
    return retval;
  }
    
  fc.stop();

  return -1;
}



// Functor that changes each decimal number to its square.
class MyTransformFunc {
public:
  float operator()(float input) const;
};

float MyTransformFunc::operator()(float input) const {
  float retval = 0.0;
  retval = input * input;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
  return retval;
}



// Functor that prints out string 
class MyOutputFunc {
public:
  MyOutputFunc(){}
  void operator()(float input) const;
};

void MyOutputFunc::operator()(float input) const {
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::cout << "Answer = " << input << std::endl;
}



int run_pipeline(int nthreads, int pid) {

  // Need more than one token in flight per thread to keep all threads
  // busy; 2-4 works, 
  // run in serial_in_order or parallel

  switch(pid) {
    case 0: // sss
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
        oneapi::tbb::make_filter<void, float>(
          oneapi::tbb::filter_mode::serial_in_order, MyInputFunc())     &
        oneapi::tbb::make_filter<float, float>(
          oneapi::tbb::filter_mode::serial_in_order, MyTransformFunc()) &
        oneapi::tbb::make_filter<float, void>(
          oneapi::tbb::filter_mode::serial_in_order, MyOutputFunc())
      );
      break;

    case 1: // sps
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
        oneapi::tbb::make_filter<void, float>(
          oneapi::tbb::filter_mode::serial_in_order, MyInputFunc()) &
        oneapi::tbb::make_filter<float, float>(
          oneapi::tbb::filter_mode::parallel, MyTransformFunc()) &
        oneapi::tbb::make_filter<float, void>(
          oneapi::tbb::filter_mode::serial_in_order, MyOutputFunc())
      );
      break;

    case 2: // ppp
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
          oneapi::tbb::make_filter<void, float>(
            oneapi::tbb::filter_mode::parallel, MyInputFunc()) &
          oneapi::tbb::make_filter<float, float>(
            oneapi::tbb::filter_mode::parallel, MyTransformFunc()) &
          oneapi::tbb::make_filter<float, void>(
            oneapi::tbb::filter_mode::parallel, MyOutputFunc())
      );
      break;

    case 3: // ssp
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
          oneapi::tbb::make_filter<void, float>(
            oneapi::tbb::filter_mode::serial_in_order, MyInputFunc()) &
          oneapi::tbb::make_filter<float, float>(
            oneapi::tbb::filter_mode::serial_in_order, MyTransformFunc()) &
          oneapi::tbb::make_filter<float, void>(
            oneapi::tbb::filter_mode::parallel, MyOutputFunc())
      );
      break;

    case 4: // spp
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
          oneapi::tbb::make_filter<void, float>(
            oneapi::tbb::filter_mode::serial_in_order, MyInputFunc()) &
          oneapi::tbb::make_filter<float, float>(
            oneapi::tbb::filter_mode::parallel, MyTransformFunc()) &
          oneapi::tbb::make_filter<float, void>(
            oneapi::tbb::filter_mode::parallel, MyOutputFunc())
      );
      break;

    case 5: // pss
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
          oneapi::tbb::make_filter<void, float>(
            oneapi::tbb::filter_mode::parallel, MyInputFunc()) &
          oneapi::tbb::make_filter<float, float>(
            oneapi::tbb::filter_mode::serial_in_order, MyTransformFunc()) &
          oneapi::tbb::make_filter<float, void>(
            oneapi::tbb::filter_mode::serial_in_order, MyOutputFunc())
      );
      break;
    
    case 6: // psp
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
          oneapi::tbb::make_filter<void, float>(
            oneapi::tbb::filter_mode::parallel, MyInputFunc()) &
          oneapi::tbb::make_filter<float, float>(
            oneapi::tbb::filter_mode::serial_in_order, MyTransformFunc()) &
          oneapi::tbb::make_filter<float, void>(
            oneapi::tbb::filter_mode::parallel, MyOutputFunc())
      );
      break;
    
    case 7: // pps
      oneapi::tbb::parallel_pipeline(
        nthreads * 4,
          oneapi::tbb::make_filter<void, float>(
            oneapi::tbb::filter_mode::parallel, MyInputFunc()) &
          oneapi::tbb::make_filter<float, float>(
            oneapi::tbb::filter_mode::parallel, MyTransformFunc()) &
          oneapi::tbb::make_filter<float, void>(
            oneapi::tbb::filter_mode::serial_in_order, MyOutputFunc())
      );
      break;
  }

  inputfile.close();
    
  return 1;
}


int main(int argc, char* argv[]) {
  generate_if_needed(InputFileName.c_str());
   
  std::vector<std::string> pattern{"sss", "sps", "ppp", "ssp", "spp", "pss", "psp", "pps"}; 

  utility::thread_number_range threads(utility::get_default_num_threads, 0);

  //oneapi::tbb::global_control c(oneapi::tbb::global_control::max_allowed_parallelism,
  //                              utility::get_default_num_threads());
    

  // pattern 0 : sss | 1 : sps | 2 : ppp

  int pid             = std::atoi(argv[1]);
  int running_threads = std::atoi(argv[2]);

  oneapi::tbb::tick_count mainStartTime = oneapi::tbb::tick_count::now();
  
  //for (int i = 0; i < 2; ++i) { 
  //  if (!run_pipeline(running_threads, pid)) {
  //    return -1;
  //  }
  //}
  //if (!run_pipeline(utility::get_default_num_threads())) {
  //  return -1;
  //}

  //utility::report_elapsed_time((oneapi::tbb::tick_count::now() - mainStartTime).seconds());

  oneapi::tbb::global_control c(oneapi::tbb::global_control::max_allowed_parallelism,
                                running_threads);

  if (!run_pipeline(running_threads, pid)) {
    return -1;
  }
  
  std::cout << "Runtime of " 
            << pattern[pid] 
            << " = " 
            << (oneapi::tbb::tick_count::now() - mainStartTime).seconds() << std::endl;

  return 0;
}
