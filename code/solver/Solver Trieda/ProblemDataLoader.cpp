#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include <iostream>

using namespace std;

ProblemDataLoader::ProblemDataLoader( char *inputFile, ProblemData* data )
{
   this->inputFile = inputFile;
   this->problemData  = data;
}

ProblemDataLoader::~ProblemDataLoader()
{
}

void ProblemDataLoader::load()
{
   std::cout << "Loading file..." << std::endl;
   root = std::auto_ptr<TriedaInput>(TriedaInput_(inputFile, xml_schema::flags::dont_validate));
   std::cout << "Extracting data..." << std::endl;
   problemData->le_arvore(*root);

   std::cout << "Some preprocessing..." << std::endl;
   /* processamento */

   // ---------
   relacionaCredsRegras();

   // ---------
   carregaDiasLetivosCampusUnidadeSala();

   // ---------
   carregaDiasLetivosDiscs();

   // ---------
   divideDisciplinas();

   // ---------
   referenciaCampusUnidadesSalas();

   // ---------
   referenciaDisciplinas();

   // ---------
   referenciaOfertas();

   // ---------
   gera_refs();

   // ---------
   cria_blocos_curriculares();

   // ---------
   calculaTamanhoMedioSalasCampus();

   // ---------
   relacionaCampusDiscs();

   // ---------
   calculaDemandas();

   // ---------
   relacionaDiscOfertas();

   // ---------
   estima_turmas();

   // ---------
   associaDisciplinasSalas();

   // ---------
   criaConjuntoSalasUnidade();
   //criaConjuntoSalasCampus();

   // ---------
   cache();

   // ---------
   estabeleceDiasLetivosBlocoCampus();

   // ---------
   estabeleceDiasLetivosDisciplinasSalas();

   // ---------
   estabeleceDiasLetivosDiscCjtSala();

   // ---------
   calculaCredsLivresSalas();

   // ---------
   combinacaoDivCreditos();

   // ---------
   print_stats();
   //print_csv();

}

void ProblemDataLoader::relacionaCredsRegras()
{
   ITERA_GGROUP(it_Regra,problemData->regras_div,DivisaoCreditos)
   { 
      problemData->creds_Regras[it_Regra->creditos].add(*it_Regra);
   }
}

