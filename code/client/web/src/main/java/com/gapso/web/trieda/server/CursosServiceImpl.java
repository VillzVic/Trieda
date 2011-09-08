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
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.services.CursosService;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

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
		Campus campus = Campus.find(
			campusDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< Oferta > ofertas
			= new ArrayList< Oferta >( campus.getOfertas() );
		Collections.sort( ofertas );

		List< ResumoCursoDTO > resumoCursoDTOList
			= new ArrayList< ResumoCursoDTO >();

		Map< String, ResumoCursoDTO > nivel1Map
			= new HashMap< String, ResumoCursoDTO >();

		Map< String, Map< String, ResumoCursoDTO > > nivel2Map
			= new HashMap< String, Map< String, ResumoCursoDTO > >();

		Map< String, Map< String, Map< String, ResumoCursoDTO > > > nivel3Map
			= new HashMap< String, Map< String, Map< String, ResumoCursoDTO > > >();

		for ( Oferta oferta : ofertas )
		{
			List< AtendimentoRelatorioDTO > atendimentoRelatorioDTOList
				= new ArrayList< AtendimentoRelatorioDTO >();

			List< AtendimentoTatico > atendimentoTaticoList
				= AtendimentoTatico.findAllBy( getInstituicaoEnsinoUser(), oferta );

			if (!atendimentoTaticoList.isEmpty()) {
				for (AtendimentoTatico atendimentoTatico : atendimentoTaticoList) {
					atendimentoRelatorioDTOList.add(ConvertBeans
							.toAtendimentoTaticoDTO(atendimentoTatico));
				}
			}
			else
			{
				List< AtendimentoOperacional > atendimentoOperacionalList
					= AtendimentoOperacional.findAllBy( oferta, getInstituicaoEnsinoUser() );

				for ( AtendimentoOperacional atendimentoOperacional : atendimentoOperacionalList )
				{
					atendimentoRelatorioDTOList.add(
						ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
				}
			}

			for ( AtendimentoRelatorioDTO atendimentoRelatorioDTO : atendimentoRelatorioDTOList )
			{
				ResumoCursoDTO resumoDTO = new ResumoCursoDTO();

				resumoDTO.setOfertaId(oferta.getId());
				resumoDTO.setCampusId(oferta.getCampus().getId());
				resumoDTO.setCampusString(oferta.getCampus().getNome());
				resumoDTO.setTurnoId(atendimentoRelatorioDTO.getTurnoId());
				resumoDTO.setTurnoString(atendimentoRelatorioDTO
						.getTurnoString());
				resumoDTO.setCursoId(atendimentoRelatorioDTO.getCursoId());
				resumoDTO
						.setCursoString(atendimentoRelatorioDTO.getCursoNome());
				resumoDTO.setMatrizCurricularId(atendimentoRelatorioDTO
						.getCurriculoId());
				resumoDTO.setMatrizCurricularString(atendimentoRelatorioDTO
						.getCurriculoString());
				resumoDTO.setPeriodo(Integer.valueOf(atendimentoRelatorioDTO
						.getPeriodoString()));
				resumoDTO.setQuantidadeAlunos(atendimentoRelatorioDTO
						.getQuantidadeAlunos());
				resumoDTO.setDisciplinaId(atendimentoRelatorioDTO
						.getDisciplinaId());
				resumoDTO.setDisciplinaString(atendimentoRelatorioDTO
						.getDisciplinaString());
				resumoDTO.setTurma(atendimentoRelatorioDTO.getTurma());
				resumoDTO.setTipoCreditoTeorico(atendimentoRelatorioDTO
						.isTeorico());
				resumoDTO.setCreditos(atendimentoRelatorioDTO
						.getTotalCreditos());
				resumoDTO.setCustoDocente(new TriedaCurrency(oferta.getCampus()
						.getValorCredito()));
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
			Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3) {
		List<ResumoCursoDTO> list = new ArrayList<ResumoCursoDTO>();

		for (String key1 : map1.keySet()) {
			ResumoCursoDTO rc1DTO = map1.get(key1);
			list.add(rc1DTO);

			for (String key2 : map2.get(key1).keySet()) {
				ResumoCursoDTO rc2DTO = map2.get(key1).get(key2);
				rc1DTO.add(rc2DTO);

				for (String key3 : map3.get(key1).get(key2).keySet()) {
					ResumoCursoDTO rc3DTO = map3.get(key1).get(key2).get(key3);
					rc2DTO.add(rc3DTO);
				}
			}
		}

		return list;
	}

	private void calculaResumo3(
			Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3,
			List<ResumoCursoDTO> atendimentoResumoList) {
		Map<String, Double> rateioMap = createRateioMap(atendimentoResumoList);

		for (String key1 : map3.keySet()) {
			for (String key2 : map3.get(key1).keySet()) {
				for (String key3 : map3.get(key1).get(key2).keySet()) {
					ResumoCursoDTO resumo3DTO = map3.get(key1).get(key2)
							.get(key3);

					Double rateio = TriedaUtil.roundTwoDecimals(rateioMap
							.get(key3));
					Double docente = resumo3DTO.getCustoDocente()
							.getDoubleValue();
					Double receita = resumo3DTO.getReceita().getDoubleValue();
					int qtdAlunos = resumo3DTO.getQuantidadeAlunos();
					int creditos = resumo3DTO.getCreditos();
					resumo3DTO.setRateio(rateio);

					resumo3DTO.setCustoDocente(TriedaUtil
							.parseTriedaCurrency(creditos * docente * rateio
									* 4.5 * 6));

					resumo3DTO.setReceita(TriedaUtil
							.parseTriedaCurrency(receita * creditos * qtdAlunos
									* 4.5 * 6));

					resumo3DTO.setMargem(TriedaUtil
							.parseTriedaCurrency(resumo3DTO.getReceita()
									.getDoubleValue()
									- resumo3DTO.getCustoDocente()
											.getDoubleValue()));

					resumo3DTO
							.setMargemPercente(TriedaUtil
									.roundTwoDecimals(resumo3DTO.getMargem()
											.getDoubleValue()
											/ resumo3DTO.getReceita()
													.getDoubleValue()));
				}
			}
		}
	}

	private void calculaResumo1e2(Map<String, ResumoCursoDTO> map1,
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3) {
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
					ResumoCursoDTO rc3 = map3.get(key1).get(key2).get(key3);

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
		String key1 = resumoCursoDTO.getCampusId() + "-"
				+ resumoCursoDTO.getTurnoId() + "-"
				+ resumoCursoDTO.getCursoId();

		return key1;
	}

	private String getKeyNivel2(ResumoCursoDTO resumoCursoDTO) {
		String key1 = getKeyNivel1(resumoCursoDTO);

		String key2 = key1 + "-" + resumoCursoDTO.getMatrizCurricularId() + "-"
				+ resumoCursoDTO.getPeriodo();

		return key2;
	}

	private String getKeyNivel3(ResumoCursoDTO resumoCursoDTO) {
		String key2 = getKeyNivel2(resumoCursoDTO);

		String key3 = key2 + "-" + resumoCursoDTO.getDisciplinaId() + "-"
				+ resumoCursoDTO.getTurma() + "-"
				+ resumoCursoDTO.getTipoCreditoTeorico();

		return key3;
	}

	private void createResumoNivel1(Map<String, ResumoCursoDTO> map1,
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3,
			ResumoCursoDTO resumoCursoDTO) {
		String key1 = getKeyNivel1(resumoCursoDTO);

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
			map3.put(key1, new HashMap<String, Map<String, ResumoCursoDTO>>());
		}
	}

	private void createResumoNivel2(
			Map<String, Map<String, ResumoCursoDTO>> map2,
			Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3,
			ResumoCursoDTO resumoCursoDTO) {
		String key1 = getKeyNivel1(resumoCursoDTO);
		String key2 = getKeyNivel2(resumoCursoDTO);

		if (!map2.get(key1).containsKey(key2)) {
			ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();

			resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
			resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
			resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
			resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
			resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
			resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());
			resumoCursoDTONew.setMatrizCurricularId(resumoCursoDTO
					.getMatrizCurricularId());
			resumoCursoDTONew.setMatrizCurricularString(resumoCursoDTO
					.getMatrizCurricularString());
			resumoCursoDTONew.setPeriodo(resumoCursoDTO.getPeriodo());

			map2.get(key1).put(key2, resumoCursoDTONew);
			map3.get(key1).put(key2, new HashMap<String, ResumoCursoDTO>());
		}
	}

	private void createResumoNivel3(
			Map<String, Map<String, Map<String, ResumoCursoDTO>>> map,
			ResumoCursoDTO resumoCursoDTO) {
		String key1 = getKeyNivel1(resumoCursoDTO);
		String key2 = getKeyNivel2(resumoCursoDTO);
		String key3 = getKeyNivel3(resumoCursoDTO);

		if (!map.get(key1).get(key2).containsKey(key3)) {
			ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();

			resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
			resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
			resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
			resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
			resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
			resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());
			resumoCursoDTONew.setMatrizCurricularId(resumoCursoDTO
					.getMatrizCurricularId());
			resumoCursoDTONew.setMatrizCurricularString(resumoCursoDTO
					.getMatrizCurricularString());
			resumoCursoDTONew.setPeriodo(resumoCursoDTO.getPeriodo());
			resumoCursoDTONew.setDisciplinaId(resumoCursoDTO.getDisciplinaId());
			resumoCursoDTONew.setDisciplinaString(resumoCursoDTO
					.getDisciplinaString());
			resumoCursoDTONew.setQuantidadeAlunos(resumoCursoDTO
					.getQuantidadeAlunos());
			resumoCursoDTONew.setTurma(resumoCursoDTO.getTurma());
			resumoCursoDTONew.setTipoCreditoTeorico(resumoCursoDTO
					.getTipoCreditoTeorico());
			resumoCursoDTONew.setCreditos(resumoCursoDTO.getCreditos());
			resumoCursoDTONew.setCustoDocente(resumoCursoDTO.getCustoDocente());
			resumoCursoDTONew.setReceita(resumoCursoDTO.getReceita());

			map.get(key1).get(key2).put(key3, resumoCursoDTONew);
		}
	}

	private Map<String, Double> createRateioMap(
			List<ResumoCursoDTO> resumoCursoList) {
		Map<String, List<ResumoCursoDTO>> rateioAuxMap = new HashMap<String, List<ResumoCursoDTO>>();

		for (ResumoCursoDTO rcDTO : resumoCursoList) {
			String key3 = getKeyNivel3(rcDTO);

			List<ResumoCursoDTO> listDto = rateioAuxMap.get(key3);

			if (listDto == null) {
				listDto = new ArrayList<ResumoCursoDTO>();
				rateioAuxMap.put(key3, listDto);
			}

			// Adiciona um resumo curso a uma linha do nível 3
			listDto.add(rcDTO);
		}

		Map<String, Double> rateioMap = new HashMap<String/* Chave do nível 3 */, Double/*
																					 * Rateio
																					 * do
																					 * curso
																					 */>();

		for (Entry<String, List<ResumoCursoDTO>> key_ListResumos : rateioAuxMap
				.entrySet()) {
			Double total = 0.0;

			List<ResumoCursoDTO> listResumos = key_ListResumos.getValue();

			Map<Long, Double> rateioAux2Map = new HashMap<Long /* Id do Curso */, Double /*
																						 * Alunos
																						 * do
																						 * Curso
																						 */>();

			for (ResumoCursoDTO rcDTO : listResumos) {
				// Quantidade atual de alunos do curso
				Double qtdAlunos = rateioAux2Map.get(rcDTO.getCursoId());

				// Incrementa o número de alunos do curso no total de alunos
				// desse curso
				rateioAux2Map.put(rcDTO.getCursoId(),
						rcDTO.getQuantidadeAlunos()
								+ (qtdAlunos == null ? 0.0 : qtdAlunos));

				total += rcDTO.getQuantidadeAlunos();
			}

			for (ResumoCursoDTO rcDTO : listResumos) {
				// Parta cada linha do nível 3, procura
				// pelo número de alunos de cada curso
				// relacionados à disciplina e turma em questão,
				// em relação ao total de alunos da disciplina e turma
				String key = getKeyNivel3(rcDTO);
				Double numerador = rateioAux2Map.get(rcDTO.getCursoId());
				rateioMap.put(key, numerador / total);
			}
		}

		return rateioMap;
	}
}
