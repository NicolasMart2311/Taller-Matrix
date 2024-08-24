#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Función para crear una matriz dinámica
int** createMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    return matrix;
}

// Función para liberar la memoria de una matriz dinámica
void deleteMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Función para guardar una matriz en un archivo de texto con un encabezado
void saveMatrixToTextFile(int** matrix, int rows, int cols, const char* filename, const string& header) {
    ofstream outfile(filename, ios::app);  // Abrir en modo de agregar para no sobrescribir el archivo

    outfile << header << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            outfile << matrix[i][j] << " ";
        }
        outfile << endl;
    }

    outfile.close();
}

// Función para modificar un elemento de la matriz
void modifyMatrixElement(int** matrix, int row, int col, int newValue) {
    matrix[row][col] = newValue;
}

// Función para multiplicar matrices
int** multiplyMatrices(int** matrixA, int** matrixB, int rowsA, int colsA, int colsB) {
    int** result = createMatrix(rowsA, colsB);
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return result;
}

// Función para guardar el contenido de un archivo de texto en un archivo binario
void saveToBinaryFile(const string& inputFileName, const string& outputFileName) {
    ifstream infile(inputFileName);
    ofstream outfile(outputFileName, ios::binary);

    if (!infile || !outfile) {
        cerr << "Error al abrir los archivos." << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        // Escribir la línea en el archivo binario
        outfile.write(line.c_str(), line.size());
        // Escribir un salto de línea
        outfile.write("\n", 1);
    }

    infile.close();
    outfile.close();
}

// Función para leer un archivo binario y mostrar su contenido en la consola
void readBinaryFileAndDisplay(const string& filename) {
    ifstream infile(filename, ios::binary);

    if (!infile) {
        cerr << "No se pudo abrir el archivo binario." << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        cout << line << endl;
    }

    infile.close();
}

// Función para sobrescribir un valor específico en el archivo de texto
void overwriteMatrixValueInTextFile(const char* filename, int matrixNumber, int row, int col, int newValue, int rows, int cols) {
    ifstream infile(filename);
    ofstream tempFile("temp.txt");

    string line;
    int currentMatrix = 0;
    int currentRow = 0;
    bool modified = false;

    while (getline(infile, line)) {
        if (line.find("Datos de la matriz") != string::npos) {
            currentMatrix++;
            tempFile << line << endl;
            currentRow = 0;
            continue;
        }

        if (currentMatrix == matrixNumber && currentRow == row) {
            istringstream iss(line);
            vector<string> elements;
            string element;
            while (iss >> element) {
                elements.push_back(element);
            }
            elements[col] = to_string(newValue);  // Modificar el valor
            for (const auto& e : elements) {
                tempFile << e << " ";
            }
            tempFile << endl;
            modified = true;
        } else {
            tempFile << line << endl;
        }

        currentRow++;
    }

    infile.close();
    tempFile.close();

    remove(filename);
    rename("temp.txt", filename);

    if (!modified) {
        cerr << "No se pudo modificar el valor en el archivo." << endl;
    }
}

