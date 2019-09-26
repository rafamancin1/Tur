---------------Simulador de uma Máquina de Turing Determinística reconhecedora de linguagens------------------
    Feito por: Rafael Mancini Santos    


    Informações sobre como executar o programa são dadas ao simplesmente executar o programa sem argumentos(Ex.: ./Sim).
Entradas do programa:
----------------
    A primeira entrada do programa será um arquivo contendo a descrição da máquina. O arquivo deve estar no formato do exemplo(desc_mt1.txt). Se
alguma função de transição da descrição tiver uma leitura ou escrita inválida(conter caracteres não presentes no alfabeto da fita), ela é considerada
como inválida, e o programa aborta.
----------------
    A segunda entrada será a palavra a ser reconhecida. Se a palavra conter caracteres que não estão contidos no alfabeto de entrada da máquina
descrita pela entrada anterior, o programa aborta.
----------------
    Por fim, a terceira entrada deve ser o nome do arquivo de saída. Este arquivo conterá todas as configurações da simulação e uma palavra "aceita"
ou "rejeita". Explicando o óbvio, "aceita" indica que a palavra fornecida como entrada é aceita pela linguagem, e "rejeita" indica o oposto.

Exemplo de execução:

[computador@user TuringMachineSim]$ ./Sim desc_mt1.txt aaabbb saida.txt
    


