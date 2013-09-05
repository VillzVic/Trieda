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
		return this.<Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>>>fillInExcelImpl(workbook, templateWorkbook);
	}
	
	private boolean getAtendimentosRelatorioDTOListByAluno(Campus campus, Turno turno, Set<AlunoDemanda> alunosDemanda,
		Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>> mapControl,
		Map<Long,CampusDTO> campusIdToCampusDTOMap,
		Map<Long,TurnoDTO> turnoIdToTurnoDTOMap,
		AtendimentosServiceImpl service,
		IAtendimentosServiceDAO dao)
	{
		boolean result = false;
		
		Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>> mapTurnoControl = mapControl.get(campus);
		if(mapTurnoControl == null){
			mapTurnoControl = new HashMap<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>();
			mapControl.put(campus, mapTurnoControl);
		}
		Map<Aluno, AtendimentoServiceRelatorioResponse> mapAluno = mapTurnoControl.get(turno);
		if(mapAluno == null){
			mapAluno = new HashMap<Aluno, AtendimentoServiceRelatorioResponse>();
			mapTurnoControl.put(turno, mapAluno);
		}
		
		for(AlunoDemanda alunoDemanda : alunosDemanda){
			Aluno aluno = alunoDemanda.getAluno();
			AtendimentoServiceRelatorioResponse quinteto = mapAluno.get(aluno);
			if(quinteto == null){
				RelatorioVisaoAlunoFiltro filtro = new RelatorioVisaoAlunoFiltro();
				filtro.setAlunoMatricula(aluno.getMatricula());
				filtro.setAlunoNome(aluno.getNome());
				
				try {
					quinteto = service.getAtendimentosParaGradeHorariaVisaoAluno(filtro,dao);
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
		Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>> mapControl = 
			(Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>>) mapControlT;
		
		boolean result = false;
		
		if(this.getFilter() == null){
			Map<Long,CampusDTO> campusIdToCampusDTOMap = new HashMap<Long,CampusDTO>();
			Map<Long,TurnoDTO> turnoIdToCampusDTOMap = new HashMap<Long,TurnoDTO>();
			
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
				// preenche estrutura para organizar os atendimentos por [aluno-turno-campus]
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
					public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(String alunoNome, String alunoMatricula) {
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
					public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(String alunoNome, String alunoMatricula) {return Collections.<AtendimentoOperacionalDTO>emptyList();}
					
					@Override
					public Map<Long,SemanaLetiva> buscaSemanasLetivas() {
						return finalSemanaLetivasMap;
					}
				};
				
				for(AtendimentoTatico atendimento : atdTaticoList){
					if(getAtendimentosRelatorioDTOListByAluno(atendimento.getOferta().getCampus(), 
						atendimento.getOferta().getTurno(), atendimento.getAlunosDemanda(), mapControl,
						campusIdToCampusDTOMap, turnoIdToCampusDTOMap, service, daoTatico)) result = true;
				}
			}
			else{
				List<AtendimentoOperacional> atdOperacionalList = new ArrayList<AtendimentoOperacional>(cenario.getAtendimentosOperacionais());//AtendimentoOperacional.findByCenario(this.instituicaoEnsino, cenario);
				// preenche estrutura para organizar os atendimentos por [aluno-turno-campus]
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
					public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {return Collections.<AtendimentoTaticoDTO>emptyList();}
					@Override
					public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {return Collections.<AtendimentoOperacionalDTO>emptyList();}
					@Override
					public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(String alunoNome, String alunoMatricula) {return Collections.<AtendimentoTaticoDTO>emptyList();}
					
					@Override
					public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(String alunoNome, String alunoMatricula) {
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
						atendimento.getOferta().getTurno(), atendimento.getAlunosDemanda(), mapControl,
						campusIdToCampusDTOMap, turnoIdToCampusDTOMap, service, daoOp)) result = true;
				}
			}
		}
		else{
			AtendimentosServiceImpl service = new AtendimentosServiceImpl();
			AtendimentoServiceRelatorioResponse quinteto;
			RelatorioVisaoAlunoFiltro filter = this.getFilter();
			
			try {
				quinteto = service.getAtendimentosParaGradeHorariaVisaoAluno(filter);
				
				Map<Aluno, AtendimentoServiceRelatorioResponse> mapAluno = new HashMap<Aluno, AtendimentoServiceRelatorioResponse>();
				mapAluno.put(Aluno.findOneByNomeMatricula(instituicaoEnsino, filter.getAlunoNome(), filter.getAlunoMatricula()), quinteto);
				Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>> mapTurnoControl = 
					new HashMap<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>();
				mapTurnoControl.put(Turno.find(quinteto.getAtendimentosDTO().get(0).getTurnoId(), instituicaoEnsino), mapAluno);
				mapControl.put(Campus.find(quinteto.getAtendimentosDTO().get(0).getCampusId(), instituicaoEnsino), mapTurnoControl);
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
	protected Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>> getStructureReportControl(){
		return new HashMap<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>>();
	}
	
	@SuppressWarnings("unchecked")
	protected <T> void processStructureReportControl(T mapControlT){
		Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>> mapControl = 
			(Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>>) mapControlT;
		// linha a partir da qual a escrita será iniciada no excel
		int nextRow = this.initialRow;
		
		// varre estrutura para coletar disciplinas envolvidas
		Set<Long> disciplinas = new HashSet<Long>();
		for(Campus campus : mapControl.keySet()){
			Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>> turnoMap = mapControl.get(campus);
			for(Turno turno : turnoMap.keySet()){
				Map<Aluno, AtendimentoServiceRelatorioResponse> alunoMap = turnoMap.get(turno);
				for(Aluno aluno : alunoMap.keySet()){
					AtendimentoServiceRelatorioResponse quinteto = alunoMap.get(aluno);
					List<AtendimentoRelatorioDTO> atendimentos = quinteto.getAtendimentosDTO();
					for (AtendimentoRelatorioDTO atendimento : atendimentos) {
						Long disciplinaId = atendimento.getDisciplinaSubstitutaId() != null ? atendimento.getDisciplinaSubstitutaId() : atendimento.getDisciplinaId();
						disciplinas.add(disciplinaId);
					}
				}
			}
		}
		buildCodigoDisciplinaToColorMap(disciplinas);

		// para cada bloco curricular
		for(Campus campus : mapControl.keySet()){
			Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>> turnoMap = mapControl.get(campus);
			for(Turno turno : turnoMap.keySet()){
				Map<Aluno, AtendimentoServiceRelatorioResponse> alunoMap = turnoMap.get(turno);
				for(Aluno aluno : alunoMap.keySet()){
					AtendimentoServiceRelatorioResponse quinteto = alunoMap.get(aluno);
					ParDTO<Integer, Integer> mdcTemposAulaNumSemanasLetivas = quinteto.getMdcTemposAulaNumSemanasLetivas();
					List<AtendimentoRelatorioDTO> atendimentos = quinteto.getAtendimentosDTO();
					List<String> horariosDaGradeHoraria = quinteto.getLabelsDasLinhasDaGradeHoraria();
					List<String> horariosDeInicioDeAula = quinteto.getHorariosDeInicioDeAula();
					List<String> horariosDeFimDeAula = quinteto.getHorariosDeFimDeAula();
					
					if(atendimentos.isEmpty()) continue;
					
					boolean ehTatico = atendimentos.get(0) instanceof AtendimentoTaticoDTO;

					nextRow = writeAluno(campus, turno, aluno, atendimentos, nextRow, mdcTemposAulaNumSemanasLetivas.getPrimeiro(), ehTatico, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
				}
			}
		}
	}

	@Override
	protected int writeHeader(List<List<ParDTO<String,?>>> rowsHeadersPairs, int row, boolean temInfoDeHorarios) {
		// mescla células relativas ao nome do aluno
		mergeCells(row+1,row+1,4,6,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()]);
		return super.writeHeader(rowsHeadersPairs,row,temInfoDeHorarios);
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

	private int writeAluno(Campus campus, Turno turno, Aluno aluno, List<AtendimentoRelatorioDTO> atendimentos, int row, int mdcTemposAula, 
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
		row = writeHeader(getRowsHeadersPairs(campus, turno, aluno), row, temInfoDeHorarios);
		
		return writeAulas(atendimentos, row, mdcTemposAula, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
	}
	
	protected void onWriteAula(int row, int col, AtendimentoRelatorioDTO aula) {
		// informação para hyperlink
		hyperlinkInfo.add(TrioDTO.create(row,col,aula.getSalaString()));
	}
		
	protected List<List<ParDTO<String, ?>>> getRowsHeadersPairs(Campus campus, Turno turno, Aluno aluno){
		List<List<ParDTO<String, ?>>> list = new ArrayList<List<ParDTO<String, ?>>>(); 
		
		List<ParDTO<String, ?>> row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().campus(), campus.getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().turno(), turno.getNome()));
		
		list.add(row);
		
		row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().nomeAluno(), aluno.getNome()));
		
		list.add(row);
		
		return list;
	}
}