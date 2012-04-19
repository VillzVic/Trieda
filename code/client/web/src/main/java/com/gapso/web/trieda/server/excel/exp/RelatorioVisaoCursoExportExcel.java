package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;

public class RelatorioVisaoCursoExportExcel	extends RelatorioVisaoExportExcel{
	private List<Integer> qtdColunasPorDiaSemana;
	protected RelatorioVisaoCursoFiltro relatorioFiltro;

	public RelatorioVisaoCursoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino )
	{
		super(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino);
	}
	
	public RelatorioVisaoCursoExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,	InstituicaoEnsino instituicaoEnsino)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino);
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

	public Set< Map< String, Object > > opcoesBuscaOperacional( Cenario cenario )
	{
		List< AtendimentoTatico > atdTaticoList
			= AtendimentoTatico.findByCenario( this.instituicaoEnsino, cenario );

		List< AtendimentoOperacional > atdOperacionalList
			= AtendimentoOperacional.findByCenario( this.instituicaoEnsino, cenario );

		List< AtendimentoRelatorioDTO > atdRelatorioList
			= new ArrayList< AtendimentoRelatorioDTO >(
				atdTaticoList.size() + atdOperacionalList.size() );

		for ( AtendimentoTatico atdTatico : atdTaticoList )
		{
			atdRelatorioList.add(
				ConvertBeans.toAtendimentoTaticoDTO( atdTatico ) );
		}

		for ( AtendimentoOperacional atdOperacional : atdOperacionalList )
		{
			atdRelatorioList.add(
				ConvertBeans.toAtendimentoOperacionalDTO( atdOperacional ) );
		}

		Set< Map< String, Object > > opcoes = new HashSet< Map< String, Object > >();

		for ( AtendimentoRelatorioDTO atendimentoRelatorio : atdRelatorioList )
		{
			Map< String, Object > opcao = new HashMap< String, Object >();

			opcao.put( "CurriculoDTO", atendimentoRelatorio.getCurriculoId() );
			opcao.put( "Periodo", Integer.valueOf( atendimentoRelatorio.getPeriodoString() ) );
			opcao.put( "TurnoDTO", atendimentoRelatorio.getTurnoId() );
			opcao.put( "CampusDTO", atendimentoRelatorio.getCampusId() );
			opcao.put( "CursoDTO", atendimentoRelatorio.getCursoId() );

			opcoes.add( opcao );
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
		List<AtendimentoServiceRelatorioResponse> atendimentosInfo = 
			(List<AtendimentoServiceRelatorioResponse>) mapControlT;
	
		Cenario cenario = getCenario();
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		Set<Map<String,Object>> opcoes = opcoesBuscaOperacional(cenario);

		if (this.relatorioFiltro != null) {
			opcoes = this.filtraAtendimentos(opcoes);
		}

		for (Map<String, Object> opcao : opcoes) {
			RelatorioVisaoCursoFiltro filtro = new RelatorioVisaoCursoFiltro();
			
			Long curriculoId = (Long) opcao.get("CurriculoDTO");
			Curriculo curriculo = Curriculo.find(curriculoId, this.instituicaoEnsino);
			filtro.setCurriculoDTO(ConvertBeans.toCurriculoDTO(curriculo));

			filtro.setPeriodo((Integer) opcao.get("Periodo"));

			Long turnoId = (Long) opcao.get("TurnoDTO");
			Turno turno = Turno.find(turnoId, this.instituicaoEnsino);
			filtro.setTurnoDTO(ConvertBeans.toTurnoDTO(turno));

			Long campusId = (Long) opcao.get("CampusDTO");
			Campus campus = Campus.find(campusId, this.instituicaoEnsino);
			filtro.setCampusDTO(ConvertBeans.toCampusDTO(campus));

			Long cursoId = (Long) opcao.get("CursoDTO");
			Curso curso = Curso.find(cursoId, this.instituicaoEnsino);
			filtro.setCursoDTO(ConvertBeans.toCursoDTO(curso));

			AtendimentoServiceRelatorioResponse sexteto = service.getAtendimentosParaGradeHorariaVisaoCurso(filtro);
			atendimentosInfo.add(sexteto);
		}
		
		return !atendimentosInfo.isEmpty();
	}
	
	@SuppressWarnings("unchecked")
	protected List<AtendimentoServiceRelatorioResponse> getStructureReportControl(){
		return new ArrayList<AtendimentoServiceRelatorioResponse>();
	}
	
	@SuppressWarnings("unchecked")
	protected <T> void processStructureReportControl(T mapControlT){
		List<AtendimentoServiceRelatorioResponse> atendimentosInfo = 
			(List<AtendimentoServiceRelatorioResponse>) mapControlT;
		// linha a partir da qual a escrita será iniciada no excel
		int nextRow = this.initialRow;

		// para cada bloco curricular
		for(AtendimentoServiceRelatorioResponse sexteto : atendimentosInfo) {
			Integer mdcTemposAula = sexteto.getMdcTemposAula();
			List<AtendimentoRelatorioDTO> aulas = sexteto.getAtendimentosDTO();
			List<Integer> qtdColunasPorDiaSemana = sexteto.getQtdColunasPorDiaSemana();
			List<String> labelsDasLinhasDaGradeHoraria = sexteto.getLabelsDasLinhasDaGradeHoraria();
			
			if(aulas.isEmpty()) continue;

			Oferta oferta = Oferta.find(aulas.get(0).getOfertaId(),this.instituicaoEnsino);
			Integer periodo = Integer.valueOf(aulas.get(0).getPeriodoString());
			boolean ehTatico = aulas.get(0) instanceof AtendimentoTaticoDTO;

			nextRow = writeCurso(oferta, periodo, mdcTemposAula, aulas, qtdColunasPorDiaSemana, nextRow, ehTatico, labelsDasLinhasDaGradeHoraria);
		}
	}
	
	private int writeCurso(Oferta oferta, Integer periodo, Integer mdcTemposAula, List<AtendimentoRelatorioDTO> aulas, 
		List<Integer> qtdColunasPorDiaSemana, int row, boolean ehTatico, List<String> labelsDasLinhasDaGradeHoraria)
	{
		// escreve cabeçalho da grade horária do bloco curricular
		this.qtdColunasPorDiaSemana = qtdColunasPorDiaSemana;
		row = writeHeader(getRowsHeadersPairs(oferta, periodo), row, ehTatico);

		return writeAtendimento(aulas, row, mdcTemposAula, ehTatico, labelsDasLinhasDaGradeHoraria);
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
			HSSFCellStyle style = this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()];
			mergeCells(row, row, col, col + qtd - 1, sheet, style);
			col = col + qtd;
		}

		return ++row;
	}

}
