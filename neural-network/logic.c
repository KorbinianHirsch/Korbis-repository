#include "logic.h"

// --- INITIALISIERUNG des Netzes---
// Erstellt ein einzelnes Neuron
Neuron create_neuron(int inputs){
    Neuron n;
    n.num_inputs = inputs;
    n.bias = 0.0;
    // Wenn Input-Layer --> Nichts reservieren!
    if(inputs > 0){
        n.weights = (double*)malloc(inputs * sizeof(double));
        for(int i = 0; i < inputs; i++){
            n.weights[i] = ((double)rand() / (double)RAND_MAX) - 0.5;
        }
    }else{
        n.weights = NULL;   // Input Layer hat keine Gewichte
    }
    n.weighted_sum = 0.0;   //Setze weightedsum auf 0
    n.activation = 0.0;     //Setze activation auf 0
    n.delta = 0.0;          //Setze delta auf 0
    return n;               //gebe das erstellte Neuron zurück
}

// Erstellt einen Layer von Neuronen
Layer create_layer(int neurons, int inputs_per_neuron){
    Layer l;
    l.num_neurons = neurons;
    l.neurons = (Neuron*)malloc(l.num_neurons * sizeof(Neuron));
    for(int i = 0; i < l.num_neurons; i++){                           //Der Layer hat num_neurons Neuronen
        l.neurons[i] = create_neuron(inputs_per_neuron);              //Erstelle für den Layer an der Stelle i ein Neuron
    }
    return l;                                                         //gebe den erstellten Layer zurück
}

// Erstellt das gesamte Netzwerk basierend auf einem Array von Größen
// Beispiel: layer_sizes = {784, 30, 10} erstellt Input(784) -> Hidden(30) -> Output(10)
Network create_network(int num_layers, int* layer_sizes){
    Network net;
    net.num_layers = num_layers;
    net.layers = (Layer*)malloc(net.num_layers * sizeof(Layer));                            //Reserviere Speicher für das Array an Layern im Netz
    for(int i = 0; i < net.num_layers; i++){
        int num_inputs_from_prev_layer = (i == 0) ? 0 : layer_sizes[i-1];                   //Layer 0 (Input) hat keine eingehenden Gewichte von einem vorherigen Layer
        net.layers[i] = create_layer(layer_sizes[i], num_inputs_from_prev_layer);           //Erstelle für das Netz Array an der Stelle i einen Layer
    }
    return net;
}

// Freigeben des Speichers um Memory-leaks zu verhindern
void free_network(Network* net){
    for(int i = 0; i < net->num_layers; i++){
        for(int j = 0; j < net->layers[i].num_neurons; j++){
            free(net->layers[i].neurons[j].weights);
        }
        free(net->layers[i].neurons);
    }
    free(net->layers);
}

// --- MATHEMATIK ---
//Sigmoid-Aktivierungsfunktion
double sigmoid(double x){
    return 1.0 / (1.0 + exp(-x));
}

//Ableitung der Sigmoid-Funktion für Backpropagation
double sigmoid_derivative(double output){
    return output * (1.0 - output); 
}

//Forward Propagation
// Berechnet die Ausgaben des Netzwerks basierend auf Input
void forward_prop(Network* net, double* input_data){
    Layer* l0 = &net->layers[0];                                                // 1. Input Layer (Layer 0) setzen
    for(int i = 0; i < l0->num_neurons; i++){
        l0->neurons[i].activation = input_data[i];                              // Hier kopieren wir nur die Eingabedaten in die Aktivierung der ersten Schicht
    }

    for(int i = 1; i < net->num_layers; i++){                                   // 2. Durch alle folgenden Schichten propagieren (Hidden -> Output)
        Layer* prev_layer = &net->layers[i-1];                                  // Setze den Vorgänger-Layer
        Layer* curr_layer = &net->layers[i];                                    // Setze den aktuellen Layer

        for(int j = 0; j < curr_layer->num_neurons; j++){                                       // Iteriere durch die Anzahl an Neuronen im aktuellen Layer
            double sum = curr_layer->neurons[j].bias;                                           // Setze die Summe auf den bias des Neurons j im aktuellen Layer
            for(int k = 0; k < prev_layer->num_neurons; k++){                                   // Iteriere durch die Anzahl an Neuronen im Vorgänger-Layer (Inputs für Neuronen im aktuellen Layer)
                sum += prev_layer->neurons[k].activation * curr_layer->neurons[j].weights[k];   // Gewichtete Summe berechnen: Summe(Vorherige Aktivierung * Gewicht)
            }
            curr_layer->neurons[j].weighted_sum = sum;                                          // Setze die Gewichtete Summe des Neurons j im aktuellen Layer auf sum
            curr_layer->neurons[j].activation = sigmoid(curr_layer->neurons[j].weighted_sum);   // Berechne die Activation des Neurons j im aktuellen Layer mit der Sigmoid-Funktion
        }
    }
}

