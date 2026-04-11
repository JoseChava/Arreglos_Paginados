# Arreglos_Paginados

## Requisitos
- Sistema operativo: Windows
- Compilador: g++ (MinGW) o Visual Studio Community 2026 (Importante que sea la versión 2026)

## Compilación de los ejecutables
Primeramente, es importante indicar que de no usar la versión 2026 de Visual Studio, es probable que el archivo tenga problemas para poder ser compilado, por tanto, se recomienda hacer uso de la versión 2026 de Visual Studio Community o en su defecto utilizar g++ como también se enseñará a continuación.
El primer paso es clonar el repositorio desde la pestaña de Código como se muestra a continuación:

<img width="1916" height="934" alt="image" src="https://github.com/user-attachments/assets/38cc032a-2dff-4254-9d0b-c663ea46e32f" />

Aquí copie el URL del repositorio y diríjase al explorador de archivos en documentos para la clonación del repositorio. Una vez se encuentre en documentos, borre lo que este puesto en la barra de direcciones, escriba "cmd" y haga click en Enter para entrar al CMD desde esa carpeta.

<img width="2560" height="852" alt="image" src="https://github.com/user-attachments/assets/b8a348ae-7a84-4de4-87b1-0a42d759d6e2" />

Una vez se encuentre en el CMD, escriba el comando git clone más el link del repositorio que copió anteriormente como se muestra:

<img width="2560" height="606" alt="image" src="https://github.com/user-attachments/assets/89acf25b-163a-420a-8948-e1ae904c5f40" />

Cuando se termine de clonar el repositorio, busque la carpeta correspondiente dentro de documentos e ingrese en ella, aquí encontrará las carpetas de los programas sorter y generator. 

<img width="2560" height="776" alt="image" src="https://github.com/user-attachments/assets/388541cb-6f0a-489f-a7d9-e0b998531fb4" />

A partir de aquí la forma de ejecutar los programas varía según el programa que se decida usar (VS Studio o g++). 

## Con g++:
Si cuenta con el compilador g++ y es de su preferencia siga el siguiente procedimiento para poder construir los ejecutables y poder ejecutar los programas de generación de archivos y ordenamiento. 

### Creacion de carpeta de pruebas y compilado de ejecutables
El primer paso es desde la carpeta principal del repositorio escriba "cmd" en la barra de direcciones y de enter para ingresar al CMD. Una vez en el CMD ejecute el comando: **mkdir pruebas** para crear una nueva carpeta **pruebas** en la raíz del repositorio como se muestra:

 <img width="2560" height="628" alt="image" src="https://github.com/user-attachments/assets/ac415435-edb2-4b43-be23-e8aa65071b15" />

 Luego cierre CMD y diríjase a la carpeta **generator** en la raíz del repositorio y una vez dentro ingrese al CMD como ya se explicó desde la barra de direcciones. Una vez aquí, ejecute el comando: **g++ -O2 -o ..\pruebas\generator generator.cpp** para compilar y crear el ejecutable del generador de archivos en la carpeta **pruebas** creada anteriormente. 

 <img width="2560" height="484" alt="image" src="https://github.com/user-attachments/assets/ae7aaba5-a2d6-46a3-8fe3-a4f6e665f5be" />

Una vez hecho esto cierre el CMD y diríjase nuevamente a la raíz del repositorio y entre en la carpeta **sorter**, ejecute ahora en CMD dentro de la carpeta **sorter** el siguiente comando para compilar el programa de ordenado de los archivos: **g++ -O2 -o ..\pruebas\sorter sorter.cpp PagedArray.cpp ..\algoritmos_ordenamiento\Comb_Sort.cpp ..\algoritmos_ordenamiento\Heap_Sort.cpp ..\algoritmos_ordenamiento\Merge_Sort.cpp ..\algoritmos_ordenamiento\Quick_Sort.cpp ..\algoritmos_ordenamiento\Radix_Sort.cpp** este se encarga de crear el ejecutable **sorter** compilando los archivos de sorter.cpp y PagedArray.cpp los cuales son necesarios para el funcionamiento del programa de ordenado.

 <img width="2560" height="528" alt="image" src="https://github.com/user-attachments/assets/f6e5e43a-9459-4440-82af-f4e0062e0231" />

 Una vez compilados y creados abos ejecutables se peude empezar a probar el programa utilizando CMD desde la carpeta de **pruebas** donde encontrará ambos archivos .exe necesarios para poder probar la generación y ordenamiento de archivos. 
