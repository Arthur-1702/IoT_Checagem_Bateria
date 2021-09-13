#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

int inicializa_e_espera(uint16_t porta)
{
  int welcomeSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(porta);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //S permanece “escutando” o socket continuamente, à espera de requisicoes de C;
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)); 

  if(listen(welcomeSocket,5)==0)
    printf("Esperando solicitção\n");
  else
    printf("Erro\n");

  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  return newSocket;
}


int main ()
{
  int clientSocket = inicializa_e_espera(7891); 
  char requisicao_de_c [1024];
  char *vivo = "Vivo?";
  char *sim = "Sim!";
  char carga [5];
  float carga_atual = 1.00;
  
  while (1)
  {
    while (1)
    {
      recv(clientSocket, requisicao_de_c, 1024, 0);

      cout << requisicao_de_c << endl;
      //Quando S recebe de C uma requisicao de “vivo?”, S envia a C uma
      //mensagem afirmativa;
      if(strcmp ( requisicao_de_c, vivo ) == 0){
        send ( clientSocket , sim , strlen ( sim ) + 1 ,0) ;
        cout << sim << endl;
          
      }else{
        //Quando S recebe de C uma requisicao de “carga?” (só recebe vivo ou carga), S envia a C um
        //valor do tipo float entre 0 e 1, indicando o nıvel atual da carga;
        sprintf(carga, "%f", carga_atual);
        send ( clientSocket , carga , strlen ( carga ) + 1 ,0) ;
        cout << carga << "\n\n" << endl;
      }
      carga_atual = carga_atual - 0.01; //O nıvel da carga ́e reduzido continuamente.
    }
  }
  
 return 0;
}