//Backpropagation
// Berechnet die Fehler und passt Gewichte an
void backpropagation(Network* net, int target_label, double learning_rate){
    int last_idx = net->num_layers - 1;                                                                                 // Setze den letzten Layer-Index
    Layer* output_layer = &net->layers[last_idx];                                                                       // Definiere den Outout-Layer

    // 1. Fehler im Output-Layer berechnen
    for(int i = 0; i < output_layer->num_neurons; i++){                                                                 // Iteriere durch die Anzahl an Neuronen im Output-Layer (Klassen)
        double curr_activation = output_layer->neurons[i].activation;                                                   // Setze den Output auf die Activation des Neurons i (der Klasse i)
        double curr_target = (i == target_label) ? 1.0 : 0.0;                                                           // Wenn i == target ist, setze target auf 0 sonst 1
        output_layer->neurons[i].delta = (curr_activation - curr_target) * sigmoid_derivative(curr_activation);         // Setze das delta (Fehler) des Neurons i
    }

    // 2. Fehler rückwärts durch die Hidden Layers propagieren
    for(int i = last_idx - 1; i > 0; i--){                                                                      // Iteriere rückwärts durch die Anzahl an Layern vom Output-Layer bis zum ersten Hidden-Layer 
        Layer* curr_layer = &net->layers[i];                                                                    // Setze den aktuellen Layer
        Layer* next_layer = &net->layers[i+1];                                                                  // Setze den nächsten Layer

        for(int j = 0; j < curr_layer->num_neurons; j++){                                                      // Iteriere durch die Anzahl an Neuronen im aktuellen Layer
            double curr_error_sum = 0.0;                                                                        // Setze die aktuelle Summe der Fehler auf 0
            for(int k = 0; k < next_layer->num_neurons; k++){                                                   // Iteriere durch die Anzahl an Neuronen des nächsten Layer 
                curr_error_sum += next_layer->neurons[k].delta * next_layer->neurons[k].weights[j];             // Addiere zur Fehlersumme
            }
            // Delta = Fehlersumme * Ableitung-Sigmoid(Activation)
            curr_layer->neurons[j].delta = curr_error_sum * sigmoid_derivative(curr_layer->neurons[j].activation);  // Setze den Fehler des Neurons j im aktuellen Layer
        }
    }

    // 3. Gewichte und Bias aktualisieren (Gradient Descent)
    for(int i = 1; i < net->num_layers; i++){                                                       // Iteriere forwärts durch die Layer vom ersten Hidden-Layer bis zum Output-Layer
        Layer* curr_layer = &net->layers[i];
        Layer* prev_layer = &net->layers[i-1];

        for(int j = 0; j < curr_layer->num_neurons; j++){                                           // Iteriere durch die Anzahl an Neuronen im aktuellen Layer
            curr_layer->neurons[j].bias -= learning_rate * curr_layer->neurons[j].delta;            // Bias Update
            for(int k = 0; k < prev_layer->num_neurons; k++){                                       // Iteriere durch die Anzahl an Neuronen im Vorgänger-Layer (Inputs des Neurons im aktuellen Layer)
                curr_layer->neurons[j].weights[k] -= learning_rate * curr_layer->neurons[j].delta * prev_layer->neurons[k].activation;
            }
        }
    }
}

