#ifndef _LOGIC_H
#define _LOGIC_H 1

#include "common.h"
#include "csv.h"

Neuron create_neuron(int inputs);

Layer create_layer(int neurons, int inputs_per_neuron);

Network create_network(int num_layers, int* layer_sizes);

void free_network(Network* net);

double sigmoid(double x);

double sigmoid_derivative(double output);

void forward_prop(Network* net, double* input_data);

void backpropagation(Network* net, int target_label, double learning_rate);

void train(int samples, int d, double** full_x, int* full_y, Network* network, int epochs, double learning_rate, int test_n, int test_d, double** test_x, int* test_y);

void free_net(Network *net, int n, double **x, int *y, double *recall, double *precision, double *f_value, int test_n, double **test_x, int *test_y);

void create_run(Network *net, int num_layers, int *layer_sizes, int e, double learning_rate, int read_count, int d, double **x, int *y, double *time_per_epoch, int test_n, int test_d, double** test_x, int* test_y);

#endif
