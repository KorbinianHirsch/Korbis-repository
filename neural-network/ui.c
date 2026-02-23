#include "ui.h"

void calculate_statistics(int output_layer_size, int **confusion_matrix, double *recall, double *precision, double *f_value, double *avg_precision, double *avg_recall, double *avg_f_value){
    for(int i = 0; i < output_layer_size; i++){
        double corr = confusion_matrix[i][i];
        double sum_recall = 0.0;
        double sum_precision = 0.0;
        for(int j = 0; j < output_layer_size; j++){
            sum_recall += confusion_matrix[i][j];
            sum_precision += confusion_matrix[j][i];
        }
        recall[i] = (sum_recall == 0.0) ? 0.0 : (corr / sum_recall) * 100;
        *avg_recall += recall[i] / output_layer_size;
        precision[i] = (sum_precision == 0.0) ? 0.0 : (corr / sum_precision) * 100;
        *avg_precision += precision[i] / output_layer_size;
        f_value[i] = (precision[i] + recall[i] > 0) ? 2 * ((precision[i] * recall[i]) / (precision[i] + recall[i])) : 0.0;
        *avg_f_value += f_value[i] / output_layer_size;
    }
}

void calculate_confusion_matrix(Network *net, int test_read_count, int num_layers, double **test_x, int *test_y, int output_layer_size, int **confusion_matrix, int *correct_predictions, double *accuracy){
    for (int i = 0; i < test_read_count; i++){
        forward_prop(net, test_x[i]);
        int best_guess = 0;
        double max_val = -1.0;
        int output_layer_idx = num_layers - 1;
        for(int k=0; k < output_layer_size; k++){
            if(net->layers[output_layer_idx].neurons[k].activation > max_val){
                max_val = net->layers[output_layer_idx].neurons[k].activation;
                best_guess = k;
            }
        }
        int actual_label = test_y[i];
        if(best_guess == actual_label){
            (*correct_predictions)++;
        }else{
            save_wrong_predictions(test_x, best_guess, actual_label, 784, i);
        }
        if (actual_label >= 0 && actual_label <= output_layer_size-1 && best_guess >= 0 && best_guess <= output_layer_size-1){
            confusion_matrix[actual_label][best_guess]++;
        }
    }
    *accuracy = (double)*correct_predictions / test_read_count * 100.0;
}

void print_confusion_matrix(int **confusion_matrix, double accuracy, int correct_predictions, int test_read_count, int output_layer_size, double *recall, double *f_value, double *precision, double time_per_epoch){
    printf("\nGesamt-Genauigkeit (Accuracy): %.2f%%\n", accuracy);
    printf("(%d von %d korrekt)\n\n", correct_predictions, test_read_count);

    printf("Konfusionsmatrix (Zeile = Echte Klasse, Spalte = Vorhersage):\n");
    printf("   |  ");
    for(int i = 0; i < output_layer_size; i++){
        printf("%d      ", i);
    }
    printf("| Recall |  F1  ");
    printf("\n");
    printf("---------------------------------------------------------------------------------------------\n");

    for(int row = 0; row < output_layer_size; row++){
        printf(" %d |", row);
        for(int col = 0; col < output_layer_size; col++){
            printf("%3d    ", confusion_matrix[row][col]);
        }
        printf("|   %.2f   |   %.2f  ", recall[row], f_value[row]);
        printf("\n");
    }
    printf("   |  ");
    for(int i = 0; i < output_layer_size; i++){
        printf("%.2f  ", precision[i]);
    }
    printf("\n");
    printf("Zeit benoetigt pro Epoche: %f sek\n", time_per_epoch);
}

void print_header(int test_read_count){
    printf("\n========================================\n");
    printf("Starte Test auf %d Testdaten...\n", test_read_count);
    printf("========================================\n");
}

void statistics(int test_read_count, Network *net, double **test_x, int *test_y, double time_per_epoch, double *avg_precision, double *avg_recall, double *avg_f_value){
    
    print_header(test_read_count);
    
    int num_layers = net->num_layers;
    Layer output_layer = net->layers[num_layers-1];
    int output_layer_size = output_layer.num_neurons;
    int correct_predictions = 0;

    // Init
    int **confusion_matrix = malloc(output_layer_size * sizeof(int *));
    for(int i = 0; i < output_layer_size; i++){
        confusion_matrix[i] = malloc(output_layer_size * sizeof(int));
    }
    for(int i = 0; i < output_layer_size; i++){
        for(int j = 0; j < output_layer_size; j++){
            confusion_matrix[i][j] = 0;
        }
    }

    double accuracy;
    double *recall = malloc(output_layer_size * sizeof(double));
    double *f_value = malloc(output_layer_size * sizeof(double));
    double *precision = malloc(output_layer_size * sizeof(double));
    
    calculate_confusion_matrix(net, test_read_count, num_layers, test_x, test_y, output_layer_size, confusion_matrix, &correct_predictions, &accuracy);
    
    calculate_statistics(output_layer_size, confusion_matrix, recall, precision, f_value, avg_precision, avg_recall, avg_f_value);

    print_confusion_matrix(confusion_matrix, accuracy, correct_predictions, test_read_count, output_layer_size, recall, f_value, precision, time_per_epoch);
}
    