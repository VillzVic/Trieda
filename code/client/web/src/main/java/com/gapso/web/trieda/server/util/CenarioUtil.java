package com.gapso.web.trieda.server.util;

import java.util.HashSet;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Incompatibilidade;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;

@Transactional
public class CenarioUtil
{
	private SemanaLetiva semanaLetiva;
	private Cenario cenario;
	private Set< Turno > turnoList = new HashSet< Turno >();
	private Set< HorarioAula > horarioAulaList = new HashSet< HorarioAula >();
	private Set< Curso > cursoList = new HashSet< Curso >();
	private Set< Campus > campusList = new HashSet< Campus >();
	private Set< DeslocamentoCampus > deslocamentoCampusList = new HashSet< DeslocamentoCampus >();
	private Set< Unidade > unidadeList = new HashSet< Unidade >();
	private Set< DeslocamentoUnidade > deslocamentoUnidadeList = new HashSet< DeslocamentoUnidade >();
	private Set< Sala > salaList = new HashSet< Sala >();
	private Set< GrupoSala > grupoSalaList = new HashSet< GrupoSala >();
	private Set< Disciplina > disciplinaList = new HashSet< Disciplina >();
	private Set< Incompatibilidade > compatibilidadeList = new HashSet< Incompatibilidade >();
	private Set< Equivalencia > equivalenciaList = new HashSet< Equivalencia >();
	private Set< DivisaoCredito > divisaoCreditoList = new HashSet< DivisaoCredito >();
	private Set< Professor > professorList = new HashSet< Professor >();
	private Set< ProfessorDisciplina > professorDisciplinaList = new HashSet< ProfessorDisciplina >();
	private Set< Curriculo > curriculoList = new HashSet< Curriculo >();
	private Set< CurriculoDisciplina > curriculoDisciplinaList = new HashSet< CurriculoDisciplina >();
	private Set< Oferta > ofertaList = new HashSet< Oferta >();
	private Set< Demanda > demandaList = new HashSet< Demanda >();
	private Set< HorarioDisponivelCenario > horarioDisponivelCenarioList = new HashSet< HorarioDisponivelCenario >();

	@Transactional
	public void criarCenario( Cenario cenario,
		SemanaLetiva semanaLetiva, Set< Campus > campi )
	{
		if ( cenario == null || semanaLetiva == null )
		{
			System.out.println( "Erro ao criar um novo cenário:" );

			if ( cenario == null )
			{
				System.out.println( "---> cenário inválido." );
			}

			if ( semanaLetiva == null )
			{
				System.out.println( "---> semana letiva inválida." );
			}

			return;
		}

		new Criar( cenario, semanaLetiva, campi );
	}

	@Transactional
	public void clonarCenario( Cenario cenario )
	{
		if ( cenario == null )
		{
			System.out.println( "Erro ao criar um novo cenário: cenário inválido." );
			return;
		}

		new Clonar( cenario );
	}

	/* *************************** */
	/* ** CRIAR UM NOVO CENÁRIO ** */
	/* *************************** */
	@Transactional
	private class Criar
	{
		public Criar( Cenario cen,
			SemanaLetiva sl, Set< Campus > campi )
		{
			cenario = cen;
			semanaLetiva = sl;
			campusList = campi;

			this.popularListas();
			detachList();
			saveList();
		}

		@Transactional
		private void popularListas()
		{
			cenario.setSemanaLetiva( semanaLetiva );

			Set< HorarioAula > horariosAula
				= semanaLetiva.getHorariosAula();

			for ( HorarioAula horarioAula : horariosAula )
			{
				horarioAulaList.add( horarioAula );
				turnoList.add( horarioAula.getTurno() );
				horarioDisponivelCenarioList.addAll(
					horarioAula.getHorariosDisponiveisCenario() );
			}

			for ( Campus c : campusList )
			{
				c.setCenario( cenario );

				deslocamentoCampusList.addAll( c.getDeslocamentos() );
				unidadeList.addAll( c.getUnidades() );
				ofertaList.addAll( c.getOfertas() );

				for ( Oferta o : ofertaList )
				{
					Curriculo curriculo = o.getCurriculo();

					curriculoList.add( curriculo );
					cursoList.add( curriculo.getCurso() );

					curriculoDisciplinaList.addAll( curriculo.getDisciplinas() );
				}
			}

			for ( Unidade u : unidadeList )
			{
				deslocamentoUnidadeList.addAll( u.getDeslocamentos() );
				salaList.addAll( u.getSalas() );
				grupoSalaList.addAll( u.getGruposSalas() );
			}

			divisaoCreditoList.addAll( cenario.getDivisoesCredito() ); 
			disciplinaList.addAll( cenario.getDisciplinas() );

			for ( Disciplina d : disciplinaList )
			{
				compatibilidadeList.addAll( d.getIncompatibilidades() );
				equivalenciaList.addAll( d.getEquivalencias() );
				divisaoCreditoList.add( d.getDivisaoCreditos() );
				demandaList.addAll( d.getDemandas() );
			}

			professorList.addAll( cenario.getProfessores() );
			for ( Professor p : professorList )
			{
				professorDisciplinaList.addAll( p.getDisciplinas() );
			}
		}
	}

	/* ********************************** */
	/* ** CLONAR A PARTIR DE UM CENÁRIO ** */
	/* ********************************** */
	@Transactional
	private class Clonar
	{
		public Clonar( Cenario cen )
		{
			cenario = cen;

			this.popularListas();
			detachList();
			saveList();
		}

