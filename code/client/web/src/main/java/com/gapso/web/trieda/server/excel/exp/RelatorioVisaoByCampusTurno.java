package com.gapso.web.trieda.server.excel.exp;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.Atendimento;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoMap;

public abstract class RelatorioVisaoByCampusTurno extends RelatorioVisaoExportExcel{
	
	private boolean deveProcessarAtendimentosTaticos;
	
	public RelatorioVisaoByCampusTurno(boolean removeUnusedSheets, boolean deveProcessarAtendimentosTaticos,
			Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension);
		this.deveProcessarAtendimentosTaticos = deveProcessarAtendimentosTaticos;
	}
	
	protected abstract <V> boolean addAtendimentosInMap(Atendimento atendimento, Map<V, AtendimentoServiceRelatorioResponse> mapEntity);
	protected abstract <V> void getAtendimentosByFilter(RelatorioVisaoMap<Campus, Turno, V> mapEntity);
	
	protected <V> boolean getAtendimentosByEntity(Atendimento atendimento, RelatorioVisaoMap<Campus, Turno, V> mapControl){
		Map<Turno, Map<V, AtendimentoServiceRelatorioResponse>> mapTurnoControl = mapControl.get(atendimento.getCampus());
		if(mapTurnoControl == null){
			mapTurnoControl = new HashMap<Turno, Map<V, AtendimentoServiceRelatorioResponse>>();
			mapControl.put(atendimento.getCampus(), mapTurnoControl);
		}
		Map<V, AtendimentoServiceRelatorioResponse> mapEntity = mapTurnoControl.get(atendimento.getTurno());
		if(mapEntity == null){
			mapEntity = new HashMap<V, AtendimentoServiceRelatorioResponse>();
			mapTurnoControl.put(atendimento.getTurno(), mapEntity);
		}
		
		return this.addAtendimentosInMap(atendimento, mapEntity);
	}
	
	@Override
	@SuppressWarnings("unchecked")	
	protected <V> boolean getAtendimentosRelatorioDTOList(V mapControlT){
		RelatorioVisaoMap<Campus, Turno, ?> mapControl = (RelatorioVisaoMap<Campus, Turno, ?>) mapControlT;
		boolean result = false;
		
		if(this.getFilter() == null){
			// se nao ha filtro, entao lista todos os alunos associados ao cenario
			Cenario cenario = getCenario();
			List<AtendimentoTatico> atdTaticoList = AtendimentoTatico.findByCenario(this.instituicaoEnsino, cenario);
			if(atdTaticoList.size() != 0){
				if (deveProcessarAtendimentosTaticos) {
					for(AtendimentoTatico atendimento : atdTaticoList){
						if(getAtendimentosByEntity(new Atendimento(atendimento), mapControl)) result = true;
					}
				}
			}
			else{
				List<AtendimentoOperacional> atdOperacionalList = AtendimentoOperacional.findByCenario(this.instituicaoEnsino, cenario);
				for(AtendimentoOperacional atendimento : atdOperacionalList){
					if(getAtendimentosByEntity(new Atendimento(atendimento), mapControl)) result = true;
				}
			}
		}
		else{
			this.getAtendimentosByFilter(mapControl);
			result = true;
		}

		return result;
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook, Workbook templateWorkbook){
		return this.<RelatorioVisaoMap<Campus, Turno, ?>>fillInExcelImpl(workbook, templateWorkbook);
	}
	
	@Override
	@SuppressWarnings("unchecked")
	protected <T> void processStructureReportControl(T mapControlT){
		this.processStructureReportControlImpl((RelatorioVisaoMap<Campus, Turno, ?>) mapControlT);
	}
	
	private <V> void processStructureReportControlImpl(RelatorioVisaoMap<Campus, Turno, V> mapControl){
		// linha a partir da qual a escrita será iniciada no excel
		int nextRow = this.initialRow;
		
		// varre estrutura para coletar disciplinas envolvidas
		Set<Long> disciplinas = new HashSet<Long>();
		for(Campus campus : mapControl.keySet()){
			Map<Turno, Map<V, AtendimentoServiceRelatorioResponse>> turnoMap = mapControl.get(campus);
			for(Turno turno : turnoMap.keySet()){
				Map<V, AtendimentoServiceRelatorioResponse> entityMap = sortEntityMap(turnoMap.get(turno));
				for(V entity : entityMap.keySet()){
					AtendimentoServiceRelatorioResponse quinteto = entityMap.get(entity);
					List<AtendimentoRelatorioDTO> atendimentos = quinteto.getAtendimentosDTO();
					for (AtendimentoRelatorioDTO atendimento : atendimentos) {
						Long disciplinaId = atendimento.getDisciplinaSubstitutaId() != null ? atendimento.getDisciplinaSubstitutaId() : atendimento.getDisciplinaId();
						disciplinas.add(disciplinaId);
					}
				}
			}
		}
		buildCodigoDisciplinaToColorMap(disciplinas);

		for(Campus campus : mapControl.keySet()){
			Map<Turno, Map<V, AtendimentoServiceRelatorioResponse>> turnoMap = mapControl.get(campus);
			for(Turno turno : turnoMap.keySet()){
				Map<V, AtendimentoServiceRelatorioResponse> entityMap = sortEntityMap(turnoMap.get(turno));
				for(V entity : entityMap.keySet()){
					AtendimentoServiceRelatorioResponse quinteto = entityMap.get(entity);
					
					List<AtendimentoRelatorioDTO> atendimentos = quinteto.getAtendimentosDTO();
					if(atendimentos.isEmpty()) continue;
					
					ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quinteto.getMdcTemposAulaNumSemanasLetivas();
					List<String> horariosDaGradeHoraria = quinteto.getLabelsDasLinhasDaGradeHoraria();
					List<String> horariosDeInicioDeAula = quinteto.getHorariosDeInicioDeAula();
					List<String> horariosDeFimDeAula = quinteto.getHorariosDeFimDeAula();
					boolean ehTatico = atendimentos.get(0) instanceof AtendimentoTaticoDTO;

					nextRow = writeEntity(campus, turno, entity, atendimentos, nextRow, mdcTemposAulaNumSemanasLetivas.getPrimeiro(), ehTatico, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
				}
			}
		}
	}
	
	protected abstract <V> Map<V, AtendimentoServiceRelatorioResponse> sortEntityMap(Map<V, AtendimentoServiceRelatorioResponse> unsortMap);
	
	protected abstract <T> int writeEntity(Campus campus, Turno turno, T entity, List<AtendimentoRelatorioDTO> atendimentos,
		int row, int mdcTemposAula, boolean ehTatico, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula
	);
	
}
