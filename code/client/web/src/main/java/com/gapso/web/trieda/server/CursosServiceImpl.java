package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

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
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.services.CursosService;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.google.gwt.dev.util.Pair;

@Transactional
public class CursosServiceImpl
	extends RemoteService implements CursosService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public CursoDTO getCurso( Long id )
	{
		return ConvertBeans.toCursoDTO(
			Curso.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public ListLoadResult< CursoDTO > getListAll()
	{
		List< CursoDTO > list = new ArrayList< CursoDTO >();
		List< Curso > cursos = Curso.findAll( getInstituicaoEnsinoUser() );

		for ( Curso curso : cursos )
		{
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO( curso );
			list.add( cursoDTO );
		}

		return new BasePagingLoadResult< CursoDTO >( list );
	}

	@Override
	public ListLoadResult<CursoDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(null, loadConfig.get("query").toString(), null,
				loadConfig);
	}

	@Override
	public PagingLoadResult< CursoDTO > getBuscaList( String nome, String codigo,
		TipoCursoDTO tipoCursoDTO, PagingLoadConfig config )
	{
		List< CursoDTO > list = new ArrayList< CursoDTO >();
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

		TipoCurso tipoCurso = null;
		if ( tipoCursoDTO != null )
		{
			tipoCurso = ConvertBeans.toTipoCurso( tipoCursoDTO );
		}

		List< Curso > listDomains = Curso.findBy( getInstituicaoEnsinoUser(),
			codigo, nome, tipoCurso, config.getOffset(), config.getLimit(), orderBy );

		for ( Curso curso : listDomains )
		{
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO( curso );

			list.add( cursoDTO );
		}

		BasePagingLoadResult< CursoDTO > result
			= new BasePagingLoadResult< CursoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Curso.count(
			getInstituicaoEnsinoUser(), codigo, nome, tipoCurso ) );

		return result;
	}

	@Override
	public ListLoadResult<CursoDTO> getListByCampus(CampusDTO campusDTO,
			List<CursoDTO> retirarCursosDTO) {
		List<CursoDTO> list = new ArrayList<CursoDTO>();

		Campus campus = (campusDTO == null) ? null :
			ConvertBeans.toCampus( campusDTO );

		List< Curso > listDomains = Curso.findByCampus(
			getInstituicaoEnsinoUser(), campus );

		for ( Curso curso : listDomains )
		{
			list.add( ConvertBeans.toCursoDTO( curso ) );
		}

		list.removeAll( retirarCursosDTO );

		ListLoadResult< CursoDTO > result
			= new BaseListLoadResult< CursoDTO >( list );

		return result;
	}

	@Override
	public void save(CursoDTO cursoDTO) {
		Curso curso = ConvertBeans.toCurso(cursoDTO);

		if (curso.getId() != null && curso.getId() > 0) {
			curso.merge();
		} else {
			curso.persist();
		}
	}

	@Override
	public void remove(List<CursoDTO> cursoDTOList) {
		for (CursoDTO cursoDTO : cursoDTOList) {
			ConvertBeans.toCurso(cursoDTO).remove();
		}
	}

	@Override
	public List<ResumoCursoDTO> getResumos(
		CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		Campus campus = Campus.find(campusDTO.getId(), this.getInstituicaoEnsinoUser() );
		
		Map<Long,Professor> professoresMap = null;

		List< Oferta > ofertas = new ArrayList< Oferta >( campus.getOfertas() );
		Collections.sort( ofertas );

		List< ResumoCursoDTO > resumoCursoDTOList = new ArrayList< ResumoCursoDTO >();
		
		// [Campus-Turno-Curso -> ResumoCursoDTO]
		Map< String, ResumoCursoDTO > nivel1Map = new HashMap< String, ResumoCursoDTO >();
		// [Campus-Turno-Curso -> [Campus-Turno-Curso-Curriculo-Periodo -> ResumoCursoDTO]]
		Map< String, Map< String, ResumoCursoDTO > > nivel2Map = new HashMap< String, Map< String, ResumoCursoDTO > >();
		// [Campus-Turno-Curso -> [Campus-Turno-Curso-Curriculo-Periodo -> [Campus-Turno-Curso-Curriculo-Periodo-Disciplina-Turma-TipoCredito -> ResumoCursoDTO]]]
		Map< String, Map< String, Map< String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>> > > > nivel3Map = new HashMap< String, Map< String, Map< String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>> > > >();

		for ( Oferta oferta : ofertas ) {
			// monta a lista de atendimentos associados a uma oferta
			List< AtendimentoRelatorioDTO > atendimentoRelatorioDTOList = new ArrayList< AtendimentoRelatorioDTO >();
			List< AtendimentoTatico > atendimentoTaticoList = AtendimentoTatico.findAllBy( getInstituicaoEnsinoUser(), oferta );
			boolean ehTatico = true;
			if (!atendimentoTaticoList.isEmpty()) {
				for (AtendimentoTatico atendimentoTatico : atendimentoTaticoList) {
					atendimentoRelatorioDTOList.add(ConvertBeans.toAtendimentoTaticoDTO(atendimentoTatico));
				}
			}
			else {
				List< AtendimentoOperacional > atendimentoOperacionalList = AtendimentoOperacional.findAllBy( oferta, getInstituicaoEnsinoUser() );
				List< AtendimentoOperacionalDTO > atendimentoOperacionalDTOList = new ArrayList<AtendimentoOperacionalDTO>(); 
				for ( AtendimentoOperacional atendimentoOperacional : atendimentoOperacionalList ) {
					atendimentoOperacionalDTOList.add(ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
				}
				AtendimentosServiceImpl service = new AtendimentosServiceImpl();
				atendimentoRelatorioDTOList.addAll(service.transformaAtendimentosPorHorarioEmAtendimentosPorAula(atendimentoOperacionalDTOList));
				ehTatico = false;
				List<Professor> professores = Professor.findAll(this.getInstituicaoEnsinoUser());
				professoresMap = Professor.getProfessoresMap(professores);
			}

			// A partir dos atendimentos, monta as estruturas nivel1Map, nivel2Map e nivel3Map, além de converter um atendimento
			// em um objeto ResumoCursoDTO e armazená-los em uma lista
			for ( AtendimentoRelatorioDTO atendimentoRelatorioDTO : atendimentoRelatorioDTOList ) {
				ResumoCursoDTO resumoDTO = new ResumoCursoDTO();

				resumoDTO.setOfertaId(oferta.getId());
				resumoDTO.setCampusId(oferta.getCampus().getId());
				resumoDTO.setCampusString(oferta.getCampus().getNome());
				resumoDTO.setTurnoId(atendimentoRelatorioDTO.getTurnoId());
				resumoDTO.setTurnoString(atendimentoRelatorioDTO.getTurnoString());
				resumoDTO.setCursoId(atendimentoRelatorioDTO.getCursoId());
				resumoDTO.setCursoString(atendimentoRelatorioDTO.getCursoNome());
				resumoDTO.setMatrizCurricularId(atendimentoRelatorioDTO.getCurriculoId());
				resumoDTO.setMatrizCurricularString(atendimentoRelatorioDTO.getCurriculoString());
				resumoDTO.setPeriodo(Integer.valueOf(atendimentoRelatorioDTO.getPeriodoString()));
				resumoDTO.setQuantidadeAlunos(atendimentoRelatorioDTO.getQuantidadeAlunos());
				resumoDTO.setDisciplinaId(atendimentoRelatorioDTO.getDisciplinaId());
				resumoDTO.setDisciplinaString(atendimentoRelatorioDTO.getDisciplinaString());
				resumoDTO.setTurma(atendimentoRelatorioDTO.getTurma());
				resumoDTO.setTipoCreditoTeorico(atendimentoRelatorioDTO.isTeorico());
				resumoDTO.setCreditos(atendimentoRelatorioDTO.getTotalCreditos());
				resumoDTO.setSalaId(atendimentoRelatorioDTO.getSalaId());
				resumoDTO.setSalaString(atendimentoRelatorioDTO.getSalaString());
				resumoDTO.setDiaSemana(atendimentoRelatorioDTO.getSemana());
				if (ehTatico) {
					resumoDTO.setCustoDocente(new TriedaCurrency(oferta.getCampus().getValorCredito()));
				} else {
					Long profId = ((AtendimentoOperacionalDTO)atendimentoRelatorioDTO).getProfessorId();
					Professor prof = professoresMap.get(profId);
					if (prof != null) {
						resumoDTO.setCustoDocente(new TriedaCurrency(prof.getValorCredito()));
					} else {
						resumoDTO.setCustoDocente(new TriedaCurrency(oferta.getCampus().getValorCredito()));
					}
				}
				resumoDTO.setReceita(new TriedaCurrency(oferta.getReceita()));

				createResumoNivel1(nivel1Map, nivel2Map, nivel3Map, resumoDTO);
				createResumoNivel2(nivel2Map, nivel3Map, resumoDTO);
				createResumoNivel3(nivel3Map, resumoDTO);

				resumoCursoDTOList.add(resumoDTO);
			}
		}

		calculaResumo3(nivel3Map, resumoCursoDTOList);
		calculaResumo1e2(nivel1Map, nivel2Map, nivel3Map);

		return createResumoEstrutura(nivel1Map, nivel2Map, nivel3Map);
	}

	private List<ResumoCursoDTO> createResumoEstrutura(
			Map<String, ResumoCursoDTO> map1,
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, Pair<ResumoCursoDTO, List<ResumoCursoDTO>>>>> map3) {
		List<ResumoCursoDTO> list = new ArrayList<ResumoCursoDTO>();

		for (String key1 : map1.keySet()) {
			ResumoCursoDTO rc1DTO = map1.get(key1);
			list.add(rc1DTO);

			for (String key2 : map2.get(key1).keySet()) {
				ResumoCursoDTO rc2DTO = map2.get(key1).get(key2);
				rc1DTO.add(rc2DTO);

				for (String key3 : map3.get(key1).get(key2).keySet()) {
					Pair<ResumoCursoDTO, List<ResumoCursoDTO>> pair = map3.get(key1).get(key2).get(key3);
					ResumoCursoDTO rc3DTO = pair.getLeft();
					rc2DTO.add(rc3DTO);
				}
			}
		}

		return list;
	}

	private void calculaResumo3(
			Map<String, Map<String, Map<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>> map3,
			List<ResumoCursoDTO> atendimentoResumoList) {
		Map<String, Double> rateioMap = createRateioMap(atendimentoResumoList);

		for (String key1 : map3.keySet()) {
			for (String key2 : map3.get(key1).keySet()) {
				for (String key3 : map3.get(key1).get(key2).keySet()) {
					Pair<ResumoCursoDTO,List<ResumoCursoDTO>> pair = map3.get(key1).get(key2).get(key3);
					Double rateio = TriedaUtil.roundTwoDecimals(rateioMap.get(key3));
					
					ResumoCursoDTO mainDTO = pair.getLeft();
					mainDTO.setRateio(rateio);
					
					for (ResumoCursoDTO dto : pair.getRight()) {
						// acumula a qtde de créditos
						mainDTO.setCreditos(dto.getCreditos() + (mainDTO.getCreditos() != null ? mainDTO.getCreditos(): 0));
						// calcula e acumula custo docente
						Double docente = dto.getCustoDocente().getDoubleValue();
						Double receita = dto.getReceita().getDoubleValue();
						int qtdAlunos = dto.getQuantidadeAlunos();
						int creditos = dto.getCreditos();
						double custoDocenteLocal = creditos * docente * rateio * 4.5 * 6.0;
						mainDTO.setCustoDocente(TriedaUtil.parseTriedaCurrency(custoDocenteLocal + mainDTO.getCustoDocente().getDoubleValue()));
						// calcula e acumula receita
						double receitaLocal = creditos * receita * qtdAlunos * 4.5 * 6.0;
						mainDTO.setReceita(TriedaUtil.parseTriedaCurrency(receitaLocal + mainDTO.getReceita().getDoubleValue()));
					}

					// calcula margem
					double margem = mainDTO.getReceita().getDoubleValue() - mainDTO.getCustoDocente().getDoubleValue();
					double margemPercent = 0.0;
					if (Double.compare(mainDTO.getReceita().getDoubleValue(),0.0) != 0) {
						margemPercent = margem / mainDTO.getReceita().getDoubleValue(); 
					}
					mainDTO.setMargem(TriedaUtil.parseTriedaCurrency(margem));
					mainDTO.setMargemPercente(TriedaUtil.roundTwoDecimals(margemPercent));
				}
			}
		}
	}

	private void calculaResumo1e2(Map<String, ResumoCursoDTO> map1,
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>> map3) {
		for (String key1 : map3.keySet()) {
			ResumoCursoDTO rc1 = map1.get(key1);

			rc1.setCustoDocente(TriedaUtil.parseTriedaCurrency(0.0));
			rc1.setReceita(TriedaUtil.parseTriedaCurrency(0.0));
			rc1.setMargem(TriedaUtil.parseTriedaCurrency(0.0));
			rc1.setMargemPercente(0.0);

			for (String key2 : map3.get(key1).keySet()) {
				ResumoCursoDTO rc2 = map2.get(key1).get(key2);

				rc2.setCustoDocente(TriedaUtil.parseTriedaCurrency(0.0));
				rc2.setReceita(TriedaUtil.parseTriedaCurrency(0.0));
				rc2.setMargem(TriedaUtil.parseTriedaCurrency(0.0));
				rc2.setMargemPercente(0.0);

				for (String key3 : map3.get(key1).get(key2).keySet()) {
					Pair<ResumoCursoDTO, List<ResumoCursoDTO>> pair = map3.get(key1).get(key2).get(key3);
					ResumoCursoDTO rc3 = pair.getLeft();

					rc1.setCustoDocente(TriedaUtil.parseTriedaCurrency(rc1
							.getCustoDocente().getDoubleValue()
							+ rc3.getCustoDocente().getDoubleValue()));

					rc2.setCustoDocente(TriedaUtil.parseTriedaCurrency(rc2
							.getCustoDocente().getDoubleValue()
							+ rc3.getCustoDocente().getDoubleValue()));

					rc1.setReceita(TriedaUtil.parseTriedaCurrency(rc1
							.getReceita().getDoubleValue()
							+ rc3.getReceita().getDoubleValue()));

					rc2.setReceita(TriedaUtil.parseTriedaCurrency(rc2
							.getReceita().getDoubleValue()
							+ rc3.getReceita().getDoubleValue()));

					rc1.setMargem(TriedaUtil.parseTriedaCurrency(rc1
							.getMargem().getDoubleValue()
							+ rc3.getMargem().getDoubleValue()));

					rc2.setMargem(TriedaUtil.parseTriedaCurrency(rc2
							.getMargem().getDoubleValue()
							+ rc3.getMargem().getDoubleValue()));

					rc1.setMargemPercente(TriedaUtil.roundTwoDecimals(rc1
							.getMargem().getDoubleValue()
							/ rc1.getReceita().getDoubleValue()));

					rc2.setMargemPercente(TriedaUtil.roundTwoDecimals(rc2
							.getMargem().getDoubleValue()
							/ rc2.getReceita().getDoubleValue()));
				}
			}
		}
	}

	private String getKeyNivel1(ResumoCursoDTO resumoCursoDTO) {
		return resumoCursoDTO.getCampusId() + "-" + resumoCursoDTO.getTurnoId() + "-" + resumoCursoDTO.getCursoId();
	}

	private String getKeyNivel2(ResumoCursoDTO resumoCursoDTO) {
		return getKeyNivel1(resumoCursoDTO) + "-" + resumoCursoDTO.getMatrizCurricularId() + "-" + resumoCursoDTO.getPeriodo();
	}

	private String getKeyNivel3(ResumoCursoDTO resumoCursoDTO) {
		return getKeyNivel2(resumoCursoDTO) + "-" + resumoCursoDTO.getDisciplinaId() + "-" + resumoCursoDTO.getTurma() + "-" + resumoCursoDTO.getTipoCreditoTeorico();
	}
	
	private String getKeyCompartilhamentoTurma(ResumoCursoDTO resumoCursoDTO) {
		return resumoCursoDTO.getDisciplinaId() + "-" + resumoCursoDTO.getTurma() + "-" + resumoCursoDTO.getDiaSemana() + "-" + resumoCursoDTO.getSalaId();
	}

	private void createResumoNivel1(Map<String, ResumoCursoDTO> map1,
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>> map3,
			ResumoCursoDTO resumoCursoDTO) {
		String key1 = getKeyNivel1(resumoCursoDTO); // Campus-Turno-Curso

		if (!map1.containsKey(key1)) {
			ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();

			resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
			resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
			resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
			resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
			resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
			resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());

			map1.put(key1, resumoCursoDTONew);
			map2.put(key1, new HashMap<String, ResumoCursoDTO>());
			map3.put(key1, new HashMap<String, Map<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>());
		}
	}

	private void createResumoNivel2(
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>> map3,
			ResumoCursoDTO resumoCursoDTO) {
		String key1 = getKeyNivel1(resumoCursoDTO); // Campus-Turno-Curso
		String key2 = getKeyNivel2(resumoCursoDTO); // Campus-Turno-Curso-Curriculo-Periodo

		if (!map2.get(key1).containsKey(key2)) {
			ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();

			resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
			resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
			resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
			resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
			resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
			resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());
			resumoCursoDTONew.setMatrizCurricularId(resumoCursoDTO.getMatrizCurricularId());
			resumoCursoDTONew.setMatrizCurricularString(resumoCursoDTO.getMatrizCurricularString());
			resumoCursoDTONew.setPeriodo(resumoCursoDTO.getPeriodo());

			map2.get(key1).put(key2, resumoCursoDTONew);
			map3.get(key1).put(key2, new HashMap<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>());
		}
	}

	private void createResumoNivel3(
			Map<String, Map<String, Map<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>> map,
			ResumoCursoDTO resumoCursoDTO) {
		String key1 = getKeyNivel1(resumoCursoDTO); // Campus-Turno-Curso
		String key2 = getKeyNivel2(resumoCursoDTO); // Campus-Turno-Curso-Curriculo-Periodo
		String key3 = getKeyNivel3(resumoCursoDTO); // Campus-Turno-Curso-Curriculo-Periodo-Disciplina-Turma-TipoCredito

		Pair<ResumoCursoDTO,List<ResumoCursoDTO>> pair = map.get(key1).get(key2).get(key3);
		if (pair == null) {
			List<ResumoCursoDTO> list = new ArrayList<ResumoCursoDTO>();
			ResumoCursoDTO dtoMain = new ResumoCursoDTO();
			dtoMain.setCampusId(resumoCursoDTO.getCampusId());
			dtoMain.setCampusString(resumoCursoDTO.getCampusString());
			dtoMain.setTurnoId(resumoCursoDTO.getTurnoId());
			dtoMain.setTurnoString(resumoCursoDTO.getTurnoString());
			dtoMain.setCursoId(resumoCursoDTO.getCursoId());
			dtoMain.setCursoString(resumoCursoDTO.getCursoString());
			dtoMain.setMatrizCurricularId(resumoCursoDTO.getMatrizCurricularId());
			dtoMain.setMatrizCurricularString(resumoCursoDTO.getMatrizCurricularString());
			dtoMain.setPeriodo(resumoCursoDTO.getPeriodo());
			dtoMain.setDisciplinaId(resumoCursoDTO.getDisciplinaId());
			dtoMain.setDisciplinaString(resumoCursoDTO.getDisciplinaString());
			dtoMain.setQuantidadeAlunos(resumoCursoDTO.getQuantidadeAlunos());
			dtoMain.setTurma(resumoCursoDTO.getTurma());
			dtoMain.setTipoCreditoTeorico(resumoCursoDTO.getTipoCreditoTeorico());
			pair = Pair.create(dtoMain,list);
			map.get(key1).get(key2).put(key3, pair);
		}
		
		ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();

		resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
		resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
		resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
		resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
		resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
		resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());
		resumoCursoDTONew.setMatrizCurricularId(resumoCursoDTO.getMatrizCurricularId());
		resumoCursoDTONew.setMatrizCurricularString(resumoCursoDTO.getMatrizCurricularString());
		resumoCursoDTONew.setPeriodo(resumoCursoDTO.getPeriodo());
		resumoCursoDTONew.setDisciplinaId(resumoCursoDTO.getDisciplinaId());
		resumoCursoDTONew.setDisciplinaString(resumoCursoDTO.getDisciplinaString());
		resumoCursoDTONew.setQuantidadeAlunos(resumoCursoDTO.getQuantidadeAlunos());
		resumoCursoDTONew.setTurma(resumoCursoDTO.getTurma());
		resumoCursoDTONew.setTipoCreditoTeorico(resumoCursoDTO.getTipoCreditoTeorico());
		resumoCursoDTONew.setCreditos(resumoCursoDTO.getCreditos());
		resumoCursoDTONew.setCustoDocente(resumoCursoDTO.getCustoDocente());
		resumoCursoDTONew.setReceita(resumoCursoDTO.getReceita());
	
		pair.getRight().add(resumoCursoDTONew);
	}

	private Map<String, Double> createRateioMap(List<ResumoCursoDTO> resumoCursoList) {
		// [Disciplina-Turma-DiaSemana-Sala -> List<ResumoCursoDTO>]
		Map<String,List<ResumoCursoDTO>> compartilhamentosTurmasMap = new HashMap<String, List<ResumoCursoDTO>>();
		for (ResumoCursoDTO rcDTO : resumoCursoList) {
			String key = getKeyCompartilhamentoTurma(rcDTO);
			List<ResumoCursoDTO> list = compartilhamentosTurmasMap.get(key);
			if (list == null) {
				list = new ArrayList<ResumoCursoDTO>();
				compartilhamentosTurmasMap.put(key,list);
			}
			list.add(rcDTO);
		}
		
		// [Campus-Turno-Curso-Curriculo-Periodo-Disciplina-Turma-TipoCredito -> Rateio]
		Map<String, Double> rateioMap = new HashMap<String, Double>();
		
		for (Entry<String, List<ResumoCursoDTO>> entry : compartilhamentosTurmasMap.entrySet()) {
			double totalAlunosCompartilhados = 0.0;
			for (ResumoCursoDTO dto : entry.getValue()) {
				totalAlunosCompartilhados += dto.getQuantidadeAlunos();
			}
			
			for (ResumoCursoDTO dto : entry.getValue()) {
				rateioMap.put(getKeyNivel3(dto),dto.getQuantidadeAlunos()/totalAlunosCompartilhados);
			}
		}
		
		return rateioMap;
	}
}
