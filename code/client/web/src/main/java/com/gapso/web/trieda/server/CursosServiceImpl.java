package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;

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
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.CursoDescompartilha;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.services.CursosService;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.dev.util.Pair;

@Transactional
public class CursosServiceImpl
	extends RemoteService implements CursosService {
	private static final long serialVersionUID = 5250776996542788849L;
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CursosService#insereBDProibicaoCompartilhamentoCursos(com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO)
	 */
	@Override
	public void insereBDProibicaoCompartilhamentoCursos(CursoDescompartilhaDTO dto) throws TriedaException {
		try {
			CursoDescompartilha objBD = ConvertBeans.toCursoDescompartilha(dto);
			if (objBD.getId() != null) {
				objBD.merge();
			} else {
				objBD.persist();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CursosService#removeBDProibicaoCompartilhamentoCursos(java.util.List)
	 */
	@Override
	public void removeBDProibicaoCompartilhamentoCursos(List<CursoDescompartilhaDTO> dtos) throws TriedaException {
		try {
			for (CursoDescompartilhaDTO dto : dtos) {
				ConvertBeans.toCursoDescompartilha(dto).remove();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CursosService#getParesCursosQueNaoPermitemCompartilhamentoDeTurmas()
	 */
	@Override
	public List<CursoDescompartilhaDTO> getParesCursosQueNaoPermitemCompartilhamentoDeTurmas(ParametroDTO dto) throws TriedaException {
		try {
			Parametro parametro = ConvertBeans.toParametro(dto);
			List<CursoDescompartilha> paresDeCursosBD = CursoDescompartilha.findAll(parametro);
			
			List<CursoDescompartilhaDTO> paresDeCursosDTO = new ArrayList<CursoDescompartilhaDTO>(paresDeCursosBD.size());
			for (CursoDescompartilha parDeCursosBD : paresDeCursosBD) {
				paresDeCursosDTO.add(ConvertBeans.toCursoDescompartilhaDTO(parDeCursosBD));
			}
			
			return paresDeCursosDTO;
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public CursoDTO getCurso( Long id )
	{
		return ConvertBeans.toCursoDTO(
			Curso.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public ListLoadResult< CursoDTO > getListAll( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< CursoDTO > list = new ArrayList< CursoDTO >();
		List< Curso > cursos = Curso.findByCenario( getInstituicaoEnsinoUser(), cenario );

		for ( Curso curso : cursos )
		{
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO( curso );
			list.add( cursoDTO );
		}

		return new BasePagingLoadResult< CursoDTO >( list );
	}

	@Override
	public ListLoadResult<CursoDTO> getList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig) {
		return getBuscaList(cenarioDTO, null, loadConfig.get("query").toString(), null, 
				null, null, null, null, null, null, null, null, null, null, null,
				loadConfig);
	}

	@Override
	public PagingLoadResult< CursoDTO > getBuscaList( CenarioDTO cenarioDTO, String nome,
		String codigo, TipoCursoDTO tipoCursoDTO, 
		String operadorMinPercentualDoutor, Integer minPercentualDoutor,
		String operadorMinPercentualMestre, Integer minPercentualMestre,
		String operadorMinTempoIntegralParcial, Integer minTempoIntegralParcial,
		String operadorMinTempoIntegral, Integer minTempoIntegral,
		String operadorMaxDisciplinasProfessor, Integer maxDisciplinasProfessor,
		Boolean maisDeUmaDisciplinaProfessor,
		PagingLoadConfig config )
	{
		List< CursoDTO > list = new ArrayList< CursoDTO >();
		String orderBy = config.getSortField();
		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());

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

		List< Curso > listDomains = Curso.findBy( getInstituicaoEnsinoUser(), cenario,
			codigo, nome, tipoCurso, 
			operadorMinPercentualDoutor, minPercentualDoutor,
			operadorMinPercentualMestre, minPercentualMestre,
			operadorMinTempoIntegralParcial, minTempoIntegralParcial,
			operadorMinTempoIntegral, minTempoIntegral,
			operadorMaxDisciplinasProfessor, maxDisciplinasProfessor,
			maisDeUmaDisciplinaProfessor,
			config.getOffset(), config.getLimit(), orderBy );

		for ( Curso curso : listDomains )
		{
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO( curso );

			list.add( cursoDTO );
		}

		BasePagingLoadResult< CursoDTO > result
			= new BasePagingLoadResult< CursoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Curso.count(
			getInstituicaoEnsinoUser(), cenario, codigo, nome, tipoCurso,
			operadorMinPercentualDoutor, minPercentualDoutor,
			operadorMinPercentualMestre, minPercentualMestre,
			operadorMinTempoIntegralParcial, minTempoIntegralParcial,
			operadorMinTempoIntegral, minTempoIntegral,
			operadorMaxDisciplinasProfessor, maxDisciplinasProfessor,
			maisDeUmaDisciplinaProfessor) );

		return result;
	}

	@Override
	public ListLoadResult<CursoDTO> getListByCampi(List<CampusDTO> campiDTOs,
			List<CursoDTO> retirarCursosDTO) {
		List<CursoDTO> list = new ArrayList<CursoDTO>();

		List<Campus> campi = new ArrayList<Campus>(campiDTOs.size());
		for (CampusDTO dto : campiDTOs) {
			campi.add(ConvertBeans.toCampus(dto));
		}

		List< Curso > listDomains = Curso.findByCampi(getInstituicaoEnsinoUser(),campi);

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
	public List<ResumoCursoDTO> getResumos(CampusDTO campusDTO) {
		Campus campus = Campus.find(campusDTO.getId(),this.getInstituicaoEnsinoUser());
		List<Professor> professores = Professor.findAll(this.getInstituicaoEnsinoUser());
		List<AtendimentoTatico> atendimentosTaticos = AtendimentoTatico.findAll(getInstituicaoEnsinoUser());
		List<AtendimentoOperacional> atendimentosOperacionais = AtendimentoOperacional.findAll(getInstituicaoEnsinoUser());
		
		Map<Long,Professor> professoresMap = Professor.getProfessoresMap(professores);
		
		List<Oferta> ofertas = new ArrayList<Oferta>(campus.getOfertas());
		Collections.sort(ofertas);

		// organiza os atendimentos por oferta
		// táticos
		Map<Long,List<AtendimentoTatico>> ofertaIdToAtendimentosTaticosMap = new HashMap<Long,List<AtendimentoTatico>>();
		for (AtendimentoTatico at : atendimentosTaticos) {
			List<AtendimentoTatico> atendimentosDaOferta = ofertaIdToAtendimentosTaticosMap.get(at.getOferta().getId());
			if (atendimentosDaOferta == null) {
				atendimentosDaOferta = new ArrayList<AtendimentoTatico>();
				ofertaIdToAtendimentosTaticosMap.put(at.getOferta().getId(),atendimentosDaOferta);
			}
			atendimentosDaOferta.add(at);
		}
		// operacionais
		Map<Long,List<AtendimentoOperacional>> ofertaIdToAtendimentosOperacionaisMap = new HashMap<Long,List<AtendimentoOperacional>>();
		for (AtendimentoOperacional ao : atendimentosOperacionais) {
			List<AtendimentoOperacional> atendimentosDaOferta = ofertaIdToAtendimentosOperacionaisMap.get(ao.getOferta().getId());
			if (atendimentosDaOferta == null) {
				atendimentosDaOferta = new ArrayList<AtendimentoOperacional>();
				ofertaIdToAtendimentosOperacionaisMap.put(ao.getOferta().getId(),atendimentosDaOferta);
			}
			atendimentosDaOferta.add(ao);
		}
		
		// [Campus-Turno-Curso -> ResumoCursoDTO]
		Map<String,ResumoCursoDTO> nivel1Map = new HashMap<String,ResumoCursoDTO>();
		// [Campus-Turno-Curso -> [Campus-Turno-Curso-Curriculo-Periodo -> ResumoCursoDTO]]
		Map<String,Map<String,ResumoCursoDTO>> nivel2Map = new HashMap<String,Map<String,ResumoCursoDTO>>();
		// [Campus-Turno-Curso -> [Campus-Turno-Curso-Curriculo-Periodo -> [Campus-Turno-Curso-Curriculo-Periodo-Disciplina-Turma-TipoCredito -> ResumoCursoDTO]]]
		Map<String,Map<String,Map<String,Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>> nivel3Map = new HashMap<String,Map<String,Map<String,Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>>();

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		List<ResumoCursoDTO> resumoCursoDTOList = new ArrayList<ResumoCursoDTO>();
		for (Oferta oferta : ofertas) {
			// monta a lista de atendimentos associados a uma oferta
			List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
			List<AtendimentoTatico> atendimentoTaticoList = ofertaIdToAtendimentosTaticosMap.get(oferta.getId());
			boolean ehTatico = true;
			if (atendimentoTaticoList != null && !atendimentoTaticoList.isEmpty()) {
				for (AtendimentoTatico atendimentoTatico : atendimentoTaticoList) {
					aulas.add(ConvertBeans.toAtendimentoTaticoDTO(atendimentoTatico));
				}
			}
			else {
				List<AtendimentoOperacional> atendimentoOperacionalList = ofertaIdToAtendimentosOperacionaisMap.get(oferta.getId());
				if (atendimentoOperacionalList != null) {
					List<AtendimentoOperacionalDTO> atendimentoOperacionalDTOList = new ArrayList<AtendimentoOperacionalDTO>();
					for (AtendimentoOperacional atendimentoOperacional : atendimentoOperacionalList) {
						atendimentoOperacionalDTOList.add(ConvertBeans.toAtendimentoOperacionalDTO(atendimentoOperacional));
					}
					aulas.addAll(service.extraiAulas(atendimentoOperacionalDTOList));
					ehTatico = false;
				}
			}
			
			// A partir dos atendimentos, monta as estruturas nivel1Map, nivel2Map e nivel3Map, além de converter um atendimento
			// em um objeto ResumoCursoDTO e armazená-los em uma lista
			for (AtendimentoRelatorioDTO aula : aulas) {
				ResumoCursoDTO resumoDTO = new ResumoCursoDTO();

				resumoDTO.setOfertaId(oferta.getId());
				resumoDTO.setCampusId(oferta.getCampus().getId());
				resumoDTO.setCampusString(oferta.getCampus().getNome() + "(" + oferta.getCampus().getCodigo() + ")");
				resumoDTO.setTurnoId(aula.getTurnoId());
				resumoDTO.setTurnoString(aula.getTurnoString());
				resumoDTO.setCursoId(aula.getCursoId());
				resumoDTO.setCursoString(aula.getCursoNome() + "(" + aula.getCursoString() + ")");
				resumoDTO.setMatrizCurricularId(aula.getCurriculoId());
				resumoDTO.setMatrizCurricularString(aula.getCurriculoString());
				resumoDTO.setPeriodo(Integer.valueOf(aula.getPeriodoString()));
				resumoDTO.setQuantidadeAlunos(aula.getQuantidadeAlunos());
				resumoDTO.setDisciplinaId(aula.getDisciplinaSubstitutaId() == null ? aula.getDisciplinaId() : aula.getDisciplinaSubstitutaId());
				resumoDTO.setDisciplinaString(aula.getDisciplinaSubstitutaId() == null ? 
						aula.getDisciplinaNome() + "(" + aula.getDisciplinaString() + ")" :
						aula.getDisciplinaSubstitutaNome() + "(" + aula.getDisciplinaSubstitutaString() + ")");
				resumoDTO.setTurma(aula.getTurma());
				resumoDTO.setTipoCreditoTeorico(aula.isTeorico());
				resumoDTO.setCreditos(aula.getTotalCreditos());
				resumoDTO.setSalaId(aula.getSalaId());
				resumoDTO.setSalaString(aula.getSalaString());
				resumoDTO.setDiaSemana(aula.getSemana());
				if (ehTatico) {
					resumoDTO.setCustoDocente(new TriedaCurrency(oferta.getCampus().getValorCredito()));
					resumoDTO.setProfessorCPF("");
					resumoDTO.setProfessorNome("");
				} else {
					Long profId = ((AtendimentoOperacionalDTO)aula).getProfessorId();
					Professor prof = professoresMap.get(profId);
					if (prof != null) {
						resumoDTO.setCustoDocente(new TriedaCurrency(prof.getValorCredito()));
						resumoDTO.setProfessorCPF(prof.getCpf());
						resumoDTO.setProfessorNome(prof.getNome());
					} else {
						resumoDTO.setCustoDocente(new TriedaCurrency(oferta.getCampus().getValorCredito()));
						resumoDTO.setProfessorCPF(aula.getProfessorVirtualId().toString());
						resumoDTO.setProfessorNome(aula.getProfessorVirtualString());
					}
				}
				resumoDTO.setReceita(new TriedaCurrency(oferta.getReceita()));

				createResumoNivel1(nivel1Map,nivel2Map,nivel3Map,resumoDTO);
				createResumoNivel2(nivel2Map,nivel3Map,resumoDTO);
				createResumoNivel3(nivel3Map,resumoDTO);

				resumoCursoDTOList.add(resumoDTO);
			}
		}

		calculaResumo3(nivel3Map,resumoCursoDTOList);
		calculaResumo1e2(nivel1Map,nivel2Map,nivel3Map);

		return createResumoEstrutura(nivel1Map,nivel2Map,nivel3Map);
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
		
		Locale pt_BR = new Locale("pt","BR");
		CurrencyFormatter currencyFormatter = new CurrencyFormatter();

		for (String key1 : map3.keySet()) {
			for (String key2 : map3.get(key1).keySet()) {
				for (String key3 : map3.get(key1).get(key2).keySet()) {
					Pair<ResumoCursoDTO,List<ResumoCursoDTO>> pair = map3.get(key1).get(key2).get(key3);
					Double rateio = rateioMap.get(key3);
					
					ResumoCursoDTO mainDTO = pair.getLeft();
					mainDTO.setRateio(rateio);
					mainDTO.setRateioString(TriedaUtil.round(rateio*100.0,2)+"%");
					
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
						mainDTO.setCustoDocenteString(currencyFormatter.print(mainDTO.getCustoDocente().getDoubleValue(),pt_BR));
						// calcula e acumula receita
						double receitaLocal = TriedaUtil.round(creditos * receita * qtdAlunos * 4.5 * 6.0, 2);
						mainDTO.setReceita(TriedaUtil.parseTriedaCurrency(receitaLocal + mainDTO.getReceita().getDoubleValue()));
						mainDTO.setReceitaString(currencyFormatter.print(mainDTO.getReceita().getDoubleValue(),pt_BR));
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
					mainDTO.setMargemPercenteString(TriedaUtil.round(margemPercent*100.0,2)+"%");
				}
			}
		}
	}

	private void calculaResumo1e2(Map<String, ResumoCursoDTO> map1,
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, Pair<ResumoCursoDTO,List<ResumoCursoDTO>>>>> map3) {
		Locale pt_BR = new Locale("pt","BR");
		CurrencyFormatter currencyFormatter = new CurrencyFormatter();
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

					rc1.setCustoDocente(TriedaUtil.parseTriedaCurrency(rc1.getCustoDocente().getDoubleValue() + rc3.getCustoDocente().getDoubleValue()));
					rc2.setCustoDocente(TriedaUtil.parseTriedaCurrency(rc2.getCustoDocente().getDoubleValue() + rc3.getCustoDocente().getDoubleValue()));
					rc1.setCustoDocenteString(currencyFormatter.print(rc1.getCustoDocente().getDoubleValue(),pt_BR));
					rc2.setCustoDocenteString(currencyFormatter.print(rc2.getCustoDocente().getDoubleValue(),pt_BR));

					rc1.setReceita(TriedaUtil.parseTriedaCurrency(rc1.getReceita().getDoubleValue() + rc3.getReceita().getDoubleValue()));
					rc2.setReceita(TriedaUtil.parseTriedaCurrency(rc2.getReceita().getDoubleValue() + rc3.getReceita().getDoubleValue()));
					rc1.setReceitaString(currencyFormatter.print(rc1.getReceita().getDoubleValue(),pt_BR));
					rc2.setReceitaString(currencyFormatter.print(rc2.getReceita().getDoubleValue(),pt_BR));

					rc1.setMargem(TriedaUtil.parseTriedaCurrency(rc1.getMargem().getDoubleValue() + rc3.getMargem().getDoubleValue()));
					rc2.setMargem(TriedaUtil.parseTriedaCurrency(rc2.getMargem().getDoubleValue() + rc3.getMargem().getDoubleValue()));
					rc1.setMargemString(currencyFormatter.print(rc1.getMargem().getDoubleValue(),pt_BR));
					rc2.setMargemString(currencyFormatter.print(rc2.getMargem().getDoubleValue(),pt_BR));
					
					double doubleValue = 0.0;
					if (Double.compare(rc1.getReceita().getDoubleValue(),0.0) != 0) {
						doubleValue = rc1.getMargem().getDoubleValue() / rc1.getReceita().getDoubleValue();
					}
					rc1.setMargemPercente(doubleValue);
					
					doubleValue = 0.0;
					if (Double.compare(rc2.getReceita().getDoubleValue(),0.0) != 0) {
						doubleValue = rc2.getMargem().getDoubleValue() / rc2.getReceita().getDoubleValue();
					}
					rc2.setMargemPercente(doubleValue);
					
					rc1.setMargemPercenteString(TriedaUtil.round(rc1.getMargemPercente()*100.0,2)+"%");
					rc2.setMargemPercenteString(TriedaUtil.round(rc2.getMargemPercente()*100.0,2)+"%");
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
		return getKeyNivel2(resumoCursoDTO) + "-" + resumoCursoDTO.getDisciplinaId() + "-" + resumoCursoDTO.getTurma() + "-" + resumoCursoDTO.getTipoCreditoTeorico() + "-" + resumoCursoDTO.getProfessorCPF();
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
		String key3 = getKeyNivel3(resumoCursoDTO); // Campus-Turno-Curso-Curriculo-Periodo-Disciplina-Turma-TipoCredito-ProfessorCPF

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
			dtoMain.setProfessorCPF(resumoCursoDTO.getProfessorCPF());
			dtoMain.setProfessorNome(resumoCursoDTO.getProfessorNome());
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
		
		// [Campus-Turno-Curso-Curriculo-Periodo-Disciplina-Turma-TipoCredito-ProfessorCPF -> Rateio]
		Map<String, Double> rateioMap = new HashMap<String, Double>();
		
		for (Entry<String, List<ResumoCursoDTO>> entry : compartilhamentosTurmasMap.entrySet()) {
			double totalAlunosCompartilhados = 0.0;
			Map<String,Integer> keyNivel3ToQtdAlunosMap = new HashMap<String,Integer>();
			for (ResumoCursoDTO dto : entry.getValue()) {
				// aculuma o total de alunos por key nivel 3
				String keyNivel3 = getKeyNivel3(dto);
				Integer qtdAlunos = keyNivel3ToQtdAlunosMap.get(keyNivel3);
				if (qtdAlunos == null) {
					qtdAlunos = 0;
				}
				keyNivel3ToQtdAlunosMap.put(keyNivel3,qtdAlunos+dto.getQuantidadeAlunos());
				
				// acumula o total de alunos
				totalAlunosCompartilhados += dto.getQuantidadeAlunos();
			}
			
			for (String keyNivel3 : keyNivel3ToQtdAlunosMap.keySet()) {
				double qtdAlunos = keyNivel3ToQtdAlunosMap.get(keyNivel3);
				rateioMap.put(keyNivel3,qtdAlunos/totalAlunosCompartilhados);
			}
		}
		
		return rateioMap;
	}
}
