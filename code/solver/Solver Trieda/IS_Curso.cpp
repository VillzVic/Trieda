#include "IS_Curso.h"

IS_Curso::IS_Curso(Curso * crs, ProblemData * pD)
{     
   curso = crs;

   problem_Data = pD;

   // Para cada currículo
   ITERA_GGROUP(it_Curriculo,curso->curriculos,Curriculo)
   {
      GGroup<pair<int/*periodo*/,int/*disciplina_id*/> >::iterator 
         it_Prd_Disc = it_Curriculo->disciplinas_periodo.begin();

      // Criando as grades horárias para cada currículo em relação aos respectivos períodos
      for(; it_Prd_Disc != it_Curriculo->disciplinas_periodo.end(); ++it_Prd_Disc)
      {
         Disciplina * pt_Disc = problem_Data->refDisciplinas[(*it_Prd_Disc).second];
         int periodo = (*it_Prd_Disc).first;

         if(grade_Horaria_Periodo.find(make_pair(*it_Curriculo,periodo)) == grade_Horaria_Periodo.end())
         {
            int creditos = 4; // FIX-ME
            int dias = 6; // FIX-ME

            grade_Horaria_Periodo[make_pair(*it_Curriculo,periodo)] = 
               new Matrix<vector<pair<Disciplina*,int/*turma*/> > > (creditos,dias);

            for(int r = 0; r < creditos; r++ )
            { grade_Horaria_Periodo[make_pair(*it_Curriculo,periodo)]->setRowName(r,r); }

            for(int c = 0; c < dias; c++)
            { grade_Horaria_Periodo[make_pair(*it_Curriculo,periodo)]->setColName(c,(c+2)); }
         }
      }
   }
}

IS_Curso::IS_Curso(IS_Curso const & is_curso)
{
   cerr << "COPY CONSTRUCTOR OF <IS_Curso> NOT IMPLEMENTED YET" << endl;
   exit(1);
}

IS_Curso::~IS_Curso(void)
{
}

vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator IS_Curso::verificaDisponibilidadeSemSubBloco(
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_Credito,
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator it_Variacoes_Regra_Credito,
   pair<Curriculo*,int/*periodo*/> & chave,
   Disciplina * disciplina
   //vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator it_Variacoes_Regra_Credito_END
   )
{
   // Obtendo a grade curricular correta
   map<pair<Curriculo*,int/*periodo*/>,
      Matrix<vector<pair<Disciplina*,int/*turma*/> > > * >::iterator 
      it_Grade_Horaria_Periodo = grade_Horaria_Periodo.find(chave);

   // ---------------------------------

   /*
   Estrutura que armazena o total de créditos livres que uma grade horária dispõe para cada dia.
   */
   vector<pair<int/*dia*/, int/*numCreditos*/> > creditos_Livres_Grade;

   // Para cada dia da grade considerada
   for(int dia = 2; dia < 7; dia++)
   {
      // Adicionando e setando um dia
      creditos_Livres_Grade.push_back(make_pair(dia,0));

      /*
      Contabilizando o número de créditos livres para o dia em questão
      */
      for(int cred = 0; cred < 4 /* FIX-ME num creds*/; cred++)
      {
         bool teste_Horario_Livre = false;
         //bool teste_Mesma_Disc_Alocada = false;

         // ---------------------------------

         if( (*it_Grade_Horaria_Periodo->second)(
            cred,
            it_Grade_Horaria_Periodo->second->getColIdByName(dia)
            ).empty() )
         { teste_Horario_Livre = true; }
         //else
         //{

         //   // ---------------------------------

         //   ///* Verificando o caso em que todas as disciplinas alocadas no horário
         //   //são as mesmas (ou seja, temos uma disciplina alocada em salas diferentes). */

         //   bool disciplinas_Iguais = true;

         //   vector<pair<Disciplina*,int/*turma*/> >::iterator
         //      it_Discs_Alocadas = (*it_Grade_Horaria_Periodo->second)(
         //      cred,
         //      it_Grade_Horaria_Periodo->second->getColIdByName(dia)
         //      ).begin();

         //   // Para cada disciplina alocada
         //   for(; it_Discs_Alocadas != (*it_Grade_Horaria_Periodo->second)(
         //      cred,
         //      it_Grade_Horaria_Periodo->second->getColIdByName(dia)
         //      ).end(); 
         //   ++it_Discs_Alocadas )
         //   {
         //      if(disciplina != it_Discs_Alocadas->first)
         //      {
         //         disciplinas_Iguais = false;
         //         break;
         //      }
         //   }

         //   if(disciplinas_Iguais)
         //   { teste_Mesma_Disc_Alocada = true; }

         //}

         // ---------------------------------

         //if(teste_Horario_Livre || teste_Mesma_Disc_Alocada)
         if(teste_Horario_Livre)
         { ++creditos_Livres_Grade.back().second; }
      }
   }

   // ---------------------------------

   /*
   Agora, sabendo o total de créditos livres em cada dia da grade em questão, devemos
   verificar a compatibilidade com alguma das variações da regra de crédito.
   */

   // Para cada variação da regra de crédito.
   for(; it_Variacoes_Regra_Credito != variacoes_Regra_Credito.end(); ++it_Variacoes_Regra_Credito)
   {
      bool encontrou_Alguma_Variacao_Compativel = true;

      vector<pair<int/*dia*/, int/*numCreditos*/> >::iterator
         it_Dias_Regra = it_Variacoes_Regra_Credito->begin();

      // Para cada dia da variação da regra de crédito
      for(; it_Dias_Regra != it_Variacoes_Regra_Credito->end(); ++it_Dias_Regra)
      {
         // Referência para o dia da grade considerado.
         //int dia_Grade = 0;

         // Para cada dia considerado em uma variação da regra de crédito.
         //for(;it_Dias_Regra != it_Variacoes_Regra_Credito->end(); ++it_Dias_Regra)
         //for(int dia = 2; dia < 7; dia++)
         for(int dia = 0; dia < creditos_Livres_Grade.size(); ++dia)
         {
            // Testando se estou comparando o dia certo.
            if(it_Dias_Regra->first == creditos_Livres_Grade.at(dia).first)
            {
               // Testando se o dia da grade possui o mínimo de créditos livres demandados.
               //if(creditos_Livres_Grade.at(dia).second >= it_Dias_Regra->second)
               //{ ++dia_Grade; }
               //else // Caso não possua, a regra é incompatível.
               if(!(creditos_Livres_Grade.at(dia).second >= it_Dias_Regra->second))
               { encontrou_Alguma_Variacao_Compativel = false; }
               break;
            }
         }

         if(!encontrou_Alguma_Variacao_Compativel)
         { break; }

      }

      if(encontrou_Alguma_Variacao_Compativel)
      {
         return it_Variacoes_Regra_Credito;
      }
   }

   // ---------------------------------

   return variacoes_Regra_Credito.end();
}

//vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator IS_Curso::verificaDisponibilidadeSemSubBloco(
//   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_Credito,
//   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator it_Variacoes_Regra_Credito,
//   pair<Curriculo*,int/*periodo*/> & chave,
//   Disciplina * disciplina
//   //vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator it_Variacoes_Regra_Credito_END
//   )
//{
//   // Obtendo a grade curricular correta
//   map<pair<Curriculo*,int/*periodo*/>,
//      Matrix<vector<pair<Disciplina*,int/*turma*/> > > * >::iterator 
//      it_Grade_Horaria_Periodo = grade_Horaria_Periodo.find(chave);
//
//   // ---------------------------------
//
//   /*
//   Estrutura que armazena o total de créditos livres que uma grade horária dispõe para cada dia.
//   */
//   vector<pair<int/*dia*/, int/*numCreditos*/> > creditos_Livres_Grade;
//
//   // Para cada dia da grade considerada
//   for(int dia = 2; dia < 7; dia++)
//   {
//      // Adicionando e setando um dia
//      creditos_Livres_Grade.push_back(make_pair(dia,0));
//
//      /*
//      Contabilizando o número de créditos livres para o dia em questão
//      */
//      for(int cred = 0; cred < 4 /* FIX-ME num creds*/; cred++)
//      {
//         bool teste_Horario_Livre = false;
//         bool teste_Mesma_Disc_Alocada = false;
//
//         // ---------------------------------
//
//         if( (*it_Grade_Horaria_Periodo->second)(
//            cred,
//            it_Grade_Horaria_Periodo->second->getColIdByName(dia)
//            ).empty() )
//         { teste_Horario_Livre = true; }
//         else
//         {
//
//            // ---------------------------------
//
//            ///* Verificando o caso em que todas as disciplinas alocadas no horário
//            //são as mesmas (ou seja, temos uma disciplina alocada em salas diferentes). */
//
//            bool disciplinas_Iguais = true;
//
//            vector<pair<Disciplina*,int/*turma*/> >::iterator
//               it_Discs_Alocadas = (*it_Grade_Horaria_Periodo->second)(
//               cred,
//               it_Grade_Horaria_Periodo->second->getColIdByName(dia)
//               ).begin();
//
//            // Para cada disciplina alocada
//            for(; it_Discs_Alocadas != (*it_Grade_Horaria_Periodo->second)(
//               cred,
//               it_Grade_Horaria_Periodo->second->getColIdByName(dia)
//               ).end(); 
//            ++it_Discs_Alocadas )
//            {
//               if(disciplina != it_Discs_Alocadas->first)
//               {
//                  disciplinas_Iguais = false;
//                  break;
//               }
//            }
//
//            if(disciplinas_Iguais)
//            { teste_Mesma_Disc_Alocada = true; }
//
//         }
//
//         // ---------------------------------
//
//         if(teste_Horario_Livre || teste_Mesma_Disc_Alocada)
//         { ++creditos_Livres_Grade.back().second; }
//      }
//   }
//
//   // ---------------------------------
//
//   /*
//   Agora, sabendo o total de créditos livres em cada dia da grade em questão, devemos
//   verificar a compatibilidade com alguma das variações da regra de crédito.
//   */
//
//   // Para cada variação da regra de crédito.
//   for(; it_Variacoes_Regra_Credito != variacoes_Regra_Credito.end(); ++it_Variacoes_Regra_Credito)
//   {
//      bool encontrou_Alguma_Variacao_Compativel = true;
//
//      vector<pair<int/*dia*/, int/*numCreditos*/> >::iterator
//         it_Dias_Regra = it_Variacoes_Regra_Credito->begin();
//
//      // Para cada dia da variação da regra de crédito
//      for(; it_Dias_Regra != it_Variacoes_Regra_Credito->end(); ++it_Dias_Regra)
//      {
//         // Referência para o dia da grade considerado.
//         //int dia_Grade = 0;
//
//         // Para cada dia considerado em uma variação da regra de crédito.
//         //for(;it_Dias_Regra != it_Variacoes_Regra_Credito->end(); ++it_Dias_Regra)
//         //for(int dia = 2; dia < 7; dia++)
//         for(int dia = 0; dia < creditos_Livres_Grade.size(); ++dia)
//         {
//            // Testando se estou comparando o dia certo.
//            if(it_Dias_Regra->first == creditos_Livres_Grade.at(dia).first)
//            {
//               // Testando se o dia da grade possui o mínimo de créditos livres demandados.
//               //if(creditos_Livres_Grade.at(dia).second >= it_Dias_Regra->second)
//               //{ ++dia_Grade; }
//               //else // Caso não possua, a regra é incompatível.
//               if(!(creditos_Livres_Grade.at(dia).second >= it_Dias_Regra->second))
//               { encontrou_Alguma_Variacao_Compativel = false; }
//               break;
//            }
//         }
//
//         if(!encontrou_Alguma_Variacao_Compativel)
//         { break; }
//
//      }
//
//      if(encontrou_Alguma_Variacao_Compativel)
//      {
//         return it_Variacoes_Regra_Credito;
//      }
//   }
//
//   // ---------------------------------
//
//   return variacoes_Regra_Credito.end();
//}

