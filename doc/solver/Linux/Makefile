# ---------------------------------------------------------------------
# Compiler options
# ---------------------------------------------------------------------
CCC = g++
CC  = gcc
CCOPT = -O3 -fPIC -fexceptions -DNDEBUG -DIL_STD -Wno-deprecated "-DDOUBLE=int"
COPT  = -O3 -fPIC
CCOBJ = -c -O3 -W -Wall

# ---------------------------------------------------------------------
# Compilation and linking flags enabling Multi-threading
# ---------------------------------------------------------------------
CMTFLAGS = -DILOUSEMT -D_REENTRANT
LDMTFLAGS = -lilocplex -lcplex -lpthread

# ---------------------------------------------------------------------
# Code optimization/debugging options
# ---------------------------------------------------------------------
DEBUG = -O1 -DNDEBUG

# ---------------------------------------------------------------------
# codelib and OPT_LP paths
# ---------------------------------------------------------------------
CODELIBDIR = /home/pedro/Documentos/codelib
OPTLPDIR = /home/pedro/Documentos/OPT_LP
OPTLPINCDIR = $(OPTLPDIR)/include
OPTLPSRCDIR = $(OPTLPDIR)/source

# ---------------------------------------------------------------------
# xml parser paths
# ---------------------------------------------------------------------
SYNTHESISDIR = /home/pedro/Documentos/code_synthesis_3_3 #code_sythesis
SYSTHESISXSDDIR = $(SYNTHESISDIR)/libxsd
XERCESDIR = /home/pedro/Documentos/xerces-c-3.1.1
XERCESINCDIR = $(XERCESDIR)/include

# ---------------------------------------------------------------------
# Gurobi paths
# ---------------------------------------------------------------------
#Gurobi 4.02
SYSTEM = linux32
GUROBIVERSION = gurobi402
GUROBIDIR = /opt/gurobi/$(GUROBIVERSION)/$(SYSTEM)

# All
GUROBIINCDIR = $(GUROBIDIR)/include
GUROBILIBDIR = $(GUROBIDIR)/lib

# ---------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------
EXDIR = $(GUROBIDIR)/examples
EXCPP = $(EXDIR)/c++
EXDATA = $(EXDIR)/data

# ---------------------------------------------------------------------
# Flags
# ---------------------------------------------------------------------
CLIBS = -I$(CODELIBDIR) -I$(OPTLPINCDIR) -I$(SYSTHESISXSDDIR) -I$(XERCESINCDIR) -I$(GUROBIINCDIR)

# Flags de link-edicao
LDFLAGS="-lsocket -lnsl"

# Bibliotecas necessarias para a link-edicao
LDLIBS= -L$(GUROBILIBDIR)

# ---------------------------------------------------------------------
# Comands
# ---------------------------------------------------------------------
PRINTLN = echo

#---------------------------------------------------------
# Files
#---------------------------------------------------------
PROBLEM = MDMSTProblem
MODEL1 = SECFormulation
MODEL2 = CutSetFormulation
MODEL3 = MTZFormulation
MODEL4 = Reformulation
MODEL5 = QuadraticFormulation
BC = BranchAndCut
FLOWNET = Flownet
GREEDYSETS = Greedysets
KRUSKAL = Kruskal

#---------------------------------------------------------
# Modules
#---------------------------------------------------------
OBJ = Structures/Edge.o Structures/Node.o $(PROBLEM)/$(PROBLEM).o \
	  $(MODEL1)/$(MODEL1).o $(MODEL2)/$(MODEL2).o $(MODEL3)/$(MODEL3).o $(MODEL4)/$(MODEL4).o $(MODEL5)/$(MODEL5).o $(BC)/$(BC).o \
	  $(FLOWNET)/$(FLOWNET).o $(FLOWNET)/maxflow/glib.o $(FLOWNET)/maxflow/flow.o \
	  $(FLOWNET)/dinic/dinic.o $(GREEDYSETS)/$(GREEDYSETS).o $(KRUSKAL)/$(KRUSKAL).o