		@Transactional
		private void popularListas()
		{
			semanaLetiva = cenario.getSemanaLetiva();

			Set< HorarioAula > horariosAula
				= semanaLetiva.getHorariosAula();

			for ( HorarioAula horarioAula : horariosAula )
			{
				horarioAulaList.add( horarioAula );
				turnoList.add( horarioAula.getTurno() );
				horarioDisponivelCenarioList.addAll(
					horarioAula.getHorariosDisponiveisCenario() );
			}

			cursoList.addAll( cenario.getCursos() );
			campusList.addAll( cenario.getCampi() );

			for ( Campus c : campusList )
			{
				deslocamentoCampusList.addAll( c.getDeslocamentos() );
				unidadeList.addAll( c.getUnidades() );
				ofertaList.addAll( c.getOfertas() );
			}

			for ( Unidade u : unidadeList )
			{
				deslocamentoUnidadeList.addAll( u.getDeslocamentos() );
				salaList.addAll( u.getSalas() );
				grupoSalaList.addAll( u.getGruposSalas() );
			}

			divisaoCreditoList.addAll( cenario.getDivisoesCredito() ); 
			disciplinaList.addAll( cenario.getDisciplinas() );

			for ( Disciplina d : disciplinaList )
			{
				compatibilidadeList.addAll( d.getIncompatibilidades() );
				equivalenciaList.addAll( d.getEquivalencias() );
				divisaoCreditoList.add( d.getDivisaoCreditos() );
				demandaList.addAll( d.getDemandas() );
			}

			professorList.addAll( cenario.getProfessores() );

			for ( Professor p : professorList )
			{
				professorDisciplinaList.addAll( p.getDisciplinas() );
			}

			curriculoList.addAll( cenario.getCurriculos() );

			for ( Curriculo c : curriculoList )
			{
				curriculoDisciplinaList.addAll( c.getDisciplinas() );
			}
		}
	}

	@Transactional
	private void detachList()
	{
		semanaLetiva.setId( null );
		semanaLetiva.detach();

		cenario.setId( null );
		cenario.detach();

		for ( Turno o : turnoList )
		{
			o.setId( null );
			o.detach();
		}

		for ( HorarioAula o : horarioAulaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Curso o : cursoList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Campus o : campusList )
		{
			o.setId( null );
			o.detach();
		}

		for ( DeslocamentoCampus o : deslocamentoCampusList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Unidade o : unidadeList )
		{
			o.setId( null );
			o.detach();
		}

		for ( DeslocamentoUnidade o : deslocamentoUnidadeList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Sala o : salaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( GrupoSala o : grupoSalaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Disciplina o : disciplinaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Incompatibilidade o : compatibilidadeList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Equivalencia o : equivalenciaList ) 	
		{
			o.setId( null );
			o.detach();
		}

		for ( DivisaoCredito o : divisaoCreditoList )  
		{
			o.setId( null );
			o.detach();
		}

		for ( Professor o : professorList )
		{
			o.setId( null );
			o.detach();
		}

		for ( ProfessorDisciplina o : professorDisciplinaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Curriculo o : curriculoList )
		{
			o.setId( null );
			o.detach();
		}

		for ( CurriculoDisciplina o : curriculoDisciplinaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Oferta o : ofertaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( Demanda o : demandaList )
		{
			o.setId( null );
			o.detach();
		}

		for ( HorarioDisponivelCenario o : horarioDisponivelCenarioList )
		{
			o.setId( null );
			o.detach();
		}
	}

	@Transactional
	private void saveList()
	{
		semanaLetiva.persist();
		cenario.persist();

		semanaLetiva.setCenario( cenario );
		semanaLetiva.merge();

		for ( Turno o : turnoList ) 			
		{
			o.persist();
		}

		for ( HorarioAula o : horarioAulaList )							
		{
			o.persist();
		}

		for ( Curso o : cursoList ) 
		{
			o.persist();
		}

		for ( Campus o : campusList ) 						
		{
			o.persist();
		}

		for ( DeslocamentoCampus o : deslocamentoCampusList ) 	
		{
			o.persist();
		}

		for ( Unidade o : unidadeList ) 						
		{
			o.persist();
		}

		for ( DeslocamentoUnidade o : deslocamentoUnidadeList ) 			
		{
			o.persist();
		}

		for ( Sala o : salaList ) 			
		{
			o.persist();
		}

		for ( GrupoSala o : grupoSalaList ) 
		{
			o.persist();
		}

		for ( Disciplina o : disciplinaList ) 			
		{
			o.persist();
		}

		for ( Incompatibilidade o : compatibilidadeList ) 					
		{
			o.persist();
		}

		for ( Equivalencia o : equivalenciaList ) 		
		{
			o.persist();
		}

		for ( DivisaoCredito o : divisaoCreditoList )
		{
			o.persist();
		}

		for ( Professor o : professorList) 						
		{
			o.persist();
		}

		for ( ProfessorDisciplina o : professorDisciplinaList ) 			
		{
			o.persist();
		}

		for ( Curriculo o : curriculoList ) 								
		{
			o.persist();
		}

		for ( CurriculoDisciplina o : curriculoDisciplinaList ) 			
		{
			o.persist();
		}

		for ( Oferta o : ofertaList ) 										
		{
			o.persist();
		}

		for ( Demanda o : demandaList )									
		{
			o.persist();
		}

		for ( HorarioDisponivelCenario o : horarioDisponivelCenarioList )
		{
			o.persist();
		}
	}
}
