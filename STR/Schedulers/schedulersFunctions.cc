#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include "schedulersFunctions.h"
#include "customFunctions.h"

schedulers::schedulers(){
    UTILIZATION_FACTOR = 0.0;
    MAX_COST = -1;
    MIN_DEADLINE = 10000;
    NUMBER_OF_TASKS = 0;
    h = -1;
}

void schedulers::read() {
    char linea[MAXIMUM_CHARACTERS_INLINE];
    int exit_code = -1;
    float c_aux;
    int t_aux, d_aux;

    while (fgets(linea, sizeof(linea), stdin)) {
        exit_code = sscanf(linea, "%f %d %d", &c_aux, &t_aux, &d_aux);
        if (exit_code != 3) {
            printf("Error: wrong format\n");
            continue;
        }

        // Agregar a los vectores
        c.push_back(c_aux);
        t.push_back(t_aux);
        d.push_back(d_aux);
        orderRate.insert(make_pair(t_aux, NUMBER_OF_TASKS));
        orderDead.insert(make_pair(d_aux, NUMBER_OF_TASKS));
        orderCost.insert(make_pair(c_aux, NUMBER_OF_TASKS));

        // Actualizar valores globales
        UTILIZATION_FACTOR += (double)c_aux / (double)t_aux;
        MAX_COST = MAX_COST > c_aux ? MAX_COST : c_aux;
        MIN_DEADLINE = (NUMBER_OF_TASKS == 0) ? d_aux : min(MIN_DEADLINE, d_aux);

        NUMBER_OF_TASKS++;
    }
}


int schedulers::check() {
    if (MAX_COST - MIN_DEADLINE > 0) {
        printf("El sistema no es planificable, revise los datos de entrada, hay un coste mayor que un deadline\n");
        return -1;
    }
    
    if (UTILIZATION_FACTOR <= 1) printf("El sistema cumple la condicion del factor de utlización: %f <= 1\n", UTILIZATION_FACTOR);
    else {
        printf("El sistema no cumple la condicion del factor de utlización: %f <= 1\n", UTILIZATION_FACTOR);
        return -1;
    }

    h = lcm(t, NUMBER_OF_TASKS);// HyperPeriod (𝐻) --> Es el mínimo común múltiplo (LCM, por sus siglas en inglés) de los períodos de todas las tareas en el sistema. El hiperperíodo es el tiempo más corto después del cual el patrón de ejecución de las tareas se repite.
    if (h < 0) return -1;
    printf("El sistema tiene el siguiente hiperperíodo (H): %d ms\n", h);


    return 0;
}

void schedulers::cyclicScheduler() {
    printScheduleHeader("CYCLIC");
    int original_tasks = NUMBER_OF_TASKS;
    bool success = false;
    int limit = 0;

    while (!success && limit < 5) {
        // Paso 1: Buscar posibles valores de Ts
        vector<int> posibles_ts;
        Ts = MIN_DEADLINE;
        
        while (Ts >= MAX_COST) {
            if (h % Ts == 0) {
                posibles_ts.push_back(Ts);
            }
            Ts--;
        }

        // Paso 2: Probar cada Ts potencial
        for (int current_ts : posibles_ts) {
            // Verificar si es válido con la configuración actual
            if (validateCyclicSchedule(current_ts)) {
                Ts = current_ts;
                success = true;
                break;
            }
        }

        // Paso 3: Si no se encuentra, dividir tareas
        if (!success) {
            splitTasks();
            limit++;
            cout << "Reintentando con " << NUMBER_OF_TASKS << " tareas..." << endl;
        }
    }

    // Paso 4: Mostrar resultados
    if (success) {
        multimap<int, int> deadline_order;
        for (int i = 0; i < NUMBER_OF_TASKS; ++i) {
            deadline_order.insert({d[i], i});
        }
        
        int current_time = 0;
        while (current_time < h) {  // Todo el hiperperíodo
            for (auto& task : deadline_order) {
                int task_id = task.second;
                if (current_time % t[task_id] == 0) {
                    int end_time = current_time + static_cast<int>(c[task_id]);
                    printScheduleEvent(task_id, current_time, end_time);
                    current_time = end_time;
                }
            }
        }
    }

    printScheduleFooter("CYCLIC");
}

