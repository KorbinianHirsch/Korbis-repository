#include "common.h"
#include "logic.h"

void calculate_statistics(int output_layer_size, int **confusion_matrix, double *recall, double *precision, double *f_value, double *avg_precision, double *avg_recall, double *avg_f_value);

void calculate_confusion_matrix(Network *net, int test_read_count, int num_layers, double **test_x, int *test_y, int output_layer_size, int **confusion_matrix, int *correct_predictions, double *accuracy);

void print_confusion_matrix(int **confusion_matrix, double accuracy, int correct_predictions, int test_read_count, int output_layer_size, double *recall, double *f_value, double *precision, double time_per_epoch);

void statistics(int test_read_count, Network *net, double **test_x, int *test_y, double time_per_epoch, double *avg_precision, double *avg_recall, double *avg_f_value);

void print_header(int test_read_count);