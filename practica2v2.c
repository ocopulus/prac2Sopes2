#include <linux/module.h>  // Needed by all modules
#include <linux/kernel.h>  // Needed for KERN_INFO
#include <linux/fs.h>      // Needed by filp
#include <asm/uaccess.h>   // Needed by segment descriptors

//Set to the file you are targeting
void escribir(char* dump_filename, char* datos)
{
    struct file *file;
    //int block_count; //Set me to something
    loff_t pos = 0;
    mm_segment_t old_fs;

    old_fs = get_fs();  //Save the current FS segment
    set_fs(get_ds());

    file = filp_open(dump_filename, O_WRONLY|O_CREAT, 0644);
    if(file){
        vfs_write(file, datos, strlen(datos), &pos);
        filp_close(file,NULL);
    }
    set_fs(old_fs); //Reset to save FS
    filp_close(file, NULL);
}

long media (int *lista, int tam){
    int i;
    int val =0;
    for(i=0;i<tam;i++){
        val = val + lista[i];
    }
    long salida;
    salida = val/tam;
    return salida;
}

long moda(int *lista, int tam, int *aux){
    int contador = 0;
    for(contador = 0; contador < tam; contador++){
        aux[contador] = 0;
    }
    for(contador = 0; contador < tam; contador++){
        int numero = lista[contador];
        int contador2;
        for (contador2 = 0; contador2 < tam; contador2++)
        {
            if(lista[contador2] == numero) aux[contador]++;
        }
    }

    int index = 0;
    int bandera = 0;
    for(contador = 0; contador < tam; contador++){
        if(aux[contador] > bandera){
            bandera = aux[contador];
            index = contador;
        }
    }

    return lista[index];
}

long maximo(int *lista, int tam){
    int index = 0;
    int valorMaximo = 0;
    for(index = 0; index < tam; index++){
        if(lista[index] > valorMaximo) valorMaximo = lista[index];
    }  
    return valorMaximo; 
}

long minimo(int *lista, int tam){
    int index = 0;
    int valorMinimo = lista[0];
    for(index = 0; index < tam; index++){
        if(lista[index] < valorMinimo) valorMinimo = lista[index];
    }
    return valorMinimo;
}

long mediana(int *lista, int tam, int *aux){
    int bandera = 0;
    int contador = 0;
    for(contador = 0; contador < tam; contador++){
        aux[contador] = 0;
    }
    for(contador = 0; contador < tam; contador++){
        aux[contador] = lista[contador];
    }
    for(contador = tam; contador > 0 && bandera == 0; contador--){
        bandera = 1;
        int contador2 = 0;
        for (contador2 = 0; contador2 < contador; contador++)
        {
            if(aux[contador2]>aux[contador2+1]){
                int numero = aux[contador2];
                aux[contador2] = aux[contador2+1];
                aux[contador2+1] = numero;
                bandera = 0;
            }
        }
    }

    if(tam % 2 == 0){
        int index = (tam / 2) + 1;
        int val1 = aux[index];
        int val2 = aux[index+1];
        return (val1 + val2)/2;
    } else {
        int index = (tam / 2) + 1;
        return aux[index];
    }
}

int init_module(void)
{
    // Create variables
    struct file *f;
    char buf[128];
    char buf2[128];
    int lista [128];
    int aux [128];
    int cantidad = 0;
    mm_segment_t fs;
    int i;
    // Init the buffer with 0
    for(i=0;i<128;i++)
        buf[i] = 0;
    // To see in /var/log/messages that the module is operating
    printk(KERN_INFO "My module is loaded\n");
    // I am using Fedora and for the test I have chosen following file
    // Obviously it is much smaller than the 128 bytes, but hell with it =)
    f = filp_open("/statistics/datos.txt", O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        // Get current segment descriptor
        fs = get_fs();
        // Set segment descriptor associated to kernel space
        set_fs(get_ds());
        // Read the file
        f->f_op->read(f, buf, 128, &f->f_pos);
        // Restore segment descriptor
        set_fs(fs);
        // See what we read from file
        printk(KERN_INFO "buf:%s\n",buf);
    }
    filp_close(f,NULL);

    int tam = strlen(buf);
    int index;
    int temporal = 0;

    for(index = 0; index < tam; index++){
        if(buf[index] == ','){
            lista[cantidad] = temporal;
            cantidad++;
            //printk("\n Val temp: %i", temporal);
            temporal = 0;
            continue;
        }

        if(buf[index] == '\0') break;

        temporal = temporal * 10;
        
        if(buf[index] == '0'){
            temporal = temporal + 0;
        }else if(buf[index] == '1'){
            temporal = temporal + 1;
        } else if(buf[index] == '2'){
           temporal = temporal + 2;
        } else if(buf[index] == '3'){
            temporal = temporal + 3;
        } else if(buf[index] == '4'){
            temporal = temporal + 4;
        } else if(buf[index] == '5'){
            temporal = temporal + 5;
        } else if(buf[index] == '6'){
            temporal = temporal + 6;
        } else if(buf[index] == '7'){
            temporal = temporal + 7;
        } else if(buf[index] == '8'){
            temporal = temporal + 8;
        } else if(buf[index] == '9'){
            temporal = temporal + 9;
        }
    }
    lista[cantidad] = temporal/10;
    cantidad++;
    temporal = 0;
    printk("Parser teminado");
    printk("Cantidad de datos: %i", cantidad);
    for(index = 0; index < cantidad; index++){
        printk("\nEl numero en pos: %i, es el: %i", index, lista[index]);
    }
    printk("\nTerminado de vec de numeros");
    printk("\nInicio de procesos estadisticos\n");

    long Valmedia = media(lista, cantidad);
    printk("La media es: %d\n", Valmedia);

    long Valmoda = moda(lista, cantidad, aux);
    printk("La moda es: %d\n", Valmoda);

    long ValMaximo = maximo(lista, cantidad);
    printk("El Valor maximo es: %d\n", ValMaximo);

    long ValMinimo = minimo(lista, cantidad);
    printk("El Valor minimo es: %d\n", ValMinimo);

    long ValMediana = mediana(lista, cantidad, aux);
    printk("El Valor de la Mediana es: %d\n", ValMediana);
    /*parsear(buf);
    printk("Voy a escribir");
    escribir("/statistics/salida.JSON", buf);
    printk("Ya escribi papu");*/
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "My module is unloaded\n");
}