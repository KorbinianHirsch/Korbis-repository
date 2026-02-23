#ifndef _COMMON_H
#define _COMMON_H 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

typedef struct {
    int num_inputs;       // Anzahl der Eingänge von der vorherigen Schicht
    double* weights;      // Gewichte für jeden Eingang
    double weighted_sum;  // Gewichtete Summe (vor der Aktivierungsfunktion)
    double activation;    // Der endgültige Ausgabewert des Neurons (nach Sigmoid)
    double bias;          // Der Bias-Wert (Verschiebung)
    double delta;         // Fehlerwert für Backpropagation
} Neuron;

typedef struct {
    int num_neurons;      // Wie viele Neuronen sind in dieser Schicht?
    Neuron* neurons;      // Array der Neuronen
} Layer;

typedef struct {
    int num_layers;       // Gesamtzahl der Schichten (Input + Hidden + Output)
    Layer* layers;        // Array der Schichten
} Network;

#endif