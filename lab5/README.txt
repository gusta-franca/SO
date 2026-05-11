Etapas para compilação:
- make

Etapas para execução:
- cd build
- ./ex_i arg1 arg2 ... argn

Bibliotecas utilizadas:
- <ctype.h>: conversão de caracteres  
- <errno.h>: macros para condições de erro
- <fcntl.h>: controle de arquivos  
- <pthread.h>: threads conforme o padrão POSIX  
- <signal.h>: manipulação de sinais de software  
- <stdio.h>
- <stdlib.h>
- <string.h> 
- <sys/mman.h>: mapeamento de memória compartilhada  
- <sys/shm.h>: gerenciamento de memória compartilhada  
- <sys/socket.h>: sockets  
- <sys/stat.h>: status e permissões de arquivos
- <sys/types.h>: tipos de dados do sistema
- <sys/un.h>: sockets AF_UNIX
- <sys/wait.h>: espera de processos filhos
- <unistd.h>: chamadas de sistema

Exemplos de uso (considerando os terminais T1 e T2):
- Em T1: ./build/ex_1_fifo_server, em T2: ./build/ex_1_fifo_client
- ./ex_4 e p    # "e" é a quantidade de elementos e "p" é a quantidade de processos. Por padrão, e = 20 e p = 5
- Em T1: ./ex_5_socket_server, em T2: ./nuild/ex5_socket_client
