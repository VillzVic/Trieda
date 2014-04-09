package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;

import org.springframework.format.number.NumberFormatter;
import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.services.AlunosService;
import com.gapso.web.trieda.shared.util.view.RelatorioAlunoFiltro;
import com.gapso.web.trieda.shared.util.view.TriedaException;

@Transactional
public class AlunosServiceImpl
	extends RemoteService
	implements AlunosService
{
	private static final long serialVersionUID = -3593829435380014345L;

	@Override
	public AlunoDTO getAluno( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		Aluno aluno = Aluno.find(
			id, getInstituicaoEnsinoUser() );

		if ( aluno == null )
		{
			return null;
		}

		return ConvertBeans.toAlunoDTO( aluno );
	}
	
	@Override
	public PagingLoadResult<AlunoDTO> getBuscaList(CenarioDTO cenarioDTO, String nome, String matricula, PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());

		List<AlunoDTO> list = new ArrayList<AlunoDTO>();
		String orderBy = config.getSortField();

		if (orderBy != null) {
			if (config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = (orderBy + " asc");
			}
			else {
				orderBy = (orderBy + " desc");
			}
		}

		List<Aluno> alunos = Aluno.findBy(this.getInstituicaoEnsinoUser(),cenario,nome,matricula,config.getOffset(),config.getLimit(),orderBy);
		for (Aluno aluno : alunos) {
			list.add(ConvertBeans.toAlunoDTO(aluno));
		}

		BasePagingLoadResult<AlunoDTO> result = new BasePagingLoadResult<AlunoDTO> (list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Aluno.count(this.getInstituicaoEnsinoUser(),cenario,nome,matricula));

		return result;
	}
	
	@Override
	public ListLoadResult< AlunoDTO > getAutoCompleteList(
		CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, String tipoComboBox )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< AlunoDTO > list = new ArrayList< AlunoDTO >();
		
		List< Aluno > listDomains = new ArrayList< Aluno >();
		
		if ( tipoComboBox.equals(AlunoDTO.PROPERTY_ALUNO_NOME) )
		{
			listDomains = Aluno.findBy( getInstituicaoEnsinoUser(), cenario,
					loadConfig.get("query").toString(), null, loadConfig.getOffset(), loadConfig.getLimit() );
		}
		else if ( tipoComboBox.equals(AlunoDTO.PROPERTY_ALUNO_MATRICULA) )
		{
			listDomains = Aluno.findBy( getInstituicaoEnsinoUser(), cenario,
					null, loadConfig.get("query").toString(), loadConfig.getOffset(), loadConfig.getLimit() );
		}

		for ( Aluno aluno : listDomains )
		{
			list.add( ConvertBeans.toAlunoDTO( aluno ) );
		}

		BasePagingLoadResult< AlunoDTO > result
			= new BasePagingLoadResult< AlunoDTO >( list );
		result.setOffset( loadConfig.getOffset() );

		return result;
	}

	@Override
	public PagingLoadResult< AlunoDTO > getAlunosList(
		String nome, String matricula )
	{
		List< Aluno > listDomains = null;

		// Carregar uma quantidade menor de dados
		if ( ( nome == null || nome.equals( "" ) )
			&& ( matricula == null || matricula.equals( "" ) ) )
		{
			listDomains = Aluno.findMaxResults(
				getInstituicaoEnsinoUser(), 20 );
		}
		// Todos os alunos que segundo
		// à busca realizada pelo usuário
		else
		{
			listDomains = Aluno.findByNomeMatricula(
				getInstituicaoEnsinoUser(), nome, matricula );
		}

		List< AlunoDTO > list	
			= new ArrayList< AlunoDTO >();

		list.addAll( ConvertBeans.toListAlunoDTO( listDomains ) );

		Collections.sort( list );

		BasePagingLoadResult< AlunoDTO > result
			= new BasePagingLoadResult< AlunoDTO >( list );

		result.setTotalLength( list.size() );

		return result;
	}

	@Override
	public PagingLoadResult<AlunoDTO> getAlunosListByCampus(CampusDTO campusDTO){
		Campus campus = Campus.find(campusDTO.getId(), this.getInstituicaoEnsinoUser());
		boolean tatico = AtendimentoTatico.findAllByCampus(this.getInstituicaoEnsinoUser(), campus).size() != 0;

		List<Aluno> listDomains = Aluno.findByCampus(getInstituicaoEnsinoUser(), campus, tatico);

		List<AlunoDTO> list = new ArrayList<AlunoDTO>();
		list.addAll( ConvertBeans.toListAlunoDTO(listDomains));

		BasePagingLoadResult<AlunoDTO> result = new BasePagingLoadResult<AlunoDTO>(list);
		result.setTotalLength(list.size());

		return result;
	}
	
	@Override
	public void saveAluno( AlunoDTO alunoDTO )
	{
		onlyAdministrador();

		Aluno aluno = ConvertBeans.toAluno( alunoDTO );

		try
		{
			if ( aluno.getId() != null
				&& aluno.getId() > 0 )
			{
				aluno.merge();
			}
			else
			{
				aluno.persist();
			}			
		}
		catch( Exception e )
		{
			System.out.println( e.getCause() );
		}
	}

	@Override
	public void removeAlunos( List< AlunoDTO > list )
	{
		for ( AlunoDTO alunoDTO : list )
		{
			Aluno aluno = ConvertBeans.toAluno( alunoDTO );
			aluno.remove();
		}
	}
	
	@Override
	public List<RelatorioDTO> getRelatorio(CenarioDTO cenarioDTO, RelatorioAlunoFiltro alunoFiltro, RelatorioDTO currentNode) throws TriedaException {
		List<RelatorioDTO> list = new ArrayList<RelatorioDTO>();
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());
		if (currentNode == null){
			// disponibiliza uma pasta para cada campus no relatório de resumo por campi
			List<Campus> campi = new ArrayList<Campus>(cenario.getCampi());
			Collections.sort(campi);
			for (Campus campus : campi) {
				RelatorioDTO nodeDTO = new RelatorioDTO(campus.getCodigo() + "(" + campus.getNome() + ")");
				nodeDTO.setCampusId(campus.getId());
				getRelatorioParaCampus(cenario, campus, alunoFiltro, nodeDTO);
				list.add( nodeDTO );
			}
		}

		return list;
	}
	
	public void getRelatorioParaCampus(Cenario cenario, Campus campus, RelatorioAlunoFiltro alunoFiltro, RelatorioDTO currentNode) throws TriedaException {
		
		Curso curso = alunoFiltro.getCurso() != null ? 
				Curso.find(alunoFiltro.getCurso().getId(), getInstituicaoEnsinoUser()) : null;
		Boolean formando = alunoFiltro.getFormando() == true ? true : null;
		
		List<Aluno> todosAlunos = Aluno.findBy(getInstituicaoEnsinoUser(), cenario, null,
				formando, alunoFiltro.getPeriodo());
		
		List<AlunoDemanda> alunosUteis = todosAlunos.size() > 0 ?
				AlunoDemanda.findByAlunos(getInstituicaoEnsinoUser(), todosAlunos, curso) : new ArrayList<AlunoDemanda>();
		int alunosAtendidos = todosAlunos.size() > 0 ?
				AlunoDemanda.findByAlunosAtendidos(getInstituicaoEnsinoUser(), todosAlunos, curso, formando).size() : 0;
		if (curso != null && alunosUteis.size() == 0)
		{
			throw new TriedaException("O curso filtrado não possui nenhuma demanda");
		}
		
		int alunosFormandos = 0;
		
		for(AlunoDemanda ad : alunosUteis){
			if(ad.getAluno().getFormando()){
				alunosFormandos++;
			}
		}

		
		Locale pt_BR = new Locale("pt","BR");
		NumberFormatter numberFormatter = new NumberFormatter();

		RelatorioDTO perfil = new RelatorioDTO( "<b>Perfil</b>");
		
		if(curso == null)
			perfil.add( new RelatorioDTO( "Total de Alunos Cadastrados: <b>" + numberFormatter.print(todosAlunos.size(),pt_BR)) );
		
		perfil.add( new RelatorioDTO( "Total de Alunos Formandos: <b>" + numberFormatter.print(alunosFormandos,pt_BR)) );
		perfil.add( new RelatorioDTO( "Total de Alunos Uteis (Alunos com Demanda): <b>" + numberFormatter.print(alunosUteis.size(),pt_BR)) );
		currentNode.add(perfil);
		
		RelatorioDTO atendimento = new RelatorioDTO( "<b>Atendimento</b>");
		atendimento.add( new RelatorioDTO( "Total de Alunos Atendidos: <b>" + numberFormatter.print(alunosAtendidos,pt_BR) + "</b>") );
		currentNode.add(atendimento);
		
		RelatorioDTO histogramas = new RelatorioDTO("<b>Histogramas</b>");
		RelatorioDTO histograma1 = new RelatorioDTO( "Faixa de Créditos Atendidos" );
		histograma1.setButtonText(histograma1.getText());
		histograma1.setButtonIndex(0);
		histogramas.add( histograma1 );
		RelatorioDTO histograma2 = new RelatorioDTO( "Quantidade de Disciplinas Não Atendidas" );
		histograma2.setButtonText(histograma2.getText());
		histograma2.setButtonIndex(1);
		histogramas.add( histograma2 );
		currentNode.add(histogramas);
	}
}
