Etapas para compilação:
- make

Etapas para execução:
- cd build
- ./ex_i arg1 arg2 ... argn

Bibliotecas utilizadas:
- <stdio.h>
- <stdlib.h>
- <string.h>
- <sys/types.h>: tipagem relacionada a processos, como pid_t
- <sys/wait.h>: sincronização entre processos pai e filho
- <unistd.h>: chamadas de sistema

Exemplos de uso:
- ./ex_1 n      # "n" é o nível da hierarquia de processos requerida. Por padrão, n = 3
- ./ex_2 c      # "c" é um comando disponível em sistemas Linux
- ./ex_3 v p n  # "v" é o valor a ser buscado, "p" é a quantidade de processos e "n" é o tamanho do vetor. Por padrão, v = 4, p = 3 e n = 10
- ./ex_4 c      # "c" é um comando disponível em sistemas Linux
- ./ex_4 c &    # "c" é um comando disponível em sistemas Linux; executa c em segundo plano
