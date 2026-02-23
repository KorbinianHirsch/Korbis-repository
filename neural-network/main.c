#include "common.h"
#include "logic.h"
#include "csv.h"
#include "ui.h"

int main(int argc, char** argv){
    srand(time(NULL));
    if (argc < 12){
        printf("Nutzung: %s <n> <d> <train.csv> <test_n> <test_d> <test.csv> <neurons hiddenlayer 1> <neurons hiddenlayer 2> <epochs> <learningrate> <classes>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]); 
    int d = atoi(argv[2]); 
    char* filename = argv[3];
    int test_n = atoi(argv[4]); 
    int test_d = atoi(argv[5]); 
    char* test_filename = argv[6];
    int n_hl1 = atoi(argv[7]); 
    int n_hl2 = atoi(argv[8]); 
    int e = atoi(argv[9]);
    double learning_rate = atof(argv[10]);
    int output_layer_size = atoi(argv[11]);

    double time_per_epoch = 0.0;

    int neurons_hidden_layer1 = n_hl1;
    int neurons_hidden_layer2 = n_hl2;
    int layer_sizes[] = {d, neurons_hidden_layer1, neurons_hidden_layer2, output_layer_size}; 
    int num_layers = 4;
    Network network;

    double** x = malloc(n * sizeof(double*));
    double** test_x = malloc(test_n * sizeof(double*));

    for(int i=0; i<n; i++) x[i] = malloc(d * sizeof(double));
    for(int i=0; i<test_n; i++) test_x[i] = malloc(test_d * sizeof(double));

    int* y = malloc(n * sizeof(int));
    int* test_y = malloc(test_n * sizeof(int));

    int read_count, test_read_count;
    double avg_precision = 0.0;
    double avg_recall = 0.0;
    double avg_f_value = 0.0;

    //Run neuronal_net
    import_data(filename, n, d, x, y, &read_count);                                                                                                                                     //import Trainingsdaten

    import_data(test_filename, test_n, test_d, test_x, test_y, &test_read_count);                                                                                                       //import Testdaten

    create_run(&network, num_layers, layer_sizes, e, learning_rate, read_count, d, x, y, &time_per_epoch, test_n, test_d, test_x, test_y);                                              // Lade oder erstelle Netzwerk

    statistics(test_read_count, &network, test_x, test_y, time_per_epoch, &avg_precision, &avg_recall, &avg_f_value);                                                                   // Printe Statistik auf die Konsole

    print_experiment(neurons_hidden_layer1, neurons_hidden_layer2, output_layer_size, n, d, test_n, test_d, e, time_per_epoch, learning_rate, avg_precision, avg_recall, avg_f_value);  // Speicher die Experimente

    free_network(&network);                                                                                                                                                             //Gib reservierten Speicher frei

    return 0;
}