void IS_Curso::aloca(
                     int turma,
                     Disciplina * disciplina,
                     vector<pair<int/*dia*/,int/*num creds*/> > & regra_De_Credito,
                     Matrix<vector<pair<Disciplina*,int/*turma*/> > > * pt_Grade_Horaria_Curso
                     )
{
   vector<pair<int/*dia*/,int/*num Creds*/> >::iterator 
      it_Regra_De_Credito = regra_De_Credito.begin();

   // Para cada par<dia,numCreds> da regra de credito
   for(; it_Regra_De_Credito != regra_De_Credito.end(); it_Regra_De_Credito++)
   {
      int dia = it_Regra_De_Credito->first;
      int num_Creds_A_Alocar = it_Regra_De_Credito->second;

      // Se para o dia em questão, a regra de credito possui algum credito a ser alocado
      if(num_Creds_A_Alocar > 0)
      {      
         // ---------------------------------

         // Declarando algumas estruturas auxiliares

         /*
         Estrutura que armazena os créditos livres de um determinado dia da grade.
         */
         vector<int/*cred*/> creditos_Livres;

         /*
         Estrutura que armazena os créditos em que todas disciplinas alocadas são iguais.
         Esses créditos são preferenciais para alocação.
         */
         //vector<int/*cred*/> creditos_Disc_Iguais;

         // ---------------------------------

         for(int cred = 0; cred < 4 /* FIX-ME num creds*/; cred++)
         {
            if(num_Creds_A_Alocar > 0)
            {
               // ---------------------------------

               /*
               Verificando se o crédito está vago
               */

               if((*pt_Grade_Horaria_Curso)(
                  cred,
                  pt_Grade_Horaria_Curso->getColIdByName(dia)
                  ).empty())
               { creditos_Livres.push_back(cred); }
               //else
               //{
               //   // ---------------------------------

               //   /*
               //   Verificando o caso em que todas as disciplinas alocadas no horário
               //   são as mesmas.
               //   */

               //   bool disciplinas_Iguais = true;

               //   vector<pair<Disciplina*,int/*turma*/> >::iterator
               //      it_Discs_Alocadas = (*pt_Grade_Horaria_Curso)(
               //      cred,
               //      pt_Grade_Horaria_Curso->getColIdByName(dia)
               //      ).begin();

               //   // Para cada disciplina alocada
               //   for(; it_Discs_Alocadas != (*pt_Grade_Horaria_Curso)(
               //      cred,
               //      pt_Grade_Horaria_Curso->getColIdByName(dia)
               //      ).end(); 
               //   ++it_Discs_Alocadas )
               //   {
               //      if(disciplina != it_Discs_Alocadas->first)
               //      { disciplinas_Iguais = false; }
               //   }

               //   if(disciplinas_Iguais)
               //   { 
               //      creditos_Disc_Iguais.push_back(cred);

               //      // Atualizando o número de créditos que ainda devem ser alocados.
               //      --num_Creds_A_Alocar;
               //   }
               //}
            }
         }

         /*
         Agora que tenho as estruturas <creditos_Disc_Iguais> e <creditos_Livres> preenchidas, 
         tento alocar o máximo possível os créditos referenciados por <creditos_Disc_Iguais>. 

         Pode ser que ainda assim, restem alguns créditos para serem alocados. 
         Então, começo a alocar nos créditos referenciados por <creditos_Livres>.
         */

         num_Creds_A_Alocar = it_Regra_De_Credito->second;

         //if(creditos_Disc_Iguais.empty())
         {
            while(!creditos_Livres.empty() && (num_Creds_A_Alocar > 0))
            {
               int credito = creditos_Livres.back();

               (*pt_Grade_Horaria_Curso)(
                  credito,
                  pt_Grade_Horaria_Curso->getColIdByName(dia)
                  ).push_back(make_pair(disciplina,turma));

               // Atualizando
               creditos_Livres.pop_back();

               --num_Creds_A_Alocar;
            }

            if(num_Creds_A_Alocar > 0)
            {
               cerr << "Erro em IS_Curso::aloca() -> num_Creds_A_Alocar > 0. NAO DEVERIA" << endl;
               exit(1);
            }
         }
         //else
         //{
         //   while(!creditos_Disc_Iguais.empty())
         //   {
         //      int credito = creditos_Disc_Iguais.back();

         //      (*pt_Grade_Horaria_Curso)(
         //         credito,
         //         pt_Grade_Horaria_Curso->getColIdByName(dia)
         //         ).push_back(make_pair(disciplina,turma));

         //      // Atualizando
         //      creditos_Disc_Iguais.pop_back();

         //      --num_Creds_A_Alocar;
         //   }

         //   // Checando se ainda existem créditos a serem alocados.
         //   if(num_Creds_A_Alocar > 0)
         //   {
         //      int credito = creditos_Livres.back();

         //      (*pt_Grade_Horaria_Curso)(
         //         credito,
         //         pt_Grade_Horaria_Curso->getColIdByName(dia)
         //         ).push_back(make_pair(disciplina,turma));

         //      // Atualizando
         //      creditos_Livres.pop_back();

         //      --num_Creds_A_Alocar;
         //   }

         //   if(num_Creds_A_Alocar > 0)
         //   {
         //      cerr << "Erro em IS_Curso::aloca() -> num_Creds_A_Alocar > 0. NAO DEVERIA" << endl;
         //      exit(1);
         //   }
         //}
      }
   }

   // ------------------------
   //cout << "GRADE CURRICULAR DO CURSO " << curso->codigo << endl;

   //for(int c = 0; c < 5; ++c)         
   //{
   //   cout << "==========================" << endl;
   //   cout << "Dia: " << (c+2) << endl;

   //   for(int r = 0; r < 4; ++r)
   //   {
   //      cout << "Credito: " << (r) << endl;

   //      int tot_Discs_Aloc = pt_Grade_Horaria_Curso->getCol(c).at(r).size();

   //      for(int disc = 0; disc < tot_Discs_Aloc; ++disc)
   //      {
   //         cout << "[ " << pt_Grade_Horaria_Curso->getCol(c).at(r).at(disc).first->codigo
   //            << "; " << pt_Grade_Horaria_Curso->getCol(c).at(r).at(disc).second << "]" << endl;
   //      }

   //      cout << "---------" << endl;
   //   }
   //}
   // ------------------------
}

