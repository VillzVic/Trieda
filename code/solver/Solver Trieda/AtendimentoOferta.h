#ifndef _ATENDIMENTO_OFERTA_H_
#define _ATENDIMENTO_OFERTA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "Oferta.h"

class Demanda;

class AtendimentoOferta :
   public OFBase
{
public:
   AtendimentoOferta( int );
   AtendimentoOferta( void );
   virtual ~AtendimentoOferta( void );

   Oferta * oferta;
   Demanda * demanda;

   void setOfertaCursoCampiId( std::string value ) { this->oferta_curso_campi_id = value; }
   void setDisciplinaId( int value ) { this->disciplina_id = value; }
   void setQuantidade( int value ) { this->quantidade = value; }
   void addQuantidade( int value ) { this->quantidade += value; }
   void setTurma( int value ) { this->turma = value; }
   void setTurmaStr( string value ) { this->turmaStr_ = value; }
   void setDisciplinaSubstitutaId( int value ) { this->disciplina_substituta_id = value; }
   void setFixar( bool value ) { this->fixar_ = value; }

   std::string getOfertaCursoCampiId() const { return this->oferta_curso_campi_id; }
   int getDisciplinaId() const { return this->disciplina_id; }
   int getQuantidade() const { return this->quantidade; }
   int getTurma() const { return this->turma; }
   string getTurmaStr() const { return this->turmaStr_; }
   int getDisciplinaSubstitutaId() const { return this->disciplina_substituta_id;}
   bool fixar() const { return this->fixar_; }

   Disciplina * disciplina; // Disciplina atendida. Se tiver havido substituição, é a substituta. Caso contrario, é a original

   GGroup< int > alunosDemandasAtendidas;

private:
   std::string oferta_curso_campi_id;
   int disciplina_id; // id da disciplina original, sempre
   int quantidade;
   int turma;
   int disciplina_substituta_id;
   bool fixar_;

   string turmaStr_;

   static int globalId_;
};

std::ostream & operator << ( std::ostream &, AtendimentoOferta & );

std::istream & operator >> ( std::istream &file, AtendimentoOferta* const &ptrAtendOferta );

#endif