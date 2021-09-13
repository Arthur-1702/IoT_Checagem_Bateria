# IoT_Checagem_Bateria
Cliente/Servidor em internet das coisas socket TCP em C/C++.

Cliente/Servidor em internet das coisas socket TCP em C/C++. Numa aplicação de internet das coisas (IoT, internet of things), uma rede de sensores se comunica com uma 
central C (que tem o papel de um servidor). Cada sensor S (que tem o papel de um cliente)  é alimentado por uma bateria, cuja carga se esvai com o tempo. 
Periodicamente, a cada ciclo segundos, a central envia uma requisição a S, com o objetivo de saber se o sensor está “vivo”. Caso o sensor responda afirmativamente, 
a central lhe envia outra requisição, agora com o objetivo de saber o nível atual da bateria do sensor. Assim que S recebe essa requisição, ele realiza uma medição do
nível atual da bateria. Caso o nível recebido por C seja menor do que o limiar, a central mostra ao usuário um alerta. Como o sistema pode ser composto por vários sensores,
C precisa garantir que os valores recebidos sejam processados na ordem de chegada. Logo, cada valor recebido de cada sensor é armazenado numa fila em C. Assim que um valor
é processado (é comparado com limiar), ele é removido da fila. O projeto consiste na implementação desse sistema com uma central e um sensor.