PLED = $(FLOWNET)/pled/maxflow_init.o $(FLOWNET)/pled/maxflow.o \
	   $(FLOWNET)/pled/permute.o $(FLOWNET)/pled/queue.o $(FLOWNET)/pled/netio.o \
	   $(FLOWNET)/pled/dyn_tree_maxflow.o  $(FLOWNET)/pled/fib_heap_maxflow.o \
	   $(FLOWNET)/pled/big_relabel.o $(FLOWNET)/pled/dyn_splay_maxflow.o
	   
all: clear compile

open: clean
	kate BranchAndCut/BranchAndCut.* Flownet/Flownet.* Makefile Util/Util.h MDMSTProblem/MDMSTProblem.cpp &

compile: compileAtendimentos

runc: clear compile 58

run: clear 58

runct: clear compile 1

# ./program <input> <output>
val: clear compile
# 	valgrind --leak-check=full -v --show-reachable=yes ./mdmst.e ../inputs/sym_crd/sym300.dat sym300_3.out 3
	MALLOC_CHECK_=1 valgrind --tool=callgrind --tool=cachegrind --tool=memcheck --leak-check=full -v --show-reachable=yes ./mdmst.e ../inputs/sym_crd/sym300.dat sym300_3.out 3

rval:
	valgrind ./mdmst.e ../inputs/sym_crd/crd301.dat crd301_5.out 5 

test:
	./mdmst.e ../inputs/test/test1.dat test.out 3

alm:
	./mdmst.e ../inputs/alm/tb8ch500_0.txt saida.txt 5

1:
	./mdmst.e ../inputs/sym_crd/sym300.dat sym300_3.out 3

2:
	./mdmst.e ../inputs/sym_crd/sym301.dat sym301_3.out 3

3:
	./mdmst.e ../inputs/sym_crd/sym302.dat sym302_3.out 3

4:
	./mdmst.e ../inputs/sym_crd/sym300.dat sym300_5.out 5

5:
	./mdmst.e ../inputs/sym_crd/sym301.dat sym301_5.out 5

6:
	./mdmst.e ../inputs/sym_crd/sym302.dat sym302_5.out 5

7:
	./mdmst.e ../inputs/sym_crd/sym500.dat sym500_3.out 3

8:
	./mdmst.e ../inputs/sym_crd/sym501.dat sym501_3.out 3

9:
	./mdmst.e ../inputs/sym_crd/sym502.dat sym502_3.out 3

10:
	./mdmst.e ../inputs/sym_crd/sym500.dat sym500_5.out 5

11:
	./mdmst.e ../inputs/sym_crd/sym501.dat sym501_5.out 5

12:
	./mdmst.e ../inputs/sym_crd/sym502.dat sym502_5.out 5

13:
	./mdmst.e ../inputs/sym_crd/sym500.dat sym500_10.out 10

14:
	./mdmst.e ../inputs/sym_crd/sym501.dat sym501_10.out 10

15:
	./mdmst.e ../inputs/sym_crd/sym502.dat sym502_10.out 10

16:
	./mdmst.e ../inputs/sym_crd/crd300.dat crd300_3.out 3

17:
	./mdmst.e ../inputs/sym_crd/crd301.dat crd301_3.out 3

18:
	./mdmst.e ../inputs/sym_crd/crd302.dat crd302_3.out 3

19:
	./mdmst.e ../inputs/sym_crd/crd300.dat crd300_5.out 5

20:
	./mdmst.e ../inputs/sym_crd/crd301.dat crd301_5.out 5

21:
	./mdmst.e ../inputs/sym_crd/crd302.dat crd302_5.out 5

22:
	./mdmst.e ../inputs/sym_crd/crd500.dat crd500_3.out 3

23:
	./mdmst.e ../inputs/sym_crd/crd501.dat crd501_3.out 3

24:
	./mdmst.e ../inputs/sym_crd/crd502.dat crd502_3.out 3

25:
	./mdmst.e ../inputs/sym_crd/crd500.dat crd500_5.out 5

26:
	./mdmst.e ../inputs/sym_crd/crd501.dat crd501_5.out 5