//Rate Monotonics
void schedulers::rateMonotonic(){
    printScheduleHeader("RATE_MONOTONIC");
    // condition 1
    double aux = (float(NUMBER_OF_TASKS)*float((pow(2,(1.0/NUMBER_OF_TASKS))-1)));
    if (UTILIZATION_FACTOR <= aux) cout << "CONDITION 1 FULLFILED: ";
    else cout << "CONDITION 1 NOT FULLFILED: ";
    cout << "U = " << UTILIZATION_FACTOR << "<= " << aux << endl;
    // condition 2
    aux = 1;
    for (int i = 0; i < NUMBER_OF_TASKS; ++i) {
        aux *= c[i]/t[i];
    }
    if (aux <= 2) {
        cout << "CONDITION 2 FULLFILED: "; 
    }
    else cout << "CONDITION 2 NOT FULLFILED: ";
    cout << "(U1 + 1)*(U2 + 1)...*(Un + 1) = " << UTILIZATION_FACTOR << "<= " << 2 << endl;
    //response time analysis
    cout << "----RESPONSE TIME ANALYSIS----" << endl;
        
    bool success = true;
    int counter = 1;
    for (auto it = orderRate.begin(); it != orderRate.end(); ++it) {                
        int W = 0;
        cout << "Task " << it->second << ":" << endl;
        for (int i = 0; i < counter; ++i) {
            W = c[it->second] + (((W + t[i] - 1) / t[i])*c[i]);
            cout << i << "--> W = " << W << " <= " << d[it->second] << endl;
            if (W > d[it->second]) {
                success = false;
            }
        }
        ++counter;
    }
    
    if (!success) {
        cout << "RESPONSE TIME ANALYSIS FAILED" << endl;
    } else {
        cout << "RESPONSE TIME ANALYSIS WAS SUCCESFUL" << endl;
    }
    printScheduleFooter("RATE_MONOTONIC");
}

//Deadline Monotonic
void schedulers::deadlineMonotonic(){
    printScheduleHeader("DEADLINE_MONOTONIC");
    cout << "----RESPONSE TIME ANALYSIS----" << endl;
        
    bool success = true;
    int counter = 1;
    for (auto it = orderDead.begin(); it != orderDead.end(); ++it) {                
        int W = 0;
        cout << "Task " << it->second << ":" << endl;
        for (int i = 0; i < counter; ++i) {
            W = c[it->second] + (((W + t[i] - 1) / t[i])*c[i]);
            cout << i << "--> W = " << W << " <= " << d[it->second] << endl;
            if (W > d[it->second]) {
                success = false;
            }
        }
        ++counter;
    }
    
    if (!success) {
        cout << "RESPONSE TIME ANALYSIS FAILED" << endl;
    } else {
        cout << "RESPONSE TIME ANALYSIS WAS SUCCESFUL" << endl;
    }
    printScheduleFooter("DEADLINE_MONOTONIC");
}

