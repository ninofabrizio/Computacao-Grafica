# Trabalho 1

## Objetivo:
--------------------------------
Mudar o programa original constituído pelos arquivos main_Nino.c , image_Nino.c e image_Nino.h para que o mesmo saiba contar objetos numa imagem, o importante é que ele conte certo na maioria das vezes.

## Como pode ser feito:
--------------------------------
É permitido alcançar o objetivo colocando mais botões e implementando mais funções.

## O que implementei:
--------------------------------
- main_Nino.c:
	- Novo Handle "change" que representa um novo botão na interface
	- Novo Callback "change_cb" para o Handle "change"
	- Mudanças no Callback "save_cb"
	- Mudanças no Callback "erode_cb"
	- Mudanças no Callback "dilate_cb"
	- Mudanças no Callback "count_cb"

- image_Nino.c:
	- Nova Function pública "imgChangeBinary" chamada por "change_cb"
	- Nova Function pública "imgErotion" chamada por "erode_cb"
	- Nova Function pública "imgDilatation" chamada por "dilate_cb"
	- Nova Function pública "imgCount" chamada por "count_cb"
	- Nova Function privada "hitErotion" chamada por "imgErotion"
	- Nova Function privada "hitDilatation" chamada por "imgDilatation"
	- Nova Function privada "markAsRead" chamada por "imgCount"
	- Nova Function privada "changeNumber" chamada por "markAsRead"

Obs.: Para mais informações sobre as Functions, ver documentação em image_Nino.h

## Como compilar/executar:
-------------------------------
No terminal do Ubuntu, usar comando:

gcc *.c -o trab -liup -liupgl -lm -lGL -lGLU

Executar com:

./trab

Para usar debugger:

gcc -g *.c -o trab -liup -liupgl -lm -lGL -lGLU

Executar com:

gdb ./trab