27:
	./mdmst.e ../inputs/sym_crd/crd502.dat crd502_5.out 5

28:
	./mdmst.e ../inputs/sym_crd/crd500.dat crd500_10.out 10

29:
	./mdmst.e ../inputs/sym_crd/crd501.dat crd501_10.out 10

30:
	./mdmst.e ../inputs/sym_crd/crd502.dat crd502_10.out 10

38:
	./mdmst.e ../inputs/sym_crd/sym701.dat sym701_10.out 10

# 58:
# 	./mdmst.e ../inputs/alm/density_20p/tb8ch100_0.txt tb8ch100_0.out 10 tb8ch100_0.txt-10.sol

58:
	./mdmst.e ../inputs/alm/complete/tb8ch100_0.txt tb8ch100_0.out 10 ../outputs/alm/solutions/tb8ch100_0.txt-10.sol

82:
	./mdmst.e ../inputs/alm/density_10p/tb8ch500_0.txt-10 tb8ch500_0.out 5 ../outputs/alm/solutions/tb8ch500_0.txt-5.sol

compileAtendimentos: AtendimentoCampus.o AtendimentoCampusSolucao.o AtendimentoDiaSemana.o AtendimentoDiaSemanaSolucao.o AtendimentoHorarioAula.o \
	AtendimentoOferta.o AtendimentoOfertaSolucao.o AtendimentoSala.o AtendimentoSalaSolucao.o AtendimentoTatico.o AtendimentoTaticoSolucao.o \ 	
	AtendimentoTurno.o AtendimentoUnidade.o AtendimentoUnidadeSolucao.o


AtendimentoCampus.o: AtendimentoCampus.cpp AtendimentoCampus.h ofbase.h AtendimentoUnidade.h Campus.h
	$(CCC) $(CCOBJ) AtendimentoCampus.cpp -o AtendimentoCampus.o

AtendimentoCampusSolucao.o: AtendimentoCampusSolucao.cpp AtendimentoCampusSolucao.h ofbase.h AtendimentoUnidade.h Campus.h
	$(CCC) $(CCOBJ) AtendimentoCampusSolucao.cpp -o AtendimentoCampusSolucao.o

AtendimentoDiaSemana.o: AtendimentoDiaSemana.cpp AtendimentoDiaSemana.h ofbase.h AtendimentoUnidade.h Campus.h
	$(CCC) $(CCOBJ) AtendimentoDiaSemana.cpp -o AtendimentoDiaSemana.o

AtendimentoDiaSemanaSolucao.o: AtendimentoDiaSemanaSolucao.cpp AtendimentoDiaSemanaSolucao.h ofbase.h AtendimentoUnidade.h Campus.h
	$(CCC) $(CCOBJ) AtendimentoDiaSemanaSolucao.cpp -o AtendimentoDiaSemanaSolucao.o

AtendimentoHorarioAula.o: AtendimentoHorarioAula.cpp AtendimentoHorarioAula.h ofbase.h AtendimentoUnidade.h Campus.h
	$(CCC) $(CCOBJ) AtendimentoHorarioAula.cpp -o AtendimentoHorarioAula.o



./Structures/Edge.o: ./Structures/Edge.cpp ./Structures/Edge.h
	$(CCC) $(CCOBJ) ./Structures/Edge.cpp -o ./Structures/Edge.o

./Structures/Node.o: ./Structures/Node.cpp ./Structures/Node.h
	$(CCC) $(CCOBJ) ./Structures/Node.cpp -o ./Structures/Node.o

compileProblem: ./$(PROBLEM)/$(PROBLEM).o

./$(PROBLEM)/$(PROBLEM).o: ./$(PROBLEM)/$(PROBLEM).cpp ./$(PROBLEM)/$(PROBLEM).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(PROBLEM)/$(PROBLEM).cpp -o ./$(PROBLEM)/$(PROBLEM).o

compileModels: ./$(MODEL1)/$(MODEL1).o ./$(MODEL2)/$(MODEL2).o ./$(MODEL3)/$(MODEL3).o ./$(MODEL4)/$(MODEL4).o ./$(MODEL5)/$(MODEL5).o

