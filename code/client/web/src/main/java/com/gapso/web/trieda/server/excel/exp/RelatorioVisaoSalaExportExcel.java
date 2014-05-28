package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.dao.EmptyResultDataAccessException;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.QuartetoDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;

public class RelatorioVisaoSalaExportExcel extends RelatorioVisaoExportExcel{
	RelatorioVisaoSalaFiltro relatorioFiltro;
	private List<TrioDTO<Integer,Integer,String>> hyperlinkInfo;
	
	public RelatorioVisaoSalaExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension);
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();
	}
	
	public RelatorioVisaoSalaExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension);
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();
	}

	protected String getReportSheetName(){
		return ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName();
	}
	
	protected String getReportEntity(){
		return "Sala";
	}

	@Override
	protected RelatorioVisaoSalaFiltro getFilter(){
		return this.relatorioFiltro;
	}
	
	@Override
	protected void setFilter(ExportExcelFilter filter){
		this.relatorioFiltro = (RelatorioVisaoSalaFiltro) filter;
	}

	@SuppressWarnings("unchecked")
	protected <T> boolean getAtendimentosRelatorioDTOList(T mapControlT){
		List<AtendimentoRelatorioDTO> atdRelatorioList = (List< AtendimentoRelatorioDTO >) mapControlT;

		List< AtendimentoTatico > atdTaticoList = null;
		List< AtendimentoOperacional > atdOperacionalList = null;
		Cenario cenario = getCenario();
		
		if(this.getFilter() == null){
			atdTaticoList = AtendimentoTatico.findByCenario(this.instituicaoEnsino, cenario);
			atdOperacionalList = AtendimentoOperacional.findByCenario(this.instituicaoEnsino, cenario);
		}
		else{
			try {
				RelatorioVisaoSalaFiltro filter = (RelatorioVisaoSalaFiltro) this.getFilter();
	
				Sala sala = Sala.findByCodigo(this.instituicaoEnsino, cenario, filter.getSalaCodigo());
	
				atdTaticoList = AtendimentoTatico.findByCenario(this.instituicaoEnsino, cenario, sala, null);
	
				atdOperacionalList = AtendimentoOperacional.findByCenario(this.instituicaoEnsino, cenario, sala, null);
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

		List<AtendimentoTaticoDTO> listAtendTaticoDTO = ConvertBeans.toListAtendimentoTaticoDTO(atdTaticoList);
		List<AtendimentoOperacionalDTO> listAtendOpDTO = ConvertBeans.toListAtendimentoOperacionalDTO(atdOperacionalList);

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		listAtendOpDTO = service.ordenaPorHorarioAula(listAtendOpDTO);

		atdRelatorioList.addAll(listAtendTaticoDTO);
		atdRelatorioList.addAll(listAtendOpDTO);

		return !atdRelatorioList.isEmpty();
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook, Workbook templateWorkbook){
		return this.<List<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>>>fillInExcelImpl(workbook, templateWorkbook);
	}

	@SuppressWarnings("unchecked")
	protected List<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>> getStructureReportControl(){
		return new ArrayList<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>>();
	}
	
	@SuppressWarnings("unchecked")
	protected <T> void processStructureReportControl(T mapControlT){
		List<AtendimentoRelatorioDTO> atendimentosDTO = 
			(List<AtendimentoRelatorioDTO>) mapControlT;
		// linha a partir da qual a escrita será iniciada no excel
		int nextRow = this.initialRow;
		
		boolean ehTatico = atendimentosDTO.get(0) instanceof AtendimentoTaticoDTO;
		boolean temInfoDeHorarios = !atendimentosDTO.isEmpty() ? (atendimentosDTO.iterator().next().getHorarioAulaId() != null) : false;

		// Estruturas auxiliares
		// [SalaId -> Sala]
		Map<Long,Sala> salaIdToSalaMap = new HashMap<Long,Sala>();
		// [SemanaLetivaId -> SemanaLetiva]
		Map<Long,SemanaLetiva> semanaLetivaIdTosemanaLetivaMap = new HashMap<Long,SemanaLetiva>();
		// [SalaId -> [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]]
		Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>> atendimentosPorSalaSemanaLetivaMap = new HashMap<Long,Map<Long,List<AtendimentoRelatorioDTO>>>();
		// disciplinas
		Set<Long> disciplinas = new HashSet<Long>();
		
		// preenche estruturas auxiliares
		for (AtendimentoRelatorioDTO atendimento : atendimentosDTO) {
			Long disciplinaId = atendimento.getDisciplinaSubstitutaId() != null ? atendimento.getDisciplinaSubstitutaId() : atendimento.getDisciplinaId();
			disciplinas.add(disciplinaId);
			
			// preenche os maps salaIdToSalaMap e semanaLetivaIdTosemanaLetivaMap
			if (!salaIdToSalaMap.containsKey(atendimento.getSalaId())) {
				salaIdToSalaMap.put(atendimento.getSalaId(),Sala.find(atendimento.getSalaId(),this.instituicaoEnsino));
			}
			Long semanaLetivaASerConsideradaId = (atendimento.getDisciplinaSubstitutaSemanaLetivaId() != null) ? atendimento.getDisciplinaSubstitutaSemanaLetivaId() : atendimento.getSemanaLetivaId(); 
			if (!semanaLetivaIdTosemanaLetivaMap.containsKey(semanaLetivaASerConsideradaId)) {
				semanaLetivaIdTosemanaLetivaMap.put(semanaLetivaASerConsideradaId,SemanaLetiva.find(semanaLetivaASerConsideradaId,instituicaoEnsino));
			}

			//  [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]
			Map<Long,List<AtendimentoRelatorioDTO>> salaAtendimentosPorSemanaLetivaMap = atendimentosPorSalaSemanaLetivaMap.get(atendimento.getSalaId());
			if (salaAtendimentosPorSemanaLetivaMap == null) {
				salaAtendimentosPorSemanaLetivaMap = new HashMap<Long,List<AtendimentoRelatorioDTO>>();
				atendimentosPorSalaSemanaLetivaMap.put(atendimento.getSalaId(),salaAtendimentosPorSemanaLetivaMap);
			}

			// armazena os atendimentos associados a uma dada sala, um dado turno e uma dada semana letiva
			List<AtendimentoRelatorioDTO> atendimentos = salaAtendimentosPorSemanaLetivaMap.get(semanaLetivaASerConsideradaId);
			if (atendimentos == null) {
				atendimentos = new ArrayList<AtendimentoRelatorioDTO>();
				salaAtendimentosPorSemanaLetivaMap.put(semanaLetivaASerConsideradaId,atendimentos);
			}
			atendimentos.add(atendimento);
		}
		
		buildCodigoDisciplinaToColorMap(disciplinas);

		// ordena as salas por campus e depois pela capacidade
		List<Sala> salasOrdenadasPorCampusEPelaCapacidade = new ArrayList<Sala>(salaIdToSalaMap.values());
		Collections.<Sala>sort(salasOrdenadasPorCampusEPelaCapacidade,new Comparator<Sala>() {
			@Override
			public int compare(Sala o1, Sala o2) {
				Unidade u1 = o1.getUnidade();
				Unidade u2 = o2.getUnidade();
				Campus c1 = u1.getCampus();
				Campus c2 = u2.getCampus();
				int result = c1.getCodigo().compareTo(c2.getCodigo());
				if(result == 0)
					result = u1.getCodigo().compareTo(u2.getCodigo());
				if(result == 0)
					result = -(o1.getTipoSala().getId().compareTo(o2.getTipoSala().getId()));
				if (result == 0) {
					result = -(o1.getCapacidadeInstalada().compareTo(o2.getCapacidadeInstalada())); 
				}
				return result;
			}
		});
		
		// imprime uma grade de horários para cada sala
		int count = 0, total=salasOrdenadasPorCampusEPelaCapacidade.size(); System.out.print(" "+total);//TODO: debug
		for (Sala sala : salasOrdenadasPorCampusEPelaCapacidade) {
			// [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]
			Map<Long,List<AtendimentoRelatorioDTO>> salaAtendimentosPorSemanaLetivaMap = atendimentosPorSalaSemanaLetivaMap.get(sala.getId());
			
			Set<Long> semanasLetivasIDs = new HashSet<Long>();
			List<AtendimentoRelatorioDTO> atendimentosDeTodasSemanasLetivas = new ArrayList<AtendimentoRelatorioDTO>();
			for (Long semLetId : salaAtendimentosPorSemanaLetivaMap.keySet()) {
				semanasLetivasIDs.add(semLetId);
				// acumula os atendimentos das semanas letivas
				atendimentosDeTodasSemanasLetivas.addAll(salaAtendimentosPorSemanaLetivaMap.get(semLetId));
			}
			
			AtendimentosServiceImpl service = new AtendimentosServiceImpl();
			QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = service.calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDs,semanaLetivaIdTosemanaLetivaMap,temInfoDeHorarios);
			ParDTO<Integer, Boolean> mdcTemposAula = quarteto.getPrimeiro();
			List<String> horariosDaGradeHoraria = quarteto.getSegundo();
			List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
			List<String> horariosDeFimDeAula = quarteto.getQuarto();
			
			nextRow = writeSala(sala, atendimentosDeTodasSemanasLetivas, nextRow, mdcTemposAula.getPrimeiro(), ehTatico, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
			count++;total--;if (count == 100) {System.out.println("\t   Faltam "+total+" salas"); count = 0;}//TODO: debug
		}
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName());
		if (mapLevel2 != null && !mapLevel2.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			for (Entry<String,Map<String,String>> entry : mapLevel2.entrySet()) {
				String cellValue = entry.getKey();
				if (cellValue.equals(ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName())) { 
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

	private int writeSala(Sala sala, List<AtendimentoRelatorioDTO> atendimentos, int row, int mdcTemposAula, 
		boolean ehTatico, boolean temInfoDeHorarios, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula)
	{
		String[] sheetsTargets = {
			ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName(),
			ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName(),
			ExcelInformationType.RELATORIO_VISAO_ALUNO.getSheetName()
		};
		registerHyperlink(
			sheetsTargets,
			ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName(),
			sala.getCodigo(), 
			"'"+ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName()+"'!B"+row
		);
			
		// escreve cabeçalho da grade horária da sala
		row = writeHeader(getRowsHeadersPairs(sala), row, temInfoDeHorarios);
		
		// processa os atendimentos lidos do BD para que os mesmos sejam visualizados na visão sala
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
		if(!ehTatico){
			List<AtendimentoOperacionalDTO> atendimentosOperacionaisDTO = new ArrayList<AtendimentoOperacionalDTO>();
			for(AtendimentoRelatorioDTO atendimentoDTO : atendimentos){
				atendimentosOperacionaisDTO.add((AtendimentoOperacionalDTO) atendimentoDTO);
			}
			aulas.addAll(atendimentosService.extraiAulas(atendimentosOperacionaisDTO));
		}
		else aulas.addAll(atendimentos);
		List<AtendimentoRelatorioDTO> aulasParaVisaoSala = atendimentosService.uneAulasQuePodemSerCompartilhadas(aulas);
		
		return writeAulas(aulasParaVisaoSala, row, mdcTemposAula, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
	}
	
	protected void onWriteAula(int row, int col, AtendimentoRelatorioDTO aula) {
		// informação para hyperlink
		if(!aula.isTatico())
			hyperlinkInfo.add(TrioDTO.create(row,col,(aula.isProfessorVirtual() ? aula.getProfessorVirtualId().toString()+"#" : aula.getProfessorId().toString())));
	}
	
	protected List<List<ParDTO<String, ?>>> getRowsHeadersPairs(Sala sala){
		List<List<ParDTO<String, ?>>> list = new ArrayList<List<ParDTO<String, ?>>>(); 
		
		List<ParDTO<String, ?>> row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().campus(), sala.getUnidade().getCampus().getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().idAmbiente(), sala.getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().capacidadeInstalada(), sala.getCapacidadeInstalada()));
		
		list.add(row);
		
		row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().unidade(), sala.getUnidade().getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().nomeAmbiente(), sala.getDescricao()));
		row.add(ParDTO.create(this.getI18nConstants().tipo(), sala.getTipoSala().getNome()));
		
		list.add(row);
		
		return list;
	}

}