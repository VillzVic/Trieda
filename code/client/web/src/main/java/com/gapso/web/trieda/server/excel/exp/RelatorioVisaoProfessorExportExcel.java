package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.dao.EmptyResultDataAccessException;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.Atendimento;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.ProfessorWrapper;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoMap;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;


public class RelatorioVisaoProfessorExportExcel	extends RelatorioVisaoByCampusTurno{
	RelatorioVisaoProfessorFiltro relatorioFiltro;
	private boolean isVisaoProfessor;
	private List<TrioDTO<Integer,Integer,String>> hyperlinkInfo;

	public RelatorioVisaoProfessorExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		this(true, cenario, i18nConstants, i18nMessages, null, isVisaoProfessor, instituicaoEnsino, fileExtension);
	}

	public RelatorioVisaoProfessorExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter, boolean isVisaoProfessor, 
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		this(true, cenario, i18nConstants, i18nMessages, filter, isVisaoProfessor, instituicaoEnsino, fileExtension);
	}

	public RelatorioVisaoProfessorExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, boolean isVisaoProfessor, 
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		this(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, isVisaoProfessor, instituicaoEnsino, fileExtension);
	}

	public RelatorioVisaoProfessorExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, ExportExcelFilter filter, 
		boolean isVisaoProfessor, InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(removeUnusedSheets, false, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension);

		this.isVisaoProfessor = isVisaoProfessor;
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();
	}
	
	protected String getReportSheetName(){
		return ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName();
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}

	protected String getReportEntity(){
		return "Professor";
	}
	
	@Override
	protected RelatorioVisaoProfessorFiltro getFilter(){
		return this.relatorioFiltro;
	}
	
	@Override
	protected void setFilter(ExportExcelFilter filter){
		this.relatorioFiltro = (RelatorioVisaoProfessorFiltro) filter;
	}
	
	@Override
	@SuppressWarnings("unchecked")
	protected <V> boolean addAtendimentosInMap(Atendimento atendimento, Map<V, AtendimentoServiceRelatorioResponse> mapEntity){
		ProfessorWrapper professor = null;
		Professor atProf = atendimento.getProfessor();
		if(atProf.getId() == null) professor = new ProfessorWrapper(atendimento.getProfessorVirtual());
		else professor = new ProfessorWrapper(atProf);
		
		AtendimentoServiceRelatorioResponse quinteto = mapEntity.get((V) professor);
		if(quinteto == null){
			RelatorioVisaoProfessorFiltro filtro = new RelatorioVisaoProfessorFiltro();
			if(professor.getProfessor() != null)filtro.setProfessorNome(professor.getProfessor().getNome());
			if(professor.getProfessor() != null)filtro.setProfessorCpf(professor.getProfessor().getCpf());
			if(professor.getProfessorVirtual() != null)filtro.setProfessorVirtualDTO(ConvertBeans.toProfessorVirtualDTO(professor.getProfessorVirtual()));
			filtro.setTurnoDTO(ConvertBeans.toTurnoDTO(atendimento.getTurno()));
		
			AtendimentosServiceImpl service = new AtendimentosServiceImpl();
			try {
				quinteto = service.getAtendimentosParaGradeHorariaVisaoProfessor(filtro, this.isVisaoProfessor);
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
			mapEntity.put((V) professor, quinteto);
			
			return true;
		}
		
		return false;
	}
	
	@SuppressWarnings("unchecked")
	protected <V> void getAtendimentosByFilter(RelatorioVisaoMap<Campus, Turno, V> mapEntity){
		RelatorioVisaoMap<Campus, Turno, ProfessorWrapper> mapControl = 
			(RelatorioVisaoMap<Campus, Turno, ProfessorWrapper>) mapEntity;
	
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		AtendimentoServiceRelatorioResponse quinteto;
		
		try {
			ProfessorWrapper professor = null;
			Professor atProf = Professor.findByNomeCpf(instituicaoEnsino, this.relatorioFiltro.getProfessorNome(), this.relatorioFiltro.getProfessorCpf());
			if(atProf == null) professor = new ProfessorWrapper(ConvertBeans.toProfessorVirtual(this.relatorioFiltro.getProfessorVirtualDTO()));
			else professor = new ProfessorWrapper(atProf);

			quinteto = service.getAtendimentosParaGradeHorariaVisaoProfessor(this.relatorioFiltro, this.isVisaoProfessor);
			Map<ProfessorWrapper, AtendimentoServiceRelatorioResponse> mapAluno = new HashMap<ProfessorWrapper, AtendimentoServiceRelatorioResponse>();
			mapAluno.put(professor, quinteto);
			Map<Turno, Map<ProfessorWrapper, AtendimentoServiceRelatorioResponse>> mapTurnoControl = 
				new HashMap<Turno, Map<ProfessorWrapper, AtendimentoServiceRelatorioResponse>>();
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
	}
	
	@SuppressWarnings("unchecked")
	protected RelatorioVisaoMap<Campus, Turno, ProfessorWrapper> getStructureReportControl(){
		return new RelatorioVisaoMap<Campus, Turno, ProfessorWrapper>();
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName());
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
	
	@SuppressWarnings({"unchecked", "rawtypes"})
	protected <V> Map<V, AtendimentoServiceRelatorioResponse> sortEntityMap(Map<V, AtendimentoServiceRelatorioResponse> unsortMap){
		List<ProfessorWrapper> list = new LinkedList(unsortMap.keySet());
		
        Collections.<ProfessorWrapper>sort(list, new Comparator<ProfessorWrapper>(){
             public int compare(ProfessorWrapper o1, ProfessorWrapper o2){
            	 if(o1.isVirtual()){
            		 return (!o2.isVirtual()) ? -1 : o1.getProfessorVirtual().getNome().compareTo(o2.getProfessorVirtual().getNome());
            	 }
            	 return (o2.isVirtual()) ? 1 : o1.getProfessor().getNome().compareTo(o2.getProfessor().getNome());
             }
        });
 
		Map<V, AtendimentoServiceRelatorioResponse> sortedMap = new LinkedHashMap<V, AtendimentoServiceRelatorioResponse>();
		for(ProfessorWrapper professor : list){
		     sortedMap.put((V) professor, unsortMap.get(professor));
		}
	
		return sortedMap;
	}

	protected <T> int writeEntity(Campus campus, Turno turno, T entity, List<AtendimentoRelatorioDTO> atendimentos, int row, 
		int mdcTemposAula, boolean ehTatico, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula)
	{
		String professorKey;
		ProfessorWrapper professor = (ProfessorWrapper) entity;
		
		if(professor.getProfessor() == null) professorKey = professor.getProfessorVirtual().getId().toString() + "#";
		else professorKey = professor.getProfessor().getId().toString();
		
		registerHyperlink(
			ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName(),
			ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName(),
			professorKey, 
			"'"+ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName()+"'!B"+row
		);
		
		boolean temInfoDeHorarios = !atendimentos.isEmpty() ? (atendimentos.iterator().next().getHorarioAulaId() != null) : false;
		
		row = writeHeader(getRowsHeadersPairs(campus, professor.getProfessor(), professor.getProfessorVirtual(), turno), row, temInfoDeHorarios);
		
		return writeAulas(atendimentos, row, mdcTemposAula, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
	}
	
	protected void onWriteAula(int row, int col, AtendimentoRelatorioDTO aula) {
		// informação para hyperlink
		hyperlinkInfo.add(TrioDTO.create(row,col,aula.getSalaString()));
	}
	
	protected List<List<ParDTO<String, ?>>> getRowsHeadersPairs(Campus campus, Professor professor, 
		ProfessorVirtual professorVirtual, Turno turno)
	{
		List<List<ParDTO<String, ?>>> list = new ArrayList<List<ParDTO<String, ?>>>(); 
		
		String professorNome, professorVirtualNome;
		if(professor == null){
			professorNome = "";
			professorVirtualNome = professorVirtual.getNome();
		}
		else{
			professorNome = professor.getNome();
			professorVirtualNome = "";
		}
		
		List<ParDTO<String, ?>> row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().campus(), campus.getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().professor(), professorNome));
		
		list.add(row);
		
		row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().turno(), turno.getNome()));
		row.add(ParDTO.create(this.getI18nConstants().professorVirtual(), professorVirtualNome));
		
		list.add(row);
		
		return list;
	}

}
