package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.AtendimentosServiceImpl.IAtendimentosServiceDAO;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;

public class RelatorioVisaoCursoExportExcel	extends RelatorioVisaoExportExcel{
	private List<Integer> qtdColunasPorDiaSemana;
	protected RelatorioVisaoCursoFiltro relatorioFiltro;
	private List<TrioDTO<Integer,Integer,String>> hyperlinkInfo;

	public RelatorioVisaoCursoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension);
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();
	}
	
	public RelatorioVisaoCursoExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension);
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();
	}

	protected String getReportSheetName(){
		return ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName();
	}

	protected String getReportEntity(){
		return "Curso";
	}
	
	@Override
	protected RelatorioVisaoCursoFiltro getFilter(){
		return this.relatorioFiltro;
	}
	
	@Override
	protected void setFilter(ExportExcelFilter filter){
		this.relatorioFiltro = (RelatorioVisaoCursoFiltro) filter;
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook){
		return this.<List<AtendimentoServiceRelatorioResponse>>fillInExcelImpl(workbook);
	}
	
	protected List<AtendimentoRelatorioDTO> getAtendimentosRelatorioDTOFromCenario(Cenario cenario) {
		Set<AtendimentoTatico> atendimentosTatico = cenario.getAtendimentosTaticos();//;AtendimentoTatico.findByCenario( this.instituicaoEnsino, cenario );
		Set<AtendimentoOperacional> atendimentosOperacional = cenario.getAtendimentosOperacionais();//;AtendimentoOperacional.findByCenario( this.instituicaoEnsino, cenario );
		
		List<AtendimentoRelatorioDTO> atendimentos = new ArrayList<AtendimentoRelatorioDTO>(atendimentosTatico.size()+atendimentosOperacional.size());
		for (AtendimentoTatico atdTatico : atendimentosTatico) {
			atendimentos.add(ConvertBeans.toAtendimentoTaticoDTO(atdTatico));
		}
		for (AtendimentoOperacional atdOperacional : atendimentosOperacional) {
			atendimentos.add(ConvertBeans.toAtendimentoOperacionalDTO(atdOperacional));
		}
		
		return atendimentos;
	}

	public Set<Map<String,Object>> opcoesBuscaOperacional(Cenario cenario, Map<String,List<AtendimentoRelatorioDTO>> atendimentosMap) {
		List<AtendimentoRelatorioDTO> atendimentos = getAtendimentosRelatorioDTOFromCenario(cenario);

		Set<Map<String,Object>> opcoes = new HashSet<Map<String,Object>>();
		for (AtendimentoRelatorioDTO atendimentoRelatorio : atendimentos) {
			Map<String,Object> opcao = new HashMap<String,Object>();

			opcao.put("CurriculoDTO",atendimentoRelatorio.getCurriculoId());
			Integer periodo = Integer.valueOf(atendimentoRelatorio.getPeriodoString());
			opcao.put("Periodo",periodo);
			opcao.put("TurnoDTO",atendimentoRelatorio.getTurnoId());
			opcao.put("CampusDTO",atendimentoRelatorio.getCampusId());
			opcao.put("CursoDTO",atendimentoRelatorio.getCursoId());

			opcoes.add(opcao);
			
			String key = atendimentoRelatorio.getCurriculoId() + "-" + periodo + "-" + atendimentoRelatorio.getTurnoId() + "-" + atendimentoRelatorio.getCampusId() + "-" + atendimentoRelatorio.getCursoId();
			List<AtendimentoRelatorioDTO> atendimentosDaKey = atendimentosMap.get(key);
			if (atendimentosDaKey == null) {
				atendimentosDaKey = new ArrayList<AtendimentoRelatorioDTO>();
				atendimentosMap.put(key,atendimentosDaKey);
			}
			atendimentosDaKey.add(atendimentoRelatorio);
		}

		return opcoes;
	}

	private Set< Map< String, Object > > filtraAtendimentos( Set< Map< String, Object > > opcoes )
	{
		Set< Map< String, Object > > result = new HashSet< Map< String, Object > >();

		for ( Map< String, Object > opcao : opcoes )
		{
			Long curriculoId = (Long) opcao.get( "CurriculoDTO" );
			Integer periodo = (Integer) opcao.get( "Periodo" );
			Long turnoId = (Long) opcao.get( "TurnoDTO" );
			Long campusId = (Long) opcao.get( "CampusDTO" );
			Long cursoId = (Long) opcao.get( "CursoDTO" );
			
			RelatorioVisaoCursoFiltro filter = (RelatorioVisaoCursoFiltro) this.getFilter();

			if ( curriculoId == filter.getCurriculoDTO().getId()
				&& periodo == filter.getPeriodo()
				&& turnoId == filter.getTurnoDTO().getId()
				&& campusId == filter.getCampusDTO().getId()
				&& cursoId == filter.getCursoDTO().getId() )
			{
				result.add( opcao );
			}
		}

		return result;
	}

	@SuppressWarnings("unchecked")
	protected <T> boolean getAtendimentosRelatorioDTOList(T mapControlT){
		Cenario cenario = getCenario();
		
		// cria estruturas auxiliares
		// [CurriculoId -> Curriculo]
		Map<Long,Curriculo> curriculoIdToCurriculoMap = new HashMap<Long,Curriculo>();
		for (Curriculo curriculo : cenario.getCurriculos()) {
			curriculoIdToCurriculoMap.put(curriculo.getId(),curriculo);
		}
		// [TurnoId -> Turno]
		Map<Long,Turno> turnoIdToTurnoMap = new HashMap<Long,Turno>();
		for (Turno turno : cenario.getTurnos()) {
			turnoIdToTurnoMap.put(turno.getId(),turno);
		}
		// [CampusId -> Campus]
		Map<Long,Campus> campusIdToCampusMap = new HashMap<Long,Campus>();
		for (Campus campus : cenario.getCampi()) {
			campusIdToCampusMap.put(campus.getId(),campus);
		}
		// [CursoId -> Curso]
		Map<Long,Curso> cursoIdToCursoMap = new HashMap<Long,Curso>();
		for (Curso curso : cenario.getCursos()) {
			cursoIdToCursoMap.put(curso.getId(),curso);
		}
		Map<String,List<AtendimentoRelatorioDTO>> atendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		
		// map de semanas letivas
		Map<Long,SemanaLetiva> semanaLetivasMap = new HashMap<Long,SemanaLetiva>();
		for (Turno turno : cenario.getTurnos()) {
			for (HorarioAula ha : turno.getHorariosAula()) {
				SemanaLetiva sl = ha.getSemanaLetiva();
				semanaLetivasMap.put(sl.getId(),sl);
			}
		}
		final Map<Long,SemanaLetiva> finalSemanaLetivasMap = semanaLetivasMap;
		
		Set<Map<String,Object>> opcoes = opcoesBuscaOperacional(cenario,atendimentosMap);
		if (this.relatorioFiltro != null) {
			opcoes = this.filtraAtendimentos(opcoes);
		}
	
		final Map<String,List<AtendimentoRelatorioDTO>> finalAtendimentosMap = atendimentosMap;
		IAtendimentosServiceDAO dao = new IAtendimentosServiceDAO() {
			@Override
			public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {
				List<AtendimentoTaticoDTO> atendimentosTaticoDTO = new ArrayList<AtendimentoTaticoDTO>();
				String key = curriculoDTO.getId() + "-" + periodo + "-" + turnoDTO.getId() + "-" + campusDTO.getId() + "-" + cursoDTO.getId();
				List<AtendimentoRelatorioDTO> atendimentosDaKey = finalAtendimentosMap.get(key);
				if (atendimentosDaKey != null) {
					for (AtendimentoRelatorioDTO at : atendimentosDaKey) {
						atendimentosTaticoDTO.add((AtendimentoTaticoDTO)at);
					}
				}				
				return atendimentosTaticoDTO;
			}
			
			@Override
			public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {
				List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = new ArrayList<AtendimentoOperacionalDTO>();
				String key = curriculoDTO.getId() + "-" + periodo + "-" + turnoDTO.getId() + "-" + campusDTO.getId() + "-" + cursoDTO.getId();
				List<AtendimentoRelatorioDTO> atendimentosDaKey = finalAtendimentosMap.get(key);
				if (atendimentosDaKey != null) {
					for (AtendimentoRelatorioDTO at : atendimentosDaKey) {
						atendimentosOperacionalDTO.add((AtendimentoOperacionalDTO)at);
					}
				}				
				return atendimentosOperacionalDTO;
			}

			@Override
			public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(AlunoDTO alunoDTO, TurnoDTO turnoDTO, CampusDTO campusDTO) {return null;}
			@Override
			public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(AlunoDTO alunoDTO, TurnoDTO turnoDTO, CampusDTO campusDTO) {return null;}

			@Override
			public Map<Long, SemanaLetiva> buscaSemanasLetivas() {
				return finalSemanaLetivasMap;
			}
		};

		List<AtendimentoServiceRelatorioResponse> aulasInfo = (List<AtendimentoServiceRelatorioResponse>) mapControlT;
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		for (Map<String, Object> opcao : opcoes) {
			RelatorioVisaoCursoFiltro filtro = new RelatorioVisaoCursoFiltro();
			
			Long curriculoId = (Long)opcao.get("CurriculoDTO");
			Curriculo curriculo = curriculoIdToCurriculoMap.get(curriculoId);//Curriculo.find(curriculoId,this.instituicaoEnsino);
			filtro.setCurriculoDTO(ConvertBeans.toCurriculoDTO(curriculo));

			filtro.setPeriodo((Integer) opcao.get("Periodo"));

			Long turnoId = (Long)opcao.get("TurnoDTO");
			Turno turno = turnoIdToTurnoMap.get(turnoId);//Turno.find(turnoId, this.instituicaoEnsino);
			filtro.setTurnoDTO(ConvertBeans.toTurnoDTO(turno));

			Long campusId = (Long)opcao.get("CampusDTO");
			Campus campus = campusIdToCampusMap.get(campusId);//Campus.find(campusId, this.instituicaoEnsino);
			filtro.setCampusDTO(ConvertBeans.toCampusDTO(campus));

			Long cursoId = (Long)opcao.get("CursoDTO");
			Curso curso = cursoIdToCursoMap.get(cursoId);//Curso.find(cursoId, this.instituicaoEnsino);
			filtro.setCursoDTO(ConvertBeans.toCursoDTO(curso));

			AtendimentoServiceRelatorioResponse sexteto = service.getAtendimentosParaGradeHorariaVisaoCurso(filtro,dao);
			aulasInfo.add(sexteto);
		}
		
		return !aulasInfo.isEmpty();
	}
	
	@SuppressWarnings("unchecked")
	protected List<AtendimentoServiceRelatorioResponse> getStructureReportControl(){
		return new ArrayList<AtendimentoServiceRelatorioResponse>();
	}
	
	@SuppressWarnings("unchecked")
	protected <T> void processStructureReportControl(T mapControlT){
		List<AtendimentoServiceRelatorioResponse> aulasInfo = (List<AtendimentoServiceRelatorioResponse>) mapControlT;
		// linha a partir da qual a escrita será iniciada no excel
		int nextRow = this.initialRow;
		
		// cria estrutura auxiliares
		// [OfertaId -> Oferta]
		Map<Long,Oferta> ofertaIdToOfertaMap = new HashMap<Long,Oferta>();
		for (Campus campus : getCenario().getCampi()) {
			for (Oferta oferta : campus.getOfertas()) {
				ofertaIdToOfertaMap.put(oferta.getId(),oferta);
			}
		}
		
		// varre estrutura para coletar disciplinas envolvidas
		Set<Long> disciplinas = new HashSet<Long>();
		for(AtendimentoServiceRelatorioResponse sexteto : aulasInfo) {
			List<AtendimentoRelatorioDTO> aulas = sexteto.getAtendimentosDTO();
			for (AtendimentoRelatorioDTO aula : aulas) {
				Long disciplinaId = aula.getDisciplinaSubstitutaId() != null ? aula.getDisciplinaSubstitutaId() : aula.getDisciplinaId();
				disciplinas.add(disciplinaId);
			}
		}
		buildCodigoDisciplinaToColorMap(disciplinas);

		// para cada bloco curricular
		for(AtendimentoServiceRelatorioResponse sexteto : aulasInfo) {
			Integer mdcTemposAula = sexteto.getMdcTemposAula();
			List<AtendimentoRelatorioDTO> aulas = sexteto.getAtendimentosDTO();
			List<Integer> qtdColunasPorDiaSemana = sexteto.getQtdColunasPorDiaSemana();
			List<String> horariosDaGradeHoraria = sexteto.getLabelsDasLinhasDaGradeHoraria();
			List<String> horariosDeInicioDeAula = sexteto.getHorariosDeInicioDeAula();
			List<String> horariosDeFimDeAula = sexteto.getHorariosDeFimDeAula();
			
			if(aulas.isEmpty()) continue;

			Oferta oferta = ofertaIdToOfertaMap.get(aulas.get(0).getOfertaId());//Oferta.find(aulas.get(0).getOfertaId(),this.instituicaoEnsino);
			Integer periodo = Integer.valueOf(aulas.get(0).getPeriodoString());
			boolean ehTatico = aulas.get(0) instanceof AtendimentoTaticoDTO;

			nextRow = writeCurso(oferta, periodo, mdcTemposAula, aulas, qtdColunasPorDiaSemana, nextRow, ehTatico, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
		}
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName());
		if (mapLevel2 != null && !mapLevel2.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			for (Entry<String,Map<String,String>> entry : mapLevel2.entrySet()) {
				String cellValue = entry.getKey();
				if (cellValue.equals(ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName())) { 
					for (TrioDTO<Integer,Integer,String> trio : hyperlinkInfo) {
						String cellHyperlink = entry.getValue().get(trio.getTerceiro());
						if (cellHyperlink != null) {
							setCellWithHyperlink(trio.getPrimeiro(),trio.getSegundo(),sheet,cellHyperlink,false);
						}
					}
				}
			}
		}
		hyperlinkInfo.clear();
	}
	
	private int writeCurso(Oferta oferta, Integer periodo, Integer mdcTemposAula, List<AtendimentoRelatorioDTO> aulas, 
		List<Integer> qtdColunasPorDiaSemana, int row, boolean ehTatico, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula)
	{
		registerHyperlink(
			ExcelInformationType.DEMANDAS.getSheetName(),
			ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName(),
			oferta.getNaturalKeyString()+"-"+periodo, 
			"'"+ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName()+"'!B"+row
		);
		registerHyperlink(
			ExcelInformationType.DEMANDAS_POR_ALUNO.getSheetName(),
			ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName(),
			oferta.getNaturalKeyString()+"-"+periodo, 
			"'"+ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName()+"'!B"+row
		);
		
		boolean temInfoDeHorarios = !aulas.isEmpty() ? (aulas.iterator().next().getHorarioAulaId() != null) : false;
		
		// escreve cabeçalho da grade horária do bloco curricular
		this.qtdColunasPorDiaSemana = qtdColunasPorDiaSemana;
		row = writeHeader(getRowsHeadersPairs(oferta, periodo), row, temInfoDeHorarios);

		return writeAulas(aulas, row, mdcTemposAula, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
	}
	
	protected void onWriteAula(int row, int col, AtendimentoRelatorioDTO aula) {
		// informação para hyperlink
		hyperlinkInfo.add(TrioDTO.create(row,col,aula.getSalaString()));
	}
	
	protected List<List<ParDTO<String, ?>>> getRowsHeadersPairs(Oferta oferta, Integer periodo){
		List<List<ParDTO<String, ?>>> list = new ArrayList<List<ParDTO<String, ?>>>(); 
		
		List<ParDTO<String, ?>> row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().curso(), oferta.getCurriculo().getCurso().getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().campus(), oferta.getCampus().getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().turno(), oferta.getTurno().getNome()));
		
		list.add(row);
		
		row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().matrizCurricular(), oferta.getCurriculo().getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().periodo(), periodo));
		
		list.add(row);
		
		return list;
	}

	protected int setSemanasCell(int col, int row){
		for(Semanas semanas : Semanas.values()){
			int qtd = qtdColunasPorDiaSemana.get(Semanas.toInt(semanas));
			setCell(row, col, sheet, semanas.name());
			CellStyle style = this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()];
			mergeCells(row, row, col, col + qtd - 1, sheet, style);
			col = col + qtd;
		}

		return ++row;
	}

}