// Función para mostrar una matriz en la consola
void displayMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int choice, rowsA, colsA, rowsB, colsB;
    int operationCount = 1;
    int** matrixA = nullptr;
    int** matrixB = nullptr;
    int** resultMatrix = nullptr;
    string inputFileName, outputFileName;
    char modify;

    cout << "Calculadora de Operaciones de Matrices" << endl;

    do {
        cout << "Seleccione una operación:" << endl;
        cout << "1. Multiplicación de matrices" << endl;
        cin >> choice;

        // Solicitar las dimensiones de la primera matriz
        cout << "Ingrese el número de filas y columnas de la primera matriz: ";
        cin >> rowsA >> colsA;

        // Crear y llenar la primera matriz
        matrixA = createMatrix(rowsA, colsA);
        cout << "Ingrese los valores de la primera matriz:" << endl;
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsA; j++) {
                cin >> matrixA[i][j];
            }
        }

        // Guardar la primera matriz en un archivo de texto con encabezado y mostrarla en consola
        inputFileName = "matrix_input_" + to_string(operationCount) + ".txt";
        saveMatrixToTextFile(matrixA, rowsA, colsA, inputFileName.c_str(), "Datos de la matriz 1");
        cout << "Primera matriz:" << endl;
        displayMatrix(matrixA, rowsA, colsA);

        // Solicitar las dimensiones de la segunda matriz si es necesario
        if (choice == 1) {
            cout << "Ingrese el número de filas y columnas de la segunda matriz: ";
            cin >> rowsB >> colsB;

            // Verificar que las dimensiones sean compatibles para la multiplicación
            if (colsA != rowsB) {
                cerr << "Error: las dimensiones no son compatibles para la multiplicación." << endl;
                deleteMatrix(matrixA, rowsA);
                return 1;
            }

            matrixB = createMatrix(rowsB, colsB);
            cout << "Ingrese los valores de la segunda matriz:" << endl;
            for (int i = 0; i < rowsB; i++) {
                for (int j = 0; j < colsB; j++) {
                    cin >> matrixB[i][j];
                }
            }

            // Guardar la segunda matriz en el archivo de texto con encabezado y mostrarla en consola
            saveMatrixToTextFile(matrixB, rowsB, colsB, inputFileName.c_str(), "Datos de la matriz 2");
            cout << "Segunda matriz:" << endl;
            displayMatrix(matrixB, rowsB, colsB);
        }

        // Preguntar si se desea modificar algún valor en las matrices
        cout << "¿Desea modificar algún valor en las matrices? (s/n): ";
        cin >> modify;

        if (modify == 's' || modify == 'S') {
            int matrixNumber, rowToModify, colToModify, newValue;
            cout << "Indique qué matriz desea modificar (1 para la primera, 2 para la segunda): ";
            cin >> matrixNumber;
            cout << "Ingrese la fila y columna del valor a modificar: ";
            cin >> rowToModify >> colToModify;

            // Mostrar el valor actual
            int currentValue = (matrixNumber == 1) ? matrixA[rowToModify][colToModify] : matrixB[rowToModify][colToModify];
            cout << "Valor actual: " << currentValue << endl;

            // Modificar el valor
            cout << "Ingrese el nuevo valor: ";
            cin >> newValue;
            if (matrixNumber == 1) {
                modifyMatrixElement(matrixA, rowToModify, colToModify, newValue);
                overwriteMatrixValueInTextFile(inputFileName.c_str(), matrixNumber, rowToModify, colToModify, newValue, rowsA, colsA);
            } else {
                modifyMatrixElement(matrixB, rowToModify, colToModify, newValue);
                overwriteMatrixValueInTextFile(inputFileName.c_str(), matrixNumber, rowToModify, colToModify, newValue, rowsB, colsB);
            }
        }

        // Realizar la operación seleccionada
        if (choice == 1) {
            resultMatrix = multiplyMatrices(matrixA, matrixB, rowsA, colsA, colsB);
        }

        // Guardar el resultado en un archivo de texto con encabezado
        outputFileName = "result_output.txt";
        ofstream outfile(outputFileName, ios::app);
        outfile << "Operacion Numero " << operationCount << endl;
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                outfile << resultMatrix[i][j] << " ";
            }
            outfile << endl;
        }
        outfile.close();

        // Guardar el archivo de resultados en un archivo binario
        saveToBinaryFile(outputFileName, "result_output.bin");

        // Leer el archivo binario y mostrar su contenido
        cout << "Contenido del archivo binario:" << endl;
        readBinaryFileAndDisplay("result_output.bin");

        cout << "Resultado de la operación guardado en " << outputFileName << endl;

        // Liberar memoria
        deleteMatrix(matrixA, rowsA);
        if (matrixB) deleteMatrix(matrixB, rowsB);
        if (resultMatrix) deleteMatrix(resultMatrix, rowsA);

        cout << "¿Desea realizar otra operación? (s/n): ";
        cin >> modify;
        if (modify == 's' || modify == 'S') {
            operationCount++;
        }

    } while (modify == 's' || modify == 'S');

    return 0;
}
