# ImageStegoLDPC

## Descrição

**ImageStegoLDPC** é uma ferramenta que combina técnicas de esteganografia e correção de erros para garantir a segurança e integridade das imagens ocultas. O projeto utiliza o método de **Low-Density Parity-Check (LDPC)** para corrigir erros, proporcionando uma maneira robusta e eficiente de esconder uma imagem dentro de outra.

### O que é Esteganografia?

A esteganografia é a prática de esconder informações dentro de outras informações, de forma que a presença das informações ocultas não seja perceptível. No contexto de imagens, isso envolve esconder uma imagem dentro de outra imagem. A ideia é que a imagem oculta seja invisível para o observador comum, mas pode ser extraída e revelada quando necessário.

### O que é Correção de Erros com Paridade de Baixa Densidade (LDPC)?

O LDPC é um tipo de código de correção de erros que é utilizado para detectar e corrigir erros em dados transmitidos ou armazenados. Ele é baseado em uma matriz de paridade de baixa densidade que permite a detecção e correção de erros de maneira eficiente. No contexto deste projeto, o LDPC é utilizado para garantir que a imagem oculta possa ser recuperada corretamente, mesmo que haja algumas corrupções ou erros na imagem carregada.

## Funcionalidades

- **Esteganografia de Imagens:** Esconde uma imagem dentro de outra imagem.
- **Correção de Erros LDPC:** Garante a integridade da imagem oculta mesmo em condições adversas.
- **Extração de Imagem:** Permite a recuperação da imagem oculta a partir da imagem carregada.

## Como Usar

1. **Instalação:**
   Clone o repositório e instale as dependências necessárias.

   ```bash
   git clone https://github.com/seuusuario/ImageStegoLDPC.git
   cd ImageStegoLDPC
