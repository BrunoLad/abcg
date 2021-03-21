# Atividade 1

## Desenvolvedor
Bruno Menezes Gottardo Ladeia - 11048216

## Descrição
O programa basicamente trata de uma renderização de molécula de DNA desenhando os pontos 2 a 2 para representar as hélices do DNA
fazendo uso das funções arco seno e arco cosseno, porém repetindo as de forma indefinidas, sendo resetadas a partir do momento que atingirem o limit superior do viewPort.

Já para as ligações peptídicas, foram desenhadas linhas (com pontos que possuem as mesmas coordenadas que as da hélices opostas), com uma cor vermelha fixa para diferenciação e leve gradiante entre vermelho e preto para dá um efeito visual mais agradável para a figura final.

## Comentários
Os valores definidos como incremento para o step foram encontrados de forma empírica, porém os testes acabaram sendo realizados numa máquina que possuía FPS bastante elevados em comparação com os 75 fixo do WebGL, infelizmente a adaptação para o webGL acabou não ficando tão legal, ou então levou a um funcionamento não tão bom em desktop.

Além disso utilizar o intervalo de tempo "puro" em si, não foi uma alternativa muito boa. Talvez utilizando um valor relativo com base no FPS (o mesmo vale para o incremento nas funções arco seno e arco cosseno) fosse uma alternativa melhor.
