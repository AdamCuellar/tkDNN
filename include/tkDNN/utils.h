#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include "cuda.h"
#include "cuda_runtime_api.h"
#include <cublas_v2.h>
#include <cudnn.h>

#define dnnType float

// Colored output
#define COL_END "\033[0m"

#define COL_RED "\033[31m"
#define COL_GREEN "\033[32m"
#define COL_ORANGE "\033[33m"
#define COL_BLUE "\033[34m"
#define COL_PURPLE "\033[35m"
#define COL_CYAN "\033[36m"

#define COL_REDB "\033[1;31m"
#define COL_GREENB "\033[1;32m"
#define COL_ORANGEB "\033[1;33m"
#define COL_BLUEB "\033[1;34m"
#define COL_PURPLEB "\033[1;35m"
#define COL_CYANB "\033[1;36m"

// Simple Timer 
#define TIMER_START timespec start, end;                               \
                    clock_gettime(CLOCK_MONOTONIC, &start);            

#define TIMER_STOP_C(col)  clock_gettime(CLOCK_MONOTONIC, &end);       \
    double t_ns = ((double)(end.tv_sec - start.tv_sec) * 1.0e9 +       \
                  (double)(end.tv_nsec - start.tv_nsec))/1.0e6;        \
    std::cout<<col<<"Time:"<<std::setw(16)<<t_ns<<" ms\n"<<COL_END; 

#define TIMER_STOP TIMER_STOP_C(COL_CYANB)

/********************************************************
 * Prints the error message, and exits
 * ******************************************************/
#define EXIT_WAIVED 0

#define FatalError(s) {                                                \
    std::stringstream _where, _message;                                \
    _where << __FILE__ << ':' << __LINE__;                             \
    _message << std::string(s) + "\n" << __FILE__ << ':' << __LINE__;\
    std::cerr << _message.str() << "\nAborting...\n";                  \
    cudaDeviceReset();                                                 \
    exit(EXIT_FAILURE);                                                \
}

#define checkCUDNN(status) {                                           \
    std::stringstream _error;                                          \
    if (status != CUDNN_STATUS_SUCCESS) {                              \
      _error << "CUDNN failure: " <<cudnnGetErrorString(status);       \
      FatalError(_error.str());                                        \
    }                                                                  \
}

#define checkCuda(status) {                                            \
    std::stringstream _error;                                          \
    if (status != 0) {                                                 \
      _error << "Cuda failure: "<<cudaGetErrorString(status);          \
      FatalError(_error.str());                                        \
    }                                                                  \
}

#define checkERROR(status) {                                           \
    std::stringstream _error;                                          \
    if (status != 0) {                                                 \
      _error << "Generic failure: " << status;                         \
      FatalError(_error.str());                                        \
    }                                                                  \
}

#define checkNULL(ptr) {                                               \
    std::stringstream _error;                                          \
    if (ptr == nullptr) {                                              \
      _error << "Null pointer";                                        \
      FatalError(_error.str());                                        \
    }                                                                  \
}

void printCenteredTitle(const char *title, char fill, int dim);
bool fileExist(const char *fname);
void readBinaryFile(std::string fname, int size, dnnType** data_h, dnnType** data_d, int seek = 0);
int checkResult(int size, dnnType *data_d, dnnType *correct_d, bool device = true, int limit = 10);
void printDeviceVector(int size, dnnType* vec_d, bool device = true);
void resize(int size, dnnType **data);

void matrixTranspose(cublasHandle_t handle, dnnType* srcData, dnnType* dstData, int rows, int cols);

void matrixMulAdd(  cublasHandle_t handle, dnnType* srcData, dnnType* dstData, 
                    dnnType* add_vector, int dim, dnnType mul);
#endif //UTILS_H