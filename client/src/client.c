#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger,"Soy un Log");
	
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config,"CLAVE");
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");

	// Loggeamos el valor de config

	log_info(logger,"Valores obtenidos del config: valor = %s ", valor);
	log_info(logger,"Valores obtenidos del config: ip = %s ", ip);
	log_info(logger,"Valores obtenidos del config: puerto = %s ", puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él
    printf("aca se supone que empieza lo de la conexion");
	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger=log_create("tp0.log","Cliente",1,LOG_LEVEL_INFO);
	if(nuevo_logger== NULL){
		printf("NO se pudo crear el logger.");
		abort();
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config=config_create("cliente.config");
	if(nuevo_config == NULL){
		printf("NO se pudo crear el config.");
		abort();
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger," >>> %s",leido);
    free(leido);
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	leido = readline("> ");
	while(strcmp(leido,"") != 0){

		log_info(logger," >>> %s",leido);
		free(leido);
		leido= readline("> ");
	}
	free(leido);


	// ¡No te olvides de liberar las lineas antes de regresar!
	

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();


	// Leemos y esta vez agregamos las lineas al paquete
	do
	{
		leido = readline("> ");
		if (strcmp(leido,"") != 0){
			
			agregar_a_paquete(paquete,leido,string_length(leido)+1);
			free(leido);
		}else{
			break;
		}
		/* code */
	} while (1);
	free(leido);
	enviar_paquete(paquete,conexion);
	


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	close(conexion);
	log_destroy(logger);
	config_destroy(config);
}
