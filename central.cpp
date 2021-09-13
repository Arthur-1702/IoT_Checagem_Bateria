#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <queue>

using namespace std;

int inicializa_e_conecta(uint16_t porta)
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(porta);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);

    return clientSocket;
}

void mostra_fila(queue<float> l)
{
    queue<float> c = l;
    while (!c.empty())
    {
        cout << '\n'
             << c.front();
        c.pop();
    }
    cout << '\n';
}

void analisa_fila(queue<float> l, float limiar)
{
    queue<float> c = l;
    while (!c.empty())
    {
        if (c.front() <= limiar)
        {
            cout << "Alerta! Carga baixa." << endl;
        }
        c.pop();
    }
    cout << '\n';
}

int main()
{
    int s_sock = inicializa_e_conecta(7891);
    queue<float> fila_cargas;
    char resposta_de_s[1024];
    char *carga = "Carga?";
    char *vivo = "Vivo?";
    int aux = 0;
    float aux1 = 0;
    float limiar, ciclo;

    //C inicia a operacao e le do usuario os valores ciclo, em segundos,
    //e limiar, do tipo float, entre 0 e 1;

    cout << "Qual o ciclo de requisicao da carga em segundos?" << endl;
    cin >> ciclo;
    ciclo = ciclo * 1000000;

    while (aux == 0)
    {
        cout << "Qual limiar?" << endl;
        cin >> limiar;
        cout << "\n"
             << endl;

        if (limiar >= 1.00 || limiar <= 0)
        {
            cout << "Limiar invalido \n"
                 << endl;
            aux = 0;
        }
        else
        {
            aux = 1;
        }
    }

    while (1)
    {
        // Assim que um cliente S se conectar, C envia a S, a cada ciclo segundos,
        // uma requisicao de “vivo?” e espera a resposta de S;
        usleep(ciclo);

        send(s_sock, vivo, strlen(vivo) + 1, 0);
        printf("%s \n", vivo);

        if (recv(s_sock, resposta_de_s, 1024, 0) > 0)
        { // A resposta nunca ser ́a negativa;
            printf("%s \n", resposta_de_s);

            //Ao receber uma resposta positiva de S, C envia a S uma requisicao de
            //“carga?” e espera a resposta de S;
            send(s_sock, carga, strlen(carga) + 1, 0);
            printf("%s \n", carga);

            if (recv(s_sock, resposta_de_s, 1024, 0) > 0)
            {
                //C recebe o valor da carga, que  ́e do tipo float, e o armazena numa
                //estrutura de fila;
                aux1 = atof(resposta_de_s);
                printf("Carga atual: %f \n", aux1);

                fila_cargas.push(aux1);

                if (fila_cargas.size() == 5)
                {
                    cout << "\n A fila é : ";
                    mostra_fila(fila_cargas);
                    //Quando a fila atinge o tamanho de 5 elementos, C processa cada valor que consta na fila:
                    //se for menor do que limiar, é emitida umamensagem de alerta ao usuario;
                    analisa_fila(fila_cargas, limiar);

                    while (fila_cargas.size() != 0)
                    { //Cada valor processado é removido da fila.
                        fila_cargas.pop();
                    }
                    cout << "\n Fila zerada \n";
                    mostra_fila(fila_cargas);
                }
            }
        }
    }

    return 0;
}
