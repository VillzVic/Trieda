package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import org.springframework.format.number.CurrencyFormatter;
import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Incompatibilidade;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.services.DisciplinasService;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.dev.util.Pair;

@Transactional
public class DisciplinasServiceImpl
	extends RemoteService
	implements DisciplinasService
{
	private static final long serialVersionUID = -4850774141421616870L;
	
	/**
	 * @see com.gapso.web.trieda.shared.services.DisciplinasService#getHorariosDisponiveis(com.gapso.web.trieda.shared.dtos.DisciplinaDTO)
	 */
	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO) {
		List<HorarioDisponivelCenarioDTO> horariosDisponiveisDTO = new ArrayList<HorarioDisponivelCenarioDTO>();
		
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId(),getInstituicaoEnsinoUser());
		if (disciplina != null) {
			// obtém todos os horários disponíveis da disciplina
			List<HorarioDisponivelCenario> horariosDisponiveisBD = disciplina.getHorarios(getInstituicaoEnsinoUser());

			// obtém as semanas letivas associadas com a disciplina em questão
			Set<SemanaLetiva> semanasLetivas = disciplina.getSemanasLetivas();
			// se necessário, filtra os horários discponíveis
			if (!semanasLetivas.isEmpty()) {
				// filtra os horários disponíveis da disciplina de acordo com as semanas letivas associadas com a disciplina
				List<HorarioDisponivelCenario> horariosDisponiveisBDFiltrados = new ArrayList<HorarioDisponivelCenario>();
				for (HorarioDisponivelCenario horario : horariosDisponiveisBD) {
					if (semanasLetivas.contains(horario.getHorarioAula().getSemanaLetiva())) {
						horariosDisponiveisBDFiltrados.add(horario);
					}
				}
				
				horariosDisponiveisBD.clear();
				horariosDisponiveisBD.addAll(horariosDisponiveisBDFiltrados);
			}
			
			// transforma os horários disponíveis em DTOs
			horariosDisponiveisDTO.addAll(ConvertBeans.toHorarioDisponivelCenarioDTO(horariosDisponiveisBD));	
		}

		return horariosDisponiveisDTO;
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.DisciplinasService#getSemanasLetivas(com.gapso.web.trieda.shared.dtos.DisciplinaDTO)
	 */
	@Override
	public List<SemanaLetivaDTO> getSemanasLetivas(DisciplinaDTO disciplinaDTO) {
		List<SemanaLetivaDTO> semanasLetivasDTO = new ArrayList<SemanaLetivaDTO>();
		
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId(),getInstituicaoEnsinoUser());
		if (disciplina != null) {
			for (SemanaLetiva semanaLetiva : disciplina.getSemanasLetivas()) {
				semanasLetivasDTO.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
			}
		}
		
		return semanasLetivasDTO;
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.DisciplinasService#associarDisciplinasSemLaboratorioATodosLaboratorios()
	 */
	@Override
	@Transactional
	public void associarDisciplinasSemLaboratorioATodosLaboratorios() throws TriedaException {
		try {
			// [CampusId -> Lista de Laboratorios]
			Map<Long,List<Sala>> campusIdToLaboratoriosMap = new HashMap<Long,List<Sala>>();
			List<CurriculoDisciplina> curriculosDisciplinasAtualizados = new ArrayList<CurriculoDisciplina>();
			
			// 
			List<Oferta> ofertas = Oferta.findAll(getInstituicaoEnsinoUser());
			for (Oferta oferta : ofertas) {
				// obtém os laboratórios do campus associado com a oferta
				List<Sala> laboratorios = campusIdToLaboratoriosMap.get(oferta.getCampus().getId());
				if (laboratorios == null) {
					laboratorios = new ArrayList<Sala>();
					laboratorios.addAll(oferta.getCampus().getLaboratorios());
					campusIdToLaboratoriosMap.put(oferta.getCampus().getId(),laboratorios);
				}
				
				// obtém os CurriculoDisciplina que serão atualizados, isto é, aqueles cuja disciplina exige laboratório, porém,
				// ainda não estão associadas a nenhum laboratório
				Set<CurriculoDisciplina> curriculosDisciplinas = oferta.getCurriculo().getDisciplinas();
				List<CurriculoDisciplina> curriculosDisciplinasASeremProcessados = new ArrayList<CurriculoDisciplina>();
				for (CurriculoDisciplina curriculoDisciplina : curriculosDisciplinas) {
					// verifica se a disciplina do curriculo exige laboratorio
					if (curriculoDisciplina.getDisciplina().getLaboratorio()) {
						// verifica se há associação com algum laboratorio
						boolean estaAssociadoComAlgumLaboratorio = false;
						for (Sala sala : curriculoDisciplina.getSalas()) {
							if (sala.isLaboratorio() && sala.getUnidade().getCampus().equals(oferta.getCampus())) {
								estaAssociadoComAlgumLaboratorio = true;
								break;
							}
						}
						 
						if (!estaAssociadoComAlgumLaboratorio) {
							curriculosDisciplinasASeremProcessados.add(curriculoDisciplina);
						}
					}
				}
				
				// atualiza, em memória, os curriculosDisplinas com os laboratórios
				for (CurriculoDisciplina curriculoDisciplina : curriculosDisciplinasASeremProcessados) {
					curriculoDisciplina.getSalas().addAll(laboratorios);
					curriculosDisciplinasAtualizados.add(curriculoDisciplina);
				}
			}
			
			// atualiza o BD com os curriculosDisplinas atualizados
			System.out.println("Começou...");
			for (CurriculoDisciplina curriculoDisciplina : curriculosDisciplinasAtualizados) {
				curriculoDisciplina.mergeWithoutFlush();
			}
			System.out.println("Terminou...");
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public DisciplinaDTO getDisciplina( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		Disciplina disciplina = Disciplina.find(
			id, getInstituicaoEnsinoUser() );

		if ( disciplina == null )
		{
			return null;
		}

		return ConvertBeans.toDisciplinaDTO( disciplina );
	}

	@Override
	public void saveHorariosDisponiveis(
		DisciplinaDTO disciplinaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO )
	{
		List< HorarioDisponivelCenario > listSelecionados
			= ConvertBeans.toHorarioDisponivelCenario( listDTO );

		Disciplina disciplina = Disciplina.find(
			disciplinaDTO.getId(), getInstituicaoEnsinoUser() );

		if ( disciplina == null )
		{
			return;
		}

		List< HorarioDisponivelCenario > adicionarList
			= new ArrayList< HorarioDisponivelCenario >( listSelecionados );

		List< HorarioDisponivelCenario > removerList
			= new ArrayList< HorarioDisponivelCenario >();

		List< HorarioDisponivelCenario > listTemp
			= disciplina.getHorarios( getInstituicaoEnsinoUser() );

		adicionarList.removeAll( listTemp );
		removerList.addAll(	listTemp );

		removerList.removeAll( listSelecionados );

		for ( HorarioDisponivelCenario o : removerList )
		{
			o.getDisciplinas().remove( disciplina );
			o.merge();
		}

		for ( HorarioDisponivelCenario o : adicionarList )
		{ 
			o.getDisciplinas().add( disciplina );
			o.merge();
		}
	}

	@Override
	public ListLoadResult< DisciplinaDTO > getListByCursos(
		List< CursoDTO > cursosDTO )
	{
		List< DisciplinaDTO > list = new ArrayList< DisciplinaDTO >();
		List< Curso > cursos = new ArrayList< Curso >();

		for ( CursoDTO cursoDTO : cursosDTO )
		{
			cursos.add( ConvertBeans.toCurso( cursoDTO ) );
		}

		List< Disciplina > disciplinas = Disciplina.findByCursos(
			getInstituicaoEnsinoUser(), cursos );

		for ( Disciplina disciplina : disciplinas )
		{
			list.add( ConvertBeans.toDisciplinaDTO( disciplina ) );
		}

		return new BaseListLoadResult< DisciplinaDTO >( list );
	}

	@Override
	public ListLoadResult< DisciplinaDTO > getListByCurriculoIdAndName(
		Long curriculoId, String nome)
	{
		List< DisciplinaDTO > list = new ArrayList< DisciplinaDTO >();

		List< Disciplina > disciplinas = Disciplina.findByCurriculoIdAndName(
			getInstituicaoEnsinoUser(), nome, curriculoId);

		for ( Disciplina disciplina : disciplinas )
		{
			list.add( ConvertBeans.toDisciplinaDTO( disciplina ) );
		}

		return new BaseListLoadResult< DisciplinaDTO >( list );
	}
	
	@Override
	public ListLoadResult< DisciplinaDTO > getListByCursoAndName(
		List< CursoDTO > cursosDTO, String nome)
	{
		List< DisciplinaDTO > list = new ArrayList< DisciplinaDTO >();
		List< Curso > cursos = new ArrayList< Curso >();

		for ( CursoDTO cursoDTO : cursosDTO )
		{
			cursos.add( ConvertBeans.toCurso( cursoDTO ) );
		}

		List< Disciplina > disciplinas = Disciplina.findByCursoAndName(
			getInstituicaoEnsinoUser(), nome, cursos );

		for ( Disciplina disciplina : disciplinas )
		{
			list.add( ConvertBeans.toDisciplinaDTO( disciplina ) );
		}

		return new BaseListLoadResult< DisciplinaDTO >( list );
	}
	
	@Override
	public ListLoadResult< DisciplinaDTO > getListByCurriculo(long curriculoId) {
		Curriculo curriculo = Curriculo.find(curriculoId,getInstituicaoEnsinoUser());
		
		Set<Disciplina> disciplinas = new HashSet<Disciplina>();
		for (CurriculoDisciplina currDisciplina : curriculo.getDisciplinas()) {
			disciplinas.add(currDisciplina.getDisciplina());
		}

		List<DisciplinaDTO> list = new ArrayList<DisciplinaDTO>();
		for (Disciplina disciplina : disciplinas) {
			list.add(ConvertBeans.toDisciplinaDTO(disciplina));
		}

		return new BaseListLoadResult<DisciplinaDTO>(list);
	}

	public ListLoadResult< DisciplinaDTO > getListBySalas(
		List< SalaDTO > salasDTO )
	{
		List< DisciplinaDTO > list
			= new ArrayList< DisciplinaDTO >();

		List< Sala > salas = new ArrayList< Sala >();

		for ( SalaDTO salaDTO : salasDTO )
		{
			salas.add( ConvertBeans.toSala( salaDTO ) );
		}

		List< Disciplina > disciplinas
			= Disciplina.findBySalas( getInstituicaoEnsinoUser(), salas );

		for ( Disciplina disciplina : disciplinas )
		{
			list.add( ConvertBeans.toDisciplinaDTO( disciplina ) );
		}

		return new BaseListLoadResult< DisciplinaDTO >( list );
	}

	@Override
	public ListLoadResult< DisciplinaDTO > getList(
		BasePagingLoadConfig loadConfig )
	{
		return getBuscaList( null, loadConfig.get(
			"query" ).toString(), null, loadConfig );
	}

	@Override
	public PagingLoadResult< DisciplinaDTO > getBuscaList(
		String nome, String codigo,
		TipoDisciplinaDTO tipoDisciplinaDTO,
		PagingLoadConfig config )
	{
		List< DisciplinaDTO > list = new ArrayList< DisciplinaDTO >();
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

		TipoDisciplina tipoDisciplina = null;

		if ( tipoDisciplinaDTO != null )
		{
			tipoDisciplina = ConvertBeans.toTipoDisciplina( tipoDisciplinaDTO );
		}

		List< Disciplina > disciplinas = Disciplina.findBy(
			getInstituicaoEnsinoUser(), codigo, nome, tipoDisciplina,
			config.getOffset(), config.getLimit(), orderBy );

		for ( Disciplina disciplina : disciplinas )
		{
			DisciplinaDTO disciplinaDTO
				= ConvertBeans.toDisciplinaDTO( disciplina );

			list.add( disciplinaDTO );
		}

		BasePagingLoadResult< DisciplinaDTO > result
			= new BasePagingLoadResult< DisciplinaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Disciplina.count(
			getInstituicaoEnsinoUser(), codigo, nome, tipoDisciplina ) );

		return result;
	}

	@Override
	public void save( DisciplinaDTO disciplinaDTO )
	{
		Disciplina disciplina = ConvertBeans.toDisciplina( disciplinaDTO );

		if ( disciplina.getId() != null && disciplina.getId() > 0 )
		{
			disciplina.merge();
		}
		else
		{
			disciplina.persistAndPreencheHorarios();

			List< SemanaLetiva > semanasLetivas
				= SemanaLetiva.findAll( getInstituicaoEnsinoUser() );

			Set< HorarioAula > horariosAula = new HashSet< HorarioAula >();

			for ( SemanaLetiva semanaLetiva : semanasLetivas )
			{
				horariosAula.addAll( semanaLetiva.getHorariosAula() );
			}

			for ( HorarioAula horarioAula : horariosAula )
			{
				Set< HorarioDisponivelCenario> horariosDisponiveis
					= horarioAula.getHorariosDisponiveisCenario();

				for ( HorarioDisponivelCenario horarioDisponivel : horariosDisponiveis )
				{
					horarioDisponivel.getDisciplinas().add( disciplina );
					horarioDisponivel.merge();
				}
			}
		}
	}

	@Override
	public void remove(
		List< DisciplinaDTO > disciplinaDTOList )
	{
		for ( DisciplinaDTO disciplinaDTO : disciplinaDTOList )
		{
			Disciplina disciplina = Disciplina.find(
				disciplinaDTO.getId(), getInstituicaoEnsinoUser() );

			if ( disciplina != null )
			{
				disciplina.remove();
			}
		}
	}

	@Override
	public DivisaoCreditoDTO getDivisaoCredito(
		DisciplinaDTO disciplinaDTO )
	{
		Disciplina disciplina = Disciplina.find(
			disciplinaDTO.getId(), getInstituicaoEnsinoUser() );

		DivisaoCredito dc = disciplina.getDivisaoCreditos();

		if ( dc == null )
		{
			return new DivisaoCreditoDTO();
		}

		return ConvertBeans.toDivisaoCreditoDTO( dc );
	}

	@Override
	public void removeDivisaoCredito( DisciplinaDTO disciplinaDTO )
	{
		Disciplina disciplina = Disciplina.find(
			disciplinaDTO.getId(), getInstituicaoEnsinoUser() );

		if ( disciplina == null )
		{
			return;
		}

		DivisaoCredito divisaoCredito
			= disciplina.getDivisaoCreditos();

		if ( divisaoCredito == null )
		{
			return;
		}

		disciplina.setDivisaoCreditos( null );
		disciplina.merge();
		divisaoCredito.remove();
	}

	@Override
	public void salvarDivisaoCredito(
		DisciplinaDTO disciplinaDTO,
		DivisaoCreditoDTO divisaoCreditoDTO )
	{
		Disciplina disciplina = Disciplina.find(
			disciplinaDTO.getId(), getInstituicaoEnsinoUser() );

		DivisaoCredito divisaoCredito
			= disciplina.getDivisaoCreditos();

		if ( divisaoCredito != null )
		{
			int d1 = divisaoCreditoDTO.getDia1();
			int d2 = divisaoCreditoDTO.getDia2();
			int d3 = divisaoCreditoDTO.getDia3();
			int d4 = divisaoCreditoDTO.getDia4();
			int d5 = divisaoCreditoDTO.getDia5();
			int d6 = divisaoCreditoDTO.getDia6();
			int d7 = divisaoCreditoDTO.getDia7();

			divisaoCredito.setDia1( d1 );
			divisaoCredito.setDia2( d2 );
			divisaoCredito.setDia3( d3 );
			divisaoCredito.setDia4( d4 );
			divisaoCredito.setDia5( d5 );
			divisaoCredito.setDia6( d6 );
			divisaoCredito.setDia7( d7 );

			divisaoCredito.setCreditos( d1 + d2 + d3 + d4 + d5 + d6 + d7 );
			divisaoCredito.merge();
		}
		else
		{
			divisaoCreditoDTO.setDisciplinaId( disciplinaDTO.getId() );
			divisaoCredito = ConvertBeans.toDivisaoCredito( divisaoCreditoDTO );

			divisaoCredito.persist();
			DivisaoCredito.entityManager().refresh( divisaoCredito );

			disciplina.setDivisaoCreditos( divisaoCredito );
			disciplina.merge();
		}
	}

	@Override
	public TipoDisciplinaDTO getTipoDisciplina( Long id )
	{
		return ConvertBeans.toTipoDisciplinaDTO(
			TipoDisciplina.find( id, getUsuario().getInstituicaoEnsino() ) );
	}

	@Override
	public ListLoadResult< TipoDisciplinaDTO > getTipoDisciplinaList()
	{
		InstituicaoEnsino instituicaoEnsino
			= getUsuario().getInstituicaoEnsino(); 

		List< TipoDisciplina > listTiposDisciplinas
			= TipoDisciplina.findAll( instituicaoEnsino );

		if ( listTiposDisciplinas.size() == 0 )
		{
			TipoDisciplina tipo1 = new TipoDisciplina();
			tipo1.setNome( "Presencial" );
			tipo1.setInstituicaoEnsino( instituicaoEnsino );
			tipo1.persist();

			TipoDisciplina tipo2 = new TipoDisciplina();
			tipo2.setNome( "Telepresencial" );
			tipo2.setInstituicaoEnsino( instituicaoEnsino );
			tipo2.persist();

			TipoDisciplina tipo3 = new TipoDisciplina();
			tipo3.setNome( "Online" );
			tipo3.setInstituicaoEnsino( instituicaoEnsino );
			tipo3.persist();

			listTiposDisciplinas
				= TipoDisciplina.findAll( instituicaoEnsino );
		}

		List< TipoDisciplinaDTO > listDTO
			= new ArrayList< TipoDisciplinaDTO >();

		for ( TipoDisciplina tipo : listTiposDisciplinas )
		{
			listDTO.add( ConvertBeans.toTipoDisciplinaDTO( tipo ) );
		}

		return new BaseListLoadResult< TipoDisciplinaDTO >( listDTO );
	}

	@Override
	@Transactional
	public List< TreeNodeDTO > getFolderChildren(
		TreeNodeDTO currentNode )
	{
		List< TreeNodeDTO > currentNodeChildren
			= new ArrayList< TreeNodeDTO >();

		if ( currentNode != null )
		{
			AbstractDTO< ? > contentNode = currentNode.getContent();

			if ( contentNode != null )
			{
				if ( contentNode instanceof OfertaDTO )
				{
					OfertaDTO ofertaDTOContentNode = (OfertaDTO) contentNode;
					Curriculo curriculo = Curriculo.find(
						ofertaDTOContentNode.getMatrizCurricularId(), getInstituicaoEnsinoUser() );

					Set< Integer > periodos = new HashSet< Integer >();
					for ( CurriculoDisciplina cd : curriculo.getDisciplinas() )
					{
						if ( periodos.add( cd.getPeriodo() ) )
						{
							CurriculoDisciplinaDTO curriculoDisciplinaDTO
								= ConvertBeans.toCurriculoDisciplinaDTO( cd );

							TreeNodeDTO newChildNode = new TreeNodeDTO(
								curriculoDisciplinaDTO, currentNode );

							newChildNode.setText( "Periodo " +
								curriculoDisciplinaDTO.getPeriodo().toString() );

							currentNodeChildren.add( newChildNode );
						}
					}
				}
				else if ( contentNode instanceof CurriculoDisciplinaDTO )
				{
					CurriculoDisciplinaDTO curricDiscDTOContentNode
						= (CurriculoDisciplinaDTO) contentNode;

					Curriculo curriculo = Curriculo.find(
						curricDiscDTOContentNode.getCurriculoId(), getInstituicaoEnsinoUser() );

					for ( CurriculoDisciplina cd : curriculo.getDisciplinas() )
					{
						if ( cd.getPeriodo().equals(
								curricDiscDTOContentNode.getPeriodo() ) )
						{
							CurriculoDisciplinaDTO curriculoDisciplinaDTO
								= ConvertBeans.toCurriculoDisciplinaDTO( cd );

							TreeNodeDTO newChildNode = new TreeNodeDTO(
								curriculoDisciplinaDTO, currentNode );

							newChildNode.setLeaf( true );
							currentNodeChildren.add( newChildNode );
						}
					}
				}
			}
		}

		Collections.sort( currentNodeChildren );

		return currentNodeChildren;
	}

	public List< TreeNodeDTO > getOfertasByTreeSalas(
		TreeNodeDTO salaTreeNodeDTO )
	{
		List< TreeNodeDTO > nodeChildrenList = new ArrayList< TreeNodeDTO >();

		SalaDTO salaDTO = (SalaDTO) salaTreeNodeDTO.getContent();
		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );
		List< Oferta > ofertas = Oferta.findAllBy(
			getInstituicaoEnsinoUser(), sala);

		for ( Oferta oferta : ofertas )
		{
			OfertaDTO ofertaDTO = ConvertBeans.toOfertaDTO( oferta );
			TreeNodeDTO nodeDTO = new TreeNodeDTO( ofertaDTO, salaTreeNodeDTO );
			nodeDTO.setEmpty( false );
			nodeChildrenList.add( nodeDTO );
		}

		Collections.sort( nodeChildrenList );

		return nodeChildrenList;
	}

	public List< TreeNodeDTO > getPeriodosByTreeSalas(
		TreeNodeDTO salaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO )
	{
		List< TreeNodeDTO > nodeChildrenList = new ArrayList< TreeNodeDTO >();

		SalaDTO salaDTO = (SalaDTO) salaTreeNodeDTO.getContent();
		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );
		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find( ofertaDTO.getId(), getInstituicaoEnsinoUser() );

		List< CurriculoDisciplina > curriculoDisciplinas
			= CurriculoDisciplina.findAllPeriodosBy(
				getInstituicaoEnsinoUser(), sala, oferta );

		for ( CurriculoDisciplina cd : curriculoDisciplinas )
		{
			CurriculoDisciplinaDTO cdDTO = ConvertBeans.toCurriculoDisciplinaDTO( cd );
			TreeNodeDTO nodeDTO = new TreeNodeDTO( cdDTO, ofertaTreeNodeDTO );

			nodeDTO.setText( "Periodo " + cdDTO.getPeriodo() );
			nodeDTO.setEmpty( false );
			nodeChildrenList.add( nodeDTO );
		}

		Collections.sort( nodeChildrenList );

		return nodeChildrenList;
	}

	@Override
	public List< TreeNodeDTO > getDisciplinasByTreeSalas(
		TreeNodeDTO salaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO,
		TreeNodeDTO curriculoDisciplinaTreeNodeDTO )
	{
		if ( ofertaTreeNodeDTO == null )
		{
			return getOfertasByTreeSalas( salaTreeNodeDTO );
		}

		if ( curriculoDisciplinaTreeNodeDTO == null )
		{
			return getPeriodosByTreeSalas( salaTreeNodeDTO, ofertaTreeNodeDTO );
		}

		List< TreeNodeDTO > nodeChildrenList = new ArrayList< TreeNodeDTO >();

		SalaDTO salaDTO = (SalaDTO) salaTreeNodeDTO.getContent();
		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );
		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find(
			ofertaDTO.getId(), getInstituicaoEnsinoUser() );

		CurriculoDisciplinaDTO curriculoDisciplinaDTO
			= (CurriculoDisciplinaDTO) curriculoDisciplinaTreeNodeDTO.getContent();

		List< CurriculoDisciplina > curriculoDisciplinas = CurriculoDisciplina.findBy(
			getInstituicaoEnsinoUser(), sala, oferta, curriculoDisciplinaDTO.getPeriodo() );

		for ( CurriculoDisciplina cd : curriculoDisciplinas )
		{
			CurriculoDisciplinaDTO cdDTO = ConvertBeans.toCurriculoDisciplinaDTO( cd );
			TreeNodeDTO nodeDTO = new TreeNodeDTO( cdDTO, curriculoDisciplinaTreeNodeDTO );

			nodeDTO.setLeaf( true );
			nodeChildrenList.add( nodeDTO );
		}

		Collections.sort( nodeChildrenList );

		return nodeChildrenList;
	}

	public List< TreeNodeDTO > getOfertasByTreeGrupoSalas(
		TreeNodeDTO grupoSalaTreeNodeDTO )
	{
		List< TreeNodeDTO > nodeChildrenList
			= new ArrayList< TreeNodeDTO >();

		GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO) grupoSalaTreeNodeDTO.getContent();
		GrupoSala grupoSala = GrupoSala.find(
			grupoSalaDTO.getId(), getInstituicaoEnsinoUser() );

		List< Oferta > ofertas = Oferta.findAllBy(
			getInstituicaoEnsinoUser(), grupoSala );

		for ( Oferta oferta : ofertas )
		{
			OfertaDTO ofertaDTO = ConvertBeans.toOfertaDTO( oferta );
			TreeNodeDTO nodeDTO = new TreeNodeDTO( ofertaDTO, grupoSalaTreeNodeDTO );

			nodeChildrenList.add( nodeDTO );
		}

		Collections.sort( nodeChildrenList );

		return nodeChildrenList;
	}

	public List< TreeNodeDTO > getPeriodosByTreeGrupoSalas(
		TreeNodeDTO grupoSalaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO )
	{
		List< TreeNodeDTO > nodeChildrenList = new ArrayList< TreeNodeDTO >();

		GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO) grupoSalaTreeNodeDTO.getContent();
		GrupoSala grupoSala = GrupoSala.find(
			grupoSalaDTO.getId(), getInstituicaoEnsinoUser() );

		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find(
			ofertaDTO.getId(), getInstituicaoEnsinoUser() );

		if ( oferta != null && grupoSala != null )
		{
			List< CurriculoDisciplina > curriculoDisciplinas
				= CurriculoDisciplina.findAllPeriodosBy(
					getInstituicaoEnsinoUser(), grupoSala, oferta );
	
			for ( CurriculoDisciplina cd : curriculoDisciplinas )
			{
				CurriculoDisciplinaDTO cdDTO
					= ConvertBeans.toCurriculoDisciplinaDTO( cd );
	
				TreeNodeDTO nodeDTO = new TreeNodeDTO( cdDTO, ofertaTreeNodeDTO );
				nodeDTO.setText( "Periodo " + cdDTO.getPeriodo() );
				nodeChildrenList.add( nodeDTO );
			}
		}

		Collections.sort( nodeChildrenList );

		return nodeChildrenList;
	}

	@Override
	public List< TreeNodeDTO > getDisciplinasByTreeGrupoSalas(
		TreeNodeDTO grupoSalaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO,
		TreeNodeDTO curriculoDisciplinaTreeNodeDTO )
	{
		if ( ofertaTreeNodeDTO == null )
		{
			return getOfertasByTreeGrupoSalas( grupoSalaTreeNodeDTO );
		}

		if ( curriculoDisciplinaTreeNodeDTO == null )
		{
			return getPeriodosByTreeGrupoSalas(
				grupoSalaTreeNodeDTO, ofertaTreeNodeDTO );
		}

		List< TreeNodeDTO > nodeChildrenList = new ArrayList< TreeNodeDTO >();

		GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO) grupoSalaTreeNodeDTO.getContent();
		GrupoSala grupoSala = GrupoSala.find(
			grupoSalaDTO.getId(), getInstituicaoEnsinoUser() );

		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find(
			ofertaDTO.getId(), getInstituicaoEnsinoUser() );

		CurriculoDisciplinaDTO curriculoDisciplinaDTO
			= (CurriculoDisciplinaDTO) curriculoDisciplinaTreeNodeDTO.getContent();

		if ( oferta != null && grupoSala != null )
		{
			List< CurriculoDisciplina > curriculoDisciplinas
				= CurriculoDisciplina.findBy( getInstituicaoEnsinoUser(),
					grupoSala, oferta, curriculoDisciplinaDTO.getPeriodo() );

			for ( CurriculoDisciplina cd : curriculoDisciplinas )
			{
				CurriculoDisciplinaDTO cdDTO
					= ConvertBeans.toCurriculoDisciplinaDTO( cd );

				TreeNodeDTO nodeDTO = new TreeNodeDTO(
					cdDTO, curriculoDisciplinaTreeNodeDTO );

				nodeDTO.setLeaf( true );
				nodeChildrenList.add( nodeDTO );
			}
		}

		Collections.sort( nodeChildrenList );

		return nodeChildrenList;
	}

	@Override
	public void saveDisciplinaToSala(
		OfertaDTO ofertaDTO, Integer periodo,
		CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO )
	{
		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );
		Set< CurriculoDisciplina > list = new HashSet< CurriculoDisciplina >();

		if ( ofertaDTO != null && periodo == null && cdDTO == null )
		{
			Oferta oferta = Oferta.find(
				ofertaDTO.getId(), getInstituicaoEnsinoUser() );
			
			if ( oferta != null )
			{
				list.addAll( oferta.getCurriculo().getDisciplinas() );
			}
		}
		else if ( ofertaDTO != null && periodo != null && cdDTO == null )
		{
			// Informou o periodo inteiro somente
			Oferta oferta = Oferta.find(
				ofertaDTO.getId(), getInstituicaoEnsinoUser() );

			if ( oferta != null )
			{
				list.addAll( CurriculoDisciplina.findAllByCurriculoAndPeriodo(
					getInstituicaoEnsinoUser(), oferta.getCurriculo(), periodo ) );
			}
		}
		else if ( ofertaDTO != null && periodo != null && cdDTO != null )
		{
			// Informou a disciplina somente
			list.add( CurriculoDisciplina.find(
				cdDTO.getId(), getInstituicaoEnsinoUser() ) );
		}

		for ( CurriculoDisciplina curriculoDisciplina : list )
		{
			curriculoDisciplina.getSalas().add( sala );
			curriculoDisciplina.persist();
		}
	}

	@Override
	public void saveDisciplinaToSala(
		OfertaDTO ofertaDTO, Integer periodo,
		CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO )
	{
		GrupoSala grupoSala = GrupoSala.find(
			grupoSalaDTO.getId(), getInstituicaoEnsinoUser() );

		Set< CurriculoDisciplina > list = new HashSet< CurriculoDisciplina >();

		if ( ofertaDTO != null && periodo == null && cdDTO == null )
		{
			Oferta oferta = Oferta.find(
				ofertaDTO.getId(), getInstituicaoEnsinoUser() );

			if ( oferta != null )
			{
				list.addAll( oferta.getCurriculo().getDisciplinas() );
			}
		}
		else if ( ofertaDTO != null && periodo != null && cdDTO == null )
		{
			// Informou o periodo inteiro somente
			Oferta oferta = Oferta.find(
				ofertaDTO.getId(), getInstituicaoEnsinoUser() );

			if ( oferta != null )
			{
				list.addAll( CurriculoDisciplina.findAllByCurriculoAndPeriodo(
					getInstituicaoEnsinoUser(), oferta.getCurriculo(), periodo ) );
			}
		}
		else if ( ofertaDTO != null && periodo != null && cdDTO != null )
		{
			// Informou a disciplina somente
			list.add( CurriculoDisciplina.find(
				cdDTO.getId(), getInstituicaoEnsinoUser() ) );
		}

		grupoSala.getCurriculoDisciplinas().addAll( list );
		grupoSala.persist();
	}

	@Override
	public void removeDisciplinaToSala(
		GrupoSalaDTO grupoSalaDTO,
		CurriculoDisciplinaDTO cdDTO )
	{
		GrupoSala grupoSala = GrupoSala.find(
			grupoSalaDTO.getId(), getInstituicaoEnsinoUser() );

		CurriculoDisciplina curriculoDisciplina
			= CurriculoDisciplina.find(
				cdDTO.getId(), getInstituicaoEnsinoUser() );

		grupoSala.getCurriculoDisciplinas().remove( curriculoDisciplina );
		grupoSala.merge();
	}

	@Override
	public void removeDisciplinaToSala(
		SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO )
	{
		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );
		CurriculoDisciplina curriculoDisciplina
			= CurriculoDisciplina.find(
				cdDTO.getId(), getInstituicaoEnsinoUser() );

		curriculoDisciplina.getSalas().remove( sala );
		curriculoDisciplina.merge();
	}

	@Override
	public List< DisciplinaIncompativelDTO > getDisciplinasIncompativeis(
		CurriculoDTO curriculoDTO, Integer periodo )
	{
		List< DisciplinaIncompativelDTO > list
			= new ArrayList< DisciplinaIncompativelDTO >();

		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );

		if ( curriculo == null )
		{
			return list;
		}

		List< CurriculoDisciplina > curriculoDisciplinas
			= curriculo.getCurriculoDisciplinasByPeriodo(periodo);

		for ( CurriculoDisciplina cd1 : curriculoDisciplinas )
		{
			for ( CurriculoDisciplina cd2 : curriculoDisciplinas )
			{
				if ( cd1.getId().equals( cd2.getId() ) )
				{
					continue;
				}

				DisciplinaIncompativelDTO di = new DisciplinaIncompativelDTO();

				if ( containsDisciplinaIncompativelDTO(
					list, cd1.getDisciplina(), cd2.getDisciplina() ) )
				{
					continue;
				}

				di.setDisciplina1Id( cd1.getDisciplina().getId() );

				di.setDisciplina1String( cd1.getDisciplina().getCodigo() +
					" (" + cd1.getDisciplina().getNome() + ")" );

				di.setDisciplina2Id( cd2.getDisciplina().getId() );

				di.setDisciplina2String( cd2.getDisciplina().getCodigo() +
					" (" + cd2.getDisciplina().getNome() + ")" );

				di.setIncompativel(cd1.getDisciplina().isIncompativelCom(
					getInstituicaoEnsinoUser(), cd2.getDisciplina() ) );

				list.add( di );
			}
		}

		return list;
	}

	@Override
	public void saveDisciplinasIncompativeis(
		List< DisciplinaIncompativelDTO > list )
	{
		for ( DisciplinaIncompativelDTO disciplinaIncompativelDTO : list )
		{
			Disciplina disciplina1 = Disciplina.find(
				disciplinaIncompativelDTO.getDisciplina1Id(), getInstituicaoEnsinoUser() );

			Disciplina disciplina2 = Disciplina.find(
				disciplinaIncompativelDTO.getDisciplina2Id(), getInstituicaoEnsinoUser() );

			Incompatibilidade incompatibilidade = disciplina1.getIncompatibilidadeWith(
				getInstituicaoEnsinoUser(), disciplina2 );

			if ( incompatibilidade == null )
			{
				if ( disciplinaIncompativelDTO.getIncompativel() )
				{
					incompatibilidade = new Incompatibilidade();
					incompatibilidade.setDisciplina1( disciplina1 );
					incompatibilidade.setDisciplina2( disciplina2 );
					incompatibilidade.persist();
				}
			}
			else
			{
				if ( !disciplinaIncompativelDTO.getIncompativel() )
				{
					incompatibilidade.remove();
				}
			}
		}
	}

	private boolean containsDisciplinaIncompativelDTO(
		List< DisciplinaIncompativelDTO > list,
		Disciplina disciplina1, Disciplina disciplina2 )
	{
		for ( DisciplinaIncompativelDTO disciplinaIncompativelDTO : list )
		{
			if ( disciplinaIncompativelDTO.getDisciplina1Id().equals( disciplina1.getId() )
					&& disciplinaIncompativelDTO.getDisciplina2Id().equals( disciplina2.getId() ) )
			{
				return true;
			}

			if ( disciplinaIncompativelDTO.getDisciplina1Id().equals( disciplina2.getId() )
					&& disciplinaIncompativelDTO.getDisciplina2Id().equals( disciplina1.getId() ) )
			{
				return true;
			}
		}

		return false;
	}

	@Override
	public List< ResumoDisciplinaDTO > getResumos(
		CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );

     	// Juntando os atendimentos tático e operacional
		List< AtendimentoTatico > listTatico = AtendimentoTatico.findAllByCampus( getInstituicaoEnsinoUser(), campus );
		List< AtendimentoTaticoDTO > listTaticoDTO = ConvertBeans.toListAtendimentoTaticoDTO(listTatico);
		
		List< AtendimentoOperacional > listOperacional = AtendimentoOperacional.findAll( campus, getInstituicaoEnsinoUser() );
		List< AtendimentoOperacionalDTO > listOperacionalDTOPorHorario = ConvertBeans.toListAtendimentoOperacionalDTO(listOperacional);
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		List< AtendimentoRelatorioDTO > listOperacionalDTOPorAula = new ArrayList<AtendimentoRelatorioDTO>(service.extraiAulas(listOperacionalDTOPorHorario));

		boolean ehTatico = !listTatico.isEmpty();
		
		List< AtendimentoRelatorioDTO > listRelatorioDTO = new ArrayList< AtendimentoRelatorioDTO >();
		listRelatorioDTO.addAll( listTaticoDTO );
		listRelatorioDTO.addAll( listOperacionalDTOPorAula );

		Map<Long,Oferta> ofertasMap = Oferta.getOfertasMap(campus.getOfertas());
		Map<Long,Professor> professoresMap = null;
		if (!ehTatico) {
			professoresMap = Professor.getProfessoresMap(Professor.findAll(this.getInstituicaoEnsinoUser())); 
		}

		// [Disciplina-Turma-TipoCredito -> List< AtendimentoRelatorioDTO >]
		Map< String, List< AtendimentoRelatorioDTO > > atendimentosMap = new HashMap< String, List< AtendimentoRelatorioDTO > >();
		// [Disciplina -> ResumoDisciplinaDTO]
		Map< String, ResumoDisciplinaDTO > nivel1Map = new HashMap< String, ResumoDisciplinaDTO >();
		// [Disciplina -> [Disciplina-Turma-TipoCredito -> Pair]]
		Map< String, Map< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> > > nivel2Map = new HashMap< String, Map< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> > >();

		for ( AtendimentoRelatorioDTO atendimento : listRelatorioDTO ) {
			String key = atendimento.getDisciplinaId() + "-" + atendimento.getTurma() + "-" + ( atendimento.isTeorico() );
			List< AtendimentoRelatorioDTO > list = atendimentosMap.get( key ); 
			if ( list == null ) {
				list = new ArrayList< AtendimentoRelatorioDTO >();
				atendimentosMap.put( key, list );
			}
			list.add( atendimento );

			Disciplina disciplina = Disciplina.find( atendimento.getDisciplinaId(), getInstituicaoEnsinoUser() );
			Oferta oferta = ofertasMap.get(atendimento.getOfertaId());

			ResumoDisciplinaDTO resumoDTO = new ResumoDisciplinaDTO();

			resumoDTO.setCampusId(atendimento.getCampusId());
			resumoDTO.setCampusString(atendimento.getCampusString());
			resumoDTO.setCursoId(atendimento.getCursoId());
			resumoDTO.setCurriculoId(atendimento.getCurriculoId());
			resumoDTO.setDisciplinaId( disciplina.getId() );
			resumoDTO.setDisciplinaString( disciplina.getCodigo() );
			resumoDTO.setTurma( atendimento.getTurma() );
			resumoDTO.setTipoCreditoTeorico( atendimento.isTeorico() );
			resumoDTO.setCreditos( atendimento.getTotalCreditos() );
			resumoDTO.setTotalCreditos( disciplina.getCreditosTotal() );
			resumoDTO.setQuantidadeAlunos( atendimento.getQuantidadeAlunos() );
			resumoDTO.setDiaSemanaId(atendimento.getSemana());
			resumoDTO.setHorarioId(atendimento.getHorarioId());
			if (ehTatico) {
				resumoDTO.setCustoDocente(new TriedaCurrency(oferta.getCampus().getValorCredito()));
			} else {
				Long profId = ((AtendimentoOperacionalDTO)atendimento).getProfessorId();
				Professor prof = professoresMap.get(profId);
				if (prof != null) {
					resumoDTO.setCustoDocente(new TriedaCurrency(prof.getValorCredito()));
				} else {
					resumoDTO.setCustoDocente(new TriedaCurrency(oferta.getCampus().getValorCredito()));
				}
			}
			resumoDTO.setReceita(new TriedaCurrency(oferta.getReceita()));
			
			createResumoNivel1( nivel1Map, nivel2Map, resumoDTO );
			createResumoNivel2( nivel2Map, resumoDTO );
		}

		calculaResumo2( nivel2Map, atendimentosMap );
		calculaResumo1( nivel1Map, nivel2Map );

		return createResumoEstrutura( nivel1Map, nivel2Map );
	}

	private List< ResumoDisciplinaDTO > createResumoEstrutura(
		Map< String, ResumoDisciplinaDTO > map1,
		Map< String, Map< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> > > map2 )
	{
		List< ResumoDisciplinaDTO > list = new ArrayList< ResumoDisciplinaDTO >();

		for ( String key1 : map1.keySet() )
		{
			ResumoDisciplinaDTO rd1DTO = map1.get( key1 );
			list.add( rd1DTO );

			for ( String key2 : map2.get( key1 ).keySet() )
			{
				Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> pair = map2.get( key1 ).get( key2 ); 
				rd1DTO.add( pair.getLeft() );
			}
		}

		return list;
	}

	private AtendimentoRelatorioDTO concatenaAtendimentosResumoDisciplina(
		List< AtendimentoRelatorioDTO > list )
	{
		if ( list == null || list.size() == 0 )
		{
			return null;
		}

		if ( list.size() == 1 )
		{
			return list.get( 0 );
		}

		Integer totalAlunos = 0;
		for ( AtendimentoRelatorioDTO at : list )
		{
			totalAlunos += at.getQuantidadeAlunos();
		}

		AtendimentoRelatorioDTO atendimento = null;

		if ( list.get( 0 ) instanceof AtendimentoTaticoDTO )
		{
			atendimento = new AtendimentoTaticoDTO( (AtendimentoTaticoDTO) list.get( 0 ) );
			( (AtendimentoTaticoDTO)atendimento ).setQuantidadeAlunos( totalAlunos );
		}
		else if ( list.get( 0 ) instanceof AtendimentoOperacionalDTO )
		{
			atendimento = new AtendimentoOperacionalDTO( (AtendimentoOperacionalDTO) list.get( 0 ) );
			( (AtendimentoOperacionalDTO) atendimento ).setQuantidadeAlunos( totalAlunos );
		}

		return atendimento;
	}

	private void calculaResumo2(
		Map< String, Map< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> > > map2,
		Map< String, List< AtendimentoRelatorioDTO > > atendimentosMap )
	{
		Locale pt_BR = new Locale("pt","BR");
		CurrencyFormatter currencyFormatter = new CurrencyFormatter();
		
		// key1 = disciplinaId
		for ( String key1 : map2.keySet() ) {
			// key2 = disciplinaId-turma-tipoCredito
			for ( String key2 : map2.get( key1 ).keySet() ) {
				Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> pair = map2.get( key1 ).get( key2 );
				ResumoDisciplinaDTO mainDTO = pair.getLeft();
				
				//System.out.println("***"+mainDTO.getDisciplinaString()+"-"+mainDTO.getTurma()+"-"+mainDTO.getTipoCreditoTeorico());//TODO: retirar
				
				// separar por curso-curriculo por conta do compartilhamento de aulas entre cursos
				Map<String,List<ResumoDisciplinaDTO>> dtoPorCursoCurriculoMap = new HashMap<String,List<ResumoDisciplinaDTO>>();
				// separar por DiaSemana-Horario
				Map<String,List<ResumoDisciplinaDTO>> dtoPorDiaSemanaMap = new HashMap<String,List<ResumoDisciplinaDTO>>();
				for (ResumoDisciplinaDTO dto : pair.getRight()) {
					// curso-curriculo
					String cursoCurriculoKey = dto.getCursoId()+"-"+dto.getCurriculoId();
					List<ResumoDisciplinaDTO> list1 = dtoPorCursoCurriculoMap.get(cursoCurriculoKey);
					if (list1 == null) {
						list1 = new ArrayList<ResumoDisciplinaDTO>();
						dtoPorCursoCurriculoMap.put(cursoCurriculoKey,list1);
					}
					list1.add(dto);
					//System.out.println("   @ "+cursoCurriculoKey);//TODO: retirar
					
					// dia semana
					String key = dto.getDiaSemanaId()+"-"+dto.getHorarioId();
					List<ResumoDisciplinaDTO> list2 = dtoPorDiaSemanaMap.get(key);
					if (list2 == null) {
						list2 = new ArrayList<ResumoDisciplinaDTO>();
						dtoPorDiaSemanaMap.put(key,list2);
					}
					list2.add(dto);
				}
				
				if (dtoPorCursoCurriculoMap.keySet().size() > 1) {
					String primeiroCursoCurriculoId = dtoPorCursoCurriculoMap.keySet().iterator().next();
					//System.out.println("   # "+primeiroCursoCurriculoId);//TODO: retirar
					
					for (ResumoDisciplinaDTO dto : dtoPorCursoCurriculoMap.get(primeiroCursoCurriculoId)) {
						// acumula a qtde de créditos
						mainDTO.setCreditos(dto.getCreditos() + (mainDTO.getCreditos() != null ? mainDTO.getCreditos(): 0));
						//System.out.println("   + "+dto.getCreditos()+" = "+mainDTO.getCreditos()+" "+dto.getCursoId()+"-"+dto.getCurriculoId());//TODO: retirar
						// calcula e acumula custo docente
						Double docente = dto.getCustoDocente().getDoubleValue();
						int creditos = dto.getCreditos();
						double custoDocenteLocal = creditos * docente * 4.5 * 6.0;
						mainDTO.setCustoDocente(TriedaUtil.parseTriedaCurrency(custoDocenteLocal + mainDTO.getCustoDocente().getDoubleValue()));
						mainDTO.setCustoDocenteString(currencyFormatter.print(mainDTO.getCustoDocente().getDoubleValue(),pt_BR));
					}
					
					for (ResumoDisciplinaDTO dto : pair.getRight()) {
						// calcula e acumula receita
						double receita = dto.getReceita().getDoubleValue();
						int qtdAlunos = dto.getQuantidadeAlunos();
						int creditos = dto.getCreditos();
						double receitaLocal = creditos * receita * qtdAlunos * 4.5 * 6.0;
						mainDTO.setReceita(TriedaUtil.parseTriedaCurrency(receitaLocal + mainDTO.getReceita().getDoubleValue()));
						mainDTO.setReceitaString(currencyFormatter.print(mainDTO.getReceita().getDoubleValue(),pt_BR));
					}
				} else {
					for (ResumoDisciplinaDTO dto : pair.getRight()) {
						// acumula a qtde de créditos
						mainDTO.setCreditos(dto.getCreditos() + (mainDTO.getCreditos() != null ? mainDTO.getCreditos(): 0));
						//System.out.println("   + "+dto.getCreditos()+" = "+mainDTO.getCreditos());//TODO: retirar
						// calcula e acumula custo docente
						Double docente = dto.getCustoDocente().getDoubleValue();
						Double receita = dto.getReceita().getDoubleValue();
						int qtdAlunos = dto.getQuantidadeAlunos();
						int creditos = dto.getCreditos();
						double custoDocenteLocal = creditos * docente * 4.5 * 6.0;
						mainDTO.setCustoDocente(TriedaUtil.parseTriedaCurrency(custoDocenteLocal + mainDTO.getCustoDocente().getDoubleValue()));
						mainDTO.setCustoDocenteString(currencyFormatter.print(mainDTO.getCustoDocente().getDoubleValue(),pt_BR));
						// calcula e acumula receita
						double receitaLocal = creditos * receita * qtdAlunos * 4.5 * 6.0;
						mainDTO.setReceita(TriedaUtil.parseTriedaCurrency(receitaLocal + mainDTO.getReceita().getDoubleValue()));
						mainDTO.setReceitaString(currencyFormatter.print(mainDTO.getReceita().getDoubleValue(),pt_BR));
					}
				}
				
				// calcula quantidade de alunos
				mainDTO.setQuantidadeAlunos(0);
				String diaSemanaHorarioId = dtoPorDiaSemanaMap.keySet().iterator().next();
				List<ResumoDisciplinaDTO> dtos = dtoPorDiaSemanaMap.get(diaSemanaHorarioId);
				for (ResumoDisciplinaDTO dto : dtos) {
					mainDTO.setQuantidadeAlunos(dto.getQuantidadeAlunos() + mainDTO.getQuantidadeAlunos());
				}
				
				// calcula margem
				double margem = mainDTO.getReceita().getDoubleValue() - mainDTO.getCustoDocente().getDoubleValue();
				double margemPercent = 0.0;
				if (Double.compare(mainDTO.getReceita().getDoubleValue(),0.0) != 0) {
					margemPercent = (margem / mainDTO.getReceita().getDoubleValue()); 
				}
				mainDTO.setMargem(TriedaUtil.parseTriedaCurrency(margem));
				mainDTO.setMargemString(currencyFormatter.print(mainDTO.getMargem().getDoubleValue(),pt_BR));
				mainDTO.setMargemPercente(margemPercent);
				mainDTO.setMargemPercenteString(TriedaUtil.round(mainDTO.getMargemPercente()*100.0,2)+"%");
			}
		}
	}

	private void calculaResumo1(
		Map< String, ResumoDisciplinaDTO > map1,
		Map< String, Map< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> > > map2 )
	{
		Locale pt_BR = new Locale("pt","BR");
		CurrencyFormatter currencyFormatter = new CurrencyFormatter();
		
		for ( String key1 : map2.keySet() )
		{
			ResumoDisciplinaDTO rc1 = map1.get( key1 );

			rc1.setCustoDocente( new TriedaCurrency( 0.0 ) );
			rc1.setReceita( new TriedaCurrency( 0.0 ) );
			rc1.setMargem( new TriedaCurrency( 0.0 ) );
			rc1.setMargemPercente( 0.0 );

			for ( String key2 : map2.get( key1 ).keySet() )
			{
				Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> pair = map2.get( key1 ).get( key2 ); 
				ResumoDisciplinaDTO rc2 = pair.getLeft();

				rc1.setCustoDocente( new TriedaCurrency(rc1.getCustoDocente().getDoubleValue() + rc2.getCustoDocente().getDoubleValue()) );
				rc1.setCustoDocenteString(currencyFormatter.print(rc1.getCustoDocente().getDoubleValue(),pt_BR));

				rc1.setReceita( new TriedaCurrency(rc1.getReceita().getDoubleValue() + rc2.getReceita().getDoubleValue()) );
				rc1.setReceitaString(currencyFormatter.print(rc1.getReceita().getDoubleValue(),pt_BR));

				rc1.setMargem( new TriedaCurrency(rc1.getMargem().getDoubleValue() + rc2.getMargem().getDoubleValue()) );
				rc1.setMargemString(currencyFormatter.print(rc1.getMargem().getDoubleValue(),pt_BR));

				// Código relacionado com a issue TRIEDA-1050
				Double margemValue = rc1.getMargem().getDoubleValue();
				Double receitaValue = rc1.getReceita().getDoubleValue();
				Double margemPercent = 0.0;
				if (Double.compare(receitaValue,0.0) != 0) {
					margemPercent = ( ( margemValue / receitaValue ) );
				}

				rc1.setMargemPercente(margemPercent);
				rc1.setMargemPercenteString(TriedaUtil.round(rc1.getMargemPercente()*100.0,2)+"%");
			}
		}
	}

	private void createResumoNivel1(
		Map< String, ResumoDisciplinaDTO > map1,
		Map< String, Map< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> > > map2,
		ResumoDisciplinaDTO resumoDTO )
	{
		String key1 = resumoDTO.getDisciplinaId().toString();
		ResumoDisciplinaDTO rdDTO = map1.get( key1 );

		if ( rdDTO == null )
		{
			rdDTO = new ResumoDisciplinaDTO();

			rdDTO.setDisciplinaId( resumoDTO.getDisciplinaId() );
			rdDTO.setDisciplinaString( resumoDTO.getDisciplinaString() );

			map1.put( key1, rdDTO );
			map2.put( key1, new HashMap< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> >() );
		}
	}

	private void createResumoNivel2( Map< String, Map< String, Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> > > map2, ResumoDisciplinaDTO resumoDTO )
	{
		// disciplina
		String key1 = resumoDTO.getDisciplinaId().toString();
		// disciplina-turma-tipoCredito
		String key2 = key1 + "-" + resumoDTO.getTurma() + "-" + resumoDTO.getTipoCreditoTeorico();

		Pair<ResumoDisciplinaDTO,List<ResumoDisciplinaDTO>> pair = map2.get(key1).get(key2);
		if ( pair == null ) {
			List<ResumoDisciplinaDTO> list = new ArrayList<ResumoDisciplinaDTO>();
			ResumoDisciplinaDTO dtoMain = new ResumoDisciplinaDTO();
			dtoMain.setCampusString( resumoDTO.getCampusString() );
			dtoMain.setCursoId( resumoDTO.getCursoId() );
			dtoMain.setCurriculoId( resumoDTO.getCurriculoId() );
			dtoMain.setDisciplinaId( resumoDTO.getDisciplinaId() );
			dtoMain.setDisciplinaString( resumoDTO.getDisciplinaString() );
			dtoMain.setTurma( resumoDTO.getTurma() );
			dtoMain.setTipoCreditoTeorico( resumoDTO.getTipoCreditoTeorico() );
			dtoMain.setTotalCreditos( resumoDTO.getTotalCreditos() );
			dtoMain.setQuantidadeAlunos( resumoDTO.getQuantidadeAlunos() );
			pair = Pair.create(dtoMain,list);
			map2.get(key1).put(key2, pair);
		}
		
		ResumoDisciplinaDTO dtoNew = new ResumoDisciplinaDTO();
		dtoNew.setCursoId( resumoDTO.getCursoId() );
		dtoNew.setCurriculoId( resumoDTO.getCurriculoId() );
		dtoNew.setDisciplinaId( resumoDTO.getDisciplinaId() );
		dtoNew.setDisciplinaString( resumoDTO.getDisciplinaString() );
		dtoNew.setTurma( resumoDTO.getTurma() );
		dtoNew.setTipoCreditoTeorico( resumoDTO.getTipoCreditoTeorico() );
		dtoNew.setCreditos( resumoDTO.getCreditos() );
		dtoNew.setTotalCreditos( resumoDTO.getTotalCreditos() );
		dtoNew.setQuantidadeAlunos( resumoDTO.getQuantidadeAlunos() );
		dtoNew.setDiaSemanaId( resumoDTO.getDiaSemanaId() );
		dtoNew.setHorarioId( resumoDTO.getHorarioId() );
		dtoNew.setCustoDocente( resumoDTO.getCustoDocente() );
		dtoNew.setReceita( resumoDTO.getReceita() );
		
		pair.getRight().add(dtoNew);
	}
}