void ProblemDataLoader::carregaDiasLetivosCampusUnidadeSala()
{
   // Para cada Campus
   ITERA_GGROUP(it_Campus,problemData->campi,Campus)
   {
      // --------------------------------------------------------
      // Adicionando os dias letivos de cada Campus
      ITERA_GGROUP(it_Horario,it_Campus->horarios,Horario)
      {
         ITERA_GGROUP_N_PT(it_Dias_Letivos,it_Horario->dias_semana,int)
         {
            it_Campus->diasLetivos.add(*it_Dias_Letivos);
         }         
      }

      // --------------------------------------------------------

      // Para cada Unidade
      ITERA_GGROUP(it_Unidade,it_Campus->unidades,Unidade)
      {
         // --------------------------------------------------------
         // Adicionando os dias letivos de cada Unidade
         ITERA_GGROUP(it_Horario,it_Unidade->horarios,Horario)
         {
            ITERA_GGROUP_N_PT(it_Dias_Letivos,it_Horario->dias_semana,int)
            {
               it_Unidade->diasLetivos.add(*it_Dias_Letivos);
            }         
         }

         // --------------------------------------------------------
         // Para cada Sala
         ITERA_GGROUP(it_Sala,it_Unidade->salas,Sala)
         {
            // --------------------------------------------------------
            // Adicionando os dias letivos de cada Sala
            ITERA_GGROUP(it_Horario,it_Unidade->horarios,Horario)
            {
               ITERA_GGROUP(it_Creds_Disp,it_Sala->creditos_disponiveis,CreditoDisponivel)
               {
                  if(it_Creds_Disp->max_creditos > 0)
                  { 
                     it_Sala->diasLetivos.add(it_Creds_Disp->dia_semana);
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::carregaDiasLetivosDiscs()
{
   ITERA_GGROUP(it_Disc,problemData->disciplinas,Disciplina)
   {
      ITERA_GGROUP(it_Horario,it_Disc->horarios,Horario)
      {
         ITERA_GGROUP_N_PT(it_Dias_Letivos,it_Horario->dias_semana,int)
         { 
            it_Disc->diasLetivos.add(*it_Dias_Letivos);
         }
      }
   }
}

void ProblemDataLoader::criaConjuntoSalasUnidade()
{
   problemData->totalConjuntosSalas = 0;

   ITERA_GGROUP(it_Campus,problemData->campi,Campus)
   {
      int idCjtSala = 1;

      ITERA_GGROUP(it_Unidade,it_Campus->unidades,Unidade)
      {
         /* Conjunto de salas (LABORATORIOS) que tiveram de ser criadas, dado que a possuiam pelo menos 
         uma disciplina com a FLAG <eLab> marcada (TRUE) */
         GGroup<ConjuntoSala*> conjunto_Salas_Disc_eLab;

         /* Conjunto de salas (SALAS OU LABORATORIOS) que foram criadas sendo que n�o possuiam nenhuma 
         disciplina com a FLAG <eLab> marcada (TRUE) */
         GGroup<ConjuntoSala*> conjunto_Salas_Disc_GERAL;

         ITERA_GGROUP(it_Sala,it_Unidade->salas,Sala)
         {
            bool exige_Conjunto_Individual = false;

            /* Checando se a sala em quest�o exige
            a cria��o de um Conjunto de Salas s� pra ela.

            Ex.: Qdo uma sala, na verdade, um laborat�rio possui pelo menos uma 
            disciplina com a FLAG <eLab> marcada (TRUE).
            */

            if(it_Sala->getTipoSalaId() == 2)
            {
               ITERA_GGROUP(it_Disc,it_Sala->disciplinasAssociadas,Disciplina)
               {
                  /* Procurando por, pelo menos, uma disciplina que possua a FLAG
                  <eLab> marcada (TRUE) */
                  if(it_Disc->eLab())
                  {
                     exige_Conjunto_Individual = true;
                     break;
                  }
               }
            }

            /* 
            Refer�ncia para algum dos GGroup de conjuntos de salas (<conjunto_Salas_Disc_eLab> ou 
            <conjunto_Salas_Disc_GERAL>).
            */
            //GGroup<ConjuntoSala*> & gg_Cjt_Salas_Esc = conjunto_Salas_Disc_eLab;
            GGroup<ConjuntoSala*> * gg_Cjt_Salas_Esc = &conjunto_Salas_Disc_eLab;

            /*
            Teste para escolher qual estrutura de dados (<conjunto_Salas_Disc_eLab> ou 
            <conjunto_Salas_Disc_GERAL>) deve-se utilizar.
            */
            if(!exige_Conjunto_Individual)
            { gg_Cjt_Salas_Esc = &conjunto_Salas_Disc_GERAL; }

            bool encontrou_Conjunto_Compat = false;

            /* Antes de criar um novo conjunto de salas (labs ou SA), deve-se
            procurar por algum conjunto de salas existente que represente a capacidade e
            o tipo da sala em quest�o. Al�m disso, a diferen�a das disciplinas associadas de ambos 
            tem que ser nula (ou seja, todas as disciplinas que forem associadas a sala em quest�o,
            tem de estar associadas ao conjunto de salas encontrado, e vice versa). */
            ITERA_GGROUP(it_Cjt_Salas_Disc,(*gg_Cjt_Salas_Esc),ConjuntoSala)
            {
               /* Se o conjunto de salas em quest�o representa a capacidade da sala em quest�o.

               Estou testando tb se o conjunto de salas representa o mesmo tipo da sala em quest�o.
               Acredito que n�o seja necess�rio no caso em que estejamos lidando APENAS com laborat�rios.
               Como n�o sei qual GGroup est� sendo referenciado, testo os 2 casos pra l�.
               */
               if(it_Cjt_Salas_Disc->getCapacidadeRepr() == it_Sala->capacidade
                  && it_Cjt_Salas_Disc->getTipoSalasRepr() == it_Sala->tipo_sala->getId() )
               {
                  bool mesmas_Disciplinas_Associadas = true;

                  // Checando se tem o mesmo tamanho.
                  if(it_Cjt_Salas_Disc->getDiscsAssociadas().size() == 
                     it_Sala->disciplinasAssociadas.size())
                  {
                     // Iterando sobre as disciplinas associadas da sala em quest�o.
                     ITERA_GGROUP(it_Disc_Assoc_Sala,it_Sala->disciplinasAssociadas,Disciplina)
                     {
                        /* Se encontrei alguma disciplina que n�o est� associada ao conjunto de salas e 
                        � sala em quest�o, paro o teste de compatibilidade entre a sala e o cjt em quest�o. */
                        if(it_Cjt_Salas_Disc->getDiscsAssociadas().find(*it_Disc_Assoc_Sala)
                           == it_Cjt_Salas_Disc->getDiscsAssociadas().end() )
                        { mesmas_Disciplinas_Associadas = false; break; }
                     }
                  }
                  else
                  { mesmas_Disciplinas_Associadas = false; }

                  if(mesmas_Disciplinas_Associadas)
                  {
                     it_Cjt_Salas_Disc->addSala(**it_Sala);

                     /* Adicionando os dias letivos ao conjunto de salas */
                     ITERA_GGROUP_N_PT(it_Dias_Letivos,it_Sala->diasLetivos,int)
                     {
                        it_Cjt_Salas_Disc->diasLetivos.add(*it_Dias_Letivos);
                     }

                     /* COMO AS DISCIPLINAS ASSOCIADAS S�O AS MESMAS, N�O H� NECESSIDADE DE 
                     ADICIONAR NENHUMA DISICIPLINA ASSOCIADA AO CONJUNTO DE SALAS EM QUEST�O. */

                     encontrou_Conjunto_Compat = true;
                     break;
                  }
               }
            }

            if(!encontrou_Conjunto_Compat)
            {
               ConjuntoSala * cjt_Sala = new ConjuntoSala();

               cjt_Sala->setId(idCjtSala);
               cjt_Sala->setCapacidadeRepr(it_Sala->capacidade);
               cjt_Sala->setTipoSalasRepr(it_Sala->getTipoSalaId());

               cjt_Sala->addSala(**it_Sala);

               // Atualizando para o pr�ximo id.
               ++idCjtSala;

               /* Adicionando os dias letivos ao conjunto de salas */
               ITERA_GGROUP_N_PT(it_Dias_Letivos,it_Sala->diasLetivos,int)
               {
                  cjt_Sala->diasLetivos.add(*it_Dias_Letivos);
               }

               /* Associando as disciplinas ao conjunto. */
               ITERA_GGROUP(it_Disc_Assoc_Sala,it_Sala->disciplinasAssociadas,Disciplina)
               { cjt_Sala->associaDisciplina(**it_Disc_Assoc_Sala); }

               // Adicionando ao respectivo conjunto.
               gg_Cjt_Salas_Esc->add(cjt_Sala);
            }
         }

         /* AGORA QUE TENHO TODOS OS CONJUNTOS DE SALAS CRIADOS, TENHO QUE ARMAZENA-LOS NA
         ESTRUTURA <conjuntoSala> da Unidade em quest�o. */

         ITERA_GGROUP(it_Cjt_Salas_Disc_Elab,conjunto_Salas_Disc_eLab,ConjuntoSala)
         {
            it_Unidade->conjutoSalas.add(*it_Cjt_Salas_Disc_Elab);
         }

         ITERA_GGROUP(it_Cjt_Salas_Disc_GERAL,conjunto_Salas_Disc_GERAL,ConjuntoSala)
         {
            it_Unidade->conjutoSalas.add(*it_Cjt_Salas_Disc_GERAL);
         }

         // ----------------------------
         std::cout << "Cod. Und.: " << it_Unidade->codigo << std::endl;

         ITERA_GGROUP(it_Cjt_Salas_Und,it_Unidade->conjutoSalas,ConjuntoSala)
         {
            std::cout << "\tCod. Cjt. Sala: " << it_Cjt_Salas_Und->getId() << std::endl;

            std::map<int/*Id Sala*/,Sala*>::iterator 
               it_Salas_Cjt = it_Cjt_Salas_Und->getTodasSalas().begin();

            for(; it_Salas_Cjt != it_Cjt_Salas_Und->getTodasSalas().end(); ++it_Salas_Cjt)
            { std::cout << "\t\tCod. Sala: " << it_Salas_Cjt->second->codigo << std::endl; }
         }
         // ----------------------------
      }
   }
}

void ProblemDataLoader::estabeleceDiasLetivosBlocoCampus()
{
   ITERA_GGROUP(it_Bloco_Curric,problemData->blocos,BlocoCurricular)
   {
      ITERA_GGROUP_N_PT(it_Dia_Letivo,it_Bloco_Curric->diasLetivos,int)
      {
         if(it_Bloco_Curric->campus->diasLetivos.find
            (*it_Dia_Letivo) != it_Bloco_Curric->campus->diasLetivos.end())
         {
            problemData->bloco_Campus_Dias
               [std::make_pair(it_Bloco_Curric->getId(),it_Bloco_Curric->campus->getId())].add
               (*it_Dia_Letivo);
         }
         else
         {
            /*
            PS: Quando tiver mais de um campus, pode acontecer que uma associa��o entre um bloco curricular 
            que n�o pertence a um determinado campus seja criada. Arrumar isso depois.
            Ou seja, essa checagem s� serve para quando se tem 1 campus. Se tiver mais de um, quando cair aqui,
            nada pode-se afirmar sobre a corretude da inst�ncia.
            */
            std::cout << "Warnning: Bloco Curricular e Campus Incompativeis. (ProblemDataLoader::estabeleceDiasLetivosBlocoCampus())" << std::endl;
            exit(1);
         }
      }
   }
}

void ProblemDataLoader::estabeleceDiasLetivosDisciplinasSalas()
{
   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itSala,itUnidade->salas,Sala)
         {
            ITERA_GGROUP(itDiscAssoc,itSala->disciplinasAssociadas,Disciplina)
            {
               ITERA_GGROUP_N_PT(itDiasLetivosDisc,itDiscAssoc->diasLetivos,int)
               {
                  /* Se o dia letivo da disciplina � tamb�m um dia letivo da sala em quest�o, 
                  adiciona-se ao map <disc_Salas_Dias> o dia em comum. */
                  if(itSala->diasLetivos.find(*itDiasLetivosDisc) != itSala->diasLetivos.end())
                  {
                     std::pair<int,int> ids_Disc_Sala 
                        (itDiscAssoc->getId(),itSala->getId());

                     problemData->disc_Salas_Dias[ids_Disc_Sala].add(*itDiasLetivosDisc);
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::estabeleceDiasLetivosDiscCjtSala()
{
   /* Os dias letivos das disciplinas em rela��o aos conjuntos de salas s�o obtidos
   via uni�o dos dias letivos das disciplinas em rela��o �s salas pertencentes ao conjunto
   de salas em quest�o. */

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         // p tds conjuntos de salas de um campus
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            // p tds as discAssoc de um conjunto
            ITERA_GGROUP(itDiscAssoc,itCjtSala->getDiscsAssociadas(),Disciplina)
            {
               std::map<int/*Id Sala*/,Sala*>::iterator itSala =
                  itCjtSala->getTodasSalas().begin();

               for(; itSala != itCjtSala->getTodasSalas().end(); itSala++)
               {
                  std::pair<int/*idDisc*/,int/*idSala*/> ids_Disc_Sala 
                     (itDiscAssoc->getId(),itSala->second->getId());

                  /* Se a disciplina se relaciona com a sala em questao. Como estamos 
                  lidando com um conjunto de salas, podemos ter o caso em que uma disciplina
                  � associada a uma sala do conjunto e a outra n�o. */
                  if(problemData->disc_Salas_Dias.find(ids_Disc_Sala) !=
                     problemData->disc_Salas_Dias.end())
                  {
                     ITERA_GGROUP_N_PT(itDiasLetDisc,
                        problemData->disc_Salas_Dias[ids_Disc_Sala],int)
                     {
                        problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                           (itDiscAssoc->getId(),itCjtSala->getId())].add
                           (*itDiasLetDisc);
                     }
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::calculaCredsLivresSalas()
{
   //ITERA_GGROUP(itCampus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
   //   {
   //      ITERA_GGROUP(itSala,itUnidade->salas,Sala)
   //      {
   //         for(int dia = 0; dia < 8; dia++)
   //         { itSala->credsLivres.push_back(0); }

   //         ITERA_GGROUP(itCredsDisp,itSala->creditos_disponiveis,CreditoDisponivel)
   //         { itSala->credsLivres.at(itCredsDisp->dia_semana) = itCredsDisp->max_creditos; }
   //      }
   //   }
   //}
}

void ProblemDataLoader::combinacaoDivCreditos(){

   std::vector<std::vector<std::pair<int/*dia*/, int/*numCreditos*/>>> combinacao_divisao_creditos; 
   std::vector<std::pair<int/*dia*/, int/*numCreditos*/> > vAux; 
   std::vector<std::pair<int/*dia*/, int/*numCreditos*/> > vec; 
   std::pair<int/*dia*/, int/*numCreditos*/> p;
   bool atualiza = false;

   ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
   {
      if(itDisc->divisao_creditos != NULL)
      {
         std::vector<std::pair<int/*dia*/, int/*numCreditos*/> > vAux; 
         for(int i = 1;i <= 7; i++)
         {
            p = make_pair(i,itDisc->divisao_creditos->dia[i]);
            vAux.push_back(p);
         }

         //verifica se a regra de divis�o de cr�ditos � v�lida
         for(int a = 0;a < 7;a++)
         {
            if(vAux[a].second != 0)
            {
               GGroup<int>::iterator itDiasLetivosDiscs = itDisc->diasLetivos.begin();
               for(; itDiasLetivosDiscs != itDisc->diasLetivos.end(); itDiasLetivosDiscs++)
               { 
                  if(vAux[a].first == *itDiasLetivosDiscs)
                  {
                     atualiza = true;
                     break;
                  }
                  else
                  {
                     atualiza = false;
                  }
               }
            }
         }
         if(atualiza)
         {
            itDisc->combinacao_divisao_creditos.push_back(vAux);
            atualiza = false;
         }

         //Para cada regra de divis�o de creditos pode existir mais 6
         for(int k = 0;k < 6;k++)
         {
            for(int j = 0;j < 7;j++)
            {
               if(j == 0)
               {
                  p = make_pair(vAux[0].first, vAux[6].second);
               }
               else
               {
                  p = make_pair(vAux[j].first, vAux[j-1].second);
               }
               vec.push_back(p);
            }
            vAux.clear();
            vAux = vec;
            vec.clear();

            //verifica se as regras de divis�o de cr�ditos criadas s�o v�lidas
            for(int b = 0;b < 7;b++)
            {
               if(vAux[b].second != 0)
               {
                  GGroup<int>::iterator itDiasLetivosDiscs = itDisc->diasLetivos.begin();
                  for(; itDiasLetivosDiscs != itDisc->diasLetivos.end(); itDiasLetivosDiscs++)
                  { 
                     if(vAux[b].first == *itDiasLetivosDiscs)
                     {
                        atualiza = true;
                        break;
                     }
                     else
                     {
                        atualiza = false;
                     }
                  }
               }
            }
            if(atualiza)
            {
               itDisc->combinacao_divisao_creditos.push_back(vAux);
               atualiza = false;
            }	
         }
      }
   }
}

template<class T> 
void ProblemDataLoader::find_and_set(int id, 
                                     GGroup<T*>& haystack, 
                                     T*& needle,bool print = false)
{
   T* finder = new T;
   finder->id = id;

   GGroup<T*>::iterator it_g = haystack.begin();

   /* Vers�o lenta... Entender o porqu� depois */
   while(it_g != haystack.end() && it_g->getId() != finder->getId())
   {
      ++it_g;
   }
   /* FIM */

   if (it_g != haystack.end())
   {
      needle = *it_g;

      if(print)
      {
         std::cout << "Found " << id << std::endl;
      }
   }
   else
   {
      std::cout << "Warnning: Problema na funcao FindAndSet do ProblemDataLoader." << std::endl;
      exit(1);
   }

   delete finder;
}

void ProblemDataLoader::divideDisciplinas()
{
   GGroup<Disciplina*> disciplinas_novas;
   //GGroup<Disciplina> disciplinas_novas;

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
   {
      if(it_disc->cred_teoricos > 0 && it_disc->cred_praticos > 0 && it_disc->e_lab)
      {
         Disciplina *nova_disc = new Disciplina();

         // >>> Implementar o operador de atribui��o seria mais correto.
         //nova_disc = *it_disc;
         // <<<

         nova_disc->setId(-it_disc->getId()); // alterado
         //nova_disc->setId(it_disc->getId()*1000); // alterado

         nova_disc->codigo = it_disc->codigo + "-P";
         //nova_disc->codigo = it_disc->codigo;
         nova_disc->nome = it_disc->nome + "PRATICA";

         //nova_disc->cred_teoricos = it_disc->cred_teoricos;
         nova_disc->cred_teoricos = 0; // alterado

         nova_disc->cred_praticos = it_disc->cred_praticos; // alterado
         //it_disc->cred_praticos = 0; // alterado
         it_disc->setCredsPraticos(0);

         //nova_disc->max_creds = it_disc->max_creds;
         nova_disc->max_creds = nova_disc->cred_praticos;
         //it_disc->max_creds = it_disc->cred_teoricos;
         it_disc->setMaxCreds(it_disc->cred_teoricos);

         nova_disc->e_lab = it_disc->e_lab; // alterado
         it_disc->e_lab = false; // alterado

         //nova_disc->max_alunos_t = it_disc->max_alunos_t; // alterado
         nova_disc->max_alunos_t = -1; // alterado

         nova_disc->max_alunos_p = it_disc->max_alunos_p; // alterado
         it_disc->max_alunos_p = -1; // alterado

         nova_disc->tipo_disciplina_id = it_disc->tipo_disciplina_id;
         nova_disc->nivel_dificuldade_id = it_disc->nivel_dificuldade_id;

         // -----

         if(it_disc->divisao_creditos)
         {
            std::map<int/*Num. Creds*/,GGroup<DivisaoCreditos*> >::iterator it_Creds_Regras;

            // Altera��es relacionadas � disciplina antiga

            delete it_disc->divisao_creditos;

            it_Creds_Regras = problemData->creds_Regras.find(it_disc->cred_teoricos);

            /* Checando se existe alguma regra de cr�dito cadastrada para o total
            de cr�ditos da nova disciplina. */
            if(it_Creds_Regras != problemData->creds_Regras.end())
            {
               if(it_Creds_Regras->second.size() == 1)
               {
                  it_disc->divisao_creditos = new DivisaoCreditos(
                     **it_Creds_Regras->second.begin());
               }
               else // Greather
               {
                  GGroup<DivisaoCreditos*>::iterator 
                     it_Regra = it_Creds_Regras->second.begin();

                  bool continuar = rand() % 2;

                  while(continuar && (it_Regra != it_Creds_Regras->second.end()) )
                  {
                     ++it_Regra;
                     continuar = rand() % 2;
                  }

                  it_disc->divisao_creditos = new DivisaoCreditos(
                     **it_Regra);
               }
            }

            // Altera��es relacionadas � nova disciplina

            it_Creds_Regras = problemData->creds_Regras.find(nova_disc->cred_praticos);

            /* Checando se existe alguma regra de cr�dito cadastrada para o total
            de cr�ditos da nova disciplina. */
            if(it_Creds_Regras != problemData->creds_Regras.end())
            {
               if(it_Creds_Regras->second.size() == 1)
               {
                  nova_disc->divisao_creditos = new DivisaoCreditos(
                     **it_Creds_Regras->second.begin());
               }
               else // Greather
               {
                  GGroup<DivisaoCreditos*>::iterator 
                     it_Regra = it_Creds_Regras->second.begin();

                  bool continuar = rand() % 2;

                  while(continuar && (it_Regra != it_Creds_Regras->second.end()) )
                  {
                     ++it_Regra;
                     continuar = rand() % 2;
                  }

                  nova_disc->divisao_creditos = new DivisaoCreditos(
                     **it_Regra);
               }
            }
         }

         //if(it_disc->divisao_creditos)
         //{
         //   nova_disc->divisao_creditos = new DivisaoCreditos();
         //   nova_disc->divisao_creditos->setId(it_disc->divisao_creditos->getId());
         //   nova_disc->divisao_creditos->creditos = it_disc->divisao_creditos->creditos;

         //   for(int i=0;i<8;i++) {
         //      nova_disc->divisao_creditos->dia[i] = it_disc->divisao_creditos->dia[i];
         //   }
         //}

         // -----

         // NAO PRECISA DOS ITENS ABAIXO ?! CONFIRMAR COM O CHICO

         /*
         GGroup<int>::iterator it_eq = it_disc->equivalentes.begin();
         for(unsigned i=0;i<it_disc->equivalentes.size();i++) {
         nova_disc->equivalentes.add(*it_eq);
         it_eq++;
         }
         */

         /*
         GGroup<int>::iterator it_inc = it_disc->incompativeis.begin();
         for(unsigned i=0;i<it_disc->incompativeis.size();i++) {
         nova_disc->incompativeis.add(*it_inc);
         it_inc++;
         }
         */

         //>>> Copiando HORARIO
         ITERA_GGROUP(it_hr,it_disc->horarios,Horario) {
            Horario *h =  new Horario;
            h->setId(it_hr->getId());

            //>>> >>> Copiando DiaSemana
            GGroup<int>::iterator it_dia = it_hr->dias_semana.begin();
            for(unsigned dia =0;dia<it_hr->dias_semana.size();dia++) {
               h->dias_semana.add(*it_dia);
               it_dia++;
            }
            // <<< <<<

            h->horarioAulaId = it_hr->horarioAulaId;

            h->turnoId = it_hr->turnoId;

            // >>> >>> Copiando TURNO
            Turno *tur;
            if(it_hr->turno != NULL) {
               tur = new Turno();
               tur->setId(it_hr->turno->getId());

               tur->nome = it_hr->turno->nome;

               tur->tempoAula = it_hr->turno->tempoAula;

               // >>> >>> >>> Copiando HorariosAula
               HorarioAula *hr_aula;
               if(it_hr->turno->horarios_aula.size() > 0){
                  ITERA_GGROUP(it_hr_aula,tur->horarios_aula,HorarioAula) {
                     //HorarioAula *hr_aula = new HorarioAula();
                     hr_aula = new HorarioAula();
                     hr_aula->setId(it_hr_aula->getId());
                     hr_aula->inicio = it_hr_aula->inicio;

                     GGroup<int>::iterator it_dia_sem = it_hr_aula->diasSemana.begin();
                     for(unsigned dia =0;dia<it_hr_aula->diasSemana.size();dia++) {
                        hr_aula->diasSemana.add(*it_dia_sem);
                        it_dia_sem++;
                     }
                     //tur->horarios_aula = hr_aula;
                  }
                  tur->horarios_aula.add(hr_aula);
               }
               // <<< <<< <<<
               h->turno = tur;
               // <<< <<<
            }

            HorarioAula *hr_aula;
            if(it_hr->horario_aula != NULL) {
               hr_aula = new HorarioAula();
               hr_aula->setId(it_hr->horario_aula->getId());
               hr_aula->inicio = it_hr->horario_aula->inicio;

               GGroup<int>::iterator it_dia_sem = it_hr->horario_aula->diasSemana.begin();
               for(unsigned dia =0;dia<it_hr->horario_aula->diasSemana.size();dia++) {
                  hr_aula->diasSemana.add(*it_dia_sem);
                  it_dia_sem++;
               }
            }

            nova_disc->horarios.add(h);
         }
         // <<<
         /*
         if(it_disc->tipo_disciplina != NULL){
         nova_disc->tipo_disciplina->setId(it_disc->tipo_disciplina->getId());
         nova_disc->tipo_disciplina->nome = it_disc->tipo_disciplina->nome;
         }

         if(it_disc->nivel_dificuldade != NULL) {
         nova_disc->nivel_dificuldade->setId(it_disc->nivel_dificuldade->getId());
         nova_disc->nivel_dificuldade->nome = it_disc->nivel_dificuldade->nome;
         }

         if( it_disc->demanda_total != -1){
         nova_disc->demanda_total = it_disc->demanda_total;
         }
         if( it_disc->max_demanda != -1){
         nova_disc->max_demanda = it_disc->max_demanda;
         }

         if( it_disc->num_turmas != -1){
         nova_disc->num_turmas = it_disc->num_turmas;
         }
         if( it_disc->min_creds != -1){
         nova_disc->min_creds = it_disc->min_creds;
         }
         */

         ITERA_GGROUP(it_cp,problemData->campi,Campus) {
            // Adicionando os dados da nova disciplina em <Campi->Unidade->Sala->disciplinasAssociadas>:
            ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
               ITERA_GGROUP(it_sala,it_und->salas,Sala) {
                  /*
                  if( it_sala->disciplinas_associadas.find(it_disc->getId()) != 
                  it_sala->disciplinas_associadas.end() ){
                  it_sala->disciplinas_associadas.add(nova_disc->getId());
                  }
                  */

                  if( (it_sala->disciplinas_associadas.find(it_disc->getId()) != 
                     it_sala->disciplinas_associadas.end() ) &&
                     (it_sala->tipo_sala_id != 1)){
                        /*
                        Removendo a associacao da disciplina teorica em questao com as salas 
                        incompativeis, no caso qualquer uma que nao seja uma sala de aula (de acordo
                        com inputTrivial)
                        */

                        it_sala->disciplinas_associadas.remove(it_disc->getId());

                        /*
                        Em relacao a nova disciplina (pratica), so adiciono uma associacao quando 
                        for com uma sala compativel, no caso LABORATORIO
                        */

                        it_sala->disciplinas_associadas.add(nova_disc->getId());
                  }

               }
            }

            // Adicionando os dados da nova disciplina em <Campi->Professor->disciplinas>:
            Magisterio *novo_mag;
            ITERA_GGROUP(it_prof,it_cp->professores,Professor) {
               ITERA_GGROUP(it_mag,it_prof->magisterio,Magisterio) {
                  if( it_mag->disciplina_id == it_disc->getId()) {
                     //Magisterio *novo_mag = new Magisterio();
                     novo_mag = new Magisterio();

                     novo_mag->setId(-1); // Nem precisava.
                     novo_mag->nota = it_mag->nota;
                     novo_mag->preferencia = it_mag->preferencia;
                     novo_mag->disciplina_id = nova_disc->getId();
                     it_prof->magisterio.add(novo_mag);

                     break; // Garantido que um mesmo professor nao possui preferencias diferentes em relacao a uma mesma disciplina.
                  }
               }
            }
         }


         /* ToDo : Fixacao (ToDo : futura issue : para criar uma nova fixacao, antes eh
         necessario saber se uma disciplina pode ser replicada. Pode acontecer  um caso 
         em que um determinada disciplina possua creditos teoricos e praticos e seja fixada
         em um determinado dia, numa sala para aulas teorica e em outro horario diferente seja
         fixada em um laboratorio. Nesse caso, nao seria necessario criar uma nova fixacao e sim,
         alterar o id da disciplina da fixacao da aula pratica para o id da nova disciplina que
         foi criada(se a nova discipliona for pratica).)

         ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao) {
         if(it_fix->disciplina_id == it_disc->getId() ) {
         }
         */
         //         std::cout << "\n\n\n\n\n\n\n\n";
         //std::cout << "WARNNING: PARA QUE AS FIXACOES FUNCIONEM CORRETAMENTE, IMPLEMENTAR A COPIA QDO DIVIDE UMA DISCIPLINA !!! metodo divideDisciplinas()" << std::endl;
         //         std::cout << "\n\n\n\n\n\n\n\n";
         /*
         }
         */

         // Adicionando os dados da nova disciplina em <GrupoCurso->curriculos>
         ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
            ITERA_GGROUP(it_curriculo,it_curso->curriculos,Curriculo) {
               /* 
               FIXME, isto est� errado, deveria-se, de algum jeito,
               saber o periodo da disciplina ou, iterar sobre todos os periodos 
               validos de um curso e nao sobre uma estimativa.
               */
               for(unsigned num_periodos = 0; num_periodos < 20; num_periodos++) {
                  DisciplinaPeriodo disc_periodo(num_periodos,it_disc->getId());
                  //std::cout << "<periodo,disc_id> : <" << disc_periodo.first << "," << disc_periodo.second << ">\n";
                  if(it_curriculo->disciplinas_periodo.find(disc_periodo) !=
                     it_curriculo->disciplinas_periodo.end()) {
                        //std::cout << "Found at <periodo,disc_id> : <" << disc_periodo.first << "," << disc_periodo.second << ">\n";
                        //DisciplinaPeriodo nova_disc_periodo(disc_periodo.first, -disc_periodo.second);
                        it_curriculo->disciplinas_periodo.add(DisciplinaPeriodo (disc_periodo.first, -disc_periodo.second));
                        break; // Garantido que uma disciplina aparece apenas uma vez em um curriculo de um curso.
                  }
               }
            }
         }

         // Adicionando os dados da nova disciplina em <Demanda>
         Demanda *nova_demanda = NULL;
         ITERA_GGROUP(it_dem,problemData->demandas,Demanda)
         {
            int num_vezes_ecncontrado = 0;

            if( it_dem->disciplina_id == it_disc->getId())
            {
               nova_demanda = new Demanda();

               //nova_demanda->setId(-it_dem->getId());
               nova_demanda->oferta_id = it_dem->oferta_id;
               nova_demanda->disciplina_id = nova_disc->getId();
               nova_demanda->quantidade = it_dem->quantidade;

               problemData->demandas.add(nova_demanda);

               if(num_vezes_ecncontrado > 0)
               {
                  std::cout << "POSSIVEL ERRO EM <divideDisciplinas()> -> " << 
                     "Encontrei mais de uma demanda para uma dada disciplina de um " <<
                     "dado curso em um determinado campus." << std::endl;
                  getchar();
               }
               num_vezes_ecncontrado++;
            }
         }

         //if(nova_demanda) { // != NULL
         //   problemData->demandas.add(nova_demanda);
         //}

         // >>> 14/10/2010
         //it_disc->foi_dividida = true;
         //nova_disc->foi_dividida = true;
         // <<< 14/10/2010

         GGroup<int>::iterator itDiasLetivosDiscs =
            it_disc->diasLetivos.begin();

         for(; itDiasLetivosDiscs != it_disc->diasLetivos.end(); itDiasLetivosDiscs++)
         { nova_disc->diasLetivos.add(*itDiasLetivosDiscs); }

         disciplinas_novas.add(nova_disc);
      }
   }

   //std::cout << "\nBEFORE: " << problemData->disciplinas.size();
   ITERA_GGROUP(it_disc,disciplinas_novas,Disciplina) {
      problemData->disciplinas.add(*it_disc);
   }
   //std::cout << "\nAFTER: " << problemData->disciplinas.size();

}

void ProblemDataLoader::referenciaCampusUnidadesSalas()
{
   ITERA_GGROUP(it_cp,problemData->campi,Campus)
   {
      problemData->refCampus[it_cp->getId()] = *it_cp;

      ITERA_GGROUP(it_Unidade,it_cp->unidades,Unidade)
      {
         problemData->refUnidade[it_Unidade->getId()] = *it_Unidade;

         ITERA_GGROUP(it_Sala,it_Unidade->salas,Sala)
         {
            problemData->refSala[it_Sala->getId()] = *it_Sala;
         }
      }
   }
}

//void ProblemDataLoader::referenciaCampus()
//{
//   ITERA_GGROUP(it_cp,problemData->campi,Campus) {
//      problemData->refCampus[it_cp->getId()] = *it_cp;
//   }
//}

void ProblemDataLoader::referenciaDisciplinas()
{
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      problemData->refDisciplinas[it_disc->getId()] = *it_disc;
   }
}

void ProblemDataLoader::referenciaOfertas()
{
   ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
   { problemData->refOfertas[itOferta->getId()] = *itOferta; }
}

void ProblemDataLoader::gera_refs()
{
   ITERA_GGROUP(it_campi,problemData->campi,Campus)
   {
      ITERA_GGROUP(it_unidades,it_campi->unidades,Unidade)
      {
         ITERA_GGROUP(it_horario,it_unidades->horarios,Horario)
         {
            find_and_set(it_horario->turnoId,
               problemData->calendario->turnos,
               it_horario->turno);
            find_and_set(it_horario->horarioAulaId,
               it_horario->turno->horarios_aula,
               it_horario->horario_aula);
         }

         ITERA_GGROUP(it_salas,it_unidades->salas,Sala)
         {
            find_and_set(it_salas->tipo_sala_id,
               problemData->tipos_sala,
               it_salas->tipo_sala);

            ITERA_GGROUP(it_horario,it_salas->horarios_disponiveis,Horario)
            {
               find_and_set(it_horario->turnoId,
                  problemData->calendario->turnos,
                  it_horario->turno);
               find_and_set(it_horario->horarioAulaId,
                  it_horario->turno->horarios_aula,
                  it_horario->horario_aula);
            }

            ITERA_GGROUP(it_credito,it_salas->creditos_disponiveis,
               CreditoDisponivel) 
            {
               find_and_set(it_credito->turno_id,
                  problemData->calendario->turnos,
                  it_credito->turno);
            }

            /* Disciplinas associadas ? 
            TODO (ou n�o) */

            ITERA_GGROUP_N_PT(it_id_Disc,it_salas->disciplinas_associadas,int)
            {
               ITERA_GGROUP(it_Disc,problemData->disciplinas,Disciplina)
               {
                  if((*it_id_Disc) == it_Disc->getId())
                  {
                     it_salas->disciplinas_Associadas_Usuario.add(*it_Disc);
                  }
               }
            }
         } // end salas
      }

      ITERA_GGROUP(it_prof,it_campi->professores,Professor)
      {
         find_and_set(it_prof->tipo_contrato_id, 
            problemData->tipos_contrato, 
            it_prof->tipo_contrato);

         ITERA_GGROUP(it_horario,it_prof->horarios,Horario)
         {
            find_and_set(it_horario->turnoId,
               problemData->calendario->turnos,
               it_horario->turno);
            find_and_set(it_horario->horarioAulaId,
               it_horario->turno->horarios_aula,
               it_horario->horario_aula);
         }

         ITERA_GGROUP(it_mag,it_prof->magisterio,Magisterio)
         {
            //std::cout << "it_mag->disciplina_id: " << it_mag->disciplina_id << std::endl;
            find_and_set(it_mag->disciplina_id,
               problemData->disciplinas,
               it_mag->disciplina);
            //getchar();
         }
      } // end professores

      ITERA_GGROUP(it_horario,it_campi->horarios,Horario)
      {
         find_and_set(it_horario->turnoId,
            problemData->calendario->turnos,
            it_horario->turno);
         find_and_set(it_horario->horarioAulaId,
            it_horario->turno->horarios_aula,
            it_horario->horario_aula);
      } 
   } // campus

   ITERA_GGROUP(it_desl,problemData->tempo_campi,Deslocamento)
   {
      find_and_set(it_desl->origem_id,
         problemData->campi, (Campus*&) it_desl->origem);
      find_and_set(it_desl->destino_id,
         problemData->campi, (Campus*&) it_desl->destino);
   } // deslocamento campi

   ITERA_GGROUP(it_desl,problemData->tempo_unidades,Deslocamento)
   {
      /* � preciso procurar a unidade nos campi */
      ITERA_GGROUP(it_campi,problemData->campi,Campus)
      {
         /* posso fazer find_and_set em todos sem ifs, porque ele
         s� seta se encontrar. Posso continuar fazendo mesmo depois de 
         encontrar pelo mesmo motivo */
         find_and_set(it_desl->origem_id,
            it_campi->unidades, (Unidade*&) it_desl->origem);
         find_and_set(it_desl->destino_id,
            it_campi->unidades, (Unidade*&) it_desl->destino);
      }
   } // deslocamento unidades 

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   {
      find_and_set(it_disc->tipo_disciplina_id,
         problemData->tipos_disciplina, it_disc->tipo_disciplina);
      find_and_set(it_disc->nivel_dificuldade_id,
         problemData->niveis_dificuldade, it_disc->nivel_dificuldade);

      ITERA_GGROUP(it_horario,it_disc->horarios,Horario)
      {
         find_and_set(it_horario->turnoId,
            problemData->calendario->turnos,
            it_horario->turno);
         find_and_set(it_horario->horarioAulaId,
            it_horario->turno->horarios_aula,
            it_horario->horario_aula);
      } 
   } // disciplinas
   ITERA_GGROUP(it_curso, problemData->cursos, Curso) 
   {
      find_and_set(it_curso->tipo_id,
         problemData->tipos_curso, it_curso->tipo_curso);
   }

   ITERA_GGROUP(it_oferta,problemData->ofertas,Oferta) 
   {
      find_and_set(it_oferta->curso_id,
         problemData->cursos,
         it_oferta->curso);

      find_and_set(it_oferta->curriculo_id,
         it_oferta->curso->curriculos,
         it_oferta->curriculo);

      find_and_set(it_oferta->turno_id,
         problemData->calendario->turnos,
         it_oferta->turno);

      find_and_set(it_oferta->campus_id,
         problemData->campi,it_oferta->campus);
   }

   ITERA_GGROUP(it_dem,problemData->demandas,Demanda) 
   {
      find_and_set(it_dem->oferta_id,
         problemData->ofertas, it_dem->oferta);
      find_and_set(it_dem->disciplina_id,
         problemData->disciplinas, it_dem->disciplina);
   }
   /* Falta: parametros (?) e fixacoes */
   ITERA_GGROUP(it_ndh,
      problemData->parametros->niveis_dificuldade_horario,
      NivelDificuldadeHorario)
   {
      find_and_set(it_ndh->nivel_dificuldade_id,
         problemData->niveis_dificuldade,
         it_ndh->nivel_dificuldade);
   }

   ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao)
   {
      find_and_set(it_fix->disciplina_id, 
         problemData->disciplinas,it_fix->disciplina);

      find_and_set(it_fix->turno_id, 
         problemData->calendario->turnos,
         it_fix->turno);

      if(it_fix->turno != NULL)
      {	
         find_and_set(it_fix->horario_id,
            it_fix->turno->horarios_aula,
            it_fix->horario);
      }

      ITERA_GGROUP(it_campi,problemData->campi,Campus)
      {
         find_and_set(it_fix->professor_id, 
            it_campi->professores, it_fix->professor);
         ITERA_GGROUP(it_unidades,it_campi->unidades,Unidade)
         {
            find_and_set(it_fix->sala_id,
               it_unidades->salas, it_fix->sala);
         }
      }
   }


   ITERA_GGROUP(it_campi,problemData->campi,Campus)
   {
      ITERA_GGROUP(it_unidades,it_campi->unidades,Unidade) 
      {
         it_unidades->id_campus = it_campi->getId();

         ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
         {
            it_salas->id_unidade = it_unidades->getId();
         }
      }
   }

   /*
   // >>> Debugging !!!
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   std::cout << it_disc->getId() << std::endl;
   std::cout << it_disc->demanda_total<< std::endl;
   std::cout << it_disc->max_demanda<< std::endl;
   std::cout << it_disc->num_turmas<< std::endl;
   std::cout << it_disc->min_creds<< std::endl;
   std::cout << it_disc->max_creds<< std::endl;
   std::cout << std::endl;
   }
   // <<<
   */
   //exit(1);

}

void ProblemDataLoader::cria_blocos_curriculares()
{
   // Contador de blocos
   int id_Bloco = 1;

   ITERA_GGROUP(it_campi,problemData->campi,Campus)
   {
      ITERA_GGROUP(it_curso,problemData->cursos,Curso)
      {
         ITERA_GGROUP(it_curr,it_curso->curriculos,Curriculo)
         {
            // Descobrindo oferta em quest�o
            Oferta * pt_Oferta = NULL;

            ITERA_GGROUP(it_Oferta,problemData->ofertas,Oferta)
            {
               if(it_Oferta->campus->getId() == it_campi->getId() &&
                  it_Oferta->curriculo->getId() == it_curr->getId() &&
                  it_Oferta->curso->getId() == it_curso->getId())
               {
                  pt_Oferta = *it_Oferta;
                  break;
               }
            }
            // ---

            GGroup<DisciplinaPeriodo>::iterator it_dp = 
               it_curr->disciplinas_periodo.begin();

            // Percorrendo todas as disciplinas de um curso cadastradas para um curr�culo.
            for(;it_dp != it_curr->disciplinas_periodo.end(); ++it_dp)
            {
               DisciplinaPeriodo dp = *it_dp;
               int periodo = dp.first;
               int disc_id = dp.second;

               Disciplina * disc = problemData->refDisciplinas[disc_id];

               // Encontrando e armazenando a demanda espec�fica da disciplina em quest�o
               Demanda * pt_Demanda = NULL;

               ITERA_GGROUP(it_Demanda,problemData->demandas,Demanda)
               {
                  if(it_Demanda->disciplina == disc &&
                     it_Demanda->oferta == pt_Oferta)
                  {
                     pt_Demanda = *it_Demanda;
                     break;
                  }
               }
               //---

               GGroup<BlocoCurricular*>::iterator it_bc = 
                  problemData->blocos.begin();

               bool found = false;

               // Verificando a exist�ncia do bloco curricular para a disciplina em quest�o.
               for(;it_bc != problemData->blocos.end(); ++it_bc) 
               {
                  //if(it_bc->getId() == id_Bloco)
                  if(it_bc->campus == *it_campi &&
                     it_bc->curso == *it_curso &&
                     it_bc->curriculo == *it_curr &&
                     it_bc->periodo == periodo)
                  {
                     it_bc->disciplinas.add(disc);

                     it_bc->disciplina_Demanda[disc] = pt_Demanda;

                     found = true;
                     break;
                  }
               }

               if(!found) 
               {
                  BlocoCurricular * b = new BlocoCurricular();

                  b->setId(id_Bloco);
                  b->periodo = periodo;
                  b->campus = *it_campi;
                  b->curso = *it_curso;

                  b->curriculo = *it_curr;

                  b->disciplinas.add(disc);

                  b->disciplina_Demanda[disc] = pt_Demanda;

                  problemData->blocos.add(b);

                  ++id_Bloco;
               }
            }
         }
      }
   }

   /* Setando os dias letivos de cada bloco. */
   ITERA_GGROUP(it_Bloco_Curric,problemData->blocos,BlocoCurricular)
   {
      ITERA_GGROUP(it_Disc,it_Bloco_Curric->disciplinas,Disciplina)
      {
         ITERA_GGROUP_N_PT(it_Dias_Letivos,it_Disc->diasLetivos,int)
         { 
            it_Bloco_Curric->diasLetivos.add(*it_Dias_Letivos);
         }
      }
   }
}

//{
//   ITERA_GGROUP(it_campi,problemData->campi,Campus)
//   {
//      ITERA_GGROUP(it_curso,problemData->cursos,Curso)
//      {
//         ITERA_GGROUP(it_curr,it_curso->curriculos,Curriculo)
//         {
//            // Descobrindo oferta em quest�o
//            Oferta * pt_Oferta = NULL;
//  
//            ITERA_GGROUP(it_Oferta,problemData->ofertas,Oferta)
//            {
//               if(it_Oferta->campus->getId() == it_campi->getId() &&
//                  it_Oferta->curriculo->getId() == it_curr->getId() &&
//                  it_Oferta->curso->getId() == it_curso->getId())
//               {
//                  pt_Oferta = *it_Oferta;
//                  break;
//               }
//            }
//            // ---
//
//            GGroup<DisciplinaPeriodo>::iterator it_dp = 
//               it_curr->disciplinas_periodo.begin();
//
//            // Percorrendo todas as disciplinas de um curso cadastradas para um curr�culo.
//            for(;it_dp != it_curr->disciplinas_periodo.end(); ++it_dp)
//            {
//               DisciplinaPeriodo dp = *it_dp;
//               int periodo = dp.first;
//               int disc_id = dp.second;
//
//               Disciplina * disc = problemData->refDisciplinas[disc_id];
//
//               // Encontrando e armazenando a demanda espec�fica da disciplina em quest�o
//               Demanda * pt_Demanda = NULL;
//
//               ITERA_GGROUP(it_Demanda,problemData->demandas,Demanda)
//               {
//                  if(it_Demanda->disciplina == disc &&
//                     it_Demanda->oferta == pt_Oferta)
//                  {
//                     pt_Demanda = *it_Demanda;
//                     break;
//                  }
//               }
//               //---
//
//               GGroup<BlocoCurricular*>::iterator it_bc = 
//                  problemData->blocos.begin();
//
//               int id_blc = it_curso->getId() * 100 + periodo;
//
//               bool found = false;
//
//               // Verificando a exist�ncia do bloco curricular para a disciplina em quest�o.
//               for(;it_bc != problemData->blocos.end(); ++it_bc) 
//               {
//                  if(it_bc->getId() == id_blc)
//                  {
//                     it_bc->disciplinas.add(disc);
//
//                     it_bc->disciplina_Demanda[disc] = pt_Demanda;
//
//                     found = true;
//                     break;
//                  }
//               }
//
//               if(!found) 
//               {
//                  BlocoCurricular * b = new BlocoCurricular();
//
//                  b->setId(id_blc);
//                  b->periodo = periodo;
//                  b->campus = *it_campi;
//                  b->curso = *it_curso;
//
//                  b->curriculo = *it_curr;
//
//                  b->disciplinas.add(disc);
//
//                  b->disciplina_Demanda[disc] = pt_Demanda;
//
//                  problemData->blocos.add(b);
//               }
//            }
//         }
//      }
//   }
//
//   /* Setando os dias letivos de cada bloco. */
//   ITERA_GGROUP(itBlocoCurric,problemData->blocos,BlocoCurricular)
//   {
//      ITERA_GGROUP(itDisc,itBlocoCurric->disciplinas,Disciplina)
//      {
//         GGroup<int>::iterator itDiasLet =
//            itDisc->diasLetivos.begin();
//
//         for(; itDiasLet != itDisc->diasLetivos.end(); itDiasLet++)
//         { itBlocoCurric->diasLetivos.add(*itDiasLet); }
//      }
//   }
//}

void ProblemDataLoader::relacionaCampusDiscs()
{
   ITERA_GGROUP(it_oferta,problemData->ofertas,Oferta)
   {
      GGroup<int> ids_discs;

      ITERA_GGROUP(it_curso,problemData->cursos,Curso)
      {
         if(it_curso->getId() == it_oferta->curso_id)
         {
            ITERA_GGROUP(it_curric,it_curso->curriculos,Curriculo)
            {
               if(it_curric->getId() == it_oferta->curriculo_id)
               {
                  GGroup<DisciplinaPeriodo>::iterator it_disc_prd =
                     it_curric->disciplinas_periodo.begin();

                  for(; it_disc_prd != it_curric->disciplinas_periodo.end(); it_disc_prd++)
                  {
                     problemData->cp_discs[it_oferta->campus_id].add((*it_disc_prd).second);
                  }

                  break;
               }
            }

            break;
         }
      }
   }
}

/**/
void ProblemDataLoader::calculaTamanhoMedioSalasCampus()
{
   unsigned somaCapSalas = 0;
   unsigned total_Salas = 0;

   ITERA_GGROUP(it_cp,problemData->campi,Campus)
   {
      ITERA_GGROUP(it_und,it_cp->unidades,Unidade)
      {
         ITERA_GGROUP(it_sala,it_und->salas,Sala)
         {
            somaCapSalas += it_sala->capacidade;

            it_und->maiorSala = std::max(((int)it_und->maiorSala),((int)it_sala->capacidade));
         }

         total_Salas += it_und->getNumSalas();

         it_cp->maiorSala = std::max(((int)it_cp->maiorSala),((int)it_und->maiorSala));
      }

      problemData->cp_medSalas[it_cp->getId()] = somaCapSalas / total_Salas;
   }
}

void ProblemDataLoader::calculaDemandas()
{
   ITERA_GGROUP(it_dem,problemData->demandas,Demanda)
   {
      int dem = it_dem->quantidade;

      it_dem->disciplina->setMaxDemanda(
         std::max(it_dem->disciplina->getMaxDemanda(),dem));

      it_dem->disciplina->adicionaDemandaTotal(dem);

      // Armazenando a demanda total de cada Campus
      std::pair<int,int> dc = std::make_pair(
         it_dem->disciplina->getId(),
         it_dem->oferta->campus->getId());

      // inicializa com zero caso ainda n�o exista;
      if(problemData->demandas_campus.find(dc) !=
         problemData->demandas_campus.end())
         problemData->demandas_campus[dc] = 0;

      problemData->demandas_campus[dc] += dem;
   }
}

// TRIEDA-416
void ProblemDataLoader::estima_turmas()
{
   /* Estimando o n�mero m�ximo de turmas de cada disciplina de acordo com o seguinte c�lculo:

   numTurmasDisc = demDisc / tamMedSalasCP

   onde:

   demDisc -> representa a demanda total de uma dada disciplina.
   tamMedSalasCP -> representa o tamanho m�dio das salas de um campus.

   */

   std::map<int/*Id Campus*/,GGroup<int>/*Id Discs*/>::iterator itCPDiscs =
      problemData->cp_discs.begin();

   for(; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++)
   {
      int tamMedSalasCP = problemData->cp_medSalas[itCPDiscs->first];

      GGroup<int>::iterator itDisc = itCPDiscs->second.begin();

      for(; itDisc != itCPDiscs->second.end(); itDisc++)
      {
         int demDisc = problemData->refDisciplinas[*itDisc]->getDemandaTotal();

         if(problemData->refDisciplinas[*itDisc]->eLab())
         {
            if(problemData->refDisciplinas[*itDisc]->max_alunos_p > 0)
            {
               //int numTurmas = (demDisc / problemData->refDisciplinas[*itDisc]->max_alunos_p);
               int numTurmas = 5; //(demDisc / 25);
               problemData->refDisciplinas[*itDisc]->num_turmas = (numTurmas > 0 ? numTurmas + 2 : 2);
            }
            else
            {
               //problemData->refDisciplinas[*itDisc]->num_turmas = (demDisc / tamMedSalasCP) + 2;
               problemData->refDisciplinas[*itDisc]->num_turmas = 5; //(demDisc / 25) + 2;
            }
         }
         else
         {
            if(problemData->refDisciplinas[*itDisc]->max_alunos_p > 0)
            {
               //int numTurmas = (demDisc / problemData->refDisciplinas[*itDisc]->max_alunos_t);
               int numTurmas = 5;//(demDisc / 50);
               problemData->refDisciplinas[*itDisc]->num_turmas = (numTurmas > 0 ? numTurmas + 1 : 1);                
            }
            else
            {
               //problemData->refDisciplinas[*itDisc]->num_turmas = (demDisc / tamMedSalasCP) + 5;
               problemData->refDisciplinas[*itDisc]->num_turmas = 5; //(demDisc / 50) + 1;
            }
         }
      }
   }

   //ITERA_GGROUP(it_Disc,problemData->disciplinas,Disiciplina)
   //{
   //   std::cout << "Disc: " << it_Disc->
   //}
}

//void ProblemDataLoader::estima_turmas() {
//
//   unsigned id_turma = 0;
//
//   std::map<int/*Id Campus*/,GGroup<int>/*Id Discs*/>::iterator it_cp_discs =
//      problemData->cp_discs.begin();
//
//   // Para todos os campus
//   for(; it_cp_discs != problemData->cp_discs.end(); it_cp_discs++) {
//
//      GGroup<int>::iterator it_discs = it_cp_discs->second.begin();
//
//      // Para todas as disciplinas do campus em quest�o
//      for(; it_discs != it_cp_discs->second.end(); it_discs++) {
//
//         // >>>
//         // Configurando o tamanho da turma.
//         int tamTurma = -1;
//
//         // N�mero de turmas considerado para a disc em quest�o.
//         unsigned numTurmas = 0;
//
//         /*
//         if (problemData->refDisciplinas[(*it_discs)]->getMaxAlunosTeo() > 0 ||
//         problemData->refDisciplinas[(*it_discs)]->getMaxAlunosPrat() > 0 ) {
//
//         tamTurma = std::min(problemData->refDisciplinas[(*it_discs)]->getMaxAlunosTeo(),
//         problemData->refDisciplinas[(*it_discs)]->getMaxAlunosPrat());
//
//         //numTurmas = problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() /
//         //tamTurma + 1;
//         }
//         else*/ {
//
//            tamTurma = std::max(problemData->refDisciplinas[(*it_discs)]->getMaxAlunosTeo(),
//               problemData->refDisciplinas[(*it_discs)]->getMaxAlunosPrat());
//
//            if(tamTurma <= 0) {
//
//               // >>>
//               /* Armazenando as capacidades das salas compat�veis com a disciplina em quest�o. */
//               std::vector<int/*Capacidade Sala*/> salasComaptiveis;
//
//               ITERA_GGROUP(it_und,problemData->refCampus[it_cp_discs->first]->unidades,Unidade) {            
//                  ITERA_GGROUP(it_sala,it_und->salas,Sala) {
//                     ITERA_GGROUP(it_d,it_sala->disciplinasAssociadas,Disciplina) {
//                        if(it_d->getId() == *it_discs) {
//                           salasComaptiveis.push_back(it_sala->capacidade);
//                           break;
//                        }
//                     }
//                  }
//               }
//
//               // Ordem crescente de capacidade.
//               std::sort(salasComaptiveis.begin(),salasComaptiveis.end());
//
//               // <<<
//
//               // N�mero m�ximo de turmas para uma disciplina. /* FIX ME */
//               unsigned maxTurmasDisc = 10;
//
//               std::vector<int/*Capacidade Sala*/>::iterator
//                  it_salasCompativeis = salasComaptiveis.begin();
//
//               for(;it_salasCompativeis != salasComaptiveis.end(); it_salasCompativeis++) {
//                  numTurmas = problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() / 
//                     *it_salasCompativeis + 1;
//
//                  if( numTurmas <= maxTurmasDisc ) {
//                     tamTurma = *it_salasCompativeis;
//                     break;
//                  }
//               }
//
//               /* Pode ser que o n�mero de turmas tenha sido sempre 
//               calculado acima de <maxTurmasDisc>. Da�, calcula-se o
//               n�mero de turmas de acordo com o tamanho m�dio das
//               salas de um campus. */
//               if(it_salasCompativeis == salasComaptiveis.end() && 
//                  numTurmas > maxTurmasDisc ) {
//                     numTurmas = problemData->cp_medSalas[it_cp_discs->first];
//               }
//            }
//         }
//         // <<<
//
//         /*
//         for(unsigned i = 0; i < numTurmas; i++) {
//         problemData->disc_turmas[(*it_discs)].add(std::make_pair<int,int>
//         (id_turma++,tamTurma));
//         }
//         */
//
//         problemData->refDisciplinas[(*it_discs)]->num_turmas = numTurmas;
//
//         /*GAMB*/
//         /*
//         tamTurma = 10;
//
//         if(problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() == 0) {
//         problemData->refDisciplinas[(*it_discs)]->num_turmas = 0;
//         }
//         else {
//         problemData->refDisciplinas[(*it_discs)]->num_turmas = 
//         problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() 
//         / tamTurma + 1;
//         }
//         */
//
//         /*
//         std::cout << "Decidi abrir " << problemData->refDisciplinas[(*it_discs)]->num_turmas << 
//         " turmas da disciplina " << problemData->refDisciplinas[(*it_discs)]->codigo << std::endl;
//         */
//      }
//   }
//}

void ProblemDataLoader::print_stats() {
   int ncampi(0),nunidades(0),nsalas(0),nconjuntoSalas(0),ndiscs(0),ndiscsDiv(0),
      nturmas(0),nturmasDiscDiv(0),nprofs(0),ncursos(0),nofertas(0),
      tdemanda(0),tdemandaDiv(0);

   ncampi = problemData->campi.size();

   ITERA_GGROUP(it_campi,problemData->campi,Campus) {
      nunidades += it_campi->unidades.size();

      ITERA_GGROUP(it_und,it_campi->unidades,Unidade) {
         nsalas += it_und->salas.size();
         nconjuntoSalas += it_und->conjutoSalas.size();
      }

      nprofs += it_campi->professores.size();
      ncursos += problemData->cursos.size();
   }

   nofertas = problemData->ofertas.size();

   ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
   {
      GGroup<DisciplinaPeriodo>::iterator itPrdDisc =
         itOferta->curriculo->disciplinas_periodo.begin();

      for(; itPrdDisc != 
         itOferta->curriculo->disciplinas_periodo.end();
         itPrdDisc++)
      {
         Disciplina * disc = problemData->refDisciplinas[(*itPrdDisc).second];
         if(disc->getId() > 0)
            nturmas += disc->num_turmas;
         else
            nturmasDiscDiv += disc->num_turmas;
      }
   }

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      if(it_disc->getId() > 0) {
         ndiscs++;
         //nturmas += it_disc->num_turmas;
         tdemanda += it_disc->getDemandaTotal();
      }
      else {
         ndiscsDiv++;
         //nturmasDiscDiv += it_disc->num_turmas;
         tdemandaDiv += it_disc->getDemandaTotal();
      }
   }

   std::cout << std::endl;
   std::cout << "Estatisticas de dados de entrada\n\n";// << std::endl;
   //std::cout << "================================" << std::endl;
   printf("<*> Campi:                 \t%4d\n",ncampi);
   printf("<*> Unidades:              \t%4d\n",nunidades);
   printf("<*> Salas:                 \t%4d\n",nsalas);

   printf("<*> Conjuntos de Salas:    \t%4d\n",nconjuntoSalas);

   //printf("Disciplinas:  \t%4d\n",ndiscs);
   printf("<*> Disciplinas:\n");
   printf("\t - Entrada:    \t%4d\n",ndiscs);
   printf("\t - Divididas:  \t%4d\n",ndiscsDiv);   
   printf("\t - Total:  \t%4d\n",ndiscs+ndiscsDiv);

   printf("<*> Blocos Curriculares:  \t%4d\n",problemData->blocos.size());

   //printf("<*> Turmas:       \t%4d\n",nturmas);
   printf("<*> Turmas:\n");
   printf("\t - Entrada:    \t%4d\n",nturmas);
   printf("\t - Divididas:  \t%4d\n",nturmasDiscDiv);   
   printf("\t - Total:  \t%4d\n",nturmas+nturmasDiscDiv);

   printf("<*> Professores:  \t%4d\n",nprofs);
   printf("<*> Cursos:       \t%4d\n",ncursos);

   printf("<*> Ofertas:      \t%4d\n",nofertas);
   //ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
   //{ 
   //   printf("\t - Oferta %d possui %d discs\n",itOferta->getId(),
   //      itOferta->curriculo->disciplinas_periodo.size());
   //}

   //printf("<*> Demanda total:\t%4d vagas\n",tdemanda);
   printf("<*> Demanda total\n");
   printf("\t - Entrada:    \t%4d\n",tdemanda);
   printf("\t - Divididas:  \t%4d\n",tdemandaDiv);   
   printf("\t - Total:  \t%4d\n",tdemanda+tdemandaDiv);

   std::cout << "================================" << std::endl
      << std::endl;
}

/* Salva algumas informa��es que s�o usadas frequentemente */
void ProblemDataLoader::cache()
{
   problemData->totalSalas = 0;
   ITERA_GGROUP(it_campus,problemData->campi,Campus)
   {
      it_campus->totalSalas = 0;
      ITERA_GGROUP(it_u,it_campus->unidades,Unidade)
      {
         it_campus->totalSalas += it_u->salas.size();
      }
      problemData->totalSalas += it_campus->totalSalas;
   }

   problemData->totalTurmas = 0;
   ITERA_GGROUP(it_disciplinas,problemData->disciplinas,Disciplina)
   {
      problemData->totalTurmas += it_disciplinas->num_turmas;
   }

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      it_bloco->total_turmas = 0;
      ITERA_GGROUP(it_disciplinas,it_bloco->disciplinas,Disciplina)
      {
         it_bloco->total_turmas += it_disciplinas->num_turmas;
      }
   }

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   {
      if (it_disc->divisao_creditos != NULL)
      {
         it_disc->min_creds = 24;
         it_disc->max_creds = 0;

         for(int t=0;t<8;t++) {
            if(it_disc->divisao_creditos->dia[t] > 0)
            {
               it_disc->min_creds = 
                  std::min(it_disc->min_creds,
                  it_disc->divisao_creditos->dia[t]);

               it_disc->max_creds = 
                  std::max(it_disc->max_creds,
                  it_disc->divisao_creditos->dia[t]);
            }
         }
      }
      else
      {
         it_disc->min_creds = 1;
         it_disc->max_creds = it_disc->cred_praticos + it_disc->cred_teoricos;
      }
   }

   /*
   ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   it_campus->maior_sala = 0;
   ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   it_u->maior_sala = 0;
   ITERA_GGROUP(it_sala,it_u->salas,Sala)
   it_u->maior_sala = std::max(it_u->maior_sala,
   it_sala->capacidade);

   it_campus->maior_sala = std::max(it_campus->maior_sala,
   it_u->maior_sala);
   }
   }
   */

   //// >>>

   //Definindo um map de compatibilidade e incompatibilidade entre 2 turmas.

   ITERA_GGROUP(it_fix_curso,problemData->cursos,Curso)
   {
      ITERA_GGROUP(it_alt_curso,problemData->cursos,Curso)
      {
         //std::pair<int,int> idCursos = std::make_pair(it_fix_curso->getId(),it_alt_curso->getId());

         std::pair<Curso*,Curso*> idCursos = 
            std::make_pair(*it_fix_curso,*it_alt_curso);

         if(it_fix_curso == it_alt_curso)
         { problemData->compat_cursos[idCursos] = true; }
         else
         { problemData->compat_cursos[idCursos] = false; }

      }
   }

   ITERA_GGROUP(it_fix_curso,problemData->cursos,Curso)
   {
      GGroup<GGroup<int>*>::iterator it_list_compat =
         problemData->parametros->permite_compart_turma.begin();

      for(;it_list_compat!=problemData->parametros->permite_compart_turma.end();++it_list_compat)
      {
         if( it_list_compat->find(it_fix_curso->getId()) != it_list_compat->end() )
         {
            ITERA_GGROUP(it_alt_curso,problemData->cursos,Curso)
            {
               if(it_list_compat->find(it_alt_curso->getId()) != it_list_compat->end())
               {
                  std::pair<Curso*,Curso*> idCursos =
                     std::make_pair(*it_fix_curso,*it_alt_curso);

                  problemData->compat_cursos[idCursos] = true;

               }
            }
         }
      }
   }

   std::map<std::pair<Curso*,Curso*>,bool>::iterator itCC = 
      problemData->compat_cursos.begin();

   for(; itCC != problemData->compat_cursos.end(); itCC++)
   {
      std::pair<Curso*,Curso*> normal = 
         std::make_pair<Curso*,Curso*>(itCC->first.first,itCC->first.second);

      std::pair<Curso*,Curso*> invertido =
         std::make_pair<Curso*,Curso*>(itCC->first.second,itCC->first.first);

      problemData->compat_cursos[invertido] = 
         problemData->compat_cursos.find(normal)->second;
   }

   //// <<<

   // >>>
   ///* Adicionando �s salas todas as disciplinas compativeis. 
   //OBS: Se a sala n�o possui disciplina compat�vel informada na
   //entrada, entao todas as disciplinas s�o compativeis.*/
   //GGroup<int> disc_proibidas;

   //ITERA_GGROUP(it_cp,problemData->campi,Campus) {
   //	ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
   //		ITERA_GGROUP(it_sala,it_und->salas,Sala) {
   //			GGroup<int>::iterator it_sala_disc_assoc = it_sala->disciplinas_associadas.begin();
   //			for(;it_sala_disc_assoc != it_sala->disciplinas_associadas.end();it_sala_disc_assoc++ ) {
   //				disc_proibidas.add(*it_sala_disc_assoc);
   //			}
   //		}
   //	}
   //}

   //ITERA_GGROUP(it_cp,problemData->campi,Campus) {
   //	ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
   //		ITERA_GGROUP(it_sala,it_und->salas,Sala) {
   //			ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //				if( it_sala->disciplinas_associadas.find(it_disc->getId()) !=
   //					it_sala->disciplinas_associadas.end()) {
   //						//it_sala->disc_assoc_PT.add(*it_disc);
   //                    it_sala->disciplinasAssociadas.add(*it_disc);
   //				}
   //			}
   //		}
   //	}
   //}

   //ITERA_GGROUP(it_cp,problemData->campi,Campus) {
   //	ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
   //		ITERA_GGROUP(it_sala,it_und->salas,Sala) {
   //			ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //				if( disc_proibidas.find(it_disc->getId()) == disc_proibidas.end() ) {
   //					if(it_disc->cred_praticos > 0) {
   //						if(it_sala->tipo_sala_id == 2 /*laboratorio, segundo instancia trivial*/) {
   //							//it_sala->disc_assoc_PT.add(*it_disc);
   //                       it_sala->disciplinasAssociadas.add(*it_disc);
   //						}
   //					}
   //					else {
   //						if(it_disc->cred_teoricos > 0) {
   //							//it_sala->disc_assoc_PT.add(*it_disc);
   //                       it_sala->disciplinasAssociadas.add(*it_disc);
   //						}
   //					}
   //				}
   //			}
   //		}
   //	}
   //}
   // <<<
}

void ProblemDataLoader::associaDisciplinasSalas()
{
   std::map<int/*Id Campus*/,GGroup<int>/*Id Discs*/>::iterator it_Cp_Discs =
      problemData->cp_discs.begin();

   // Para cada Campus
   for(; it_Cp_Discs != problemData->cp_discs.end(); it_Cp_Discs++)
   {
      Campus * pt_Campus = problemData->refCampus[it_Cp_Discs->first];

      ITERA_GGROUP(it_und,pt_Campus->unidades,Unidade)
      {
         /* Estrutura respons�vel por armazenar todas as disciplinas que foram 
         associadas a alguma sala pelo usu�rio. */
         //GGroup<Disciplina*> disc_Assoc_USUARIO;

         // PASSO 1

         /* Armazenando as disciplinas que foram associadas pelo usu�rio e n�o 
         dever�o ser consideradas para a associa��o autom�tica. */

         ITERA_GGROUP(it_sala,it_und->salas,Sala)
         {
            ITERA_GGROUP(it_Disc_Assoc_Sala,it_sala->disciplinas_Associadas_Usuario,Disciplina)
            {
               // Adicionando um ponteiro da disciplina referenciada pelo usuario.
               //disc_Assoc_USUARIO.add(*it_Disc_Assoc_Sala);

               /* Adicionando um ponteiro para qdo tiver uma dada disciplina for f�cil
               descobrir a lista de salas associadas. */
               problemData->discSalas[it_Disc_Assoc_Sala->getId()].add(*it_sala);
            }
         }

         // ------------------------

         // PASSO 2

         /* Associando as demais disciplinas �s salas */

         // Para cada disciplina associada ao campus em questao
         ITERA_GGROUP_N_PT(it_Disc,it_Cp_Discs->second,int)
         {
            Disciplina * pt_Disc = (problemData->refDisciplinas.find(*it_Disc)->second);

            // Se a disciplina foi associada pelo usu�rio.
            //if(disc_Assoc_USUARIO.find(pt_Disc) != disc_Assoc_USUARIO.end())
            //{
            // Se a disciplina tiver a FLAG <eLab> marcada (TRUE)
            if(pt_Disc->eLab())
            {
               // RESTRICAO FORTE
               // NAO DEVO CRIAR MAIS NENHUMA ASSOCIACAO.
            }
            else
            {
               // RESTRICAO FRACA
               // DEVO CRIAR ASSOCIACOES ENTRE A DISCIPLINA E TODAS AS SALAS DE AULA.

               ITERA_GGROUP(it_Unidade,pt_Campus->unidades,Unidade)
               {
                  ITERA_GGROUP(it_Sala,it_Unidade->salas,Sala)
                  {
                     if(it_Sala->disciplinas_Associadas_Usuario.find(pt_Disc) ==
                        it_Sala->disciplinas_Associadas_Usuario.end())
                     {

                        // Somente se for uma sala de aula.
                        if(it_Sala->getTipoSalaId() == 1)
                        {
                           /* Estabelecendo o crit�rio de interese��o de dias letivos.

                           I.E. S� associo uma sala de aula a uma disciplina se a sala tem, 
                           pelo menos, um dia letivo comum com a disciplina.
                           */
                           ITERA_GGROUP_N_PT(it_Dias_Let_Disc,pt_Disc->diasLetivos,int)
                           {
                              /* S� continuo quando a sala possuir o dia letivo (pertencente � disciplina) em quest�o. */
                              if(it_Sala->diasLetivos.find(*it_Dias_Let_Disc) != it_Sala->diasLetivos.end())
                              {
                                 it_Sala->disciplinas_Associadas_AUTOMATICA.add(pt_Disc);

                                 /* Adicionando um ponteiro para qdo tiver uma dada disciplina, for f�cil
                                 descobrir a lista de salas associadas. */
                                 problemData->discSalas[pt_Disc->getId()].add(*it_Sala);
                              }
                           }
                        }
                     }
                  }
               }
            }
            //}
            //else // Disciplina n�o associada pelo usu�rio.
            //{
            //}
         }
      }

      /* Com as duas estruturas <disciplinas_Associadas_Usuario> e 
      <disciplinas_Associadas_AUTOMATICA> preenchidas para cada sala,
      deve-se fazer a uniao delas preenchendo a estrutura <disciplinasAssociadas> */
      ITERA_GGROUP(it_Unidade,pt_Campus->unidades,Unidade)
      {
         ITERA_GGROUP(it_Sala,it_Unidade->salas,Sala)
         {
            ITERA_GGROUP(it_Disc_Assoc_Usr,it_Sala->disciplinas_Associadas_Usuario,Disciplina)
            {
               it_Sala->disciplinasAssociadas.add(*it_Disc_Assoc_Usr);
            }

            ITERA_GGROUP(it_Disc_Assoc_AUTO,it_Sala->disciplinas_Associadas_AUTOMATICA,Disciplina)
            {
               it_Sala->disciplinasAssociadas.add(*it_Disc_Assoc_AUTO);
            }
         }
      }
   }

   std::cout << "\n\n\nARRUMAR: ProblemDataLoader::associaDisciplinasSalas() -> METODO DA MICHELE ABAIXO !!\n\n\n";

   // CODIGO DA MIHCELE !!! -> ADAPTAR !!!!!!!!!!!!!!!!!!!!

   //Se uma disciplina est� fixada a uma determinada sala associa essa disciplina 
   //somente aquela sala (e n�o a um grupo de salas)
   //std::map<int/*Id Disc*/,GGroup<Sala*> >::iterator it_Disc_Salas =
   //   problemData->discSalas.begin();

   //for(; it_Disc_Salas != problemData->discSalas.end(); it_Disc_Salas++)
   //{
   //   ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao)
   //   {
   //      if(it_Disc_Salas->first == it_fix->disciplina_id)
   //      {
   //         problemData->discSalas[it_fix->disciplina_id].clear();
   //         problemData->discSalas[it_fix->disciplina_id].add(it_fix->sala);
   //      }
   //   }
   //}
}

void ProblemDataLoader::associaDisciplinasConjuntoSalas()
{
   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            std::map<int/*Id Sala*/,Sala*>::iterator itSala =
               itCjtSala->getTodasSalas().begin();

            for(; itSala != itCjtSala->getTodasSalas().end(); itSala++)
            {
               GGroup<Disciplina*>::iterator itDiscs = itSala->second->disciplinasAssociadas.begin();

               for(; itDiscs != itSala->second->disciplinasAssociadas.end(); itDiscs++)
               {
                  itCjtSala->getDiscsAssociadas().add(*itDiscs);
               }
            }
         }
      }
   }
}

void ProblemDataLoader::relacionaDiscOfertas()
{
   ITERA_GGROUP(it_Oferta,problemData->ofertas,Oferta)
   {
      GGroup<DisciplinaPeriodo>::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();

      for(; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end(); it_Prd_Disc++)
      { 
         int disc = (*it_Prd_Disc).second;
         problemData->ofertasDisc[disc].add(*it_Oferta);
      }
   }
}

//void ProblemDataLoader::relacionaDiscCampusOfertas()
//{
//   std::map<int/*Id disc*/, GGroup<Oferta*> >::iterator itDiscOft
//      problemData->ofertasDisc.begin();
//
//   for(; itDiscOft != problemData->ofertasDisc.end(); itDiscOft++)
//   {
//      ofertasCPDisc[std::pair<int,int>(itDiscOft->second.begin()->campus_id,
//         itDiscOft->first)]
//   }
//}

void ProblemDataLoader::print_csv(void)
{
   int ncampi,nunidades,ndiscs,nprofs,ncursos,nofertas,tdemanda;

   ncampi = problemData->campi.size();
   nunidades = 0, nprofs = 0, ncursos = 0;
   ITERA_GGROUP(it_campi,problemData->campi,Campus) {
      nunidades += it_campi->unidades.size();
      nprofs += it_campi->professores.size();
      ncursos += problemData->cursos.size();
   }
   nofertas = problemData->ofertas.size();
   ndiscs = problemData->disciplinas.size();
   tdemanda = 0;
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      //tdemanda += it_disc->demanda_total;
      tdemanda += it_disc->getDemandaTotal();
   }

   FILE *file = fopen("./CSV/PROBLEM_SETTINGS.csv","wt");
   fprintf(file,"Campi:\t%4d,\n",ncampi);
   fprintf(file,"Unidades:\t%4d,\n",nunidades);
   fprintf(file,"Salas:\t%4d,\n",problemData->campi.begin()->totalSalas);
   fprintf(file,"Disciplinas:\t%4d,\n",ndiscs);
   fprintf(file,"Cursos:\t%4d,\n",ncursos);
   fprintf(file,"Professores:\t%4d,\n",nprofs);
   fprintf(file,"Ofertas:\t%4d,\n",nofertas);
   fprintf(file,"Demanda total:\t%4d,\n",tdemanda);

   if(file)
      fclose(file);
}