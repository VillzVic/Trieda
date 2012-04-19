package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;

public class RelatorioVisaoAlunoExportExcel	extends RelatorioVisaoExportExcel{
	RelatorioVisaoAlunoFiltro relatorioFiltro;
	
	public RelatorioVisaoAlunoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino)
	{
		super(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino);
	}
	
	public RelatorioVisaoAlunoExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino);
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
	
	private boolean getAtendimentosRelatorioDTOListByAluno(Campus campus, Turno turno, Set<AlunoDemanda> alunosDemanda,
		Map<Campus, Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>> mapControl)
	{
		boolean result = false;
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		
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
				filtro.setAlunoDTO(ConvertBeans.toAlunoDTO(aluno));
				filtro.setTurnoDTO(ConvertBeans.toTurnoDTO(turno));
				filtro.setCampusDTO(ConvertBeans.toCampusDTO(campus));
				
				quinteto = service.getAtendimentosParaGradeHorariaVisaoAluno(filtro);
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
			// se nao ha filtro, entao lista todos os alunos associados ao cenario
			Cenario cenario = getCenario();
			List<AtendimentoTatico> atdTaticoList = AtendimentoTatico.findByCenario(this.instituicaoEnsino, cenario);
			if(atdTaticoList.size() != 0){
				for(AtendimentoTatico atendimento : atdTaticoList){
					if(getAtendimentosRelatorioDTOListByAluno(atendimento.getOferta().getCampus(), 
						atendimento.getOferta().getTurno(), atendimento.getAlunosDemanda(), mapControl)) result = true;
				}
			}
			else{
				List<AtendimentoOperacional> atdOperacionalList = AtendimentoOperacional.findByCenario(this.instituicaoEnsino, cenario);
				
				for(AtendimentoOperacional atendimento : atdOperacionalList){
					if(getAtendimentosRelatorioDTOListByAluno(atendimento.getOferta().getCampus(), 
						atendimento.getOferta().getTurno(), atendimento.getAlunosDemanda(), mapControl)) result = true;
				}
			}
		}
		else{
			AtendimentosServiceImpl service = new AtendimentosServiceImpl();
			AtendimentoServiceRelatorioResponse quinteto;
			RelatorioVisaoAlunoFiltro filter = this.getFilter();
			
			quinteto = service.getAtendimentosParaGradeHorariaVisaoAluno(filter);
			
			Map<Aluno, AtendimentoServiceRelatorioResponse> mapAluno = new HashMap<Aluno, AtendimentoServiceRelatorioResponse>();
			mapAluno.put(ConvertBeans.toAluno(filter.getAlunoDTO()), quinteto);
			Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>> mapTurnoControl = 
				new HashMap<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>>();
			mapTurnoControl.put(ConvertBeans.toTurno(filter.getTurnoDTO()), mapAluno);
			mapControl.put(ConvertBeans.toCampus(filter.getCampusDTO()), mapTurnoControl);
			
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

		// para cada bloco curricular
		for(Campus campus : mapControl.keySet()){
			Map<Turno, Map<Aluno, AtendimentoServiceRelatorioResponse>> turnoMap =
				mapControl.get(campus);
			for(Turno turno : turnoMap.keySet()){
				Map<Aluno, AtendimentoServiceRelatorioResponse> alunoMap = 
					turnoMap.get(turno);
				for(Aluno aluno : alunoMap.keySet()){
					AtendimentoServiceRelatorioResponse quinteto = alunoMap.get(aluno);
					Integer mdcTemposAula = quinteto.getMdcTemposAula();
					List<AtendimentoRelatorioDTO> atendimentos = quinteto.getAtendimentosDTO();
					List<String> labelsDasLinhasDaGradeHoraria = quinteto.getLabelsDasLinhasDaGradeHoraria();
					
					if(atendimentos.isEmpty()) continue;
					
					boolean ehTatico = atendimentos.get(0) instanceof AtendimentoTaticoDTO;

					nextRow = writeAluno(campus, turno, aluno, atendimentos, nextRow, mdcTemposAula, ehTatico, labelsDasLinhasDaGradeHoraria);
				}
			}
		}
	}

	private int writeAluno(Campus campus, Turno turno, Aluno aluno, List<AtendimentoRelatorioDTO> atendimentos, int row, int mdcTemposAula, 
		boolean ehTatico, List<String> labelsDasLinhasDaGradeHoraria)
		{
			// escreve cabeçalho da grade horária da sala
			row = writeHeader(getRowsHeadersPairs(campus, turno, aluno), row, ehTatico);
			
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