## Con Visual Studio Community:
### Creacion de carpeta de pruebas y compilado de ejecutables
En caso de contar con Visual Studio Community 2026 se puede realizar la construcción de los archivos ejecutables directamente desde el programa sin necesidad de utilizar CMD. Para esto desde la carpeta raíz del repositorio haga doble click sobre el archivo con extensión **.slnx** y ábralo utilizando el Visual Studio Community 2026 como se muestra:

<img width="2560" height="938" alt="image" src="https://github.com/user-attachments/assets/79483787-b9b2-4db1-b5c4-643c0e354b56" />

Una vez dentro de la solución asegúrese de poner el archivo en **Release** en la pestaña que se muestra en a continuación para un mejor desempeño en tiempos de ejecución del programa sorter:

<img width="2550" height="706" alt="image" src="https://github.com/user-attachments/assets/07c06ecd-b2ec-4cff-abb7-578abb2dbf93" />

Una vez configurado en **Release** haga click en la barra de arriba en la opción **Build** para desplegar las opciones de construcción de ejecutables, en esta pestaña seleccione la opción **Build Solution** y solo espere a que Visual Studio cree los dos ejecutables de cada programa.

<img width="2034" height="498" alt="image" src="https://github.com/user-attachments/assets/bd7d1e8a-d279-4e0a-9e6a-ddd7f84b6b9a" />

Cuando vea que ambos ejecutables compilaron y se crearon correctamente vuelva a la raíz del repositorio en el explorador de archivos donde verá una nueva carpeta llamada **x64**, ingrese en esta carpeta y luego ingrese a la carpeta **Release** donde encontrará ambos archivos ejecutables listos para utililzarse.

<img width="2560" height="806" alt="image" src="https://github.com/user-attachments/assets/8d56211c-7f5e-48c7-b7a2-6dc8c1ac12fd" />

Una vez aquí el procedimeinto es el mismo independientemente si los ejecutables se crearon mediante Visual Studio o g++. A partir de ahora se trabajará siempre desde la carpeta **pruebas** en caso que haya elegido utilizar g++ o la carpeta **Release** generada por Visual Studio.
## Creación del archivo numérico
Se continuará utilizando los archivos creados en la carpeta **Release**, sin embargo, el procedimeinto sería el mismo desde la carpeta **Pruebas**. Una vez aquí se escribirá en la barra de direcciones "cmd" para acceder al CMD y ejecute el siguiente comando para crear un archivo de tamaño SMALL (256MB) de números en binario (4B por cada número) como se muestra:

<img width="2560" height="498" alt="image" src="https://github.com/user-attachments/assets/07120252-4e55-404e-bec8-d848d0698f66" />

Note que debe colocar primero el nombre del ejecutable, en este caso **generator** seguido de **-size** y el tamaño del archivo, los tamaños disponibles son:
- P1: 8MB
- P2: 16MB
- P3: 32MB
- P4: 64MB
- P5: 128MB
- SMALL: 256MB
- MEDIUM: 512MB
- LARGE: 1GB
  
Por último ingrese **-output** seguido del path que tendrá el archivo resultante, en este caso solo se ingresa el nombre del archivo (archivoSMALL) ya que la idea es que todo se cree dentro de la misma carpeta para facilidad. A continuación se muestra el resultado de crear un nuevo archivo tamaño **SMALL** dentro de la carpeta **Release**:

<img width="2560" height="676" alt="image" src="https://github.com/user-attachments/assets/d78f9577-78ce-4e46-9362-6fd61366de74" />

Note que el tamaño reportado del **archivoSMALL** coincide con 256MB pues 262144KB*(1MB/1024KB)=256MB.
Una vez hecho esto regrese a la misma pestaña de CMD en la que creó el **archivoSMALL** y coloque el sigueinte comando para poder ejecutar el sorter y poder proceder a ordenar el archivo generado anteriormente como sigue:

<img width="2560" height="680" alt="image" src="https://github.com/user-attachments/assets/a452434b-c497-442e-96a9-218f64fc99ca" />

