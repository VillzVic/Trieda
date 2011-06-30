#!/usr/bin/bash

# Nome (e caminho) do compilador a ser utilizado
GG="g++"

# Nome (e caminho) do executavel a criar
EXEC="trieda.exe"

# Arquivo com a lista de fontes (um .cpp por linha)
SOURCES="arquivosTRIEDA.txt"

# Flags de compilação para os .cpp
CFLAGS="-g"

# Bibliotecas necessarias para a compilacao dos .cpp
CLIBS="-I /home/pedro/Documentos/codelib -I /home/pedro/Documentos/code_synthesis_3_3/libxsd -I /home/pedro/Documentos/OPT_LP/include -I /home/pedro/Documentos/xerces-c-3.1.1/include -I /opt/gurobi/gurobi402/linux32/include"

# Flags de link-edicao
# LDFLAGS="-lsocket -lnsl"
LDFLAGS="-lpthread -lm"

# Bibliotecas necessarias para a link-edicao
#LDLIBS="/opt/gurobi/gurobi402/linux32/lib/libgurobi40.so /home/pedro/Documentos/xerces-c-3.1.1/lib/libxerces-c-3.1.so"
LDLIBS="/opt/gurobi/gurobi402/linux32/lib/libgurobi40.so /home/pedro/Documentos/xerces-c-3.1.1/lib/libxerces-c-3.1.so"

# Dependencias que devem ser link-editadas, mas que nao foram compiladas
DEP=""

echo Using CFLAGS = $CFLAGS
echo Using CLIBS = $CLIBS

list=""
while read name
do
	echo "Compilando $name..."
	echo comando: $GG -c $name $CFLAGS $CLIBS
	if ! $GG -dM -c $name $CFLAGS $CLIBS ; then
		echo "Erro processando o arquivo $name."
		exit
	fi
	list="$list ${name%.cpp}.o"
done < $SOURCES

echo "Gerando o executavel $EXEC..."
if ! $GG -o $EXEC $list $DEP $LDFLAGS $LDLIBS ; then
	echo "Erro na link-edicao de $EXEC."
fi

