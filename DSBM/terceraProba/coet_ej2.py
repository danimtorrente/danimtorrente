import matplotlib.pyplot as plot
import numpy as np
import random

simul_time = 7200 # 1 hora 



Alts = np.zeros(simul_time)
Vels = np.zeros(simul_time)
Comb = np.zeros(simul_time)

Grav   = -9.8       # m/s2
Altin  = 6371000    # m
Velin  = 0          # m/s
dt     = 0.001      # s pas de temps
m_coet = 10000      # kg massa coet buit
m_comb = 500000      # kg massa de combustible  
Imp_max= 30         # m/s2 impuls maxim
eff_com= 0.003      # eficiencia combustible
alt_consigna= 10000000 
                    # volem posar el coet a 9000km
Maxalt = 0.0
Accact = Grav
Velact = Velin
Altact = Altin
Impuls = 0.0
n=0
m=0
Kp = 0.00001
Ki = 0.5
Kd = 0.3

error_anterior = 0
integral = 0

def PlotAll():
    
    fig = plot.figure(figsize=(12, 7), dpi=80)
    ax1 = fig.add_subplot(121)
    ax1.set_title("Alçada")    
    ax1.set_xlabel('t')
    ax1.set_ylabel('m')
    ax1.grid('ON')
    ax1.plot([0,simul_time],[Altin,Altin],'k-', lw=2)
    ax1.plot([0,simul_time],[Altin+alt_consigna,Altin+alt_consigna],'k--', lw=2)
    ax1.plot(Alts, c='g',label='Alt')
    
    ax1 = fig.add_subplot (222)
    ax1.set_title("Velocitat")    
    ax1.set_xlabel('')
    ax1.set_ylabel('m/s')
    ax1.plot(Vels, c='b',label='Vel')
    
    ax1 = fig.add_subplot (224)
    ax1.set_title("Combustible")    
    ax1.set_xlabel('')
    ax1.set_ylabel('kg')
    ax1.plot(Comb, c='r',label='Comb')
    plot.show()                         #pinta grafica




#**********************************************************************************

def LlegeixSensors():       # Simula la lectura dels sensors. Podem afegir soroll 
                            # per fer realista.
    alt=Altact - Altin      # Sensor d'altitud
    vel=Velact              # Sensor de velocitat
    acc=Accact              # Acceleròmetre
    comb=m_comb             # Combustible
    return(alt,vel,acc,comb)


#**********************************************************************************

def Controla_Coet (t_act,consigna):  

    global error_anterior, integral
                            # Aquest és el codi del micro que regeix el coet.
                            # Es crida cada 1ms    
    altitud, velocitat, accel, combustible = LlegeixSensors()
                            # Obtenim les dades dels sensors


    error = consigna - altitud
    integral = (integral + error) * dt
    derivada = (error - error_anterior) / dt
    impuls = Kp*error + Ki*integral + Kd*derivada
    error_anterior = error

    if (impuls>Imp_max):       # Limitem impuls
        impuls=Imp_max
    if (impuls<0):
        impuls=0
                               # Actuem sobre el Coet controlant Impuls
    return(impuls)
#**********************************************************************************



# SIMULADOR 

while (n<simul_time*1000):              # Pas de simulació 1ms
    
    Impuls=Controla_Coet (n,alt_consigna)
 
    if(Altact<Altin): 
        print("CRASH")                  # Cau a terra
        break
    if(m_comb<=0):                      # No pot haver impuls sense fuel
        Impuls=0
        m_comb=0
    m_comb = m_comb - eff_com * Impuls  # Actualitzem massa combustible
    Mtotal = m_comb + m_coet            # Actualitzem massa total
    Accact = Grav + Impuls              # Actualitzem acceleració
    Velact = Velact + Accact*dt         # Actualitzem velocitat
    Altact = Altact + Velact*dt         # Actualitzem altitud actual
    if (Altact>Maxalt):
        Maxalt=Altact                   # Registren alçada màxima   
    if(n%1000==0):                      # Cada un segon captem dades pel gràfic
        Alts[m]= Altact
        Vels[m]= Velact
        Comb[m]= m_comb
        m=m+1
    n=n+1
    
print("REPORT:")
print("Max alt=",Maxalt-Altin,"Fuel restant=",m_comb)
PlotAll()