// Training Loop
void train(int samples, int d, double** full_x, int* full_y, Network* network, int epochs, double learning_rate, int test_n, int test_d, double** test_x, int* test_y){
    int num_outputs = network->layers[network->num_layers-1].num_neurons;
    double *loss_arr = malloc(epochs * sizeof(double));                             // Reserviere Speicher für den Trainingsloss für alle Epochen
    double *loss_test_arr = malloc(epochs * sizeof(double));                        // Reserviere Speicher für den Testloss für alle Epochen
    
    for(int e = 0; e < epochs; e++){                                                // Iteriere durch die Anzahl an Epochen
        
        clock_t start_epoch = clock();                                              // Setze den Start-Timer
        double epoch_loss = 0.0;                                                    // Initlialisiere den Trainingsloss
        double epoch_test_loss = 0.0;                                               // Initlialisiere den Testloss

        // Berechne Testloss
        for(int i = 0; i < test_n; i++){                                            // Iteriere durch die Anzahl an Trainingsdatenpunkten
            forward_prop(network, test_x[i]);                                       // Führe Forward-Propagation durch
            double test_error = 0.0;                                                // Initialisiere den Fehler
            for(int j = 0; j < num_outputs; j++){                                   // Iteriere durch die Anzahl an Neuronen im Outputlayer
                double target = (test_y[i] == j) ? 1.0 : 0.0;                       // Setze das Target auf 1 oder 0 (One-Hot-Format)
                double actual = network->layers[network->num_layers-1].neurons[j].activation;   // Setze die Aktivierung des Neurons an Stelle j
                double diff = target - actual;                                      // Berechne die Differenz zwischen Target und Aktivierung
                test_error += 0.5 * diff * diff;                                    // Addiere den Fehler des Neurons zum Gesamtfehler eines Datenpunkts
            }
            epoch_test_loss += test_error;                                          // Addiere den Gesamtfehlers eines Datenpunkts zum Testfehler der Epoche
        }

        // Berechne Trainingsloss und führe Backpropagation durch
        for(int i = 0; i < samples; i++){                                           // Ablauf siehe oben
            forward_prop(network, full_x[i]);
            double sample_error = 0.0;
            for(int j = 0; j <num_outputs; j++){
                double target = (full_y[i] == j) ? 1.0 : 0.0;
                double actual = network->layers[network->num_layers-1].neurons[j].activation;
                double diff = target - actual;
                sample_error += 0.5 * diff * diff;
            }
            epoch_loss += sample_error;
            backpropagation(network, full_y[i], learning_rate);                     // Führe Backward-Propagation durch
        }

        double average_loss = epoch_loss / samples;                                 // Berechne durchschnittlichen Trainingsloss der Epoche
        double average_test_loss = epoch_test_loss / test_n;                        // Berechne durchschnittlichen Testloss der Epoche
        loss_arr[e] = average_loss;                                                 // Füge Trainingsloss an Stelle e zum Array hinzu
        loss_test_arr[e] = average_test_loss;                                       // Füge Testloss an Stelle e zum Array hinzu

        clock_t end_epoch = clock();                                                // Setze End-Timer
        int time_epoch = ( (end_epoch - start_epoch) / CLOCKS_PER_SEC);             // Berchne benötigte Zeit für diese Epoche

        int time_remaining = (epochs - (e + 1)) * time_epoch;                       // Berechne die verbleibende Zeit in Sekunden

        int min_remaining = time_remaining / 60;                                    // Berechne verbleibende Minuten
        int sek_remaining = time_remaining % 60;                                    // Berechne verbleibende Sekunden der letzten Minute

        printf("\rEpoche %d / %d abgeschlossen. Loss: %f Test-Loss: %f Zeit verbleibend: %d min %d sek", e + 1, epochs, average_loss, average_test_loss, min_remaining, sek_remaining); // Konsolen-Ausgabe
        fflush(stdout);
    }
    printf("\n");
    save_loss(loss_arr, loss_test_arr, epochs);
    free(loss_arr);
}

void free_net(Network *net, int n, double **x, int *y, double *recall, double *precision, double *f_value, int test_n, double **test_x, int *test_y){
    free_network(net);
    for(int i=0; i<n; i++){
        free(x[i]);
    }
    free(x);
    free(y);
    free(recall);
    free(precision);
    free(f_value);
    for(int i=0; i<test_n; i++){
        free(test_x[i]);
    }
    free(test_x);
    free(test_y);
}

void create_run(Network *net, int num_layers, int *layer_sizes, int e, double learning_rate, int read_count, int d, double **x, int *y, double *time_per_epoch, int test_n, int test_d, double** test_x, int* test_y){
    if(e > 0){
        printf("Erstelle neues Netzwerk...\n");
        *net = create_network(num_layers, layer_sizes);                                         // Erstelle ein neues Netzwerk

        printf("Starte Training (Epochen: %d, Rate: %.3f)...\n", e, learning_rate);
        clock_t start = clock();                                                                // Setze Start-Timer vor dem Training
        train(read_count, d, x, y, net, e, learning_rate, test_n, test_d, test_x, test_y);      // Trainiere das Modell
        clock_t end = clock();                                                                  // Setze End-Timer nach dem Training

        *time_per_epoch = (((double) (end - start)) / CLOCKS_PER_SEC) / e;                      // Berechne durchschnittliche benötigte Zeit pro Epoche

        save_network_csv(net, "network_parameters.csv");
        printf("Training beendet und gespeichert.\n");
    }else{
        printf("Lade existierendes Netzwerk...\n");
        *net = load_network_csv("network_parameters.csv");                                      // Lade Modell aus "network_parameters.csv"
    }
}