//void IS_Curso::aloca(
//           int turma,
//           Disciplina * disciplina,
//           vector<pair<int/*dia*/,int/*num creds*/> > & regra_De_Credito,
//           Matrix<vector<pair<Disciplina*,int/*turma*/> > > * pt_Grade_Horaria_Curso
//           )
//{
//   vector<pair<int/*dia*/,int/*num Creds*/> >::iterator 
//      it_Regra_De_Credito = regra_De_Credito.begin();
//
//   // Para cada par<dia,numCreds> da regra de credito
//   for(; it_Regra_De_Credito != regra_De_Credito.end(); it_Regra_De_Credito++)
//   {
//      int dia = it_Regra_De_Credito->first;
//      int num_Creds_A_Alocar = it_Regra_De_Credito->second;
//
//      // Se para o dia em questão, a regra de credito possui algum credito a ser alocado
//      if(num_Creds_A_Alocar > 0)
//      {      
//         // ---------------------------------
//
//         // Declarando algumas estruturas auxiliares
//
//         /*
//         Estrutura que armazena os créditos livres de um determinado dia da grade.
//         */
//         vector<int/*cred*/> creditos_Livres;
//
//         /*
//         Estrutura que armazena os créditos em que todas disciplinas alocadas são iguais.
//         Esses créditos são preferenciais para alocação.
//         */
//         vector<int/*cred*/> creditos_Disc_Iguais;
//
//         // ---------------------------------
//
//         for(int cred = 0; cred < 4 /* FIX-ME num creds*/; cred++)
//         {
//            if(num_Creds_A_Alocar > 0)
//            {
//               // ---------------------------------
//
//               /*
//               Verificando se o crédito está vago
//               */
//
//               if((*pt_Grade_Horaria_Curso)(
//                  cred,
//                  pt_Grade_Horaria_Curso->getColIdByName(dia)
//                  ).empty())
//               { creditos_Livres.push_back(cred); }
//               else
//               {
//                  // ---------------------------------
//
//                  /*
//                  Verificando o caso em que todas as disciplinas alocadas no horário
//                  são as mesmas.
//                  */
//
//                  bool disciplinas_Iguais = true;
//
//                  vector<pair<Disciplina*,int/*turma*/> >::iterator
//                     it_Discs_Alocadas = (*pt_Grade_Horaria_Curso)(
//                     cred,
//                     pt_Grade_Horaria_Curso->getColIdByName(dia)
//                     ).begin();
//
//                  // Para cada disciplina alocada
//                  for(; it_Discs_Alocadas != (*pt_Grade_Horaria_Curso)(
//                     cred,
//                     pt_Grade_Horaria_Curso->getColIdByName(dia)
//                     ).end(); 
//                  ++it_Discs_Alocadas )
//                  {
//                     if(disciplina != it_Discs_Alocadas->first)
//                     { disciplinas_Iguais = false; }
//                  }
//
//                  if(disciplinas_Iguais)
//                  { 
//                     creditos_Disc_Iguais.push_back(cred);
//                     
//                     // Atualizando o número de créditos que ainda devem ser alocados.
//                     --num_Creds_A_Alocar;
//                  }
//               }
//            }
//         }
//
//         /*
//         Agora que tenho as estruturas <creditos_Disc_Iguais> e <creditos_Livres> preenchidas, 
//         tento alocar o máximo possível os créditos referenciados por <creditos_Disc_Iguais>. 
//         
//         Pode ser que ainda assim, restem alguns créditos para serem alocados. 
//         Então, começo a alocar nos créditos referenciados por <creditos_Livres>.
//         */
//
//         num_Creds_A_Alocar = it_Regra_De_Credito->second;
//
//         if(creditos_Disc_Iguais.empty())
//         {
//            while(!creditos_Livres.empty() && (num_Creds_A_Alocar > 0))
//            {
//               int credito = creditos_Livres.back();
//
//               (*pt_Grade_Horaria_Curso)(
//                  credito,
//                  pt_Grade_Horaria_Curso->getColIdByName(dia)
//                  ).push_back(make_pair(disciplina,turma));
//
//               // Atualizando
//               creditos_Livres.pop_back();
//               
//               --num_Creds_A_Alocar;
//            }
//
//            if(num_Creds_A_Alocar > 0)
//            {
//               cerr << "Erro em IS_Curso::aloca() -> num_Creds_A_Alocar > 0. NAO DEVERIA" << endl;
//               exit(1);
//            }
//         }
//         else
//         {
//            while(!creditos_Disc_Iguais.empty())
//            {
//               int credito = creditos_Disc_Iguais.back();
//
//               (*pt_Grade_Horaria_Curso)(
//                  credito,
//                  pt_Grade_Horaria_Curso->getColIdByName(dia)
//                  ).push_back(make_pair(disciplina,turma));
//
//               // Atualizando
//               creditos_Disc_Iguais.pop_back();
//
//               --num_Creds_A_Alocar;
//            }
//
//            // Checando se ainda existem créditos a serem alocados.
//            if(num_Creds_A_Alocar > 0)
//            {
//               int credito = creditos_Livres.back();
//
//               (*pt_Grade_Horaria_Curso)(
//                  credito,
//                  pt_Grade_Horaria_Curso->getColIdByName(dia)
//                  ).push_back(make_pair(disciplina,turma));
//
//               // Atualizando
//               creditos_Livres.pop_back();
//               
//               --num_Creds_A_Alocar;
//            }
//
//            if(num_Creds_A_Alocar > 0)
//            {
//               cerr << "Erro em IS_Curso::aloca() -> num_Creds_A_Alocar > 0. NAO DEVERIA" << endl;
//               exit(1);
//            }
//         }
//      }
//   }
//
//   // ------------------------
//   cout << "GRADE CURRICULAR DO CURSO " << curso->codigo << endl;
//
//   for(int c = 0; c < 5; ++c)         
//   {
//      cout << "==========================" << endl;
//      cout << "Dia: " << (c+2) << endl;
//
//      for(int r = 0; r < 4; ++r)
//      {
//         cout << "Credito: " << (r) << endl;
//
//         int tot_Discs_Aloc = pt_Grade_Horaria_Curso->getCol(c).at(r).size();
//
//         for(int disc = 0; disc < tot_Discs_Aloc; ++disc)
//         {
//            cout << "[ " << pt_Grade_Horaria_Curso->getCol(c).at(r).at(disc).first->codigo
//               << "; " << pt_Grade_Horaria_Curso->getCol(c).at(r).at(disc).second << "]" << endl;
//         }
//
//         cout << "---------" << endl;
//      }
//   }
//   // ------------------------
//
//   cout << "Alocou\n\n";
//}

