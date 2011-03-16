#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoSala.h"

using namespace std;

//#define DBG_SALA

class AtendimentoUnidade:
   public OFBase
{
public:
   AtendimentoUnidade(void);
   virtual ~AtendimentoUnidade(void);

   std::string unidade_id;
   GGroup<AtendimentoSala*> * atendimentos_salas;

   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);

   // >>>

//   static GGroup<int/*ids de unidades existentes*/> *__ids_cadastrados;
//
//   void addSala(int id, std::string descricao, int __id)
//   {
//#ifdef DBG_SALA
//      std::cout << ">> > >>" << std::endl;
//#endif
//      if(__ids_cadastrados->size() == 0){
//#ifdef DBG_SALA
//         std::cout << "Ainda nao existe nenhuma UNIDADE registrada. Para registrar uma SALA," <<
//            "registre uma UNIDADE antes." << std::endl;
//#endif
//      }
//      else {
//         bool encontrou_id_cadastrado = false;
//         GGroup<int>::iterator it__ids_cadastrados = __ids_cadastrados->begin();
//         for(; it__ids_cadastrados != __ids_cadastrados->end(); it__ids_cadastrados++ ) {
//            if( *it__ids_cadastrados == __id ) {
//               encontrou_id_cadastrado = true;
//               break;
//            }
//         }
//
//         if(encontrou_id_cadastrado) {
//            AtendimentoSala *__at;
//            if( atendimentos_salas.size() == 0 ) {
//#ifdef DBG_SALA
//               std::cout << "Ainda nao existe nenhuma SALA da UNIDADE \"" << __id << "\" adicionada a base.\n\tSALA \"" <<
//                  descricao << "\" com id \"" << id << "\"  adicionada." << std::endl;
//#endif
//               __at = new AtendimentoSala;
//               __at->setId(id);
//               __at->sala_id = descricao;
//               // >>>
//               __at->__ids_cadastrados->add(id);
//               // <<<
//               atendimentos_salas.add(__at);
//            }
//            else {
//               bool __add = true;
//               ITERA_GGROUP(it_sala, atendimentos_salas,AtendimentoSala) {
//                  if(it_sala->getId() == id ) {
//#ifdef DBG_SALA
//                     std::cout << "O id \"" << id << "\" especificado, da SALA \"" 
//                        << descricao << "\"  ja existe." << std::endl;
//#endif
//                     __add = false;
//                     break;
//                  }
//               }
//               if(__add) {
//#ifdef DBG_SALA
//                  std::cout << "O id \"" << id << "\" especificado, da SALA \"" 
//                     << descricao << "\" nao consta na UNIDADE \"" << __id << "\". \n\tAdicionando .. ." << std::endl;
//#endif
//                  __at = new AtendimentoSala;
//                  __at->setId(id);
//                  __at->sala_id = descricao;
//                  // >>>
//                  __at->__ids_cadastrados->add(id);
//                  // <<<
//                  atendimentos_salas.add(__at);
//               }
//            }
//         }
//         else {
//#ifdef DBG_SALA
//            //std::cout << "O id \"" << __id << " informado para adicao da SALA \"" << descricao << "\" a UNIDADE \"" << __id << "\"  nao existe." << std::endl;
//            std::cout << "Id \"" << __id << "\" de UNIDADE invalido." << std::endl;
//#endif
//         }
//      }
//   }

   // <<<

};

std::ostream& operator << (std::ostream& out, AtendimentoUnidade& unidade);