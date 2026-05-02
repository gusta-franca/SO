Etapas para compilação:
- make

Etapas para execução:
- cd build
- ./ex_i arg1 arg2 ... argn

Bibliotecas utilizadas:
- <math.h>
- <pthread.h>: threads conforme o padrão POSIX
- <stdio.h>
- <stdlib.h>
- <time.h>

Exemplos de uso:
- ./ex_1 v t n  # "v" é o valor a ser buscado, "t" é a quantidade de threads e "n" é o tamanho do vetor. Por padrão, v = 4, t = 3 e n = 10
- ./ex_2 t m n  # "t" é a quantidade de threads, "m" é a quantidade de linhas da matriz e "n" é a quantidade de colunas. Por padrão, t = 4, m = 6, n = 8
- ./ex_2 t n    # "t" é a quantidade de threads e "n" é a dimensão de uma matriz quadrada.
