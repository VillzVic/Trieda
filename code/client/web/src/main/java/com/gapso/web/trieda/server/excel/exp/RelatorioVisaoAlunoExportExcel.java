package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.AtendimentosServiceImpl.IAtendimentosServiceDAO;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
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
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;

public class RelatorioVisaoAlunoExportExcel	extends RelatorioVisaoExportExcel{
	RelatorioVisaoAlunoFiltro relatorioFiltro;
	private List<TrioDTO<Integer,Integer,String>> hyperlinkInfo;
	
	public RelatorioVisaoAlunoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension);
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();
	}
	
	public RelatorioVisaoAlunoExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension);
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();
	}

	protected String getReportSheetName(){
		return ExcelInformationType.RELATORIO_VISAO_ALUNO.getSheetName();
	}
	
	protected String getReportEntity(){
		return "Aluno";
	}

	@Override
	protected RelatorioVisaoAlunoFiltro getFilter(){
		return this.relatorioFiltro;
	}
	
	@Override
	protected void setFilter(ExportExcelFilter filter){
		this.relatorioFiltro = (RelatorioVisaoAlunoFiltro) filter;
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook, Workbook templateWorkbook){
		return this.<Map<Campus, Map<Aluno, AtendimentoServiceRelatorioResponse>>>fillInExcelImpl(workbook, templateWorkbook);
	}
	
	private boolean getAtendimentosRelatorioDTOListByAluno(Campus campus, Set<AlunoDemanda> alunosDemanda,
		Map<Campus, Map<Aluno, AtendimentoServiceRelatorioResponse>> mapControl,
		Map<Long,CampusDTO> campusIdToCampusDTOMap,
		AtendimentosServiceImpl service,
		IAtendimentosServiceDAO dao)
	{
		boolean result = false;
		
		Map<Aluno, AtendimentoServiceRelatorioResponse> mapAluno = mapControl.get(campus);
		if(mapAluno == null){
			mapAluno = new HashMap<Aluno, AtendimentoServiceRelatorioResponse>();
			mapControl.put(campus, mapAluno);
		}
		
		for(AlunoDemanda alunoDemanda : alunosDemanda){
			Aluno aluno = alunoDemanda.getAluno();
			AtendimentoServiceRelatorioResponse quinteto = mapAluno.get(aluno);
			if(quinteto == null){
				RelatorioVisaoAlunoFiltro filtro = new RelatorioVisaoAlunoFiltro();
				filtro.setAlunoMatricula(aluno.getMatricula());
				filtro.setAlunoNome(aluno.getNome());
				
				try {
					quinteto = service.getAtendimentosParaGradeHorariaVisaoAluno(ConvertBeans.toCenarioDTO(getCenario()), filtro,dao);
				} catch (Exception e) {
					String msg = "";
					if (e instanceof EmptyResultDataAccessException) {
						msg = "Os campos do digitados no filtro não foram encontrados";
					}
					else {
						msg = ( e.getMessage() + ( e.getCause() != null ?
								e.getCause().getMessage() : "" ) );
					}
					
					this.errors.add( getI18nMessages().excelErroGenericoExportacao(
						msg ) );
					e.printStackTrace();
				}
				mapAluno.put(aluno, quinteto);
				
				result = true;
			}
		}
		
		return result;
	}

	@SuppressWarnings("unchecked")
	protected <T> boolean getAtendimentosRelatorioDTOList(T mapControlT){
		Map<Campus, Map<Aluno, AtendimentoServiceRelatorioResponse>> mapControl = 
			(Map<Campus,  Map<Aluno, AtendimentoServiceRelatorioResponse>>) mapControlT;
		
		boolean result = false;
		
		if(this.getFilter() == null){
			Map<Long,CampusDTO> campusIdToCampusDTOMap = new HashMap<Long,CampusDTO>();
			
			AtendimentosServiceImpl service = new AtendimentosServiceImpl();
			Cenario cenario = getCenario();
			
			// map de semanas letivas
			Map<Long,SemanaLetiva> semanaLetivasMap = new HashMap<Long,SemanaLetiva>();
			for (Turno turno : cenario.getTurnos()) { 
				for (HorarioAula ha : turno.getHorariosAula()) {
					SemanaLetiva sl = ha.getSemanaLetiva();
					semanaLetivasMap.put(sl.getId(),sl);
				}
			}
			final Map<Long,SemanaLetiva> finalSemanaLetivasMap = semanaLetivasMap;
			
			// se nao ha filtro, entao lista todos os alunos associados ao cenario
			List<AtendimentoTatico> atdTaticoList = new ArrayList<AtendimentoTatico>(cenario.getAtendimentosTaticos());//AtendimentoTatico.findByCenario(this.instituicaoEnsino, cenario);
			if(atdTaticoList.size() != 0){
				// preenche estrutura para organizar os atendimentos por [aluno-campus]
				Map<String,List<AtendimentoRelatorioDTO>> atendimentosDTOMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
				for (AtendimentoTatico atdTatico : atdTaticoList) {
					AtendimentoTaticoDTO dto = ConvertBeans.toAtendimentoTaticoDTO(atdTatico);
					for (AlunoDemandaDTO aldDTO : dto.getAlunosDemandas()) {
						String key = aldDTO.getAlunoString() + "-" + aldDTO.getAlunoMatricula(); 
						List<AtendimentoRelatorioDTO> atendimentosDaKey = atendimentosDTOMap.get(key);
						if (atendimentosDaKey == null) {
							atendimentosDaKey = new ArrayList<AtendimentoRelatorioDTO>();
							atendimentosDTOMap.put(key,atendimentosDaKey);
						}
						atendimentosDaKey.add(dto);
					}
				}
				
				// cria DAO
				final Map<String,List<AtendimentoRelatorioDTO>> finalAtendimentosDTOMap = atendimentosDTOMap;
				IAtendimentosServiceDAO daoTatico = new IAtendimentosServiceDAO() {
					@Override
					public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {return Collections.<AtendimentoTaticoDTO>emptyList();}
					@Override
					public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {return Collections.<AtendimentoOperacionalDTO>emptyList();}

					@Override
					public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(Cenario cenario, String alunoNome, String alunoMatricula) {
						List<AtendimentoTaticoDTO> atendimentosTaticoDTO = new ArrayList<AtendimentoTaticoDTO>();
						String key = alunoNome + "-" + alunoMatricula;
						List<AtendimentoRelatorioDTO> atendimentosDaKey = finalAtendimentosDTOMap.get(key);
						if (atendimentosDaKey != null) {
							for (AtendimentoRelatorioDTO at : atendimentosDaKey) {
								atendimentosTaticoDTO.add((AtendimentoTaticoDTO)at);
							}
						}				
						return atendimentosTaticoDTO;
					}
					
					@Override
					public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(Cenario cenario, String alunoNome, String alunoMatricula) {return Collections.<AtendimentoOperacionalDTO>emptyList();}
					
					@Override
					public Map<Long,SemanaLetiva> buscaSemanasLetivas() {
						return finalSemanaLetivasMap;
					}
				};
				
				for(AtendimentoTatico atendimento : atdTaticoList){
					if(getAtendimentosRelatorioDTOListByAluno(atendimento.getOferta().getCampus(), 
						atendimento.getAlunosDemanda(), mapControl,
						campusIdToCampusDTOMap, service, daoTatico)) result = true;
				}
			}
			else{
				List<AtendimentoOperacional> atdOperacionalList = new ArrayList<AtendimentoOperacional>(cenario.getAtendimentosOperacionais());//AtendimentoOperacional.findByCenario(this.instituicaoEnsino, cenario);
				// preenche estrutura para organizar os atendimentos por [aluno-campus]
				Map<String,List<AtendimentoRelatorioDTO>> atendimentosDTOMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
				for (AtendimentoOperacional atdOp : atdOperacionalList) {
					AtendimentoOperacionalDTO dto = ConvertBeans.toAtendimentoOperacionalDTO(atdOp);
					for (AlunoDemandaDTO aldDTO : dto.getAlunosDemandas()) {
						String key = aldDTO.getAlunoString() + "-" + aldDTO.getAlunoMatricula();
						List<AtendimentoRelatorioDTO> atendimentosDaKey = atendimentosDTOMap.get(key);
						if (atendimentosDaKey == null) {
							atendimentosDaKey = new ArrayList<AtendimentoRelatorioDTO>();
							atendimentosDTOMap.put(key,atendimentosDaKey);
						}
						atendimentosDaKey.add(dto);
					}
				}
				
				// cria DAO
				final Map<String,List<AtendimentoRelatorioDTO>> finalAtendimentosDTOMap = atendimentosDTOMap;
				IAtendimentosServiceDAO daoOp = new IAtendimentosServiceDAO() {
					@Override
					public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO,  CampusDTO campusDTO, CursoDTO cursoDTO) {return Collections.<AtendimentoTaticoDTO>emptyList();}
					@Override
					public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO,  CampusDTO campusDTO, CursoDTO cursoDTO) {return Collections.<AtendimentoOperacionalDTO>emptyList();}
					@Override
					public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(Cenario cenario, String alunoNome, String alunoMatricula) {return Collections.<AtendimentoTaticoDTO>emptyList();}
					
					@Override
					public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(Cenario cenario, String alunoNome, String alunoMatricula) {
						List<AtendimentoOperacionalDTO> atendimentosOpDTO = new ArrayList<AtendimentoOperacionalDTO>();
						String key = alunoNome + "-" + alunoMatricula;
						List<AtendimentoRelatorioDTO> atendimentosDaKey = finalAtendimentosDTOMap.get(key);
						if (atendimentosDaKey != null) {
							for (AtendimentoRelatorioDTO at : atendimentosDaKey) {
								atendimentosOpDTO.add((AtendimentoOperacionalDTO)at);
							}
						}				
						return atendimentosOpDTO;
					}
					
					@Override
					public Map<Long,SemanaLetiva> buscaSemanasLetivas() {
						return finalSemanaLetivasMap;
					}
				};
				
				for(AtendimentoOperacional atendimento : atdOperacionalList){
					if(getAtendimentosRelatorioDTOListByAluno(atendimento.getOferta().getCampus(), 
						atendimento.getAlunosDemanda(), mapControl,
						campusIdToCampusDTOMap, service, daoOp)) result = true;
				}
			}
		}
		else{
			AtendimentosServiceImpl service = new AtendimentosServiceImpl();
			AtendimentoServiceRelatorioResponse quinteto;
			RelatorioVisaoAlunoFiltro filter = this.getFilter();
			
			try {
				quinteto = service.getAtendimentosParaGradeHorariaVisaoAluno(ConvertBeans.toCenarioDTO(getCenario()), filter);
				
				Map<Aluno, AtendimentoServiceRelatorioResponse> mapAluno = new HashMap<Aluno, AtendimentoServiceRelatorioResponse>();
				mapAluno.put(Aluno.findOneByNomeMatricula(instituicaoEnsino, getCenario(), filter.getAlunoNome(), filter.getAlunoMatricula()), quinteto);
				mapControl.put(Campus.find(quinteto.getAtendimentosDTO().get(0).getCampusId(), instituicaoEnsino), mapAluno);
			} catch (Exception e) {
				String msg = "";
				if (e instanceof EmptyResultDataAccessException) {
					msg = "Os campos do digitados no filtro não foram encontrados";
				}
				else {
					msg = ( e.getMessage() + ( e.getCause() != null ?
							e.getCause().getMessage() : "" ) );
				}
				
				this.errors.add( getI18nMessages().excelErroGenericoExportacao(
					msg ) );
				e.printStackTrace();
			}
			
			result = true;
		}

		return result;
	}
	
	@SuppressWarnings("unchecked")
	protected Map<Campus, Map<Aluno, AtendimentoServiceRelatorioResponse>> getStructureReportControl(){
		return new HashMap<Campus,Map<Aluno, AtendimentoServiceRelatorioResponse>>();
	}
	
	@SuppressWarnings("unchecked")
	protected <T> void processStructureReportControl(T mapControlT){
		Map<Campus,  Map<Aluno, AtendimentoServiceRelatorioResponse>> mapControl = 
			(Map<Campus,  Map<Aluno, AtendimentoServiceRelatorioResponse>>) mapControlT;
		// linha a partir da qual a escrita será iniciada no excel
		int nextRow = this.initialRow;
		
		// varre estrutura para coletar disciplinas envolvidas
		Set<Long> disciplinas = new HashSet<Long>();
		for(Campus campus : mapControl.keySet()){
			Map<Aluno, AtendimentoServiceRelatorioResponse> alunoMap = mapControl.get(campus);
			for(Aluno aluno : alunoMap.keySet()){
				AtendimentoServiceRelatorioResponse quinteto = alunoMap.get(aluno);
				List<AtendimentoRelatorioDTO> atendimentos = quinteto.getAtendimentosDTO();
				for (AtendimentoRelatorioDTO atendimento : atendimentos) {
					Long disciplinaId = atendimento.getDisciplinaSubstitutaId() != null ? atendimento.getDisciplinaSubstitutaId() : atendimento.getDisciplinaId();
					disciplinas.add(disciplinaId);
				}
			}
		}
		buildCodigoDisciplinaToColorMap(disciplinas);

		// para cada bloco curricular
		for(Campus campus : mapControl.keySet()){
			Map<Aluno, AtendimentoServiceRelatorioResponse> alunoMap = mapControl.get(campus);
			for(Aluno aluno : alunoMap.keySet()){
				AtendimentoServiceRelatorioResponse quinteto = alunoMap.get(aluno);
				ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quinteto.getMdcTemposAulaNumSemanasLetivas();
				List<AtendimentoRelatorioDTO> atendimentos = quinteto.getAtendimentosDTO();
				List<String> horariosDaGradeHoraria = quinteto.getLabelsDasLinhasDaGradeHoraria();
				List<String> horariosDeInicioDeAula = quinteto.getHorariosDeInicioDeAula();
				List<String> horariosDeFimDeAula = quinteto.getHorariosDeFimDeAula();
				
				if(atendimentos.isEmpty()) continue;
				
				boolean ehTatico = atendimentos.get(0) instanceof AtendimentoTaticoDTO;

				nextRow = writeAluno(campus, aluno, atendimentos, nextRow, mdcTemposAulaNumSemanasLetivas.getPrimeiro(), ehTatico, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
			}
		}
	}

	protected int writeHeaderAlunos(List<List<TrioDTO<String,Integer,?>>> rowsHeadersPairs, int row, boolean temInfoDeHorarios) {
		// mescla células relativas ao nome do aluno
		mergeCells(row,row,4,5,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()]);
		mergeCells(row,row,7,9,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()]);
		mergeCells(row+1,row+1,4,5,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()]);

		
		for(List<TrioDTO<String, Integer, ?>> headersPairs : rowsHeadersPairs){
			for(TrioDTO<String, Integer, ?> headerPair : headersPairs){
				setCell(row, headerPair.getSegundo(), sheet, this.cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],
					HtmlUtils.htmlUnescape(headerPair.getPrimeiro())
				);
				if(headerPair.getTerceiro() instanceof String)
					setCell(row, headerPair.getSegundo()+1, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],
						(String) headerPair.getTerceiro()
					);
				else if(headerPair.getSegundo() instanceof Integer)
					setCell(row, headerPair.getSegundo()+1, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],
						(Integer) headerPair.getTerceiro()
					);
				else
					setCell(row, headerPair.getSegundo()+1, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],
						(Double) headerPair.getTerceiro()
					);
			}
			row++;
		}

		int col = 2;

		// Créditos ou Horários
		setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()],
			HtmlUtils.htmlUnescape(temInfoDeHorarios ? this.getI18nConstants().horarios() : this.getI18nConstants().cargaHorariaMinutos()));

		// Dias Semana
		return setSemanasCell(col, row);
		
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.RELATORIO_VISAO_ALUNO.getSheetName());
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

	private int writeAluno(Campus campus, Aluno aluno, List<AtendimentoRelatorioDTO> atendimentos, int row, int mdcTemposAula, 
		boolean ehTatico, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula)
	{
		registerHyperlink(
			ExcelInformationType.DEMANDAS_POR_ALUNO.getSheetName(),
			ExcelInformationType.RELATORIO_VISAO_ALUNO.getSheetName(),
			aluno.getMatricula(), 
			"'"+ExcelInformationType.RELATORIO_VISAO_ALUNO.getSheetName()+"'!B"+row
		);
		
		boolean temInfoDeHorarios = !atendimentos.isEmpty() ? (atendimentos.iterator().next().getHorarioAulaId() != null) : false;
	
		// escreve cabeçalho da grade horária da sala
		row = writeHeaderAlunos(getRowsHeadersPairs(campus, aluno), row, temInfoDeHorarios);
		
		return writeAulas(atendimentos, row, mdcTemposAula, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
	}
	
	protected void onWriteAula(int row, int col, AtendimentoRelatorioDTO aula) {
		// informação para hyperlink
		hyperlinkInfo.add(TrioDTO.create(row,col,aula.getSalaString()));
	}
		
	protected List<List<TrioDTO<String, Integer,?>>> getRowsHeadersPairs(Campus campus, Aluno aluno){
		List<List<TrioDTO<String, Integer, ?>>> list = new ArrayList<List<TrioDTO<String, Integer, ?>>>(); 
		
		List<TrioDTO<String, Integer, ?>> row = new ArrayList<TrioDTO<String, Integer, ?>>();
		row.add(TrioDTO.create(this.getI18nConstants().campus(),3, campus.getCodigo()));
		row.add(TrioDTO.create(this.getI18nConstants().curso(),6, ""));
		
		list.add(row);
		
		row = new ArrayList<TrioDTO<String, Integer, ?>>();
		row.add(TrioDTO.create(this.getI18nConstants().nomeAluno(),3, aluno.getNome()));
		row.add(TrioDTO.create(this.getI18nConstants().matriculaAluno(),6, aluno.getMatricula()));
		row.add(TrioDTO.create(this.getI18nConstants().formando(),8, aluno.getFormando() ? getI18nConstants().sim()
				: HtmlUtils.htmlUnescape(getI18nConstants().nao())));
		
		list.add(row);
		
		return list;
	}
}