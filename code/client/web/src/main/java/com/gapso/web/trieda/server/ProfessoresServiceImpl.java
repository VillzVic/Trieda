package com.gapso.web.trieda.server;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.services.ProfessoresService;
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
	public ListLoadResult< ProfessorDTO > getList()
	{
		List< ProfessorDTO > list = new ArrayList< ProfessorDTO >();
		List< Professor > listProfessores
			= Professor.findAll( getInstituicaoEnsinoUser() );

		Collections.sort( listProfessores );

		for ( Professor professor : listProfessores )
		{
			list.add( ConvertBeans.toProfessorDTO( professor ) );
		}

		return new BaseListLoadResult< ProfessorDTO >( list );
	}

	@Override
	public PagingLoadResult< ProfessorDTO > getBuscaList( String cpf,
		TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO,
		AreaTitulacaoDTO areaTitulacaoDTO, PagingLoadConfig config )
	{
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
			cpf, tipoContrato, titulacao, areaTitulacao,
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
			getInstituicaoEnsinoUser(), cpf,
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
	public ListLoadResult< TipoContratoDTO > getTiposContratoAll()
	{
		List< TipoContratoDTO > listDTO
			= new ArrayList< TipoContratoDTO >();

		List< TipoContrato > listTiposContrato
			= TipoContrato.findAll( getInstituicaoEnsinoUser() );

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
	public ListLoadResult< TitulacaoDTO > getTitulacoesAll()
	{
		List< TitulacaoDTO > listDTO = new ArrayList< TitulacaoDTO >();
		List< Titulacao > list = Titulacao.findAll( this.getInstituicaoEnsinoUser() );

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
	
			Professor professor = ConvertBeans.toProfessor( professorDTO );
	
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
	public List< ProfessorDTO > getProfessoresNaoEmCampus( CampusDTO campusDTO )
	{
		onlyAdministrador();

		Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );
		Set< Professor > listAssociados = campus.getProfessores();
		List< Professor > list = Professor.findAll( getInstituicaoEnsinoUser() );
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
	public PagingLoadResult< ProfessorCampusDTO > getProfessorCampusByCurrentProfessor()
	{
		onlyProfessor();

		List< ProfessorCampusDTO > list
			= ConvertBeans.toProfessorCampusDTO( getUsuario().getProfessor() );

		return new BasePagingLoadResult< ProfessorCampusDTO >( list );
	}

	@Override
	public PagingLoadResult< ProfessorCampusDTO > getProfessorCampusList(
			CampusDTO campusDTO, ProfessorDTO professorDTO )
	{
		onlyAdministrador();

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
				= Campus.findAll( this.getInstituicaoEnsinoUser() );

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

		return new BasePagingLoadResult< ProfessorCampusDTO >( list );
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
}
