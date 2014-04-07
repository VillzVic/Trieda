package com.gapso.web.trieda.server;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;

import org.springframework.format.number.CurrencyFormatter;
import org.springframework.format.number.NumberFormatter;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TipoProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.services.ProfessoresService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class ProfessoresServiceImpl
	extends RemoteService
	implements ProfessoresService
{
	private static final long serialVersionUID = -1972558331232685995L;

	@Override
	public ProfessorDTO getProfessor( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		return ConvertBeans.toProfessorDTO(
			Professor.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public List< HorarioDisponivelCenarioDTO > getHorariosDisponiveis(ProfessorDTO professorDTO)
	{
		Professor professor = Professor.find(
			professorDTO.getId(), getInstituicaoEnsinoUser() );

		List< HorarioDisponivelCenario > list
			= professor.getHorarios( getInstituicaoEnsinoUser() );

		List< HorarioDisponivelCenarioDTO > listDTO
			= ConvertBeans.toHorarioDisponivelCenarioDTO( list );

		return listDTO;
	}

	@Override
	public void saveHorariosDisponiveis(
		ProfessorDTO professorDTO, List< HorarioDisponivelCenarioDTO > listDTO )
	{
		List< HorarioDisponivelCenario > listSelecionados
			= ConvertBeans.toHorarioDisponivelCenario( listDTO );

		Professor professor = Professor.find(
			professorDTO.getId(), getInstituicaoEnsinoUser() );

		List< HorarioDisponivelCenario > adicionarList
			= new ArrayList< HorarioDisponivelCenario >( listSelecionados );

		adicionarList.removeAll( professor.getHorarios(
			getInstituicaoEnsinoUser() ) );

		List< HorarioDisponivelCenario > removerList
			= new ArrayList< HorarioDisponivelCenario >(
				professor.getHorarios( getInstituicaoEnsinoUser() ) );

		removerList.removeAll( listSelecionados );

		for ( HorarioDisponivelCenario o : removerList )
		{
			o.getProfessores().remove( professor );
			o.merge();
		}

		for ( HorarioDisponivelCenario o : adicionarList )
		{
			o.getProfessores().add( professor );
			o.merge();
		}
	}

	@Override
	public ListLoadResult< ProfessorDTO > getList(CenarioDTO cenarioDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		List< Professor > listProfessores
			= Professor.findByCenario( getInstituicaoEnsinoUser(), cenario );

		Collections.sort( listProfessores );

		for ( Professor professor : listProfessores )
		{
			list.add( ConvertBeans.toProfessorDTO( professor ) );
		}

		return new BaseListLoadResult< ProfessorDTO >( list );
	}

	@Override
	public ListLoadResult< ProfessorDTO > getAutoCompleteList(
			CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, String tipoComboBox)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		
		List< Professor > listDomains = new ArrayList< Professor >();
		
		if ( tipoComboBox.equals(ProfessorDTO.PROPERTY_NOME) )
		{
			listDomains = Professor.findBy( getInstituicaoEnsinoUser(), cenario,
					loadConfig.get("query").toString(), null, loadConfig.getOffset(), loadConfig.getLimit() );
		}
		else if ( tipoComboBox.equals(ProfessorDTO.PROPERTY_CPF) )
		{
			listDomains = Professor.findBy( getInstituicaoEnsinoUser(), cenario,
					null, loadConfig.get("query").toString(), loadConfig.getOffset(), loadConfig.getLimit() );
		}

		for ( Professor professor : listDomains )
		{
			list.add( ConvertBeans.toProfessorDTO( professor ) );
		}

		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );
		result.setOffset( loadConfig.getOffset() );

		return result;
	}
	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaList( CenarioDTO cenarioDTO, 
		String cpf, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO,
		AreaTitulacaoDTO areaTitulacaoDTO, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		TipoContrato tipoContrato = ( tipoContratoDTO == null ) ? null
			: TipoContrato.find( tipoContratoDTO.getId(), getInstituicaoEnsinoUser() );

		Titulacao titulacao = ( titulacaoDTO == null ) ? null
			: Titulacao.find( titulacaoDTO.getId(), this.getInstituicaoEnsinoUser() );

		AreaTitulacao areaTitulacao = ( areaTitulacaoDTO == null ) ? null
			: AreaTitulacao.find( areaTitulacaoDTO.getId(), this.getInstituicaoEnsinoUser() );

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
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		List< Professor > listDomains = Professor.findBy( getInstituicaoEnsinoUser(),
			cenario, cpf, tipoContrato, titulacao, areaTitulacao,
			config.getOffset(), config.getLimit(), orderBy );

		if ( listDomains != null )
		{
			for ( Professor professor : listDomains )
			{
				list.add( ConvertBeans.toProfessorDTO( professor ) );
			}
		}

		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( Professor.count(
			getInstituicaoEnsinoUser(), cenario, cpf,
			tipoContrato, titulacao, areaTitulacao ) );

		return result;
	}

	@Override
	public TipoContratoDTO getTipoContrato( Long id )
	{
		return ConvertBeans.toTipoContratoDTO(
			TipoContrato.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public ListLoadResult< TipoContratoDTO > getTiposContratoAll( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< TipoContratoDTO > listDTO
			= new ArrayList< TipoContratoDTO >();

		List< TipoContrato > listTiposContrato
			= TipoContrato.findByCenario( getInstituicaoEnsinoUser(), cenario );

		for ( TipoContrato tipoContrato : listTiposContrato )
		{
			listDTO.add( ConvertBeans.toTipoContratoDTO( tipoContrato ) );
		}

		return new BaseListLoadResult< TipoContratoDTO >( listDTO );
	}

	@Override
	public TitulacaoDTO getTitulacao( Long id )
	{
		Titulacao titulacao = Titulacao.find(
			id, this.getInstituicaoEnsinoUser() );

		return ( titulacao == null ? null :
			ConvertBeans.toTitulacaoDTO( titulacao ) );
	}

	@Override
	public ListLoadResult< TitulacaoDTO > getTitulacoesAll( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< TitulacaoDTO > listDTO = new ArrayList< TitulacaoDTO >();
		List< Titulacao > list = Titulacao.findByCenario( this.getInstituicaoEnsinoUser(), cenario );

		for ( Titulacao titulacao : list )
		{
			listDTO.add( ConvertBeans.toTitulacaoDTO( titulacao ) );
		}

		return new BaseListLoadResult< TitulacaoDTO >( listDTO );
	}

	@Override
	public void save( ProfessorDTO professorDTO ) throws TriedaException
	{
		try {
			onlyAdministrador();
	
			Professor professor = ConvertBeans.toProfessorComCampiTrabalho(professorDTO);
	
			if ( professor.getId() != null && professor.getId() > 0 )
			{
				professor.merge();
			}
			else
			{
				professor.persistAndPreencheHorarios();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public void remove( List< ProfessorDTO > professorDTOList )
	{
		onlyAdministrador();

		for ( ProfessorDTO professorDTO : professorDTOList )
		{
			Professor p = Professor.find(
				professorDTO.getId(), getInstituicaoEnsinoUser() );

			if ( p != null )
			{
				p.remove();
			}
		}
	}

	@Override
	public ListLoadResult< ProfessorDTO > getProfessoresEmCampus(
			CampusDTO campusDTO )
	{
		onlyAdministrador();

		Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );
		List< Professor > list = new ArrayList< Professor >( campus.getProfessores() );
		Collections.sort( list );

		List< ProfessorDTO > listDTO
			= new ArrayList< ProfessorDTO >( list.size() );

		for ( Professor professor : list )
		{
			listDTO.add( ConvertBeans.toProfessorDTO( professor ) );
		}

		return new BaseListLoadResult< ProfessorDTO >( listDTO );
	}

	@Override
	public List< ProfessorDTO > getProfessoresNaoEmCampus( CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		onlyAdministrador();
		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );
		Set< Professor > listAssociados = campus.getProfessores();
		List< Professor > list = Professor.findByCenario( getInstituicaoEnsinoUser(), cenario );
		list.removeAll( listAssociados );

		List< ProfessorDTO > listDTO
			= new ArrayList< ProfessorDTO >( list.size() );

		for ( Professor professor : list )
		{
			listDTO.add( ConvertBeans.toProfessorDTO( professor ) );
		}

		return listDTO;
	}

	@Override
	public PagingLoadResult< ProfessorCampusDTO > getProfessorCampusByCurrentProfessor( CenarioDTO cenarioDTO )
	{
		onlyProfessor();

		List< ProfessorCampusDTO > list
			= ConvertBeans.toProfessorCampusDTO( getUsuario().getProfessor() );

		return new BasePagingLoadResult< ProfessorCampusDTO >( list );
	}

	@Override
	public PagingLoadResult< ProfessorCampusDTO > getProfessorCampusList(
			CenarioDTO cenarioDTO, CampusDTO campusDTO, ProfessorDTO professorDTO )
	{
		onlyAdministrador();
		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());

		List< ProfessorCampusDTO > list = null;
		if ( campusDTO != null && professorDTO == null )
		{
			list = ConvertBeans.toProfessorCampusDTO(
				Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() ) );
		}
		else if ( campusDTO == null && professorDTO != null )
		{
			Professor p = Professor.find(
				professorDTO.getId(), getInstituicaoEnsinoUser() );

			if ( p != null )
			{
				list = ConvertBeans.toProfessorCampusDTO( p );
			}
		}
		else if ( campusDTO == null && professorDTO == null )
		{
			list = new ArrayList< ProfessorCampusDTO >();

			List< Campus > listDomains
				= Campus.findByCenario( this.getInstituicaoEnsinoUser(), cenario );

			for ( Campus campus : listDomains )
			{
				list.addAll( ConvertBeans.toProfessorCampusDTO( campus ) );
			}
		}
		else if ( campusDTO != null && professorDTO != null )
		{
			Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );
			Professor professor = Professor.find(
				professorDTO.getId(), getInstituicaoEnsinoUser() );

			for ( Campus c : professor.getCampi() )
			{
				if ( campus.equals( c ) )
				{
					list = new ArrayList< ProfessorCampusDTO >( 1 );
					ProfessorCampusDTO dto = new ProfessorCampusDTO();

					dto.setProfessorId( professor.getId() );
					dto.setProfessorString( professor.getNome() );
					dto.setProfessorCpf( professor.getCpf() );
					dto.setCampusId( campus.getId() );
					dto.setCampusString( campus.getCodigo() );

					list.add( dto );
					break;
				}
			}
		}

		BasePagingLoadResult< ProfessorCampusDTO > result
			= new BasePagingLoadResult< ProfessorCampusDTO >( list );

		result.setTotalLength( list.size() );
	
		return result;
	}

	@Override
	public void salvarProfessorCampus( CampusDTO campusDTO,
			List< ProfessorDTO > professorDTOList )
	{
		onlyAdministrador();

		Campus campus = Campus.find( campusDTO.getId(),
			this.getInstituicaoEnsinoUser() );

		List< Professor > professorList = new ArrayList< Professor >(
				professorDTOList.size() );

		for ( ProfessorDTO professorDTO : professorDTOList )
		{
			professorList.add( Professor.find( professorDTO.getId(), getInstituicaoEnsinoUser() ) );
		}

		// Remove os que não estão na lista
		for ( Professor professor : campus.getProfessores() )
		{
			if ( !professorList.contains( professor ) )
			{
				professor.getCampi().remove( campus );
				professor.merge();
			}
		}

		// Adiciona na lista os que não estão
		for ( Professor professor : professorList )
		{
			if ( !campus.getProfessores().contains( professor ) )
			{
				professor.getCampi().add( campus );
				professor.merge();
			}
		}
	}

	@Override
	public void removeProfessorCampus(
		List< ProfessorCampusDTO > professorCampusDTOList )
	{
		onlyAdministrador();

		for ( ProfessorCampusDTO pcDTO : professorCampusDTOList )
		{
			Professor professor = Professor.find(
				pcDTO.getProfessorId(), getInstituicaoEnsinoUser() );

			Campus campus = Campus.find( pcDTO.getCampusId(),
				this.getInstituicaoEnsinoUser() );

			if ( campus != null )
			{
				professor.getCampi().remove( campus );
				professor.merge();
			}
		}
	}
	
	public void geraHabilitacaoParaProfessoresVirtuaisCadastrados() {
		List<Professor> professores = Professor.findAll(getInstituicaoEnsinoUser());
		
		List<Professor> professoresVirtuais = new ArrayList<Professor>(professores.size());
		for (Professor p : professores) {
			if (p.getNome().contains("Virtual")) {
				professoresVirtuais.add(p);
			}
		}
		
		List<Disciplina> disciplinas = Disciplina.findAll(getInstituicaoEnsinoUser());
		try {
			File f = new File("script_inserts.sql");
			
			f.createNewFile();
			f.setWritable(true);
			
			FileOutputStream fos = new FileOutputStream(f);
			
			int p = professoresVirtuais.size();
			int d = disciplinas.size();
			int total = p * d;
			System.out.println("Total = "+total);
			for (Professor professorVirtual : professoresVirtuais) {
				for (Disciplina disciplina : disciplinas) {
					// Insert
	//				ProfessorDisciplina newPd = new ProfessorDisciplina();
	//
	//				newPd.setDisciplina(disciplina);
	//				newPd.setProfessor(professorVirtual);
	//				newPd.setNota(1);
	//				newPd.setPreferencia(1);
	
					//newPd.persist();
					
					System.out.println(--total);
					String s = "INSERT INTO `trieda`.`professores_disciplinas` (`prf_id`,`dis_id`,`prf_nota`,`prf_preferencia`,`version`) VALUES (" + professorVirtual.getId() + "," + disciplina.getId() + ",1,1,0);\n";
					fos.write(s.getBytes());
				}
			}
			
			fos.close();
		} catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public ListLoadResult< TipoProfessorDTO > getTiposProfessor()
	{
		List<TipoProfessorDTO> tipoProfessorList = new ArrayList<TipoProfessorDTO>();
		
		TipoProfessorDTO institucional = new TipoProfessorDTO();
		institucional.setNome("Institucional");
		institucional.setInstitucional(true);
		institucional.setVirtual(false);
		tipoProfessorList.add(institucional);
		
		TipoProfessorDTO virtual = new TipoProfessorDTO();
		virtual.setNome("Virtual");
		virtual.setInstitucional(false);
		virtual.setVirtual(true);
		tipoProfessorList.add(virtual);

		return new BaseListLoadResult< TipoProfessorDTO >( tipoProfessorList );
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getProfessoresDisciplinasHabilitadas(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<RelatorioQuantidadeDTO> professoresDisciplinasHabilitadas = new ArrayList<RelatorioQuantidadeDTO>();
		
			List<Professor> professoresInstitucionaisList = Professor.findByCampus(getInstituicaoEnsinoUser(), cenario, campus);
			for (Professor professor : professoresInstitucionaisList)
			{
				int numDisciplinas = professor.getDisciplinas().size();
				
				if (professoresDisciplinasHabilitadas.size() <= numDisciplinas)
				{
					for (int i = professoresDisciplinasHabilitadas.size(); i <= numDisciplinas; i++)
					{
						String titulo = "";
						if (i == 0)
						{
							titulo = "Nenhuma disciplina habilitada";
						}
						else
						{
							titulo = i + " disciplina" + ((i != 1) ? "s " : "") + " habilitada" + ((i != 1) ? "s " : "");
						}
						RelatorioQuantidadeDTO novaFaixa = new RelatorioQuantidadeDTO(titulo);
						professoresDisciplinasHabilitadas.add(novaFaixa);
					}
				}
				professoresDisciplinasHabilitadas.get(numDisciplinas).setQuantidade(professoresDisciplinasHabilitadas.get(numDisciplinas).getQuantidade() + 1);
			}
		return professoresDisciplinasHabilitadas;
	}
	

	@Override
	public List<RelatorioQuantidadeDTO> getProfessoresTitulacoes(CenarioDTO cenarioDTO, CampusDTO campusDTO, boolean somenteAlocados)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<RelatorioQuantidadeDTO> professoresTitulacoes = new ArrayList<RelatorioQuantidadeDTO>();
		
		Map<Titulacao, Integer> titulacoesToNumProfessoresMap = new HashMap<Titulacao, Integer>();
		Map<Titulacao, Integer> titulacoesToNumProfessoresVirtuaisMap = new HashMap<Titulacao, Integer>();
		for (Titulacao titulacao : Titulacao.findByCenario(getInstituicaoEnsinoUser(), cenario))
		{
			titulacoesToNumProfessoresMap.put(titulacao, 0);
			titulacoesToNumProfessoresVirtuaisMap.put(titulacao, 0);
		}
		
		List<Professor> professoresInstitucionaisList;
		List<ProfessorVirtual> professoresVirtuaisList;
		if (somenteAlocados)
		{
			professoresInstitucionaisList = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, null, null, null, null, null, null);
			professoresVirtuaisList = AtendimentoOperacional.findProfessoresVirtuaisUtilizados(getInstituicaoEnsinoUser(), cenario, campus, null, null, null, null);
		}
		else
		{
			professoresInstitucionaisList = Professor.findByCampus(getInstituicaoEnsinoUser(), cenario, campus);
			professoresVirtuaisList = ProfessorVirtual.findBy(getInstituicaoEnsinoUser(), cenario, campus) ;
		}
		for (Professor professor : professoresInstitucionaisList)
		{
			titulacoesToNumProfessoresMap.put(professor.getTitulacao(), titulacoesToNumProfessoresMap.get(professor.getTitulacao())+1 );
		}
		for (ProfessorVirtual professor : professoresVirtuaisList)
		{
			titulacoesToNumProfessoresVirtuaisMap.put(professor.getTitulacao(), titulacoesToNumProfessoresVirtuaisMap.get(professor.getTitulacao())+1 );
		}
		
		for (Entry<Titulacao, Integer> titulacao : titulacoesToNumProfessoresMap.entrySet())
		{
			RelatorioQuantidadeDTO novaFaixa = new RelatorioQuantidadeDTO(titulacao.getKey().getNome());
			novaFaixa.setQuantidade2(titulacao.getValue());
			novaFaixa.setQuantidade(titulacoesToNumProfessoresVirtuaisMap.get(titulacao.getKey()));
			professoresTitulacoes.add(novaFaixa);
		}
		return professoresTitulacoes;
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getProfessoresAreasConhecimento(CenarioDTO cenarioDTO, CampusDTO campusDTO, TipoProfessorDTO tipoProfessorDTO, boolean somenteAlocados)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<RelatorioQuantidadeDTO> professoresTitulacoes = new ArrayList<RelatorioQuantidadeDTO>();
		
		Map<AreaTitulacao, Integer> areasTitulacaoToNumProfessoresMap = new HashMap<AreaTitulacao, Integer>();
		for (AreaTitulacao areaTitulacao : AreaTitulacao.findByCenario(getInstituicaoEnsinoUser(), cenario))
		{
			areasTitulacaoToNumProfessoresMap.put(areaTitulacao, 0);
		}
		
		if (tipoProfessorDTO == null || tipoProfessorDTO.getInstitucional())
		{
			List<Professor> professoresInstitucionaisList;
			if (somenteAlocados)
			{
				professoresInstitucionaisList = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, null, null, null, null, null, null);
			}
			else
			{
				professoresInstitucionaisList = Professor.findByCampus(getInstituicaoEnsinoUser(), cenario, campus);
			}
			for (Professor professor : professoresInstitucionaisList)
			{
				if (professor.getAreaTitulacao() != null)
				{
					areasTitulacaoToNumProfessoresMap.put(professor.getAreaTitulacao(), areasTitulacaoToNumProfessoresMap.get(professor.getAreaTitulacao())+1 );
				}
			}
		}
		
		if (tipoProfessorDTO == null || tipoProfessorDTO.getVirtual())
		{
			List<ProfessorVirtual> professoresVirtuaisList;
			if (somenteAlocados)
			{
				professoresVirtuaisList = AtendimentoOperacional.findProfessoresVirtuaisUtilizados(getInstituicaoEnsinoUser(), cenario, campus, null, null, null, null);
			}
			else
			{
				professoresVirtuaisList = ProfessorVirtual.findBy(getInstituicaoEnsinoUser(), cenario, campus) ;
			}
			for (ProfessorVirtual professor : professoresVirtuaisList)
			{
				if (professor.getAreaTitulacao() != null)
				{
				areasTitulacaoToNumProfessoresMap.put(professor.getAreaTitulacao(), areasTitulacaoToNumProfessoresMap.get(professor.getAreaTitulacao())+1 );
				}
			}
		}
		
		for (Entry<AreaTitulacao, Integer> areaTitulacao : areasTitulacaoToNumProfessoresMap.entrySet())
		{
			RelatorioQuantidadeDTO novaFaixa = new RelatorioQuantidadeDTO(areaTitulacao.getKey().getCodigo());
			novaFaixa.setQuantidade(areaTitulacao.getValue());
			professoresTitulacoes.add(novaFaixa);
		}
		return professoresTitulacoes;
	}
	
	@Override
	public List<RelatorioDTO> getRelatorio(CenarioDTO cenarioDTO, RelatorioProfessorFiltro professorFiltro, RelatorioDTO currentNode) {
		List<RelatorioDTO> list = new ArrayList<RelatorioDTO>();
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());
		if (currentNode == null){
			// disponibiliza uma pasta para cada campus no relatório de resumo por campi
			List<Campus> campi = new ArrayList<Campus>(cenario.getCampi());
			Collections.sort(campi);
			for (Campus campus : campi) {
				RelatorioDTO nodeDTO = new RelatorioDTO(campus.getCodigo() + "(" + campus.getNome() + ")");
				nodeDTO.setCampusId(campus.getId());
				getRelatorioParaCampus(cenario, campus, professorFiltro, nodeDTO);
				list.add( nodeDTO );
			}
		}

		return list;
	}
	
	public void getRelatorioParaCampus(Cenario cenario, Campus campus, RelatorioProfessorFiltro professorFiltro, RelatorioDTO currentNode) {
		
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		List<Professor> todosProfessores = Professor.findByCampus(getInstituicaoEnsinoUser(), cenario, campus);
		
		List<Professor> professoresUteis = Professor.findProfessoresUteis(getInstituicaoEnsinoUser(), cenario, campus);
		
		//List<Professor> professoresAlocados = Professor.findProfessoresAlocados(getInstituicaoEnsinoUser(), cenario, campus);
		
		List<Professor> professoresUtilizados = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario,
				campus, curso, turno, titulacao, areaTitulacao, tipoContrato, null);
		
		List<ProfessorVirtual> professoresVirtuaisUtilizados = AtendimentoOperacional.findProfessoresVirtuaisUtilizados(getInstituicaoEnsinoUser(),
				cenario, campus, curso, turno, titulacao, areaTitulacao);
		
		Map<Titulacao, Integer> docentesPorTitulacaoMap = new HashMap<Titulacao, Integer>();
		Map<TipoContrato, Integer> docentesPorTipoContratoMap = new HashMap<TipoContrato, Integer>();
		SortedMap<AreaTitulacao, Integer> docentesPorAreaTitulacaoMap = new TreeMap<AreaTitulacao, Integer>();
		Map<Titulacao, Double> custoPorTitulacaoMap = new HashMap<Titulacao, Double>();
		Map<TipoContrato, Double> custoPorTipoContratoMap = new HashMap<TipoContrato, Double>();
		SortedMap<AreaTitulacao, Double> custoPorAreaTitulacaoMap = new TreeMap<AreaTitulacao, Double>();
		double custoMedioDocenteTotal = 0;
		double mediaDisciplinasHabilitadas = 0;
		for (Professor professor : professoresUteis)
		{
			if (docentesPorTitulacaoMap.get(professor.getTitulacao()) == null)
			{
				docentesPorTitulacaoMap.put(professor.getTitulacao(), 1);
			}
			else
			{
				docentesPorTitulacaoMap.put(professor.getTitulacao(), docentesPorTitulacaoMap.get(professor.getTitulacao()) + 1);
			}
			
			if (docentesPorTipoContratoMap.get(professor.getTipoContrato()) == null)
			{
				docentesPorTipoContratoMap.put(professor.getTipoContrato(), 1);
			}
			else
			{
				docentesPorTipoContratoMap.put(professor.getTipoContrato(), docentesPorTipoContratoMap.get(professor.getTipoContrato()) + 1);
			}
			if (professor.getAreaTitulacao() != null)
			{
				if (docentesPorAreaTitulacaoMap.get(professor.getAreaTitulacao()) == null)
				{
					docentesPorAreaTitulacaoMap.put(professor.getAreaTitulacao(), 1);
				}
				else
				{
					docentesPorAreaTitulacaoMap.put(professor.getAreaTitulacao(), docentesPorAreaTitulacaoMap.get(professor.getAreaTitulacao()) + 1);
				}
				
				if (custoPorAreaTitulacaoMap.get(professor.getAreaTitulacao()) == null)
				{
					custoPorAreaTitulacaoMap.put(professor.getAreaTitulacao(), professor.getValorCredito());
				}
				else
				{
					custoPorAreaTitulacaoMap.put(professor.getAreaTitulacao(), custoPorAreaTitulacaoMap.get(professor.getAreaTitulacao()) + professor.getValorCredito());
				}
			}
			if (custoPorTitulacaoMap.get(professor.getTitulacao()) == null)
			{
				custoPorTitulacaoMap.put(professor.getTitulacao(), professor.getValorCredito());
			}
			else
			{
				custoPorTitulacaoMap.put(professor.getTitulacao(), custoPorTitulacaoMap.get(professor.getTitulacao()) + professor.getValorCredito());
			}
			
			if (custoPorTipoContratoMap.get(professor.getTipoContrato()) == null)
			{
				custoPorTipoContratoMap.put(professor.getTipoContrato(), professor.getValorCredito());
			}
			else
			{
				custoPorTipoContratoMap.put(professor.getTipoContrato(), custoPorTipoContratoMap.get(professor.getTipoContrato()) + professor.getValorCredito());
			}
			custoMedioDocenteTotal += professor.getValorCredito();
			mediaDisciplinasHabilitadas += professor.getDisciplinas().size();
		}
		mediaDisciplinasHabilitadas = mediaDisciplinasHabilitadas/professoresUteis.size();
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		Set<String> creditos = new HashSet<String>();
		Map<Professor, Set<String>> professorToTurmasMap = new HashMap<Professor, Set<String>>();
		Map<Professor, Integer> professorToCreditosMap = new HashMap<Professor, Integer>();
		Map<Professor, Set<HorarioDisponivelCenario>> professorToListHorarioDisponivelCenario = new HashMap<Professor, Set<HorarioDisponivelCenario>>();
		Map<Professor, Set<Unidade>> professoresToUnidadesMap = new HashMap<Professor, Set<Unidade>>();
		Set<Disciplina> totalDisciplinasProfessorVirtual = new HashSet<Disciplina>();
		Set<String> totalCreditosProfessorVirtual = new HashSet<String>();
		Set<String> totalTurmasProfessorVirtual = new HashSet<String>();
		
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			String keyCreditos = (atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta().getId() :
				atendimento.getDisciplina().getId()) + "-" +
				atendimento.getTurma() + "-" + atendimento.getHorarioDisponivelCenario().getId();
			String keyTurmas = (atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta().getId() :
				atendimento.getDisciplina().getId()) + "-" +
				atendimento.getTurma();
			if (atendimento.getProfessor().getCpf() != null)
			{
				creditos.add(keyCreditos);
				
				if (professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()) == null)
				{
					Set<HorarioDisponivelCenario> novoHorarioList = new HashSet<HorarioDisponivelCenario>();
					novoHorarioList.add(atendimento.getHorarioDisponivelCenario());
					professorToListHorarioDisponivelCenario.put(atendimento.getProfessor(), novoHorarioList);
				}
				else
				{
					professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()).add(atendimento.getHorarioDisponivelCenario());
				}
				if (professorToTurmasMap.get(atendimento.getProfessor()) == null)
				{
					Set<String> novaTurma = new HashSet<String>();
					novaTurma.add(keyTurmas);
					professorToTurmasMap.put(atendimento.getProfessor(), novaTurma );
				}
				else
				{
					professorToTurmasMap.get(atendimento.getProfessor()).add(keyTurmas);
				}
				
				if (professoresToUnidadesMap.get(atendimento.getProfessor()) == null)
				{
					Set<Unidade> novaUnidade = new HashSet<Unidade>();
					novaUnidade.add(atendimento.getSala().getUnidade());
					professoresToUnidadesMap.put(atendimento.getProfessor(), novaUnidade);
				}
				else
				{
					professoresToUnidadesMap.get(atendimento.getProfessor()).add(atendimento.getSala().getUnidade());
				}
				
				if (professorToCreditosMap.get(atendimento.getProfessor()) == null)
				{
					professorToCreditosMap.put(atendimento.getProfessor(), 1 );
				}
				else
				{
					professorToCreditosMap.put(atendimento.getProfessor(), professorToCreditosMap.get(atendimento.getProfessor())+1);
				}
			}
			else
			{
				totalCreditosProfessorVirtual.add(keyCreditos);
				totalTurmasProfessorVirtual.add(keyTurmas);
				totalDisciplinasProfessorVirtual.add(atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta() :
					atendimento.getDisciplina());
			}
		}
		
		
		Integer qtdProfessoresVirtuais = ( campus.isOtimizadoOperacional(getInstituicaoEnsinoUser()) ?
				AtendimentoOperacional.countProfessoresVirtuais(getInstituicaoEnsinoUser(), campus) : 0);
		
		int totalTurmasInstituicao = 0;
		
		for (Entry<Professor, Set<String>> professorTurmas : professorToTurmasMap.entrySet())
		{
			totalTurmasInstituicao += professorTurmas.getValue().size();
		}
		
		List<Professor> professoresBemAlocados = new ArrayList<Professor>();
		List<Professor> professoresMalAlocados = new ArrayList<Professor>();
		for (Entry<Professor, Integer> professorCreditos : professorToCreditosMap.entrySet())
		{
			if (professorCreditos.getValue() > 6)
			{
				professoresBemAlocados.add(professorCreditos.getKey());
			} else if(professorCreditos.getValue() < 2)
			{
				professoresMalAlocados.add(professorCreditos.getKey());
			}
		}
		
		
		
		Set<Professor> professoresComDeslocamentosUnidades = new HashSet<Professor>();
		Set<Professor> professoresComDeslocamentosCampi = new HashSet<Professor>();
		for (Entry<Professor, Set<Unidade>> professorUnidades : professoresToUnidadesMap.entrySet())
		{
			Set<Campus> campi = new HashSet<Campus>();
			if (professoresToUnidadesMap.get(professorUnidades.getKey()).size() > 1)
			{
				professoresComDeslocamentosUnidades.add(professorUnidades.getKey());
				for (Unidade unidade : professorUnidades.getValue())
				{
					campi.add(unidade.getCampus());
				}
			}
			if (campi.size() > 1)
			{
				professoresComDeslocamentosCampi.add(professorUnidades.getKey());
			}
		}
		
		Set<Professor> professoresGradeCheia = new HashSet<Professor>();
		Set<Professor> professoresComJanelas = new HashSet<Professor>();
		for (Entry<Professor, Set<HorarioDisponivelCenario>> professor : professorToListHorarioDisponivelCenario.entrySet() )
		{
			if (professor.getKey().getHorarios().size() == professor.getValue().size())
			{
				professoresGradeCheia.add(professor.getKey());
			}
			
			Map<Semanas, List<HorarioDisponivelCenario>> diaMapHorarios = new HashMap<Semanas, List<HorarioDisponivelCenario>>();
			
			for (HorarioDisponivelCenario horario : professor.getValue())
			{
				if (diaMapHorarios.get(horario.getDiaSemana()) == null)
				{
					List<HorarioDisponivelCenario> novoHorario = new ArrayList<HorarioDisponivelCenario>();
					novoHorario.add(horario);
					diaMapHorarios.put(horario.getDiaSemana(), novoHorario);
				}
				else
				{
					diaMapHorarios.get(horario.getDiaSemana()).add(horario);
				}
			}
			for (Entry<Semanas, List<HorarioDisponivelCenario>> horarioSemana : diaMapHorarios.entrySet())
			{
				int janelas = 0;
				List<HorarioDisponivelCenario> horarioProfessorOrdenado = horarioSemana.getValue();
				
				Collections.sort(horarioProfessorOrdenado, new Comparator<HorarioDisponivelCenario>() {
					@Override
					public int compare(HorarioDisponivelCenario arg1, HorarioDisponivelCenario arg2) {
						Calendar h1 = Calendar.getInstance();
						h1.setTime(arg1.getHorarioAula().getHorario());
						h1.set(1979,Calendar.NOVEMBER,6);
						
						Calendar h2 = Calendar.getInstance();
						h2.setTime(arg2.getHorarioAula().getHorario());
						h2.set(1979,Calendar.NOVEMBER,6);
						
						
						return h1.compareTo(h2);
					}
				});
				if (!horarioProfessorOrdenado.isEmpty())
				{
					Calendar h1 = Calendar.getInstance();
					h1.setTime(horarioProfessorOrdenado.get(0).getHorarioAula().getHorario());
					for (int i = 1; i < horarioProfessorOrdenado.size(); i++)
					{
						h1.add(Calendar.MINUTE,horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo());
						Calendar h2 = Calendar.getInstance();
						h2.setTime(horarioProfessorOrdenado.get(i).getHorarioAula().getHorario());
						
						if (h2.getTimeInMillis() - h1.getTimeInMillis() > (horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo() * 60000))
						{
							janelas++;
						}
					}
				}
				if (janelas >= 1)
				{
					professoresComJanelas.add(professor.getKey());
				}
			}
		}
		
		Locale pt_BR = new Locale("pt","BR");
		NumberFormatter numberFormatter = new NumberFormatter();
		CurrencyFormatter currencyFormatter = new CurrencyFormatter();

		RelatorioDTO professoresInstituicao = new RelatorioDTO("<b>PROFESSORES DA INSTITUIÇÃO</b>");
		RelatorioDTO perfil = new RelatorioDTO( "<b>Perfil</b>");
		perfil.add( new RelatorioDTO( " Total de Docentes Cadastrados: <b>" + numberFormatter.print(todosProfessores.size(),pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " Total de Docentes Úteis: <b>" + numberFormatter.print(professoresUteis.size(),pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " Total e Percentual de Docentes por Titulação: ") );
		for ( Entry<Titulacao, Integer> numDocentesPorCampo : docentesPorTitulacaoMap.entrySet())
		{
			perfil.add( new RelatorioDTO( " |---" + numDocentesPorCampo.getKey().getNome() + ": <b>" + numDocentesPorCampo.getValue() + 
					" (" + TriedaUtil.round(((double)numDocentesPorCampo.getValue())/((double)professoresUteis.size())*100.0,2) + "%)") );
		}
		perfil.add( new RelatorioDTO( " Total e Percentual de Docentes por Regime: ") );
		for ( Entry<TipoContrato, Integer> numDocentesPorCampo : docentesPorTipoContratoMap.entrySet())
		{
			perfil.add( new RelatorioDTO( " |---" + numDocentesPorCampo.getKey().getNome() + ": <b>" + numDocentesPorCampo.getValue() + 
					" (" + TriedaUtil.round(((double)numDocentesPorCampo.getValue())/((double)professoresUteis.size())*100.0,2) + "%)") );
		}
		perfil.add( new RelatorioDTO( " Total e Percentual de Docentes por Área de Conhecimento: ") );
		for ( Entry<AreaTitulacao, Integer> numDocentesPorCampo : docentesPorAreaTitulacaoMap.entrySet())
		{
			perfil.add( new RelatorioDTO( " |---" + numDocentesPorCampo.getKey().getCodigo() + ": <b>" + numDocentesPorCampo.getValue() + 
					" (" + TriedaUtil.round(((double)numDocentesPorCampo.getValue())/((double)professoresUteis.size())*100.0,2) + "%)") );
		}
		perfil.add( new RelatorioDTO( " Média de Disciplinas Habilitadas por Docente: <b>" + numberFormatter.print(TriedaUtil.round(mediaDisciplinasHabilitadas, 2) ,pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " Custo Médio Docente Total: <b>" + currencyFormatter.print(custoMedioDocenteTotal/professoresUteis.size() ,pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " Custo Médio Docente por Titulação: ") );
		for ( Entry<Titulacao, Double> custoPorCampo : custoPorTitulacaoMap.entrySet())
		{
			perfil.add( new RelatorioDTO( " |---" + custoPorCampo.getKey().getNome() + ": <b>" +
					currencyFormatter.print((double)custoPorCampo.getValue()/((double)docentesPorTitulacaoMap.get(custoPorCampo.getKey())), pt_BR)) );
		}
		perfil.add( new RelatorioDTO( " Custo Médio Docente por Regime: ") );
		for ( Entry<TipoContrato, Double> custoPorCampo : custoPorTipoContratoMap.entrySet())
		{
			perfil.add( new RelatorioDTO( " |---" + custoPorCampo.getKey().getNome() + ": <b>" +
					currencyFormatter.print((double)custoPorCampo.getValue()/((double)docentesPorTipoContratoMap.get(custoPorCampo.getKey())), pt_BR)) );
		}
		perfil.add( new RelatorioDTO( " Custo Médio Docente por Área de Conhecimento: ") );
		for ( Entry<AreaTitulacao, Double> custoPorCampo : custoPorAreaTitulacaoMap.entrySet())
		{
			perfil.add( new RelatorioDTO( " |---" + custoPorCampo.getKey().getCodigo() + ": <b>" +
					currencyFormatter.print((double)custoPorCampo.getValue()/((double)docentesPorAreaTitulacaoMap.get(custoPorCampo.getKey())), pt_BR)) );
		}
		professoresInstituicao.add(perfil);
		RelatorioDTO atendimento = new RelatorioDTO( "<b>Atendimento</b>");
		atendimento.add( new RelatorioDTO( " Total de Docentes Utilizados no Atendimento: <b>" + numberFormatter.print(professoresUtilizados.size(),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " Taxa de Uso dos Docentes da Base: <b>" + numberFormatter.print(
				TriedaUtil.round((double)professoresUtilizados.size()/(double)professoresUteis.size(), 2)*100,pt_BR) + "%</b>") );
		atendimento.add( new RelatorioDTO( " Média de Créditos Semanais por Docente: <b>" + numberFormatter.print(
				TriedaUtil.round((double)creditos.size()/(double)professoresUtilizados.size(), 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " Turmas Ministradas por Docentes da Instituição: <b>" + numberFormatter.print(
				totalTurmasInstituicao,pt_BR) + "</b>") );
		
		atendimento.add( new RelatorioDTO( " Média de Turmas por Docente: <b>" + numberFormatter.print(
				TriedaUtil.round((double)totalTurmasInstituicao/(double)professorToTurmasMap.size(), 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " Listas de Docentes Utilizados no Atendimento: ") );
		RelatorioDTO todosDaInstituicaoDocentes = new RelatorioDTO( " Docentes da Instituição Utilizados no Atendimento: " + numberFormatter.print(professoresUtilizados.size(),pt_BR));
		todosDaInstituicaoDocentes.setButtonText(todosDaInstituicaoDocentes.getText());
		todosDaInstituicaoDocentes.setButtonIndex(5);
		todosDaInstituicaoDocentes.setCampusId(campus.getId());
		atendimento.add( todosDaInstituicaoDocentes );
		RelatorioDTO docentesGradeCheia = new RelatorioDTO( " Docentes com Grade Cheia: " + numberFormatter.print(professoresGradeCheia.size(),pt_BR));
		docentesGradeCheia.setButtonText(docentesGradeCheia.getText());
		docentesGradeCheia.setButtonIndex(6);
		docentesGradeCheia.setCampusId(campus.getId());
		atendimento.add( docentesGradeCheia );
		RelatorioDTO docentesBemAlocados = new RelatorioDTO( " Docentes Bem Alocados: " + numberFormatter.print(professoresBemAlocados.size(),pt_BR));
		docentesBemAlocados.setButtonText(docentesBemAlocados.getText());
		docentesBemAlocados.setButtonIndex(7);
		docentesBemAlocados.setCampusId(campus.getId());
		atendimento.add( docentesBemAlocados );
		RelatorioDTO docentesMalAlocados = new RelatorioDTO( " Docentes Mal Alocados: " + numberFormatter.print(professoresMalAlocados.size(),pt_BR));
		docentesMalAlocados.setButtonText(docentesMalAlocados.getText());
		docentesMalAlocados.setButtonIndex(8);
		docentesMalAlocados.setCampusId(campus.getId());
		atendimento.add( docentesMalAlocados );
		RelatorioDTO docentesComJanelas = new RelatorioDTO( " Docentes com 1 ou Mais Buracos na Grade: " + numberFormatter.print(professoresComJanelas.size(),pt_BR));
		docentesComJanelas.setButtonText(docentesComJanelas.getText());
		docentesComJanelas.setButtonIndex(9);
		docentesComJanelas.setCampusId(campus.getId());
		atendimento.add( docentesComJanelas );
		RelatorioDTO docentesDeslocamentoUnidades = new RelatorioDTO( " Docentes com Deslocamentos Entre Unidades Distintas: " + numberFormatter.print(professoresComDeslocamentosUnidades.size(),pt_BR));
		docentesDeslocamentoUnidades.setButtonText(docentesDeslocamentoUnidades.getText());
		docentesDeslocamentoUnidades.setButtonIndex(10);
		docentesDeslocamentoUnidades.setCampusId(campus.getId());
		atendimento.add( docentesDeslocamentoUnidades );
		RelatorioDTO docentesDeslocamentoCampi = new RelatorioDTO( " Docentes com Deslocamentos Entre Campi: " + numberFormatter.print(professoresComDeslocamentosCampi.size(),pt_BR));
		docentesDeslocamentoCampi.setButtonText(docentesDeslocamentoCampi.getText());
		docentesDeslocamentoCampi.setButtonIndex(11);
		docentesDeslocamentoCampi.setCampusId(campus.getId());
		atendimento.add( docentesDeslocamentoCampi );
		professoresInstituicao.add(atendimento);
		RelatorioDTO professoresVirtuais = new RelatorioDTO("<b>PROFESSORES VIRTUAIS</b>");
		RelatorioDTO atendimentoVirtual = new RelatorioDTO( "<b>Atendimento</b>");
		atendimentoVirtual.add( new RelatorioDTO( " Total de Professores Virtuais Utilizados no Atendimento: <b>" + numberFormatter.print(professoresVirtuaisUtilizados.size(),pt_BR) + "</b>") );
		atendimentoVirtual.add( new RelatorioDTO( " Média de Disciplinas por Professor Virtual: <b>" + numberFormatter.print(
				TriedaUtil.round((double)totalDisciplinasProfessorVirtual.size()/(double)professoresVirtuaisUtilizados.size(), 2),pt_BR) + "</b>") );
		atendimentoVirtual.add( new RelatorioDTO( " Média de Créditos Semanais por Professor Virtual: <b>" + numberFormatter.print(
				TriedaUtil.round((double)totalCreditosProfessorVirtual.size()/(double)professoresVirtuaisUtilizados.size(), 2),pt_BR) + "</b>") );
		atendimentoVirtual.add( new RelatorioDTO( " Média de Turmas por Professor Virtual: <b>" + numberFormatter.print(
				TriedaUtil.round((double)totalTurmasProfessorVirtual.size()/(double)qtdProfessoresVirtuais, 2),pt_BR) + "</b>") );
		professoresVirtuais.add(atendimentoVirtual);
		RelatorioDTO docentesVirtuais = new RelatorioDTO( " Lista Professores Virtuais");
		docentesVirtuais.setButtonText(docentesVirtuais.getText());
		docentesVirtuais.setButtonIndex(12);
		docentesVirtuais.setCampusId(campus.getId());
		atendimentoVirtual.add( docentesVirtuais );
		RelatorioDTO histogramas = new RelatorioDTO("<b>Histogramas</b>");
		int todos = professoresUtilizados.size() + professoresVirtuaisUtilizados.size();
		RelatorioDTO histograma1 = new RelatorioDTO( " Todos os Docentes Utilizados no Atendimento: " + numberFormatter.print(todos,pt_BR));
		histograma1.setButtonText(histograma1.getText());
		histograma1.setButtonIndex(13);
		histograma1.setCampusId(campus.getId());
		histogramas.add( histograma1 );
		RelatorioDTO histograma2 = new RelatorioDTO( "Número de Buracos na Grade" );
		histograma2.setButtonText(histograma2.getText());
		histograma2.setButtonIndex(0);
		histogramas.add( histograma2 );
		RelatorioDTO histograma3 = new RelatorioDTO( "Número de Disciplinas Habilitadas" );
		histograma3.setButtonText(histograma3.getText());
		histograma3.setButtonIndex(1);
		histogramas.add( histograma3 );
		RelatorioDTO histograma4 = new RelatorioDTO( "Número de Disciplinas Lecionadas" );
		histograma4.setButtonText(histograma4.getText());
		histograma4.setButtonIndex(2);
		histogramas.add( histograma4 );
		RelatorioDTO histograma5 = new RelatorioDTO( "Professores Alocados por Titulação" );
		histograma5.setButtonText(histograma5.getText());
		histograma5.setButtonIndex(3);
		histograma5.setCampusId(campus.getId());
		histogramas.add( histograma5 );
		RelatorioDTO histograma6 = new RelatorioDTO( "Professores Alocados por Área de Conhecimento" );
		histograma6.setButtonText(histograma6.getText());
		histograma6.setButtonIndex(4);
		histograma6.setCampusId(campus.getId());
		histogramas.add( histograma6 );
		
		
		currentNode.add(professoresInstituicao);
		currentNode.add(professoresVirtuais);
		currentNode.add(histogramas);
	}

	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListAtendimentos( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
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
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		List< Professor > listDomains = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario,
				campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf, config.getOffset(), config.getLimit(), orderBy );
		
		if ( listDomains != null )
		{
			for ( Professor professor : listDomains )
			{
				list.add( ConvertBeans.toProfessorDTO( professor ) );
			}
		}

		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( AtendimentoOperacional.findProfessoresUtilizados(
				getInstituicaoEnsinoUser(), cenario,
				campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf).size() );

		return result;
	}
	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListTodosAtendimentos( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
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
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		List< Professor > listDomains = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario,
				campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf, config.getOffset(), config.getLimit(), orderBy );
		
		List< ProfessorVirtual > professoresVirtuaisList = AtendimentoOperacional.findProfessoresVirtuaisUtilizados(getInstituicaoEnsinoUser(), cenario, campus, null, null, null, null);
		
		if ( listDomains != null )
		{
			for ( Professor professor : listDomains )
			{
				list.add( ConvertBeans.toProfessorDTO( professor ) );
			}
		}
		
		if ( professoresVirtuaisList != null )
		{
			for ( ProfessorVirtual professor : professoresVirtuaisList )
			{
				list.add( ConvertBeans.toProfessorDTO( professor ) );
			}
		}


		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( AtendimentoOperacional.findProfessoresUtilizados(
				getInstituicaoEnsinoUser(), cenario,
				campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf).size() );

		return result;
	}
	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListGradeCheia( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		final String orderBy = config.getSortField();
		final SortDir sortBy = config.getSortDir();
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<HorarioDisponivelCenario>> professorToListHorarioDisponivelCenario = new HashMap<Professor, Set<HorarioDisponivelCenario>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()) == null)
				{
					Set<HorarioDisponivelCenario> novoHorarioList = new HashSet<HorarioDisponivelCenario>();
					novoHorarioList.add(atendimento.getHorarioDisponivelCenario());
					professorToListHorarioDisponivelCenario.put(atendimento.getProfessor(), novoHorarioList);
				}
				else
				{
					professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()).add(atendimento.getHorarioDisponivelCenario());
				}
			}
		}
		
		List<Professor> professoresGradeCheia = new ArrayList<Professor>();
		for (Entry<Professor, Set<HorarioDisponivelCenario>> professor : professorToListHorarioDisponivelCenario.entrySet() )
		{
			if (professor.getKey().getHorarios().size() <= professor.getValue().size())
			{
				professoresGradeCheia.add(professor.getKey());
			}
			
			Map<Semanas, List<HorarioDisponivelCenario>> diaMapHorarios = new HashMap<Semanas, List<HorarioDisponivelCenario>>();
			
			for (HorarioDisponivelCenario horario : professor.getValue())
			{
				if (diaMapHorarios.get(horario.getDiaSemana()) == null)
				{
					List<HorarioDisponivelCenario> novoHorario = new ArrayList<HorarioDisponivelCenario>();
					novoHorario.add(horario);
					diaMapHorarios.put(horario.getDiaSemana(), novoHorario);
				}
				else
				{
					diaMapHorarios.get(horario.getDiaSemana()).add(horario);
				}
			}
		}
		
		Collections.sort(professoresGradeCheia, new Comparator<Professor>() {
			@Override
			public int compare(Professor arg1, Professor arg2) {
				return compareProfessor(arg1, arg2, orderBy, sortBy);
			}
		});


		//Map de professores porque eh necessario usar professores com fetch atendimentos para calcular carga horaria e credios
		List<Professor> professoresUtilizados = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf);
		Map<Long, Professor> professorIdMapProfessor = new HashMap<Long, Professor>();
		for (Professor professor : professoresUtilizados)
		{
			professorIdMapProfessor.put(professor.getId(), professor);
		}
		if ( !professoresGradeCheia.isEmpty() )
		{
			for ( int i = config.getOffset(); (i < professoresGradeCheia.size() && i < (config.getOffset() + config.getLimit())); i++ )
			{
				list.add( ConvertBeans.toProfessorDTO( professorIdMapProfessor.get(professoresGradeCheia.get(i).getId()) ) );
			}
		}
		
		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( professoresGradeCheia.size() );

		return result;
	}
	
	public List< Professor > getBuscaListGradeCheiaExport( Cenario cenario, Long campusDTO, RelatorioProfessorFiltro professorFiltro )
	{
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<HorarioDisponivelCenario>> professorToListHorarioDisponivelCenario = new HashMap<Professor, Set<HorarioDisponivelCenario>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()) == null)
				{
					Set<HorarioDisponivelCenario> novoHorarioList = new HashSet<HorarioDisponivelCenario>();
					novoHorarioList.add(atendimento.getHorarioDisponivelCenario());
					professorToListHorarioDisponivelCenario.put(atendimento.getProfessor(), novoHorarioList);
				}
				else
				{
					professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()).add(atendimento.getHorarioDisponivelCenario());
				}
			}
		}
		
		List<Professor> professoresGradeCheia = new ArrayList<Professor>();
		for (Entry<Professor, Set<HorarioDisponivelCenario>> professor : professorToListHorarioDisponivelCenario.entrySet() )
		{
			if (professor.getKey().getHorarios().size() <= professor.getValue().size())
			{
				professoresGradeCheia.add(professor.getKey());
			}
			
			Map<Semanas, List<HorarioDisponivelCenario>> diaMapHorarios = new HashMap<Semanas, List<HorarioDisponivelCenario>>();
			
			for (HorarioDisponivelCenario horario : professor.getValue())
			{
				if (diaMapHorarios.get(horario.getDiaSemana()) == null)
				{
					List<HorarioDisponivelCenario> novoHorario = new ArrayList<HorarioDisponivelCenario>();
					novoHorario.add(horario);
					diaMapHorarios.put(horario.getDiaSemana(), novoHorario);
				}
				else
				{
					diaMapHorarios.get(horario.getDiaSemana()).add(horario);
				}
			}
		}
		return professoresGradeCheia;
	}

	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListBemAlocados( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		final String orderBy = config.getSortField();
		final SortDir sortBy = config.getSortDir();
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Integer> professorToCreditosMap = new HashMap<Professor, Integer>();
		
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professorToCreditosMap.get(atendimento.getProfessor()) == null)
				{
					professorToCreditosMap.put(atendimento.getProfessor(), 1 );
				}
				else
				{
					professorToCreditosMap.put(atendimento.getProfessor(), professorToCreditosMap.get(atendimento.getProfessor())+1);
				}
			}
		}
		
		List<Professor> professoresBemAlocados = new ArrayList<Professor>();
		for (Entry<Professor, Integer> professorTurmas : professorToCreditosMap.entrySet())
		{
			if (professorTurmas.getValue() > 6)
			{
				professoresBemAlocados.add(professorTurmas.getKey());
			}
		}
		
		Collections.sort(professoresBemAlocados, new Comparator<Professor>() {
			@Override
			public int compare(Professor arg1, Professor arg2) {
				return compareProfessor(arg1, arg2, orderBy, sortBy);
			}
		});

		//Map de professores porque eh necessario usar professores com fetch atendimentos para calcular carga horaria e credios
		List<Professor> professoresUtilizados = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf);
		Map<Long, Professor> professorIdMapProfessor = new HashMap<Long, Professor>();
		for (Professor professor : professoresUtilizados)
		{
			professorIdMapProfessor.put(professor.getId(), professor);
		}
		if ( !professoresBemAlocados.isEmpty() )
		{
			for ( int i = config.getOffset(); (i < professoresBemAlocados.size() && i < (config.getOffset() + config.getLimit())); i++ )
			{
				list.add( ConvertBeans.toProfessorDTO( professorIdMapProfessor.get(professoresBemAlocados.get(i).getId()) ) );
			}
		}

		
		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( professoresBemAlocados.size() );

		return result;
	}
	
	public List< Professor > getBuscaListBemAlocadosExport(	Cenario cenario, Long campusDTO, RelatorioProfessorFiltro professorFiltro )
	{
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<String>> professorToTurmasMap = new HashMap<Professor, Set<String>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			String keyTurmas = (atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta().getId() :
				atendimento.getDisciplina().getId()) + "-" +
				atendimento.getTurma();
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professorToTurmasMap.get(atendimento.getProfessor()) == null)
				{
					Set<String> novaTurma = new HashSet<String>();
					novaTurma.add(keyTurmas);
					professorToTurmasMap.put(atendimento.getProfessor(), novaTurma );
				}
				else
				{
					professorToTurmasMap.get(atendimento.getProfessor()).add(keyTurmas);
				}
			}
		}
		
		List<Professor> professoresBemAlocados = new ArrayList<Professor>();
		for (Entry<Professor, Set<String>> professorTurmas : professorToTurmasMap.entrySet())
		{
			if (professorTurmas.getValue().size() >= 4)
			{
				professoresBemAlocados.add(professorTurmas.getKey());
			}
		}
		
		return professoresBemAlocados;
	}
	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListMalAlocados( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		final String orderBy = config.getSortField();
		final SortDir sortBy = config.getSortDir();
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Integer> professorToCreditosMap = new HashMap<Professor, Integer>();
		
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professorToCreditosMap.get(atendimento.getProfessor()) == null)
				{
					professorToCreditosMap.put(atendimento.getProfessor(), 1 );
				}
				else
				{
					professorToCreditosMap.put(atendimento.getProfessor(), professorToCreditosMap.get(atendimento.getProfessor())+1);
				}
			}
		}
		
		List<Professor> professoresMalAlocados = new ArrayList<Professor>();
		for (Entry<Professor, Integer> professorTurmas : professorToCreditosMap.entrySet())
		{
			if (professorTurmas.getValue() < 2)
			{
				professoresMalAlocados.add(professorTurmas.getKey());
			}
		}
		
		Collections.sort(professoresMalAlocados, new Comparator<Professor>() {
			@Override
			public int compare(Professor arg1, Professor arg2) {
				return compareProfessor(arg1, arg2, orderBy, sortBy);
			}
		});

		//Map de professores porque eh necessario usar professores com fetch atendimentos para calcular carga horaria e credios
		List<Professor> professoresUtilizados = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf);
		Map<Long, Professor> professorIdMapProfessor = new HashMap<Long, Professor>();
		for (Professor professor : professoresUtilizados)
		{
			professorIdMapProfessor.put(professor.getId(), professor);
		}
		if ( !professoresMalAlocados.isEmpty() )
		{
			for ( int i = config.getOffset(); (i < professoresMalAlocados.size() && i < (config.getOffset() + config.getLimit())); i++ )
			{
				list.add( ConvertBeans.toProfessorDTO( professorIdMapProfessor.get(professoresMalAlocados.get(i).getId()) ) );
			}
		}

		
		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( professoresMalAlocados.size() );

		return result;
	}
	
	public List< Professor > getBuscaListMalAlocadosExport( Cenario cenario, 
			Long campusDTO, RelatorioProfessorFiltro professorFiltro )
	{
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<String>> professorToTurmasMap = new HashMap<Professor, Set<String>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			String keyTurmas = (atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta().getId() :
				atendimento.getDisciplina().getId()) + "-" +
				atendimento.getTurma();
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professorToTurmasMap.get(atendimento.getProfessor()) == null)
				{
					Set<String> novaTurma = new HashSet<String>();
					novaTurma.add(keyTurmas);
					professorToTurmasMap.put(atendimento.getProfessor(), novaTurma );
				}
				else
				{
					professorToTurmasMap.get(atendimento.getProfessor()).add(keyTurmas);
				}
			}
		}
		
		List<Professor> professoresMalAlocados = new ArrayList<Professor>();
		for (Entry<Professor, Set<String>> professorTurmas : professorToTurmasMap.entrySet())
		{
			if (professorTurmas.getValue().size() == 1)
			{
				professoresMalAlocados.add(professorTurmas.getKey());
			}
		}
		
		return professoresMalAlocados;
	}
	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListComJanelas( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		final String orderBy = config.getSortField();
		final SortDir sortBy = config.getSortDir();
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<HorarioDisponivelCenario>> professorToListHorarioDisponivelCenario = new HashMap<Professor, Set<HorarioDisponivelCenario>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				
				if (professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()) == null)
				{
					Set<HorarioDisponivelCenario> novoHorarioList = new HashSet<HorarioDisponivelCenario>();
					novoHorarioList.add(atendimento.getHorarioDisponivelCenario());
					professorToListHorarioDisponivelCenario.put(atendimento.getProfessor(), novoHorarioList);
				}
				else
				{
					professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()).add(atendimento.getHorarioDisponivelCenario());
				}
			}
		}
		
		List<Professor> professoresComJanelas = new ArrayList<Professor>();
		for (Entry<Professor, Set<HorarioDisponivelCenario>> professor : professorToListHorarioDisponivelCenario.entrySet() )
		{
			Map<Semanas, List<HorarioDisponivelCenario>> diaMapHorarios = new HashMap<Semanas, List<HorarioDisponivelCenario>>();
			
			for (HorarioDisponivelCenario horario : professor.getValue())
			{
				if (diaMapHorarios.get(horario.getDiaSemana()) == null)
				{
					List<HorarioDisponivelCenario> novoHorario = new ArrayList<HorarioDisponivelCenario>();
					novoHorario.add(horario);
					diaMapHorarios.put(horario.getDiaSemana(), novoHorario);
				}
				else
				{
					diaMapHorarios.get(horario.getDiaSemana()).add(horario);
				}
			}
			int janelas = 0;
			for (Entry<Semanas, List<HorarioDisponivelCenario>> horarioSemana : diaMapHorarios.entrySet())
			{
				List<HorarioDisponivelCenario> horarioProfessorOrdenado = horarioSemana.getValue();
				
				Collections.sort(horarioProfessorOrdenado, new Comparator<HorarioDisponivelCenario>() {
					@Override
					public int compare(HorarioDisponivelCenario arg1, HorarioDisponivelCenario arg2) {
						Calendar h1 = Calendar.getInstance();
						h1.setTime(arg1.getHorarioAula().getHorario());
						h1.set(1979,Calendar.NOVEMBER,6);
						
						Calendar h2 = Calendar.getInstance();
						h2.setTime(arg2.getHorarioAula().getHorario());
						h2.set(1979,Calendar.NOVEMBER,6);
						
						
						return h1.compareTo(h2);
					}
				});
				if (!horarioProfessorOrdenado.isEmpty())
				{
					Calendar h1 = Calendar.getInstance();
					h1.setTime(horarioProfessorOrdenado.get(0).getHorarioAula().getHorario());
					for (int i = 1; i < horarioProfessorOrdenado.size(); i++)
					{
						h1.add(Calendar.MINUTE,horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo());
						Calendar h2 = Calendar.getInstance();
						h2.setTime(horarioProfessorOrdenado.get(i).getHorarioAula().getHorario());
						
						if (h2.getTimeInMillis() - h1.getTimeInMillis() > (horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo() * 60000))
						{
							janelas++;
						}
					}
				}
			}
			if (janelas >= 1)
			{
				professoresComJanelas.add(professor.getKey());
			}
		}
		
		Collections.sort(professoresComJanelas, new Comparator<Professor>() {
			@Override
			public int compare(Professor arg1, Professor arg2) {
				return compareProfessor(arg1, arg2, orderBy, sortBy);
			}
		});

		List<Professor> professoresUtilizados = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf);
		Map<Long, Professor> professorIdMapProfessor = new HashMap<Long, Professor>();
		for (Professor professor : professoresUtilizados)
		{
			professorIdMapProfessor.put(professor.getId(), professor);
		}
		if ( !professoresComJanelas.isEmpty() )
		{
			for ( int i = config.getOffset(); (i < professoresComJanelas.size() && i < (config.getOffset() + config.getLimit())); i++ )
			{
				list.add( ConvertBeans.toProfessorDTO( professorIdMapProfessor.get(professoresComJanelas.get(i).getId()) ) );
			}
		}

		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( professoresComJanelas.size() );

		return result;
	}
	
	public List< Professor > getBuscaListComJanelasExport( Cenario cenario, 
			Long campusDTO, RelatorioProfessorFiltro professorFiltro )
	{
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<HorarioDisponivelCenario>> professorToListHorarioDisponivelCenario = new HashMap<Professor, Set<HorarioDisponivelCenario>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				
				if (professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()) == null)
				{
					Set<HorarioDisponivelCenario> novoHorarioList = new HashSet<HorarioDisponivelCenario>();
					novoHorarioList.add(atendimento.getHorarioDisponivelCenario());
					professorToListHorarioDisponivelCenario.put(atendimento.getProfessor(), novoHorarioList);
				}
				else
				{
					professorToListHorarioDisponivelCenario.get(atendimento.getProfessor()).add(atendimento.getHorarioDisponivelCenario());
				}
			}
		}
		
		List<Professor> professoresComJanelas = new ArrayList<Professor>();
		for (Entry<Professor, Set<HorarioDisponivelCenario>> professor : professorToListHorarioDisponivelCenario.entrySet() )
		{
			Map<Semanas, List<HorarioDisponivelCenario>> diaMapHorarios = new HashMap<Semanas, List<HorarioDisponivelCenario>>();
			
			for (HorarioDisponivelCenario horario : professor.getValue())
			{
				if (diaMapHorarios.get(horario.getDiaSemana()) == null)
				{
					List<HorarioDisponivelCenario> novoHorario = new ArrayList<HorarioDisponivelCenario>();
					novoHorario.add(horario);
					diaMapHorarios.put(horario.getDiaSemana(), novoHorario);
				}
				else
				{
					diaMapHorarios.get(horario.getDiaSemana()).add(horario);
				}
			}
			int janelas = 0;
			for (Entry<Semanas, List<HorarioDisponivelCenario>> horarioSemana : diaMapHorarios.entrySet())
			{
				List<HorarioDisponivelCenario> horarioProfessorOrdenado = horarioSemana.getValue();
				
				Collections.sort(horarioProfessorOrdenado, new Comparator<HorarioDisponivelCenario>() {
					@Override
					public int compare(HorarioDisponivelCenario arg1, HorarioDisponivelCenario arg2) {
						Calendar h1 = Calendar.getInstance();
						h1.setTime(arg1.getHorarioAula().getHorario());
						h1.set(1979,Calendar.NOVEMBER,6);
						
						Calendar h2 = Calendar.getInstance();
						h2.setTime(arg2.getHorarioAula().getHorario());
						h2.set(1979,Calendar.NOVEMBER,6);
						
						
						return h1.compareTo(h2);
					}
				});
				if (!horarioProfessorOrdenado.isEmpty())
				{
					Calendar h1 = Calendar.getInstance();
					h1.setTime(horarioProfessorOrdenado.get(0).getHorarioAula().getHorario());
					for (int i = 1; i < horarioProfessorOrdenado.size(); i++)
					{
						h1.add(Calendar.MINUTE,horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo());
						Calendar h2 = Calendar.getInstance();
						h2.setTime(horarioProfessorOrdenado.get(i).getHorarioAula().getHorario());
						
						if (h2.getTimeInMillis() - h1.getTimeInMillis() > (horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo() * 60000))
						{
							janelas++;
						}
					}
				}
			}
			if (janelas >= 1)
			{
				professoresComJanelas.add(professor.getKey());
			}
		}
		
		return professoresComJanelas;
	}
	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListDeslocamentoUnidades( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		final String orderBy = config.getSortField();
		final SortDir sortBy = config.getSortDir();
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<Unidade>> professoresToUnidadesMap = new HashMap<Professor, Set<Unidade>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professoresToUnidadesMap.get(atendimento.getProfessor()) == null)
				{
					Set<Unidade> novaUnidade = new HashSet<Unidade>();
					novaUnidade.add(atendimento.getSala().getUnidade());
					professoresToUnidadesMap.put(atendimento.getProfessor(), novaUnidade);
				}
				else
				{
					professoresToUnidadesMap.get(atendimento.getProfessor()).add(atendimento.getSala().getUnidade());
				}
			}
		}
		
		List<Professor> professoresComDeslocamentosUnidades = new ArrayList<Professor>();
		for (Entry<Professor, Set<Unidade>> professorUnidades : professoresToUnidadesMap.entrySet())
		{
			Set<Campus> campi = new HashSet<Campus>();
			if (professoresToUnidadesMap.get(professorUnidades.getKey()).size() > 1)
			{
				professoresComDeslocamentosUnidades.add(professorUnidades.getKey());
				for (Unidade unidade : professorUnidades.getValue())
				{
					campi.add(unidade.getCampus());
				}
			}
		}
		
		Collections.sort(professoresComDeslocamentosUnidades, new Comparator<Professor>() {
			@Override
			public int compare(Professor arg1, Professor arg2) {
				return compareProfessor(arg1, arg2, orderBy, sortBy);
			}
		});

		//Map de professores porque eh necessario usar professores com fetch atendimentos para calcular carga horaria e credios
		List<Professor> professoresUtilizados = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf);
		Map<Long, Professor> professorIdMapProfessor = new HashMap<Long, Professor>();
		for (Professor professor : professoresUtilizados)
		{
			professorIdMapProfessor.put(professor.getId(), professor);
		}
		if ( !professoresComDeslocamentosUnidades.isEmpty() )
		{
			for ( int i = config.getOffset(); (i < professoresComDeslocamentosUnidades.size() && i < (config.getOffset() + config.getLimit())); i++ )
			{
				list.add( ConvertBeans.toProfessorDTO( professorIdMapProfessor.get(professoresComDeslocamentosUnidades.get(i).getId()) ) );
			}
		}

		
		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( professoresComDeslocamentosUnidades.size() );

		return result;
	}
	
	public List< Professor > getBuscaListDeslocamentoUnidadesExport( Cenario cenario, 
			Long campusDTO, RelatorioProfessorFiltro professorFiltro )
	{
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<Unidade>> professoresToUnidadesMap = new HashMap<Professor, Set<Unidade>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professoresToUnidadesMap.get(atendimento.getProfessor()) == null)
				{
					Set<Unidade> novaUnidade = new HashSet<Unidade>();
					novaUnidade.add(atendimento.getSala().getUnidade());
					professoresToUnidadesMap.put(atendimento.getProfessor(), novaUnidade);
				}
				else
				{
					professoresToUnidadesMap.get(atendimento.getProfessor()).add(atendimento.getSala().getUnidade());
				}
			}
		}
		
		List<Professor> professoresComDeslocamentosUnidades = new ArrayList<Professor>();
		for (Entry<Professor, Set<Unidade>> professorUnidades : professoresToUnidadesMap.entrySet())
		{
			Set<Campus> campi = new HashSet<Campus>();
			if (professoresToUnidadesMap.get(professorUnidades.getKey()).size() > 1)
			{
				professoresComDeslocamentosUnidades.add(professorUnidades.getKey());
				for (Unidade unidade : professorUnidades.getValue())
				{
					campi.add(unidade.getCampus());
				}
			}
		}
		
		return professoresComDeslocamentosUnidades;
	}
	
	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaListDeslocamentoCampi( CenarioDTO cenarioDTO, 
			String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		final String orderBy = config.getSortField();
		final SortDir sortBy = config.getSortDir();
		
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<Unidade>> professoresToUnidadesMap = new HashMap<Professor, Set<Unidade>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professoresToUnidadesMap.get(atendimento.getProfessor()) == null)
				{
					Set<Unidade> novaUnidade = new HashSet<Unidade>();
					novaUnidade.add(atendimento.getSala().getUnidade());
					professoresToUnidadesMap.put(atendimento.getProfessor(), novaUnidade);
				}
				else
				{
					professoresToUnidadesMap.get(atendimento.getProfessor()).add(atendimento.getSala().getUnidade());
				}
			}
		}
		
		List<Professor> professoresComDeslocamentosCampi = new ArrayList<Professor>();
		for (Entry<Professor, Set<Unidade>> professorUnidades : professoresToUnidadesMap.entrySet())
		{
			Set<Campus> campi = new HashSet<Campus>();
			if (campi.size() > 1)
			{
				professoresComDeslocamentosCampi.add(professorUnidades.getKey());
			}
		}
		
		Collections.sort(professoresComDeslocamentosCampi, new Comparator<Professor>() {
			@Override
			public int compare(Professor arg1, Professor arg2) {
				return compareProfessor(arg1, arg2, orderBy, sortBy);
			}
		});

		//Map de professores porque eh necessario usar professores com fetch atendimentos para calcular carga horaria e credios
		List<Professor> professoresUtilizados = AtendimentoOperacional.findProfessoresUtilizados(getInstituicaoEnsinoUser(), cenario, campus, curso, turno, titulacao, areaTitulacao, tipoContrato, cpf);
		Map<Long, Professor> professorIdMapProfessor = new HashMap<Long, Professor>();
		for (Professor professor : professoresUtilizados)
		{
			professorIdMapProfessor.put(professor.getId(), professor);
		}
		if ( !professoresComDeslocamentosCampi.isEmpty() )
		{
			for ( int i = config.getOffset(); (i < professoresComDeslocamentosCampi.size() && i < (config.getOffset() + config.getLimit())); i++ )
			{
				list.add( ConvertBeans.toProfessorDTO( professorIdMapProfessor.get(professoresComDeslocamentosCampi.get(i).getId()) ) );
			}
		}

		
		BasePagingLoadResult< ProfessorDTO > result
			= new BasePagingLoadResult< ProfessorDTO >( list );

		result.setOffset( config.getOffset() );

		result.setTotalLength( professoresComDeslocamentosCampi.size() );

		return result;
	}

	public List< Professor > getBuscaListDeslocamentoCampiExport( Cenario cenario, 
			Long campusDTO, RelatorioProfessorFiltro professorFiltro )
	{
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		Curso curso = professorFiltro.getCurso() == null ? null :
			Curso.find(professorFiltro.getCurso().getId(), getInstituicaoEnsinoUser());
		Turno turno = professorFiltro.getTurno() == null ? null :
			Turno.find(professorFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		Titulacao titulacao = professorFiltro.getTitulacao() == null ? null :
			Titulacao.find(professorFiltro.getTitulacao().getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = professorFiltro.getAreaTitulacao() == null ? null :
			AreaTitulacao.find(professorFiltro.getAreaTitulacao().getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = professorFiltro.getTipoContrato() == null ? null :
			TipoContrato.find(professorFiltro.getTipoContrato().getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, getInstituicaoEnsinoUser(), curso,
				turno, titulacao, areaTitulacao, tipoContrato);
		
		Map<Professor, Set<Unidade>> professoresToUnidadesMap = new HashMap<Professor, Set<Unidade>>();
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			if (atendimento.getProfessor().getCpf() != null)
			{
				if (professoresToUnidadesMap.get(atendimento.getProfessor()) == null)
				{
					Set<Unidade> novaUnidade = new HashSet<Unidade>();
					novaUnidade.add(atendimento.getSala().getUnidade());
					professoresToUnidadesMap.put(atendimento.getProfessor(), novaUnidade);
				}
				else
				{
					professoresToUnidadesMap.get(atendimento.getProfessor()).add(atendimento.getSala().getUnidade());
				}
			}
		}
		
		List<Professor> professoresComDeslocamentosCampi = new ArrayList<Professor>();
		for (Entry<Professor, Set<Unidade>> professorUnidades : professoresToUnidadesMap.entrySet())
		{
			Set<Campus> campi = new HashSet<Campus>();
			if (campi.size() > 1)
			{
				professoresComDeslocamentosCampi.add(professorUnidades.getKey());
			}
		}
		
		return professoresComDeslocamentosCampi;
	}
	
	private int compareProfessor(Professor arg1, Professor arg2,
			String orderBy, SortDir sortBy) {
		
		if (orderBy != null)
		{
			if (orderBy.equals(ProfessorDTO.PROPERTY_CPF))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getCpf().compareTo(arg1.getCpf());
				}
				return arg1.getCpf().compareTo(arg2.getCpf());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_NOME))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getNome().compareTo(arg1.getNome());
				}
				return arg1.getNome().compareTo(arg2.getNome());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_CARGA_HORARIA_MIN))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getCargaHorariaMin().compareTo(arg1.getCargaHorariaMin());
				}
				return arg1.getCargaHorariaMin().compareTo(arg2.getCargaHorariaMin());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_CARGA_HORARIA_MAX))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getCargaHorariaMax().compareTo(arg1.getCargaHorariaMax());
				}
				return arg1.getCargaHorariaMax().compareTo(arg2.getCargaHorariaMax());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_TITULACAO_STRING))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getTitulacao().getNome().compareTo(arg1.getTitulacao().getNome());
				}
				return arg1.getTitulacao().getNome().compareTo(arg2.getTitulacao().getNome());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_AREA_TITULACAO_STRING))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getAreaTitulacao().getCodigo().compareTo(arg1.getAreaTitulacao().getCodigo());
				}
				return arg1.getAreaTitulacao().getCodigo().compareTo(arg2.getAreaTitulacao().getCodigo());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_CREDITO_ANTERIOR))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getCreditoAnterior().compareTo(arg1.getCreditoAnterior());
				}
				return arg1.getCreditoAnterior().compareTo(arg2.getCreditoAnterior());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_VALOR_CREDITO))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getValorCredito().compareTo(arg1.getValorCredito());
				}
				return arg1.getValorCredito().compareTo(arg2.getValorCredito());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_MAX_DIAS_SEMANA))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getMaxDiasSemana().compareTo(arg1.getMaxDiasSemana());
				}
				return arg1.getMaxDiasSemana().compareTo(arg2.getMaxDiasSemana());
			}
			else if (orderBy.equals(ProfessorDTO.PROPERTY_MIN_CREDITOS_DIA))
			{
				if (sortBy.equals(SortDir.DESC))
				{
					return arg2.getMinCreditosDia().compareTo(arg1.getMinCreditosDia());
				}
				return arg1.getMinCreditosDia().compareTo(arg2.getMinCreditosDia());
			}
			else
			{
				return arg1.getId().compareTo(arg2.getId());
			}
		}
		else
		{
			return arg1.getId().compareTo(arg2.getId());
		}
	}
}