void IS_Curso::imprimeGradesHorarias()
{
   cout << "GRADE CURRICULAR DO CURSO " << curso->codigo << endl;

   map<pair<Curriculo*,int/*periodo*/>,
      //vector<vector<vector<pair<Disciplina*,int/*turma*/> > > > > grade_Horaria_Periodos;
      Matrix<vector<pair<Disciplina*,int/*turma*/> > > * >::iterator 
      
      it_Grade_Horaria_Periodo = grade_Horaria_Periodo.begin();


   for(; it_Grade_Horaria_Periodo != grade_Horaria_Periodo.end(); ++it_Grade_Horaria_Periodo)
   {
      cout << "\tPERIODO: " << it_Grade_Horaria_Periodo->first.second << endl;

      for(int c = 0; c < 5; ++c)         
      {
         cout << "==========================" << endl;
         cout << "Dia: " << (c+2) << endl;

         for(int r = 0; r < 4; ++r)
         {
            cout << "Credito: " << (r) << endl;

            int tot_Discs_Aloc = it_Grade_Horaria_Periodo->second->getCol(c).at(r).size();

            for(int disc = 0; disc < tot_Discs_Aloc; ++disc)
            {
               cout << "[ " << it_Grade_Horaria_Periodo->second->getCol(c).at(r).at(disc).first->codigo
                  << "; " << it_Grade_Horaria_Periodo->second->getCol(c).at(r).at(disc).second << "]" << endl;
            }

            cout << "---------" << endl;
         }
      }
   }
}

bool IS_Curso::operator < (IS_Curso const & right)
{
   return curso < right.curso;
}

bool IS_Curso::operator == (IS_Curso const & right)
{
   return curso == right.curso;
}