./$(MODEL1)/$(MODEL1).o: ./$(MODEL1)/$(MODEL1).cpp ./$(MODEL1)/$(MODEL1).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(MODEL1)/$(MODEL1).cpp -o ./$(MODEL1)/$(MODEL1).o

./$(MODEL2)/$(MODEL2).o: ./$(MODEL2)/$(MODEL2).cpp ./$(MODEL2)/$(MODEL2).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(MODEL2)/$(MODEL2).cpp -o ./$(MODEL2)/$(MODEL2).o

./$(MODEL3)/$(MODEL3).o: ./$(MODEL3)/$(MODEL3).cpp ./$(MODEL3)/$(MODEL3).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(MODEL3)/$(MODEL3).cpp -o ./$(MODEL3)/$(MODEL3).o

./$(MODEL4)/$(MODEL4).o: ./$(MODEL4)/$(MODEL4).cpp ./$(MODEL4)/$(MODEL4).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(MODEL4)/$(MODEL4).cpp -o ./$(MODEL4)/$(MODEL4).o

./$(MODEL5)/$(MODEL5).o: ./$(MODEL5)/$(MODEL5).cpp ./$(MODEL5)/$(MODEL5).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(MODEL5)/$(MODEL5).cpp -o ./$(MODEL5)/$(MODEL5).o
	
compileBranchAndCut: ./$(BC)/$(BC).o

./$(BC)/$(BC).o: ./$(BC)/$(BC).cpp ./$(BC)/$(BC).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(BC)/$(BC).cpp -o ./$(BC)/$(BC).o

compileMaxFlow: $(FLOWNET)/maxflow/glib.o $(FLOWNET)/maxflow/flow.o $(FLOWNET)/dinic/dinic.o $(PLED)

$(FLOWNET)/maxflow/glib.o: $(FLOWNET)/maxflow/glib.cc $(FLOWNET)/maxflow/graphtypes.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/maxflow/glib.cc -o $(FLOWNET)/maxflow/glib.o

$(FLOWNET)/maxflow/flow.o: $(FLOWNET)/maxflow/flow.cc $(FLOWNET)/maxflow/graphtypes.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/maxflow/flow.cc -o $(FLOWNET)/maxflow/flow.o

$(FLOWNET)/dinic/dinic.o: $(FLOWNET)/dinic/dinic.cpp $(FLOWNET)/dinic/dinic.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/dinic/dinic.cpp -o $(FLOWNET)/dinic/dinic.o

$(FLOWNET)/pled/maxflow_init.o: $(FLOWNET)/pled/maxflow_init.cpp $(FLOWNET)/pled/_maxflow.h $(FLOWNET)/pled/maxflow_typedef.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/maxflow_init.cpp -o $(FLOWNET)/pled/maxflow_init.o

$(FLOWNET)/pled/maxflow.o: $(FLOWNET)/pled/maxflow.cpp $(FLOWNET)/pled/_maxflow.h $(FLOWNET)/pled/maxflow_typedef.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/maxflow.cpp -o $(FLOWNET)/pled/maxflow.o

$(FLOWNET)/pled/permute.o: $(FLOWNET)/pled/permute.cpp $(FLOWNET)/pled/_maxflow.h $(FLOWNET)/pled/maxflow_typedef.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/permute.cpp -o $(FLOWNET)/pled/permute.o

$(FLOWNET)/pled/queue.o: $(FLOWNET)/pled/queue.cpp $(FLOWNET)/pled/queue.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/queue.cpp -o $(FLOWNET)/pled/queue.o

$(FLOWNET)/pled/dyn_tree_maxflow.o: $(FLOWNET)/pled/dyn_tree_maxflow.cpp $(FLOWNET)/pled/dyn_tree_maxflow.h $(FLOWNET)/pled/_dyn_tree_maxflow.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/dyn_tree_maxflow.cpp -o $(FLOWNET)/pled/dyn_tree_maxflow.o