//Earliest Deadline First
void schedulers::EDF() {
    printScheduleHeader("EDF");
    
    // *** Paso 1: Verificación inicial de deadlines iguales a períodos
    cout << "Paso 1: Verificando si deadlines (d[i]) son iguales a períodos (t[i])..." << endl;
    bool condition = false;
    double l = 0;
    for (int i = 0; i < NUMBER_OF_TASKS; ++i) {
        if (d[i] != t[i]) {
            cout << "  - Tarea " << i+1 << ": d[i] (" << d[i] << ") ≠ t[i] (" << t[i] << ")" << endl;
            condition = true;
        } else {
            cout << "  - Tarea " << i+1 << ": d[i] == t[i] (" << t[i] << ")" << endl;
        }
        l += ((t[i]-d[i])*(c[i]/t[i]));
    }
    cout << "\nValor inicial de l = " << l << endl << endl;

    // *** Paso 2: Caso especial (deadlines = períodos)
    if (!condition) {
        cout << "Paso 2: Todos los deadlines son iguales a sus períodos → ES PLANIFICABLE" << endl;
        //l = h; // REVISAR
    }
    else l /= (1.0-UTILIZATION_FACTOR);

    // *** Paso 3: Cálculo del límite de planificación
    cout << "Paso 3: Calculando límite de planificación..." << endl;
    
    double mind = l < h ? l : h;
    cout << "  - l ajustado = " << l << endl;
    cout << "  - h = " << h << endl;
    cout << "  - mind = mín(l, h) = " << mind << endl << endl;

    // *** Paso 4: Generación de deadlines críticos
    cout << "Paso 4: Generando deadlines críticos hasta " << mind << "..." << endl;
    vector<int> deadlines;
    vector<bool> control(NUMBER_OF_TASKS, false);
    condition = false;
    int k = 0;
    float aux;
    
    while (!condition) {
        cout << "\nIteración k = " << k << ":" << endl;
        condition = true;
        
        for (int i = 0; i < NUMBER_OF_TASKS; ++i) {
            if (!control[i]) {
                condition = false;
                aux = (k*t[i]) + d[i];
                
                cout << "  - Tarea " << i+1 << ": ";
                cout << "k*t[i] + d[i] = " << k << "*" << t[i] << " + " << d[i] << " = " << aux;
                
                if (aux < mind) {
                    deadlines.push_back(aux);
                    cout << " → Añadido a deadlines" << endl;
                } else {
                    control[i] = true;
                    cout << " → Supera mind (" << mind << "). Tarea controlada." << endl;
                }
            }
        }
        ++k;
    }
    
    // *** Paso 5: Verificación final de planificabilidad
    cout << "\nPaso 5: Verificando deadlines críticos (" << deadlines.size() << " puntos)..." << endl;
    for (int i = 0; i < deadlines.size(); ++i) {
        cout << "  - Deadline " << i+1 << "/" << deadlines.size() << ": " << deadlines[i];
        if (g(0, deadlines[i]) == false) {  // *** Asumo que g() verifica disponibilidad
            cout << " → FALLO EN ESTE PUNTO. NO PLANIFICABLE." << endl;
            return;
        }
        cout << " → OK" << endl;
    }
    cout << "\n=== RESULTADO FINAL: ES PLANIFICABLE ===" << endl;

    printScheduleFooter("EDF");
}


// UTILS

// EDF
bool schedulers::g(int result, int y) { 
    for (int i = 0; i < NUMBER_OF_TASKS; ++i) {
        result += ((y+t[i]+d[i])/t[i])*c[i]; // asegurarse de que trunca
    }
    if (result <= y) return true;
    return false; 
}

void schedulers::splitTasks(){ 
    auto it = --orderCost.end();
    int original_task = it->second;
    
    // Dividir la tarea
    float new_cost = c[original_task] / 2.0;
    c[original_task] = new_cost;
    
    // Crear nueva tarea
    c.push_back(new_cost);
    t.push_back(t[original_task]);
    d.push_back(d[original_task]);
    
    // Actualizar multimaps
    orderCost.insert(make_pair(new_cost, NUMBER_OF_TASKS));
    orderRate.insert(make_pair(t[original_task], NUMBER_OF_TASKS));
    orderDead.insert(make_pair(d[original_task], NUMBER_OF_TASKS));
    
    NUMBER_OF_TASKS++;
    
    // Actualizar valores globales
    UTILIZATION_FACTOR += new_cost / t[original_task];
    MAX_COST = *max_element(c.begin(), c.end());
    MIN_DEADLINE = *min_element(d.begin(), d.end());

    h = lcm(t, NUMBER_OF_TASKS);
    cout << "Nuevo hiperperíodo: " << h << " ms" << endl;
}

bool schedulers::validateCyclicSchedule(int Ts) {
    if (Ts < MAX_COST) return false;
    if (h % Ts != 0) return false;
    
    float total_utilization = 0;
    for (int i = 0; i < NUMBER_OF_TASKS; ++i) {
        int instances = t[i] / Ts;
        if (instances == 0) return false;
        
        float required = c[i] * instances;
        if (required > Ts) return false;
        
        total_utilization += static_cast<float>(required) / Ts;
    }
    
    return total_utilization <= 1.0;
}

void schedulers::printScheduleHeader(const string& schedulerName) {
    cout << "SCHEDULER_" << schedulerName << "_START\n";
}

void schedulers::printScheduleEvent(int taskId, int start, int end) {
    cout << "TASK " << taskId << " " << start << " " << end << "\n";
}

void schedulers::printScheduleFooter(const string& schedulerName) {
    cout << "SCHEDULER_" << schedulerName << "_END\n";
}

/*
FALTA:
    - PLOT
*/