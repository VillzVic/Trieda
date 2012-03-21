package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

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
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class RelatorioVisaoCursoExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		HEADER_LEFT_TEXT( 5, 3 ),
		HEADER_CENTER_VALUE( 5, 4 ),
		HEADER_CENTER_TEXT( 7, 2 ),
		TEXT( 8, 2 );

		private int row;
		private int col;

		private ExcelCellStyleReference( int row, int col )
		{
			this.row = row;
			this.col = col;
		}

		public int getRow()
		{
			return this.row;
		}

		public int getCol()
		{
			return this.col;
		}
	}

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	private RelatorioVisaoCursoFiltroExcel relatorioFiltro;

	public RelatorioVisaoCursoExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		this(true, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino );
	}

	public RelatorioVisaoCursoExportExcel(
		boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		this(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino );
	}

	public RelatorioVisaoCursoExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino )
	{
		this(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino );
	}

	public RelatorioVisaoCursoExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino )
	{
		super(false, ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 5;
		this.setFilter( filter );
	}

	public ExportExcelFilter getFilter()
	{
		return this.relatorioFiltro;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter instanceof RelatorioVisaoCursoFiltroExcel )
		{
			this.relatorioFiltro = (RelatorioVisaoCursoFiltroExcel)filter;
		}
		else
		{
			this.relatorioFiltro = null;
		}
	}

	@Override
	public String getFileName()
	{
		return this.getI18nConstants().relatorioVisaoCurso();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return this.getI18nConstants().relatorioVisaoCurso();
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

			if ( curriculoId == this.relatorioFiltro.getCurriculoDTO().getId()
				&& periodo == this.relatorioFiltro.getPeriodo()
				&& turnoId == this.relatorioFiltro.getTurnoDTO().getId()
				&& campusId == this.relatorioFiltro.getCampusDTO().getId()
				&& cursoId == this.relatorioFiltro.getCursoDTO().getId() )
			{
				result.add( opcao );
			}
		}

		return result;
	}

	private List<QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>>> getAtendimentoRelatorioDTOList(Cenario cenario) {
		List<QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>>> atendimentosInfo = new ArrayList<QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>>>();

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		Set<Map<String,Object>> opcoes = opcoesBuscaOperacional(cenario);

		if (this.relatorioFiltro != null) {
			opcoes = this.filtraAtendimentos(opcoes);
		}

		for (Map<String, Object> opcao : opcoes) {
			Long curriculoId = (Long) opcao.get("CurriculoDTO");
			Curriculo curriculo = Curriculo.find(curriculoId,
					this.instituicaoEnsino);
			CurriculoDTO curriculoDTO = ConvertBeans.toCurriculoDTO(curriculo);

			Integer periodo = (Integer) opcao.get("Periodo");

			Long turnoId = (Long) opcao.get("TurnoDTO");
			Turno turno = Turno.find(turnoId, this.instituicaoEnsino);
			TurnoDTO turnoDTO = ConvertBeans.toTurnoDTO(turno);

			Long campusId = (Long) opcao.get("CampusDTO");
			Campus campus = Campus.find(campusId, this.instituicaoEnsino);
			CampusDTO campusDTO = ConvertBeans.toCampusDTO(campus);

			Long cursoId = (Long) opcao.get("CursoDTO");
			Curso curso = Curso.find(cursoId, this.instituicaoEnsino);
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO(curso);

			QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>> quinteto = service.getAtendimentosParaGradeHorariaVisaoCurso(curriculoDTO,periodo,turnoDTO,campusDTO,cursoDTO);
			atendimentosInfo.add(quinteto);
		}

		return atendimentosInfo;
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		boolean result = false;

		Cenario cenario = getCenario();
		
		// busca os atendimentos que deverão ser escritos no excel
		List<QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>>> atendimentosInfo = getAtendimentoRelatorioDTOList(cenario);
		
		if (!atendimentosInfo.isEmpty()) {
			// identifica se os atendimentos se referem ao modo tático ou ao modo operacional
			boolean ehTatico = atendimentosInfo.get(0).getQuarto().get(0) instanceof AtendimentoTaticoDTO;

			HSSFSheet sheet = workbook.getSheet(this.getSheetName());
			
			// monta estruturas de estilos
			fillInCellStyles(sheet);
			List<HSSFCellStyle> excelColorsPool = buildColorPaletteCellStyles(workbook);
			Map<String,HSSFCellStyle> codigoDisciplinaToColorMap = new HashMap<String,HSSFCellStyle>();
			for (QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>> quinteto : atendimentosInfo) {
				List<AtendimentoRelatorioDTO> aulas = quinteto.getQuarto();
				for (AtendimentoRelatorioDTO aula : aulas) {
					HSSFCellStyle style = codigoDisciplinaToColorMap.get(aula.getDisciplinaString());
					if (style == null) {
						int index = (codigoDisciplinaToColorMap.size()%excelColorsPool.size());
						codigoDisciplinaToColorMap.put(aula.getDisciplinaString(),excelColorsPool.get(index));
					}
				}
			}

			// linha a partir da qual a escrita será iniciada no excel
			int nextRow = this.initialRow;

			// para cada bloco curricular
			for (QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>> quinteto : atendimentosInfo) {
				Integer mdcTemposAula = quinteto.getPrimeiro();
				Integer maximoDeCreditos = quinteto.getSegundo();
				Integer tempoDeAulaDaSemanaLetivaComMaiorCargaHoraria = quinteto.getTerceiro();
				List<AtendimentoRelatorioDTO> aulas = quinteto.getQuarto();
				List<Integer> qtdColunasPorDiaSemana = quinteto.getQuinto();
				
				if (aulas.isEmpty()) {
					continue;
				}

				aulas = this.ordenaHorarioAula(aulas);
				Oferta oferta = Oferta.find(aulas.get(0).getOfertaId(),this.instituicaoEnsino);
				Integer periodo = Integer.valueOf(aulas.get(0).getPeriodoString());

				nextRow = this.writeCurso(oferta,periodo,mdcTemposAula,maximoDeCreditos,tempoDeAulaDaSemanaLetivaComMaiorCargaHoraria,aulas,qtdColunasPorDiaSemana,nextRow,sheet,codigoDisciplinaToColorMap,ehTatico);
			}

			result = true;
		}

		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(), workbook);
		}

		return result;
	}
	
	private List< AtendimentoRelatorioDTO > ordenaHorarioAula(
		List< AtendimentoRelatorioDTO > list )
	{
		if ( list == null || list.size() == 0 )
		{
			return Collections.< AtendimentoRelatorioDTO > emptyList();
		}
		
		if ( list.get( 0 ) instanceof AtendimentoTaticoDTO )
		{
			return list;
		}

		List< AtendimentoOperacionalDTO > opList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( AtendimentoRelatorioDTO ar : list )
		{
			opList.add( (AtendimentoOperacionalDTO) ar );
		}
		
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		opList = service.ordenaPorHorarioAula( opList );

		List< AtendimentoRelatorioDTO > result
			= new ArrayList< AtendimentoRelatorioDTO >();

		for ( AtendimentoOperacionalDTO atOp : opList )
		{
			result.add( (AtendimentoRelatorioDTO) atOp );
		}

		return result;
	}

	private int writeCurso(Oferta oferta, Integer periodo, Integer mdcTemposAula, Integer maximoDeCreditos, Integer tempoDeAulaDaSemanaLetivaComMaiorCargaHoraria, List<AtendimentoRelatorioDTO> aulas, List<Integer> qtdColunasPorDiaSemana, int row, HSSFSheet sheet, Map<String,HSSFCellStyle> codigoDisciplinaToColorMap, boolean ehTatico) {
		// FIXME -- Considerar apenas os horários da semana letiva

		row = writeHeader(oferta,periodo,qtdColunasPorDiaSemana,row,sheet,ehTatico);

		List<Long> horariosAulaIdsList = new ArrayList<Long>();

		int initialRow = row;
		int col = 2;
		
		int linhasDeExcelPorCredito = tempoDeAulaDaSemanaLetivaComMaiorCargaHoraria/mdcTemposAula;

		// Preenche grade com créditos e células vazias
		SemanaLetiva semanaLetiva = oferta.getCurriculo().getSemanaLetiva();
		if (ehTatico) {
			int cargaHoraria = mdcTemposAula;
			for (int indexCredito = 1; indexCredito <= maximoDeCreditos; indexCredito++) {
				// coluna de carga horária
				for (int i = 0; i < linhasDeExcelPorCredito; i++) {
					setCell((row+i),col,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],cargaHoraria);
					cargaHoraria += mdcTemposAula;
				}
				col++;

				// colunas dos dias da semana
				for (int j = 0; j < Semanas.values().length; j++) {
					for (int i = 0; i < linhasDeExcelPorCredito; i++) {
						setCell((row+i),col,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],"");
					}
					col++;
				}

				row += linhasDeExcelPorCredito;
				col = 2;
			}
		} else {
			List<HorarioAula> horariosAulaList = new ArrayList<HorarioAula>(semanaLetiva.getHorariosAula());
			Collections.sort(horariosAulaList);
			for (HorarioAula ha : horariosAulaList) {
				horariosAulaIdsList.add(ha.getId());
				// Horários
				String value = ConvertBeans.dateToString(ha.getHorario(), ha.getSemanaLetiva().getTempo());
				setCell(row,col++,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],value);

				// Dias Semana
				for (int i = 0; i < Semanas.values().length; i++) {
					setCell(row, col++, sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
				}

				row++;
				col = 2;
			}
		}

		// agrupa as aulas por dia da semana
		Map<Integer,List<AtendimentoRelatorioDTO>> diaSemanaToAulasMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
		for (AtendimentoRelatorioDTO aula : aulas) {
			List<AtendimentoRelatorioDTO> aulasDoDia = diaSemanaToAulasMap.get(aula.getSemana());
			if (aulasDoDia == null) {
				aulasDoDia = new ArrayList<AtendimentoRelatorioDTO>();
				diaSemanaToAulasMap.put(aula.getSemana(),aulasDoDia);
			}
			aulasDoDia.add(aula);
		}

		// Para cada dia da semana, escreve as aulas no excel
		for (Integer diaSemanaInt : diaSemanaToAulasMap.keySet()) {
			// linha em que a escrita será iniciada
			row = initialRow;

			// Obs.: O valor do dia da semana aqui NÃO é interpretado exatamente como o dia em que a aula é ministrada, mas sim a coluna da
			// planilha na qual o atendimento será impresso. Quando há mais de 4 créditos alocados no mesmo dia da semana, 
			// a planilha irá utilizar mais de uma coluna para esse dia da semana. Logo, teremos colunas com valor
			// superior a 8 ( o que não é consistente com o valor de dia da semana ).
			col = diaSemanaInt + 1;

			List<AtendimentoRelatorioDTO> aulasDoDia = diaSemanaToAulasMap.get(diaSemanaInt);
			if (aulasDoDia == null || aulasDoDia.size() == 0) {
				continue;
			}

			// para cada aula
			for (AtendimentoRelatorioDTO aula : aulasDoDia) {
				// obtém o estilo que será aplicado nas células que serão desenhadas
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(aula.getDisciplinaString());
				// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
				int linhasDeExcelPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos()/mdcTemposAula;
				
				if (!ehTatico) {
					AtendimentoOperacionalDTO atendimentoOp = (AtendimentoOperacionalDTO) aula;
					int index = horariosAulaIdsList.indexOf(atendimentoOp.getHorarioId());
					if (index != -1) {
						row = initialRow + index;
					}
				}

				// escreve célula principal
				setCell(row,col,sheet,style,HtmlUtils.htmlUnescape(aula.getContentVisaoCurso(ReportType.EXCEL)),HtmlUtils.htmlUnescape(aula.getContentToolTipVisaoCurso(ReportType.EXCEL)));
				// Une células de acordo com a quantidade de créditos
				int rowF = row + (ehTatico ? (aula.getTotalCreditos()*linhasDeExcelPorCreditoDaAula-1) : (aula.getTotalCreditos()-1));
				mergeCells(row,rowF,col,col,sheet,style);

				if (ehTatico) {
					row += aula.getTotalCreditos()*linhasDeExcelPorCreditoDaAula;
				}
			}
		}

		return (initialRow + (ehTatico ? (maximoDeCreditos*linhasDeExcelPorCredito) : maximoDeCreditos) + 1);
	}

	private int writeHeader( Oferta oferta, Integer periodo,
		List< Integer > tamanhoSemanaList, int row, HSSFSheet sheet, boolean ehTatico )
	{
		int col = 3;

		// Curso
		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().curso() ) );

		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getCurriculo().getCurso().getCodigo() );

		// Campus
		setCell(row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().campus() ) );

		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getCampus().getCodigo() );

		// Turno
		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().turno() ) );

		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getTurno().getNome() );

		row++;
		col = 3;

		// Curriculo
		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().matrizCurricular() ) );

		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getCurriculo().getCodigo() );

		// Periodo
		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().periodo() ) );

		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], periodo );

		row++;
		col = 2;

		// Créditos ou Horários
		setCell( row, col++, sheet,
			this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( ehTatico ? this.getI18nConstants().cargaHorariaMinutos() : this.getI18nConstants().horarios() ) );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			int qtd = tamanhoSemanaList.get( Semanas.toInt( semanas ) );
			setCell( row, col, sheet, semanas.name() );
			HSSFCellStyle style = this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ];
			mergeCells( row, row, col, col + qtd - 1, sheet, style );
			col = col + qtd;
		}

		row++;
		return row;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}

	private List< HSSFCellStyle > buildColorPaletteCellStyles( HSSFWorkbook workbook )
	{
		List< HSSFCellStyle > colorPalleteCellStylesList = new ArrayList< HSSFCellStyle >();
		HSSFSheet sheet = workbook.getSheet(
			ExcelInformationType.PALETA_CORES.getSheetName() );

		if ( sheet != null )
		{ 
			for ( int rowIndex = sheet.getFirstRowNum();
				rowIndex <= sheet.getLastRowNum(); rowIndex++ )
			{
				HSSFRow row = sheet.getRow( rowIndex );

				if ( row != null )
				{
					HSSFCell cell = row.getCell( (int) row.getFirstCellNum() );

					if ( cell != null )
					{
						colorPalleteCellStylesList.add( cell.getCellStyle() );
					}
				}
			}
		}

		return colorPalleteCellStylesList;
	}

//	private List< HSSFComment > buildExcelCommentsPool( HSSFWorkbook workbook )
//	{
//		List< HSSFComment > excelCommentsPool = new ArrayList< HSSFComment >();
//
//		HSSFSheet sheet = workbook.getSheet(
//			ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName() );
//
//		if ( sheet != null )
//		{
//			for ( int rowIndex = sheet.getFirstRowNum();
//				rowIndex <= sheet.getLastRowNum(); rowIndex++ )
//			{
//				HSSFRow row = sheet.getRow( rowIndex );
//				if ( row != null )
//				{
//					HSSFCell cell = row.getCell( 25 );
//
//					if ( cell != null && cell.getCellComment() != null )
//					{
//						excelCommentsPool.add( cell.getCellComment() );
//					}
//				}
//			}
//		}
//
//		return excelCommentsPool;
//	}
}
