# CriarMask
Algoritmo para criar mascaras de imagem através de threshold. É aplicado em todos os arquivos de uma pasta e salva com o mesmo nome em uma segunda pasta.

Parâmetros de entrada:
  1- Endereco da pasta de entrada
  2- Endereco da pasta de saida
  3- Largura da imagem de saida
  4- Altura da imagem de saida
  5- Valor do threshold de 0 a 255
  6- Preenche buracos no meio da imagem a partir da expansão dos 4 cantos da imagem para destacar o fundo. Importante o objeto de trabalho estar posicionado fora dos
  cantos da imagem, preferencialmnente no meio.
    1 para aplicar
    0 para nao.