$(FLOWNET)/pled/dyn_splay_maxflow.o: $(FLOWNET)/pled/dyn_splay_maxflow.cpp $(FLOWNET)/pled/_dyn_tree_maxflow.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/dyn_splay_maxflow.cpp -o $(FLOWNET)/pled/dyn_splay_maxflow.o

$(FLOWNET)/pled/fib_heap_maxflow.o: $(FLOWNET)/pled/fib_heap_maxflow.cpp $(FLOWNET)/pled/maxflow_typedef.h $(FLOWNET)/pled/_fib_heap_maxflow.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/fib_heap_maxflow.cpp -o $(FLOWNET)/pled/fib_heap_maxflow.o

$(FLOWNET)/pled/netio.o: $(FLOWNET)/pled/netio.cpp $(FLOWNET)/pled/netio.h $(FLOWNET)/pled/_maxflow.h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/netio.cpp -o $(FLOWNET)/pled/netio.o

$(FLOWNET)/pled/big_relabel.o: $(FLOWNET)/pled/big_relabel.cpp
	$(CCC) $(CCFLAGS) -c -Wno-deprecated $(FLOWNET)/pled/big_relabel.cpp -o $(FLOWNET)/pled/big_relabel.o

compileFlownet: ./$(FLOWNET)/$(FLOWNET).o

./$(FLOWNET)/$(FLOWNET).o: ./$(FLOWNET)/$(FLOWNET).cpp ./$(FLOWNET)/$(FLOWNET).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(FLOWNET)/$(FLOWNET).cpp -o ./$(FLOWNET)/$(FLOWNET).o

compileGreedysets: ./$(GREEDYSETS)/$(GREEDYSETS).o

./$(GREEDYSETS)/$(GREEDYSETS).o: ./$(GREEDYSETS)/$(GREEDYSETS).cpp ./$(GREEDYSETS)/$(GREEDYSETS).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(GREEDYSETS)/$(GREEDYSETS).cpp -o ./$(GREEDYSETS)/$(GREEDYSETS).o

compileKruskal: ./$(KRUSKAL)/$(KRUSKAL).o

./$(KRUSKAL)/$(KRUSKAL).o: ./$(KRUSKAL)/$(KRUSKAL).cpp ./$(KRUSKAL)/$(KRUSKAL).h
	$(CCC) $(CCFLAGS) -c -Wno-deprecated ./$(KRUSKAL)/$(KRUSKAL).cpp -o ./$(KRUSKAL)/$(KRUSKAL).o

compileMain:
	$(CCC) $(CCFLAGS) $(OBJ) $(PLED) ./Main/Main.cpp -o mdmst.e $(CCLNFLAGS)

date:
	sudo date 111209522003

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf mdmst.e
	rm -rf ./Structures/*.o ./Structures/*~
	rm -rf ./LocalBranching/*.o ./LocalBranching/*~
	rm -rf ./$(PROBLEM)/*.o ./$(PROBLEM)/*~
	rm -rf ./$(MODEL1)/*.o ./$(MODEL1)/*~
	rm -rf ./$(MODEL2)/*.o ./$(MODEL2)/*~
	rm -rf ./$(MODEL3)/*.o ./$(MODEL3)/*~
	rm -rf ./$(MODEL4)/*.o ./$(MODEL4)/*~
	rm -rf ./$(MODEL5)/*.o ./$(MODEL5)/*~
	rm -rf ./$(BC)/*.o ./$(BC)/*~
	rm -rf ./$(FLOWNET)/*.o ./$(FLOWNET)/*~
	rm -rf ./$(FLOWNET)/maxflow/*.o ./$(FLOWNET)/maxflow/*~
	rm -rf ./$(FLOWNET)/dinic/*.o ./$(FLOWNET)/dinic/*.o
	rm -rf ./$(FLOWNET)/pled/*.o ./$(FLOWNET)/pled/*.o
	rm -rf ./$(GREEDYSETS)/*.o ./$(GREEDYSETS)/*~
	rm -rf ./$(KRUSKAL)/*.o ./$(KRUSKAL)/*~
	rm -rf ./Util/*.o ./Util/*~
	rm -rf ./Main/*.o ./Main/*~
	clear

clear:
	clear
