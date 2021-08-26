//
// Example program that reads a vector of decimal integers,  
// increments one and squares it
//
#include <iostream>
#include <string>
#include <chrono>

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


std::vector<int> input_vector {1,2,3,4,5,6,7,8,9,10};
std::vector<int>::iterator vit = input_vector.begin();

class MyInputFunc {
public:
    MyInputFunc(){}
    
    ~MyInputFunc(){}
    
    float operator()(oneapi::tbb::flow_control& fc) const;
};


float MyInputFunc::operator()(oneapi::tbb::flow_control& fc) const {
    float retval = 0.0;

    if (vit == input_vector.end()) {
        fc.stop();
        return -1;
    }

    retval = (*vit) + 1;
    ++vit;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return retval;
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

// Functor that writes a TextSlice to a file.
class MyOutputFunc {

public:
    MyOutputFunc(){}
    void operator()(float input) const;
};


void MyOutputFunc::operator()(float input) const {

   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   std::cout << "Answer = " << input << std::endl;
}


int run_pipeline(int nthreads) {

    oneapi::tbb::tick_count t0 = oneapi::tbb::tick_count::now();

    // Need more than one token in flight per thread to keep all threads
    // busy; 2-4 works, 
    // run in serial_in_order or parallel
    oneapi::tbb::parallel_pipeline(
        nthreads * 4,
        oneapi::tbb::make_filter<void, float>(
            oneapi::tbb::filter_mode::serial_in_order, MyInputFunc()) &
        oneapi::tbb::make_filter<float, float>(
            oneapi::tbb::filter_mode::parallel, MyTransformFunc()) &
        oneapi::tbb::make_filter<float, void>(
            oneapi::tbb::filter_mode::parallel, MyOutputFunc())
    );

    oneapi::tbb::tick_count t1 = oneapi::tbb::tick_count::now();

    return 1;
}

int main(int argc, char* argv[]) {
    oneapi::tbb::tick_count mainStartTime = oneapi::tbb::tick_count::now();

    // The 1st argument is the function to obtain 'auto' value; the 2nd is the default value
    // The example interprets 0 threads as "run serially, then fully subscribed"
    utility::thread_number_range threads(utility::get_default_num_threads, 0);

    oneapi::tbb::global_control c(oneapi::tbb::global_control::max_allowed_parallelism,
                                  utility::get_default_num_threads());
    if (!run_pipeline(utility::get_default_num_threads()))
        return -1;

    utility::report_elapsed_time((oneapi::tbb::tick_count::now() - mainStartTime).seconds());

    return 0;
}
