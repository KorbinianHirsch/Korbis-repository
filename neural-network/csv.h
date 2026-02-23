#ifndef _CSV_H
#define _CSV_H 

#include "common.h"
#include "logic.h"

int read_csv(char* filename, int n, int d, double **x, int* y);

void save_loss(double *loss_arr, double *loss_test_arr, int epochs);

void save_wrong_predictions(double **test_x, int prediction, int actual_label, int dimension, int index);

void save_network_csv(Network* net, const char* filename);

Network load_network_csv(const char* filename);

void print_experiment(int neurons_hl1, int neurons_hl2, int output_layer_size, int num_data_training, int num_dimension_training, int num_data_test, int num_dimension_test, int num_epochs, double time_epoch, double learning_rate, double avg_precision, double avg_recall, double avg_f_value);

void import_data(char* filename, int n, int d, double** x, int* y, int *read_count);

#endif
                        