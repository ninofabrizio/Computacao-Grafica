# BUGS
Há um problema ao tentar fazer uso da funcionalidade de contar objetos. Ao ser executado, um erro é apontado e o programa é parado. Mensagem: 

corrupted double-linked list: 0x0000000000b9df60 ***

Program received signal SIGABRT, Aborted.
0x00007ffff6d2dcc9 in __GI_raise (sig=sig@entry=6)
    at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
56	../nptl/sysdeps/unix/sysv/linux/raise.c: No such file or directory.

Possivelmente é algo ligado a mal uso de memória alocada, visto que o problema se dá pelo uso de uma matriz alocada dinamicamente. Não consegui descobrir a origem do erro ainda. Talvez a melhor solução seja substituir a matriz por uma outra estrutura (como lista encadeada), já que uma possibilidade seja a matriz que aloco ser grande demais para a máquina virtual ou alguma funcionalodade/característica dela não consiga atuar bem com a dita matriz.