En este caso para ejecutar el sorter se siguen los siguientes pasos:
- Se escribe sorter para indicar cuál programa se va a ejecutar
- Se escribe -input
- Se escribe el nombre exacto que se le dió al archivo generado anteriormente (archivoSMALL en este caso)
- Se escribe -output
- Se escribe el nombre del archivo resultante ordenado, en este caso será ordenadoSMALL.bin (la extensión bin no es requerida)
- Se escribe -alg para definir el algoritmo de ordenamiento
- Se escribe una de las 5 opciones posibles de algoritmos (Se escriben con las letras clave, ejemplo: RS):
1. Radix Sort -> RS (Recomendado para cualquiera de los tamaños de archivo y superiores a LARGE)
2. Quick Sort -> QS (Recomendado para cualquiera de los tamaños de archivo)
4. Merge Sort -> MS (Recomendado para cualquiera de los tamaños de archivo)
6. Comb Sort -> CS (Recomendado para archivos de tamaño SMALL o inferior)
7. Heap Sort -> HS (Recomendado para archivos de tamaño SMALL o inferior)
   
Estos fueron listados del más rápido al más lento, dependiendo del tamaño de archivo elegido la elección de posibles algoritmos de ordenamiento se puede ver limitada por cuestiones de inviabilidad de algunos algoritmos según el tamaño del archivo.
- Se escribe -pageSize indicando el tamaño en enteros (int) que tendrá cada una de las página de PagedArray
- Se escribe el número, en este caso 4096 enteros
- Se escribe -pageCount indicando el número de páginas que pueden estar en RAM a la vez
- Se escribe el número, en este caso 1024 páginas

Note que de hecho es posible calcular la RAM total que se está simulando si se multiplica el tamaño de las páginas por su cantidad. En este caso se obtiene 4096int*1024páginas*4B=16MB RAM. Por tanto de los 256MB totales del archivo tan solo 16MB se podrán tener dentro de la RAM simulada a la vez, el resto será necesario hacer **swapping** entre las páginas en RAM y el disco para poder ordenar todo el archivo.
El orden de ejecución del programa es primero copiar el archivo que se brindó en -input para trabjar sobre una copia y no el original (así el original se puede aprovechar para otros ordenamientos), luego de copiado el archivo este es ordenado por medio del algoritmo seleccionado y el sistema de paginación que logra que el algoritmo de ordenamiento crea que recibe un arreglo clásico. Finalmente se toma el archivo binario ordenado y se pasa a una versión legible con números en ASCII y separados por comas.

<img width="2560" height="996" alt="image" src="https://github.com/user-attachments/assets/7516d84f-8a6b-4b3c-b4ea-5e68a84058f4" />

Aquí se puede apreciar y diferenciar cada una de las etapas de ejecución ya explicadas, además, el programa brinda información relevante sobre el ordenamiento como el nombre del algoritmo empleado, tiempo durado exclusivamente en el periodo de ordenamiento del algoritmo y finalmente la cantidad de **page hits** y **page faults** que corresponden a la cantidad de veces que una página ya estaba en la RAM cuando se necesitó y la cantidad de veces que se tuvo que ir a traer la página del disco (archivo) respectivamente.
Una vez hecho esto y al terminar de generar el archivo legible, este puede ser corroborado en la misma carpeta **Release** donde se ejecutó el programa:

 <img width="2560" height="846" alt="image" src="https://github.com/user-attachments/assets/f1bd2615-e56c-42b3-9c93-2685c0d1490b" />

### Revisión del archivo numérico
 Ahí se aprecian los 3 archivos generados, siendo el primero el original, luego la copia binaria ordenada y finalmente el archivo legible que puede ser abierto usando el note pad o en caso de archivos muy grandes, se puede usar programas como note pad++ si el archivo es muy grande (a partir de SMALL se recomienda usar note pad++ o similares). 

<img width="2560" height="1484" alt="image" src="https://github.com/user-attachments/assets/67b313ba-0732-43d6-bc0b-d14b5c7f2544" />

Ahí se puede ver una pequeña parte de los números ordenados, si se continua explorando el archivo se puede corroborar que está ordenado en su totalidad.
















 






