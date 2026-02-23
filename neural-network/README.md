--------------------------------------------------------------------------------------------------------------------
				AGKI Projekt Hirsch Korbinian, Hildebrand Nicolas

Untersuchung des Einflusses von Hyperparametern eines in C implementierten Neuronalen Netzes auf die Klassifizierung
--------------------------------------------------------------------------------------------------------------------

- Dieser Ordner enthält alle Dateien, zum Trainieren und Testen eines Neuronalen Netzes.

- Die Datei network_parameters.csv enthält die Gewichte des Netzes mit einer Genauigkeit von 98.14 %.
diese wird beim Ausführen des Programms überschrieben (bitte speichern unter anderem Namen!)

- Kompiliert werden die Dateien wie folgt: gcc main.c logic.c ui.c csv.c -o main

Für die Ausführung des Programms main müssen folgende Parameter übergeben werden:

- die Anzahl an Trainingsdatenpunkte
- die Dimension eines Trainingsdatenpunktes
- den Dateinamen des Trainingsdatensatzes als .csv-Datei (z.B. mnist_train.csv)
- die Anzahl an Testdatenpunkte
- die Dimension eines Testdatenpunktes 
- den Dateinamen des Testdatensatzes als .csv-Datei
- die Anzahl an Neuronen im Hidden-Layer 1
- die Anzahl an Neuronen im Hidden-Layer 2
- die Anzahl an Trainingsepochen
- die Learningrate (z.B. 0.1)
- die Anzahl an Klassen

Falls kein neues Netz erstellt werden soll, sondern ein vorhandenes Modell geladen werden soll, setze die Anzahl Epochen auf 0

Die gesamten Parameter und Ergebnisse werden in der Datei experiment.csv gespeichert

Die Architektur und Gewichte werden in der Datei network_parameters.csv gespeichert

Der Trainings- und Testloss aller Epochen beim Training wird in der Datei loss.csv gespeichert

