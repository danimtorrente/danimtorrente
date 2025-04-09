import pydoc
import matplotlib
import re
import matplotlib.pyplot as plt
from matplotlib import cm
import subprocess


def inputData():
    with open("entry.txt", 'w') as entry:
        x = int(input("Insert the number of tasks"))
        print("Insert data in the following format: Ci Ti Di\n")

        for i in range(x):
            entry.write(input(f"Data for task {i+1}: ") + "\n")

manual = input("Do you wish to insert manually your data? (y/n): ")

if manual == 'y':   #Manual
    inputData()
else:               #File
    print("Make sure your file's name is entry.txt and is located within the root folder of this project.\n")

# Ejecutar comando para ejecutar main en C y que escriba datos en fichero llamado salida.txt
# try:
#     with open("entry.txt", 'r') as entry, open("output.txt", 'w') as output:
#         subprocess.run(["./main"], stdin= entry, stdout= output, check=True)
# except Exception as ex:
#     print(f"Error executing the program: {ex}")


# # Leer datos fichero.txt y plotear datos
# with open("output.txt", 'r') as data:
#     result = data.read
#     print(result) # FALTA HACER EL PLOT
    #Para plotear habrá que decidir un formato para justo antes de cada salida de cada scheduler poner una linea donde se indique el scheduler

def parse_output(filename):
    schedulers_data = {}
    current_scheduler = None
    global_hyperperiod = None

    with open(filename, 'r') as f:
        lines = f.readlines()

    for line in lines:
        line = line.strip()
        # Capturar hiperperíodo global
        if line.startswith('HYPERPERIOD'):
            h = int(line.split()[1])
            if current_scheduler:
                schedulers_data[current_scheduler]['hyperperiod'] = h
            else:
                global_hyperperiod = h
        # Manejar inicio/fin de scheduler
        elif line.startswith('SCHEDULER_'):
            parts = line.split('_')
            if '_START' in line:
                current_scheduler = parts[1]
                schedulers_data[current_scheduler] = {
                    'tasks': [],
                    'hyperperiod': None
                }
            elif '_END' in line:
                current_scheduler = None
        # Capturar tareas
        elif current_scheduler and line.startswith('TASK'):
            parts = line.split()
            task_id = int(parts[1])
            start = int(parts[2])
            end = int(parts[3])
            schedulers_data[current_scheduler]['tasks'].append((task_id, start, end))
    
    return schedulers_data, global_hyperperiod

def plot_gantt(schedulers_data, global_hyperperiod):
    for scheduler, data in schedulers_data.items():
        tasks = data['tasks']
        if not tasks:
            continue
        h = data.get('hyperperiod', global_hyperperiod)
        
        fig, ax = plt.subplots(figsize=(12, 6))
        task_ids = sorted(set(t[0] for t in tasks))
        colors = cm.get_cmap('tab10', len(task_ids))
        
        for tid, start, end in tasks:
            ax.barh(y=tid, width=end - start, left=start, 
                    color=colors(tid), edgecolor='black', height=0.5)
        
        ax.set_yticks(task_ids)
        ax.set_yticklabels([f'Task {tid}' for tid in task_ids])
        ax.set_xlabel('Time (ms)')
        ax.set_ylabel('Tasks')
        ax.set_title(f'Schedule - {scheduler}')
        ax.set_xlim(0, h if h is not None else max(t[2] for t in tasks))
        ax.grid(True, axis='x', linestyle='--', alpha=0.7)
        
        plt.tight_layout()
        plt.savefig(f'{scheduler}_schedule.png')
        plt.close()

if __name__ == "__main__":
    # Ejecutar el programa C++ y generar output.txt
    try:
        with open("entry.txt", 'r') as entry, open("output.txt", 'w') as output:
            subprocess.run(["./main"], stdin=entry, stdout=output, check=True)
    except Exception as ex:
        print(f"Error: {ex}")
        exit(1)
    
    # Parsear y graficar
    schedulers_data, global_hyperperiod = parse_output('output.txt')
    plot_gantt(schedulers_data, global_hyperperiod)
    print("Gráficos generados exitosamente.")    