package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;

public class RelatorioVisaoSalaExportExcel extends RelatorioVisaoExportExcel{
	RelatorioVisaoSalaFiltro relatorioFiltro;
	
	public RelatorioVisaoSalaExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino)
	{
		super(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino);
	}
	
	public RelatorioVisaoSalaExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino);
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
			RelatorioVisaoSalaFiltro filter = (RelatorioVisaoSalaFiltro) this.getFilter();
			
			Campus campus = Campus.find(filter.getCampusDTO().getId(), this.instituicaoEnsino );
			Unidade unidade = Unidade.find(filter.getUnidadeDTO().getId(), this.instituicaoEnsino);
			Sala sala = Sala.find(filter.getSalaDTO().getId(), this.instituicaoEnsino);
			Turno turno = Turno.find(filter.getTurnoDTO().getId(), this.instituicaoEnsino);

			atdTaticoList = AtendimentoTatico.findByCenario(this.instituicaoEnsino, cenario, campus, unidade, sala, 
				turno, null);

			atdOperacionalList = AtendimentoOperacional.findByCenario(this.instituicaoEnsino, cenario, campus, unidade, 
				sala, turno, null);
		}

		List<AtendimentoTaticoDTO> listAtendTaticoDTO = ConvertBeans.toListAtendimentoTaticoDTO(atdTaticoList);
		List<AtendimentoOperacionalDTO> listAtendOpDTO = ConvertBeans.toListAtendimentoOperacionalDTO(atdOperacionalList);

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		listAtendOpDTO = service.ordenaPorHorarioAula(listAtendOpDTO);

		atdRelatorioList.addAll(listAtendTaticoDTO);
		atdRelatorioList.addAll(listAtendOpDTO);

		return !atdRelatorioList.isEmpty();
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

		// Estruturas auxiliares
		// [SalaId -> Sala]
		Map<Long,Sala> salaIdToSalaMap = new HashMap<Long,Sala>();
		// [TurnoId -> Turno]
		Map<Long,Turno> turnoIdToTurnoMap = new HashMap<Long,Turno>();
		// [SemanaLetivaId -> SemanaLetiva]
		Map<Long,SemanaLetiva> semanaLetivaIdTosemanaLetivaMap = new HashMap<Long,SemanaLetiva>();
		// [SalaId -> [TurnoId -> [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]]]
		Map<Long,Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>>> atendimentosPorSalaTurnoSemanaLetivaMap = new HashMap<Long,Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>>>();
		
		// preenche estruturas auxiliares
		for (AtendimentoRelatorioDTO atendimento : atendimentosDTO) {
			// preenche os maps salaIdToSalaMap, turnoIdToTurnoMap e semanaLetivaIdTosemanaLetivaMap
			if (!salaIdToSalaMap.containsKey(atendimento.getSalaId())) {
				salaIdToSalaMap.put(atendimento.getSalaId(),Sala.find(atendimento.getSalaId(),this.instituicaoEnsino));
			}
			if (!turnoIdToTurnoMap.containsKey(atendimento.getTurnoId())) {
				turnoIdToTurnoMap.put(atendimento.getTurnoId(),Turno.find(atendimento.getTurnoId(),this.instituicaoEnsino));
			}
			Long semanaLetivaASerConsideradaId = (atendimento.getDisciplinaSubstitutaSemanaLetivaId() != null) ? atendimento.getDisciplinaSubstitutaSemanaLetivaId() : atendimento.getSemanaLetivaId(); 
			if (!semanaLetivaIdTosemanaLetivaMap.containsKey(semanaLetivaASerConsideradaId)) {
				semanaLetivaIdTosemanaLetivaMap.put(semanaLetivaASerConsideradaId,SemanaLetiva.find(semanaLetivaASerConsideradaId,instituicaoEnsino));
			}

			// [TurnoId -> [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]]
			Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>> salaAtendimentosPorTurnoSemanaLetivaMap = atendimentosPorSalaTurnoSemanaLetivaMap.get(atendimento.getSalaId());
			if (salaAtendimentosPorTurnoSemanaLetivaMap == null) {
				salaAtendimentosPorTurnoSemanaLetivaMap = new HashMap<Long,Map<Long,List<AtendimentoRelatorioDTO>>>();
				atendimentosPorSalaTurnoSemanaLetivaMap.put(atendimento.getSalaId(),salaAtendimentosPorTurnoSemanaLetivaMap);
			}

			// [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]
			Map<Long,List<AtendimentoRelatorioDTO>> turnoAtendimentosPorSemanaLetivaMap = salaAtendimentosPorTurnoSemanaLetivaMap.get(atendimento.getTurnoId());
			if (turnoAtendimentosPorSemanaLetivaMap == null) {
				turnoAtendimentosPorSemanaLetivaMap = new HashMap<Long,List<AtendimentoRelatorioDTO>>();
				salaAtendimentosPorTurnoSemanaLetivaMap.put(atendimento.getTurnoId(),turnoAtendimentosPorSemanaLetivaMap);
			}

			// armazena os atendimentos associados a uma dada sala, um dado turno e uma dada semana letiva
			List<AtendimentoRelatorioDTO> atendimentos = turnoAtendimentosPorSemanaLetivaMap.get(semanaLetivaASerConsideradaId);
			if (atendimentos == null) {
				atendimentos = new ArrayList<AtendimentoRelatorioDTO>();
				turnoAtendimentosPorSemanaLetivaMap.put(semanaLetivaASerConsideradaId,atendimentos);
			}
			atendimentos.add(atendimento);
		}

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
				if (result == 0) {
					result = -(o1.getCapacidade().compareTo(o2.getCapacidade())); 
				}
				return result;
			}
		});
		
		// imprime uma grade de horários para cada sala
		for (Sala sala : salasOrdenadasPorCampusEPelaCapacidade) {
			// [TurnoId -> [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]]
			Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>> salaAtendimentosPorTurnoSemanaLetivaMap = atendimentosPorSalaTurnoSemanaLetivaMap.get(sala.getId());
			for (Long turnoId : salaAtendimentosPorTurnoSemanaLetivaMap.keySet()) {
				// [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]
				Map<Long,List<AtendimentoRelatorioDTO>> turnoAtendimentosPorSemanaLetivaMap = salaAtendimentosPorTurnoSemanaLetivaMap.get(turnoId);
				Turno turno = turnoIdToTurnoMap.get(turnoId);
				
				Set<Long> semanasLetivasIDs = new HashSet<Long>();
				List<AtendimentoRelatorioDTO> atendimentosDeTodasSemanasLetivas = new ArrayList<AtendimentoRelatorioDTO>();
				for (Long semLetId : turnoAtendimentosPorSemanaLetivaMap.keySet()) {
					semanasLetivasIDs.add(semLetId);
					// acumula os atendimentos das semanas letivas
					atendimentosDeTodasSemanasLetivas.addAll(turnoAtendimentosPorSemanaLetivaMap.get(semLetId));
				}
				
				AtendimentosServiceImpl service = new AtendimentosServiceImpl();
				TrioDTO<Integer,SemanaLetiva,List<String>> trio = service.calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDs,ehTatico,turnoId);
				int mdcTemposAula = trio.getPrimeiro();
				List<String> labelsDasLinhasDaGradeHoraria = trio.getTerceiro();
				
				nextRow = writeSala(sala, turno, atendimentosDeTodasSemanasLetivas, nextRow, mdcTemposAula, ehTatico, labelsDasLinhasDaGradeHoraria);
			}				
		}
	}

	private int writeSala(Sala sala, Turno turno, List<AtendimentoRelatorioDTO> atendimentos, int row, int mdcTemposAula, 
		boolean ehTatico, List<String> labelsDasLinhasDaGradeHoraria)
	{
		// escreve cabeçalho da grade horária da sala
		row = writeHeader(getRowsHeadersPairs(sala, turno), row, ehTatico);
		
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
		
		return writeAtendimento(aulasParaVisaoSala, row, mdcTemposAula, ehTatico, labelsDasLinhasDaGradeHoraria);
	}
	
	protected List<List<ParDTO<String, ?>>> getRowsHeadersPairs(Sala sala, Turno turno){
		List<List<ParDTO<String, ?>>> list = new ArrayList<List<ParDTO<String, ?>>>(); 
		
		List<ParDTO<String, ?>> row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().campus(), sala.getUnidade().getCampus().getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().sala(), sala.getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().capacidade(), sala.getCapacidade()));
		
		list.add(row);
		
		row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().unidade(), sala.getUnidade().getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().turno(), turno.getNome()));
		row.add(ParDTO.create(this.getI18nConstants().tipo(), sala.getTipoSala().getNome()));
		
		list.add(row);
		
		return list;
	}

}