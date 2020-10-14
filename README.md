# Integrantes:
Claudia Noche

Mauricio Bernuy

Stephano Württele

# Introducción

La indexación de una base de datos es una estrategia utilizada con el fin de apresurar operaciones que se requieran hacer. Estas, usando distintos acercamientos, agilizan las operaciones que uno tiene que realizar con una gran base de datos.

Dicho esto, cabe resaltar que hay distintos tipos de índices que se le puede aplicar a una base de datos. Esto se debe a que las estructuras varían ampliamente, y unas funcionan mejor realizando ciertas actividades que otras. En este trabajo, se compararán dos de estas indexaciónes; un Sequential File Organization y un Indexed Sequential Access Method (ISAM).

Este trabajo busca comparar la eficiencia de estas estructuras bajo distintas tareas, tales como búsqueda específica, búsqueda por rango, eliminación e inserción. Por la naturaleza del ISAM, se espera que este muestre mejores resultados que el Sequential File.

## Marco Teórico

### Indexación de archivos

Previo a mostrar nuestras implementaciónes, se va a hacer una breve descripción de los conceptos necesarios a conocer. Tal es el concepto de la misma indexación. La indexación de una base de datos "cruda" describe un proceso por el cual se reestructura cierta parte de dicha base, con el fin de volverla más eficiente para ciertas tareas.

### Sequential File
Un Sequential File (o Archivo secuencial) es un tipo de organización de archivos el cual se basa en un atributo en específico, conocido como "key", para mantenerse organizado.

Este tipo de organización no solo requiere una organización física de los registros, sino que también se tiene una "hilera" de punteros entre registros, donde un registro mantiene la posición del siguiente en el orden preestablecido.

El sequential file resalta como una organización eficiente, pues operaciones básicas se realizan de manera rápida y sin costo adicional.
La inserción de registros es una de estas operaciones constantes. Para insertar un registro, se crea un archivo extra, conocido como el "auxiliar", donde se almacena cierta cantidad de registros nuevos. Se localiza el atributo el cual se está usando como key, y dentro del archivo auxiliar, se actualizan los punteros de los demás registros, para mantener el orden. Todo esto se realiza en Ram, hasta que, luego de que se ha llegado a un límite preestablecido de registros en el auxiliar, se reescribe el archivo principal de data agregando los nuevos registros, manteniendo el orden de los punteros.

