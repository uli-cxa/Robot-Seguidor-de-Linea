# Robot Seguidor de Línea PID

## Descripción General

Este proyecto presenta el diseño y simulación de un robot seguidor de línea controlado mediante un controlPID.

El sistema utiliza un arreglo de sensores reflectivos QTR-8A para detectar la trayectoria y ajustar la velocidad de los motores con el objetivo de mantener el seguimiento de la línea en curvas. Adicionalmente, se desarrolló un entorno de simulación en CoppeliaSim para validar el comportamiento del robot antes de las pruebas experimentales fisicas.

---

## Hardware Utilizado

| Componente | Descripción |
|------------|-------------|
| ESP32 DevKit V1 | Microcontrolador principal |
| Sensor QTR-8A | Arreglo de 8 sensores reflectivos |
| Motores N20 con encoder | Sistema de tracción diferencial |
| Driver L298N | Controlador de motores |
| Chasis 2WD | Plataforma mecánica |
| Baterías AA recargables | Alimentación del sistema |

La lista completa de materiales se encuentra en:

```text
hardware/Lista_Componentes.xlsx
```

---

## Software Utilizado

- Arduino IDE
- CoppeliaSim
- Lenguaje C++
- Biblioteca QTRSensors
- GitHub

## Simulación en CoppeliaSim

Se desarrolló un entorno de simulación para validar:

- El comportamiento cinemático del robot diferencial.
- La detección de trayectoria mediante sensores virtuales.
- La respuesta del algoritmo de seguimiento.
- La geometría de la pista utilizada en las pruebas.

Los archivos de simulación se encuentran en:

```text
simulacion/
```

---

## Resultados Experimentales

Se realizaron pruebas sobre el prototipo físico para evaluar su desempeño.

### Prueba 1: Curvas Cerradas

Se evaluó la capacidad de recuperación del robot cuando pierde temporalmente la detección de la línea.

Resultados observados:

- El robot puede mantener la dirección utilizando el último error registrado durante aproximadamente 3 segundos.
- Puede separarse entre 10 cm y 15 cm de la trayectoria antes de recuperarla.
- En la mayoría de los casos logra reincorporarse a la línea sin detenerse.

### Prueba 2: Perturbación Manual

Se aplicaron perturbaciones externas desplazando manualmente el robot fuera de la trayectoria.

Resultados observados:

- El sistema logra corregir desviaciones de aproximadamente 5 cm a 8 cm respecto a la línea.
- El controlador recupera la trayectoria de forma autónoma.
- Se verificó la robustez básica del sistema frente a perturbaciones externas.

---

## Estructura del Repositorio

```text
Robot-Seguidor-de-Linea-PID/
│
├── README.md
│
├── docs/
│   ├── SistemasDinamicos_EntregaFinal.pdf
│   ├── diagrama_arquitectura.jpeg
│   ├── base.jpeg
│   └── coppelia.jpeg
│
├── hardware/
│   └── Lista_Componentes.xlsx
│
├── software/
│   └── Robot_PID.ino
│
├── simulacion/
│   ├── Line_follower.ttt
│   └── trayectoria.png
│
├── resultados/
│   ├── video_prueba_1.mp4
│   ├── video_prueba_2.mp4
│   └── robot_Armado.jpeg
│
└── LICENSE
```

---

## Evidencias

Las evidencias experimentales incluyen:

- Fotografías del robot físico.
- Entorno de simulación en CoppeliaSim.
- Videos de pruebas experimentales.
- Reporte técnico final.

---

## Autor

Ulises Chávez

Ingeniería en Instrumentación Electrónica

---

## Licencia

Este proyecto se distribuye bajo la licencia MIT.