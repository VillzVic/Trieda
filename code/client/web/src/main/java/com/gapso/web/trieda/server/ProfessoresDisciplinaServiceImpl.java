package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.services.ProfessoresDisciplinaService;
import com.google.gwt.dev.util.collect.HashMap;

public class ProfessoresDisciplinaServiceImpl
	extends RemoteService implements ProfessoresDisciplinaService
{

	private static final long serialVersionUID = -7562720011162342660L;

	@Override
	public ProfessorDisciplinaDTO getProfessorDisciplina( Long id )
	{
		return ConvertBeans.toProfessorDisciplinaDTO(
			ProfessorDisciplina.find( id, getInstituicaoEnsinoUser() ) );
	}
	
	@Override
	public PagingLoadResult<ProfessorDisciplinaDTO> getBuscaList(CenarioDTO cenarioDTO, ProfessorDTO professorDTO, 
			DisciplinaDTO disciplinaDTO, 
			String cpf, String nome,String  operadorPreferencia,Integer preferencia,String  operadorNotaDesempenho, Integer notaDesempenho,
			PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		Professor professor = (professorDTO == null)? null :
			Professor.find( professorDTO.getId(), getInstituicaoEnsinoUser() );

		Disciplina disciplina = ( disciplinaDTO == null ) ? null :
			Disciplina.find( disciplinaDTO.getId(), getInstituicaoEnsinoUser() );

		String orderBy = config.getSortField();

		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
				&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = ( orderBy + " asc" );
			}
			else
			{
				orderBy = ( orderBy + " desc" );
			}
		}

		List< ProfessorDisciplinaDTO > list	
			= new ArrayList< ProfessorDisciplinaDTO >();

		List< ProfessorDisciplina > listDomains = ProfessorDisciplina.findBy(
			getInstituicaoEnsinoUser(), cenario, professor, disciplina,
			cpf, nome,  operadorPreferencia, preferencia, operadorNotaDesempenho,notaDesempenho,
			config.getOffset(), config.getLimit(), orderBy );

		for ( ProfessorDisciplina professorDisciplina : listDomains )
		{
			list.add( ConvertBeans.toProfessorDisciplinaDTO( professorDisciplina ) );
		}

		BasePagingLoadResult< ProfessorDisciplinaDTO > result
			= new BasePagingLoadResult< ProfessorDisciplinaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( ProfessorDisciplina.count(
			getInstituicaoEnsinoUser(), cenario, professor, disciplina,
			cpf, nome,  operadorPreferencia, preferencia, operadorNotaDesempenho,notaDesempenho) );

		return result;
	}
	
	@Override
	public ParDTO<List<DisciplinaDTO>, List<DisciplinaDTO>> getDisciplinasAssociadas(CenarioDTO cenarioDTO, ProfessorDTO professorDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Professor professor = Professor.find(professorDTO.getId(), getInstituicaoEnsinoUser());
		
		List<DisciplinaDTO> disciplinasAssociadas = new ArrayList<DisciplinaDTO>();
		List<DisciplinaDTO> disciplinasNaoAssociadas = new ArrayList<DisciplinaDTO>();
		for (Disciplina disciplina : Disciplina.findByCenario(getInstituicaoEnsinoUser(), cenario))
		{
			boolean associadoAoProfessor = false;
			for (ProfessorDisciplina professorDisciplina : disciplina.getProfessores())
			{
				if (professorDisciplina.getProfessor().equals(professor))
				{
					associadoAoProfessor = true;
				}
			}
			if (associadoAoProfessor)
			{
				disciplinasAssociadas.add(ConvertBeans.toDisciplinaDTO(disciplina));
			}
			else
			{
				disciplinasNaoAssociadas.add(ConvertBeans.toDisciplinaDTO(disciplina));
			}
		}
		
		return ParDTO.create(disciplinasAssociadas, disciplinasNaoAssociadas);
	}
	
	@Override
	public void save(ProfessorDTO professorDTO, List<ProfessorDisciplinaDTO> professorDisciplinasDTO) {
		Professor professor = Professor.find(professorDTO.getId(), getInstituicaoEnsinoUser());
		professor.getDisciplinas().clear();
		for (ProfessorDisciplinaDTO professorDisciplinaDTO : professorDisciplinasDTO)
		{
			ProfessorDisciplina professorDisciplina = ConvertBeans.toProfessorDisciplina(professorDisciplinaDTO);
			professor.getDisciplinas().add(professorDisciplina);
			if(professorDisciplina.getId() != null && professorDisciplina.getId() > 0) {
				professorDisciplina.merge();
			} else {
				professorDisciplina.persist();
			}
		}
	}

	@Override
	public void save(ProfessorDisciplinaDTO professorDisciplinaDTO) {
		ProfessorDisciplina professorDisciplina = ConvertBeans.toProfessorDisciplina(professorDisciplinaDTO);
		if(professorDisciplina.getId() != null && professorDisciplina.getId() > 0) {
			professorDisciplina.merge();
		} else {
			professorDisciplina.persist();
		}
	}
	
	@Override
	public void remove(List<ProfessorDisciplinaDTO> professorDisciplinaDTOList) {
		for(ProfessorDisciplinaDTO professorDisciplinaDTO : professorDisciplinaDTOList) {
			ConvertBeans.toProfessorDisciplina(professorDisciplinaDTO).remove();
		}
	}

	@Override
	public void habilitarEquivalenciasProfessores(CenarioDTO cenarioDTO) {
		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<Professor> listaProfessores = Professor.findByCenario(getInstituicaoEnsinoUser(), cenario);
		List<Equivalencia> listaEquivalencia =  Equivalencia.find(getInstituicaoEnsinoUser());
		Map<Professor,List<Disciplina>> mapProfDisciplina = new HashMap<Professor, List<Disciplina>>();
		
		for(Professor professor : listaProfessores){
			for(ProfessorDisciplina pd : professor.getDisciplinas()){
				if(mapProfDisciplina.get(professor) == null){
					List<Disciplina> listaDisciplina = new ArrayList<Disciplina>();
					listaDisciplina.add(pd.getDisciplina());
					mapProfDisciplina.put(professor, listaDisciplina);
				} else {
					mapProfDisciplina.get(professor).add(pd.getDisciplina());
				}
			}
		}
		
		
		for(Equivalencia equivalencia : listaEquivalencia){
			for(Professor professor : listaProfessores){
				for(ProfessorDisciplina pd : professor.getDisciplinas()){
					if(pd.getDisciplina().equals(equivalencia.getCursou())){
						if(!mapProfDisciplina.get(professor).contains(equivalencia.getElimina())){
							ProfessorDisciplina elimina = new ProfessorDisciplina();
							elimina.setDisciplina(equivalencia.getElimina());
							elimina.setProfessor(professor);
							elimina.setNota(pd.getNota());
							elimina.setPreferencia(pd.getPreferencia());
							elimina.persist();
						}
					}
					
				}
			}
		}
		
	}
}