En la siguiente imágen, se puede ver un ejemplio de inserción utilizando un archivo auxiliar.
![Figura 1](https://i.imgur.com/EqgwQSF.png)
 
Este proceso, aunque costoso, simplifica la búsqueda de valores específicos dentro de su estrucutra, así como la búsqueda por rando y la eliminación de registros.

Respecto a su complejidad, una búsqueda en este tipo de organización requiere, inicialmente, hacer una búsqueda binaria en el archivo principal, aprovechandola organización de dicho archivo. Asimismo, si no es localizado en el archivo principal, se revisa linealmente el auxiliar. En síntesis, respecto a los accesos a memoria secundaria, se tiene una complejidad de O(log N), siendo N la cantidad de registros en el archivo principal.

La eliminación, por otro lado, utiliza la hilera de punteros. Cuando un registro es eliminado, simplemente se actualizan los punteros, siendo que el previo al registro eliminado apunte al que viene después. Es indiferente si esto ocurre dentro del archivo principal o dentro del auxiliar, pues al momento de reescribir el archivo principal, simplemente se saltará dicho registro. En la siguiente imágen, se puede ver un ejemplo de este proceso.

![](https://i.imgur.com/rS1Ma5o.png)

### Indexed sequential access method

Un Indexed Sequential Access Method (o ISAM) es un tipo de indexación de archivos, el cual crea niveles de "índices" que apuntan a un número de registros, conocidos como "Pages"

Nuevamente, esta estructura requiere que el archivo principal se encuentre ordenado fisicamente en base a un atributo. Asimismo, se mantienen punteros entre dichos registros dentro de un mismo page. Hay distintos tipos de indexación usando este método, pero el que se ha escogido para este trabajo es el Sparse Index File, el cual tiene la siguiente forma: 
![](https://i.imgur.com/bmG2uTo.png)

Como se puede apreciar, se tiene un archivo de índices que marcan separaciones en el archivo principal. Se usan los registros para delimitar los rangos, y cada index apunta a un page. En este ejemplo, se ha hecho un orden en base al ID de las personas, y se ha armado un índice que abarque los rangos de los registros del archivo principal.

Este tipo de insexación está construida de tal forma que se espera que no hayan muchas inserciones o eliminaciones de data, aunque igual se tienen métodos para ambas operaciones. La inserción toma el atributo escogido del registro y realiza primero una búsqueda en el índice para ver a cuál page pertenece. Luego de ir al page, se actualizan los punteros y el registro revisa si hay espacio para su insercion en dicho page. Caso no hubiera, se crea un "overflow" o un page aparte, conectado con el page principal, donde se van metiendo los registros que no entran dentro del page inicial, todo esto manteniendo actualizados los punteros entre registros. La organización física del archivo, después de realizar inserciones, no se ve afectada, y se agrega el registro al final. El nuevo orden es manejado únicamente por punteros.

La búsqueda se realiza de forma similar; inicialmente se revisa en el índice bajo cual rango cae el atributo a buscar, y se viaja al page correspondiente. Una vez ahí, se realiza una búsqueda que puede ser tanto lineal como binaria para encontrar el registro esperado. No solo se revisa el page principal, pero si el registro no es encontrado ahí, se extiende uno a través de los "Overflows" continuando la búsqueda. Se puede concluir que sólo se requieren O(logN) accesos a memoria secundaria para este proceso.

Finalmente, la eliminación que se ha implementado toma las características de la eliminación en un sequential file, realizando primero una búsqueda del valor deseado y luego se marca como un espacio vacío, habilitando que se escriba encima de su lugar en caso llegue otro registro.


## Implementaciones

#### Sequential File implementation
Para realizar una implementación de esta estructura, se inició creando un objeto Registro, el aucal almacena no solo los atributos del registro, sino también su posición, asi como el archivo donde se encuentra. 

Previo a realizar cualquier operación, se requiere leer el archivo con la data cruda. Para esto, se tiene un constructor que inicialmente llama a una función "load_data", la cual lee el archivo y pasa cada registro a su propio objeto, almacenandolos en un vector. Luego, se llama a la funcion "insertAll" y en base a un atributo específico se organiza el vector y se reescribe todo en el archivo principal.

![](https://i.imgur.com/QQPFW5F.png)

La estructura registro (s_Register) contiene no solo los atributos separados del registro, sino que tambien contiene un puntero al siguiente registro en el orden preestablecido, delimitado al momento de ordenar la data. Por otro lado, dentro de la estructura se tiene una enumeración de tipo 'a' o 'd', para poder identificar en qué archivo se encuentra el registro, y un booleano "deleted", el cual servirá para la eliminación de registros, explicado más adelante.

![](https://i.imgur.com/2HC1V4L.png)

Para realizar la inserción de registros, como ya se explicó previamente, se tiene que realizar un proceo tomando en cuenta el archivo auxiliar. Lo primero que se hace, es revisar, mediante el método de búsqueda, si el registro ya existe. En caso suceda, se termina el proceso. Caso contrario, se abre el archivo auxiliar y se escribe el registro nuevo al final del archivo. 

Se tienen las funciones "getprev" y "getnext" con el fin de poder actualizar los punteros, y que se mantenga el orden entre estos. No se ha realizado ningun cambio al orden físico hasta el momento.

Luego se revisa que no se haya superado el número de registros máximos dentro del archivo auxiliar. Si esto sucede, se activa el "merge". Dicha función "une" los dos archivos y los reescribe en el archivo principal, vaciando el auxiliar, manteniendo el orden dictado por los punteros.

El proceso de búsqueda de un registro dentro de esta estructura se realiza utilizando el método de búsqueda binaria. Para esto, se requiere que se realice un "merge" entre el auziliar y el principal, con el fin de que la búsqueda no sea tan costosa en RAM. La función que realiza la búsqueda en base a una string, "searchpos", devuelve su posición en el archivo.

![](https://i.imgur.com/sMMf91X.png)

En esta primera parte, se revisa que el archivo auxiliar no se encuentre vacío. Si tiene registros, se realiza un merge de los archivos.

![](https://i.imgur.com/j2ISSTI.png)

En esta segunda sección, se itera a travéz de los registros en el archivo principal utilizando el método de la búsqueda binaria. Si no se consigue encontrar el registro deseado, se devuelve un valor "-1" para dar a entender que no existe.

Finalmente, la eliminación es el último método que se implementó en esta estructura. Inicialmente se requiere la posición del registro a eliminar, la cual se consigue con el proceso previamente explicado. Al ubicarse el registro, se le cambia el booleano de deleted a true. Luego, se deben actualizar los punteros de los registros, utilizando las funciones auxiliares previamente explicadas "getnext" y "getprev". 

![](https://i.imgur.com/7HtmkSx.png)

Finalmente, cuando se reescriba el archivo principal se obviarán los registros que tienen esta marca.

#### Indexed Sequential File Method (ISAM)

La segunda estructura implementada es la del ISAM. Nuevamente, como en el Sequential, se necesitó crear una estructura para guardar los registros.

![](https://i.imgur.com/CC1kDfU.png)

Asimismo, se requirió estructurar los "Pages":

![](https://i.imgur.com/qUm3uDt.png)

Para el correcto funcionamiento de esta estructura, se debe utilizar información ya existente en la construcción para crear los índices. Entonces, se implementó un generador de registros en CSV que, por motivos prácticos, genera 30 registros (aunque se pueden generar más tranquilamente). Estos datos generados son parseados por el ISAM de acuerdo al tamaño definido para las páginas, y se crea una estructura inicial con índice que sirve para acceder al archivo de datos, modificado para que funcione con el ISAM.

Es bueno acotar también que se tomó la decisión de ordenar las páginas independientes por cada inserción y eliminación por dos motivos:
1. La naturaleza del ISAM es funcionar junto a data de alguna forma más "estática", que no se edita continuamente, y por lo tanto las inserciones y eliminaciones, además de raras, son poco frecuentes, por lo que el problema de la complejidad de ordenación y escritura será poco frecuente. 
1. Por otro lado, las búsquedas pueden ser mucho más frecuentes, y se pueden hacer más eficientes teniendo la información ordenada ya en las páginas. Así, se puede hacer una búsqueda binaria dentro de cada página inclusive, mejorando la propiedad más utilizada naturalmente en un arbol ISAM, la búsqueda.

### Indexación

Para generarse el índice para la estructura, primero se valida que efectivamente no exista una ya. Si ya existe, solo se carga del archivo donde esté guardada. De no ser así, se recibe una colección de información que es reformateada a que este guardada correctamente en pages distribuidas equitativamente (de forma balanceada). Luego, de ese archivo separado por pages, se dividen en el factor de bloque del índice y se guardan así los índices para estas divisiones.

### Búsqueda

Para esta estructura, se buscó simplificar la inserción y eliminación con el costo de agregar complejidad a la búsqueda. Por ello, en lugar de retornar un elemento o registro directamente, se retorna una estructura que llamamos "PageLocation" que contiene:
    - Un registro, por si se encuentra el elemento en la búsqueda.
    - La dirección de la última página donde se buscó, se haya encontrado o no el elemento.
    - El índice del último registro que se revisó en la búsqueda, se haya encontrado o no.
    - Un booleano que indica si el elemento fue encontrado o no.

Este page location se encuentra con tres pasos fundamentales:
1. Encontrar la página buscando en el índice.
1. Recorriendo la página hasta encontrar o llegar al final.
1. De no encontrarlo y existir una página de overflow, seguirla y repetir desde paso 2.

De no ser encontrado un elemento, se regresa la estructura con la ubicación del último page de donde pertenecería, en el primer espacio libre. De esta forma, se puede usar esta misma función para la inserción y eliminación de elementos.

Más adelante se detallará el funcionamiento de los locks para la concurrencia, pero se puede especificar desde ya que, si el lock que se recibe en la función de búsqueda es 1, entonces se aplica un bloqueo al término del paso 1.


### Inserción
La inserción en esta estructura se basa en la metodología usada en la búsqueda. Inicialmente, se busca el page donde debería ser ingresado el valor. Se realiza una función de búsqueds sobre este valor, y se analiza la estructura que es devuelta. Esta contiene el puntero al "Page" donde "debería estar" el valor si fuera encontrado. Esta información es utilizada para delimitar el Page donde se escribirá el registro.

Se tienen tres casos que se deben manejar en este método. El primero es cuando se realiza una inserción a un page que no está entero. Lo único que se hace en este caso es escribir el registro en el lugar que la variable "first_empty" apunta, o el índice que se retorna de la búsqueda (los cuales realmente tienen el mismo valor). Se actualizan los punteros y la variable "first_empty".
![](https://i.imgur.com/igti2ZJ.png)

El siguiente caso a considerar, debido a la estructura en la que se ha construido el index, es si se está insertando un valor de menor tamaño al inicial. Este es el único caso en el cual el index se ve alterado. Se cambia este primer índice por el nuevo atributo y se inserta dentro del page, nuevamente actualizando punteros y la variable "first_empty".

![](https://i.imgur.com/iXAEqk5.png)

Finalmente, se tiene que tomar en cuenta que hay casos donde el page ya esta lleno y se requiere crear un nuevo. Para esto, se crea un bucket vacío, al cual se está manteniendo un puntero del primer page, y se inserta el valor en esta nueva estructura.

![](https://i.imgur.com/XBYTihU.png)

Para los bloqueos de transacciones concurrentes, se tiene en cuenta el valor del lock recibido, y de acuerdo a ello se aplican al inicio y al final el bloqueo y liberación respectivamente, al igual que las modificaciones en la variable "done" de la estructura que avisa a la otra transacción que puede avanzar. En tanto este caso como la eliminación, se envía el mismo valor de lock a la búsqueda, que en general no infleuencia a la misma.

### Eliminación

La eliminación es la úlitma operacion realizada en esta estructura. Nuevamente, se hace uso de la función de búsqueda. Al ser encontrado el valor, se toma el registro en la posición previa al puntero "First_empty" y se hace un cambio con el valor a borrar. Es decir, último valor en la página objetivo se va a la posición del registro a borrar, y este ahora toma su lugar. El puntero "First_empty" se actualiza apuntando ahora al último valor del page. Después de esto, se reorganizan los registros "validos" (ignorando el último recién eliminado) con el fin de mantener el orden dentro del page.

Al igual que en la inserción, dependiendo de las señales recibidas para los locks, se aplican al inicio y al final el bloqueo y liberación respectivamente, al igual que las modificaciones en la variable "done" de la estructura que avisa a la otra transacción que puede avanzar.

![](https://i.imgur.com/jklChp1.png)


#### Manejo de Concurrencia
Para dar mayor realismo al manejo de las estructuras, se implementó una forma de manejar múltiples transacciones y operaciones al mismo tiempo, precisamente dos en esta implementación. Para ello, necesitamos incluir la librería de "threads" de C++, que necesita compilarse con un flag de -pthread con el programa.

Se uso una lógica conveniente para plasmar el significado de las combinaciones de operaciones, permutando cualquier par de Inserción, Busqueda o Eliminación dandole un valor numérico a cada una y haciendo que su suma sean valores únicos. De esta forma, no tenemos que hacer tantas comparaciones para saber como se debe llevar la concurrencia antes de ejecutar las instrucciones. Siguiendo esta premisa, los valores para cada operación y la información que necesitan para poder ejecutarse se guardan en una estructua "Query" que contiene:
- Tipo de operación (-2 si es inserción, 1 si es eliminación, 2 si es selección)
- Key para operación, que se necesita para eliminación y selección
- Un Registro, que se necesita para la inserción


![](https://i.imgur.com/EOjwVMT.png)

Utilizando dichos valores para las operaciones, se pueden hacer sumas para saber que par de operaciones se va a tener que ejecutar. Adicionalmente, se hacen comparaciones que reducen las operaciones requeridas y definen que locks se tienen que hacer en que partes de la ejecución. Para ello se usan los valores 1, 2 y 3, que se envían a las funciones de las operaciones como flags para saber donde usar los locks.

Al ejecutarse dos instrucciones exactamente iguales, se hace una reducción a la instrucción y se ejecuta una sola vez. Si son dos búsquedas se pueden ejecutar concurrentemente sin problemas. Si son dos inserciones o dos eliminaciones, dada la posiblidad que cada uno cambie los punteros en las pages, se deben ejecutar por separado; igual que si fuera una eliminación y una inserción. Finalmente, si es una combinación de eliminación o inserción con búsqueda, la búsqueda puede ejecutarse al mismo tiempo que las otras dos solo has encontrar el índice, pues ahí la inserción o eliminación debe suceder primero por si el cambio de punteros afectase. Para ello, se toma una predominancia siempre por la operación de menor valor. 
Los bloqueos entre operaciones se hacen utilizando **mutexes** y una lógica de **semáforos** que permite o niega el paso de una transacción mientras la otra este en una zona específica. Estos emularían el comportamiento de un Lock Exclusivo, por lo que se trata de reducir el área de acceso compartido, o también llamada **región crítica**, al máximo.


![](https://i.imgur.com/nqtqU5X.jpg)

## Interfaz de Usuario

Se desarroló una interfaz de usuario simple usando la herramienta Qt, desde la cual es posible realizar la creacion de las estructuras ISAM o Sequential sobre un archivo CSV de entrada, y efectuar las operaciones de Search, Insert y Delete sobre sus registros.

![](https://i.imgur.com/JMmh2ux.png)

Mediante Qt, pudimos esquematizar un diagrama de los elementos en pantalla, usando una estructura OOP para manejar eventos y actualizar sus propiedades dependiendo de los inputs dados por el usuario.
![](https://i.imgur.com/rEzaX7o.png)

![](https://i.imgur.com/4YA3CPF.png)

## Resultados

Con el fin de probar los resultados, se realizó un GUI con la herramienta QT en c++. En esta, se puede apreciar la base de datos actual con la que se está trabajando. Se tienen dos opciones a la izquierda "SEQUENTIAL" e "ISAM" las cuales sirven para decidir el tipo de organización en el cual se va a trabajar. Asimismo, se tienen 3 botones, "SEARCH", "INSERT" y "ELIMINACION", los cuales hacen respectivamente las operacioens de búsqueda, inserción y eliminación de un registro.

En la parte inferior de la pantalla, se pueden ver los accesos a memoria contados, así como el tiempo en microsegundos de la operación realizada.

Se utilizó una base de datos generada en un programa en C++, y se usaron distintos tamaños para probar los resultados. Se utilizó una base de datos de 30 registros, una de 100, una de 1000 y una de 10000.

Para cada base de datos, se realizó una prueba de las operaciones en cada estructura creada. Los resultados que obtuvimos fueron los siguientes:


### 100 Registros

Se realizó una tabla de comparación entre tiempo - Operaciones de la indexación ISAM. Se realizó una comparación con el tiempo de cada operación primero.

#### ISAM
![](https://i.imgur.com/sGKLgSr.png)

Luego, de cada acceso a memoria por operación.

![](https://i.imgur.com/YiamM1y.png)

#### Sequiential
Se realizó lo mismo con la estructura Secuencial.


![](https://i.imgur.com/8bIGCbP.png)

Accesos a memoria por operación en Sequential.

![](https://i.imgur.com/nj8BjFe.png)


### 1000 Registros

Se realizó una tabla de comparación entre tiempo - Operaciones de la indexación ISAM. Se realizó una comparación con el tiempo de cada operación primero.

#### ISAM
![](https://i.imgur.com/iKqzKw4.png)


Luego, de cada acceso a memoria por operación.

![](https://i.imgur.com/2dn9lf3.png)

#### Sequiential
Se realizó lo mismo con la estructura Secuencial. Tiempo por operación:

![](https://i.imgur.com/eGTX6kw.png)


Accesos a memoria por operación en Sequential.

![](https://i.imgur.com/4DRSe5s.png)



#### Análisis de los resultados
Como se puede apreciar, naturalmente la creación de las estructuras será siempre lo más pesado, pues implica a la construccion desde cero cada componente. Aún así, este costo es poco importante, pues solo sucede una vez en inicialización. Luego, se puede ver como el costo de búsqueda se mantiene muy bajo, cosa que es favorable para estructuras de tipo estática que realizan muchas búsquedas pero pocas inserciones y eliminaciones. Esto evidencia el trade-off decidido de incrementar el costo en inserciones y eliminaciones pero decrecer considerablemente el costo en busquedas.

Algo notable de esta comparación de las estructuras es la velocidad del ISAM, la cual es mayor a la del Sequential Order. Esto se debe a que la indexación del ISAM aglizia tareas comúnes y permite un mejor tiempo de trabajo. Asimismo, no descartamos la posiblidad de que se deba a nuestra implementación que nuestras estructuras tengan tiempos tan distintos de ejecucicón.

## Conclusiones

En conclusión, se puede ver cómo la organización física de un archivo, aunque costosa, agiliza muchas operaciones básicas de las bases de datos al usar estructuras específicas como las escogidas, volviendola un costo casi necesario que realizar previo a trabajar con estas. Ambas estructuras escogidas presentan distintos métodos de manejar la data, pero dada su naturaleza sería más conveniente que se mantengan ordenadas para un buen tiempo de ejecución en las búsquedas, que deberían ser su operación más frecuente como para elegir en primer lugar utilizarlas. Esto sin embargo toma mayor relevancia como problema al momento de realizar alguna operación de este tipo en una base de datos muy grande, pues implicaría que posiblemente se realicen queries más seguido y más archivos tengan que ser modificados.

# **Este trabajo se puede ver evidenciado en el siguiente link:**
https://drive.google.com/file/d/1Q1jJt3zYSWUMg6eHMy8p5zyJRgkUV3LA/view?usp=sharing
