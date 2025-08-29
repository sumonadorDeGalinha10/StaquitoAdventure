#!/bin/bash

# Voltar para a pasta raiz do projeto
cd ..

# Compilar a Raylib primeiro
echo "Compilando Raylib..."
cd scr/raylib/src

# Tentar compilar com make (se disponível) ou com gcc diretamente
if command -v make &> /dev/null; then
    make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC
else
    echo "Make não encontrado, compilando diretamente com gcc..."
    gcc -c *.c -I. -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -O2
    ar rcs libraylib.a *.o
fi

cd ../../..

# Compilar seu jogo
echo "Compilando o jogo..."
gcc -o MeuJogo.exe scr/main.c scr/player.c scr/powerUp.c scr/enemy.c  -Iscr/raylib/src -Lscr/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -static

echo "Compilação concluída!"