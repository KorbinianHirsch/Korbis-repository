#include "csv.h"

// CSV Lesen
int read_csv(char* filename, int n, int d, double **x, int* y){
    FILE* f = fopen(filename, "r");
    if(f == NULL){
        printf("Fehler: Datei %s konnte nicht geoeffnet werden.\n", filename);
        exit(2);
    }
    char line[65536];
    fgets(line, sizeof(line), f);
    int count = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < d; j++){
            if(fscanf(f, "%lf,", &x[i][j]) == EOF){ 
                fclose(f); 
                return count;
            }
            x[i][j] /= 255.0; // Normalisierung auf 0.0 - 1.0
        }
        if(fscanf(f, "%d", &y[i]) == EOF){
            fclose(f); 
            return count;
        }
        count++;
    }
    fclose(f);
    return count;
}

void save_loss(double *loss_arr, double *loss_test_arr, int epochs){
    FILE* f = fopen("loss.csv", "a");
    if(f == NULL){
        printf("Fehler beim Oeffnen der Datei zum Speichern!\n");
        return;
    }
    for(int i = 0; i < epochs; i++){
        fprintf(f, "%d,%f,%f\n", i, loss_arr[i], loss_test_arr[i]);
    }
    fclose(f);
    printf("Loss als CSV in '%s' gespeichert.\n", "loss.csv");
}

void save_wrong_predictions(double **test_x, int prediction, int actual_label, int dimension, int index){
    FILE* f = fopen("wrong_predictions.csv", "a");
    if(f == NULL){
        printf("Fehler beim Oeffnen der Datei zum Speichern!\n");
        return;
    }
    for(int i = 0; i < dimension; i++){
        fprintf(f, "%d,", (int)(test_x[index][i] * 255));
    }
    fprintf(f, "%d,%d\n", prediction, actual_label);
    fclose(f);
}

void save_network_csv(Network* net, const char* filename){
    FILE* f = fopen(filename, "w");
    if(f == NULL){
        printf("Fehler beim Oeffnen der Datei zum Speichern!\n");
        return;
    }
    fprintf(f, "ARCH");
    for(int i = 0; i < net->num_layers; i++){
        fprintf(f, ",%d", net->layers[i].num_neurons);
    }
    fprintf(f, "\n");
    for(int i = 1; i < net->num_layers; i++){
        Layer* l = &net->layers[i];
        for(int j = 0; j < l->num_neurons; j++){
            Neuron* n = &l->neurons[j];
            fprintf(f, "%d,%d,%.15lf", i, j, n->bias);
            for(int k = 0; k < n->num_inputs; k++){
                fprintf(f, ",%.15lf", n->weights[k]);
            }
            fprintf(f, "\n");
        }
    }
    fclose(f);
    printf("Netzwerk als CSV in '%s' gespeichert.\n", filename);
}

Network load_network_csv(const char* filename){
    FILE* f = fopen(filename, "r");
    if(f == NULL){
        printf("Fehler beim Oeffnen der Datei!\n");
        exit(1);
    }
    char line[100000]; 

    if(fgets(line, sizeof(line), f) == NULL){
        printf("Datei leer.\n"); exit(1);
    }
    int layer_sizes[100];
    int num_layers = 0;
    char* token = strtok(line, ",");
    while(token != NULL) {
        token = strtok(NULL, ",");
        if(token != NULL) {
            layer_sizes[num_layers++] = atoi(token);
        }
    }
    Network net = create_network(num_layers, layer_sizes);
    printf("Netz geladen: %d Layer.\n", num_layers);
    while(fgets(line, sizeof(line), f) != NULL){
        char* val = strtok(line, ",");

        if(val == NULL) continue;

        int l_idx = atoi(val);
        val = strtok(NULL, ",");
        
        if(val == NULL) continue; 

        int n_idx = atoi(val);
        Neuron* n = &net.layers[l_idx].neurons[n_idx];
        val = strtok(NULL, ",");

        if(val == NULL) continue;

        n->bias = atof(val);
        for(int k = 0; k < n->num_inputs; k++){
            val = strtok(NULL, ",");
            if(val == NULL){
                printf("Fehler: CSV Zeile beschaedigt bei Layer %d Neuron %d\n", l_idx, n_idx);
                break; 
            }
            n->weights[k] = atof(val);
        }
    }
    fclose(f);
    printf("Netzwerk aus CSV geladen.\n");
    return net;
}

void print_experiment(int neurons_hl1, int neurons_hl2, int output_layer_size, int num_data_training, int num_dimension_training, int num_data_test, int num_dimension_test, int num_epochs, double time_epoch, double learning_rate, double avg_precision, double avg_recall, double avg_f_value){
    FILE *file_ptr;
    file_ptr = fopen("experiment.csv", "a");
    if(file_ptr == NULL){
        printf("Error opening file!");
        return;
    }
    fprintf(file_ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%.8f,%.3f,%.2f,%.2f,%.2f\n", neurons_hl1, neurons_hl2, output_layer_size, num_data_training, num_dimension_training, num_data_test, num_dimension_test, num_epochs, time_epoch, learning_rate,avg_precision, avg_recall, avg_f_value);
    fclose(file_ptr);
}

void import_data(char* filename, int n, int d, double** x, int* y, int *read_count){
    printf("Lese Test-CSV '%s'...\n", filename);
    *read_count = read_csv(filename, n, d, x, y);
    printf("%d Datensaetze geladen.\n", (*read_count));
    if((*read_count) == 0){
        printf("WARNUNG: Keine Testdaten gelesen.\n");
    }
}