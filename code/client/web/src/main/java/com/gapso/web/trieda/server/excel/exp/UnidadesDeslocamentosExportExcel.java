package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class UnidadesDeslocamentosExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 7, 2 ),
		INTEGER( 7, 3 ),
		HEADER( 6, 2 );

		private int row;
		private int col;

		private ExcelCellStyleReference( int row, int col )
		{
			this.row = row;
			this.col = col;
		}

		public int getRow()
		{
			return row;
		}

		public int getCol()
		{
			return col;
		}
	}

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	private AtendimentosFaixaDemandaFiltroExcel filter;

	public UnidadesDeslocamentosExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}
	
	public UnidadesDeslocamentosExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, 
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public UnidadesDeslocamentosExportExcel( boolean removeUnusedSheets,
			Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.UNIDADES_DESLOCAMENTO.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 7;
		this.setFilter( filter );
	}

	@Override
	public String getFileName() {
		return getI18nConstants().deslocamentoUnidades();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().deslocamentoUnidades();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< CampusDTO > campusDTOList = null;
		if ( this.getFilter() == null
			|| this.getFilter().getCampusDTO() == null )
		{
			List< Campus > campi = Campus.findAll( this.instituicaoEnsino );
			campusDTOList = new ArrayList< CampusDTO >( campi.size() );
			for ( Campus campus : campi )
			{
				campusDTOList.add( ConvertBeans.toCampusDTO( campus ) );
			}
		}
		else
		{
			campusDTOList = new ArrayList< CampusDTO >();
			campusDTOList.add( this.getFilter().getCampusDTO() );
		}		

		if (campusDTOList.size() >= 1) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.getSheetName(),workbook);
			}
			Sheet sheet = workbook.getSheet(this.getSheetName());
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int row = this.initialRow;
			for ( CampusDTO campusDTO : campusDTOList )
			{
				Campus campus = Campus.find(campusDTO.getId(), this.instituicaoEnsino);
				
				List< DeslocamentoUnidade > deslocamentos = DeslocamentoUnidade.findAllByCampus(
					this.instituicaoEnsino, campus );
				
				List< Unidade > unidades = Unidade.findByCampus(instituicaoEnsino, campus);
					
				Map<TriedaPar<Integer, Integer>, Integer> deslocamentosRowColumnMap =
					criaDeslocamentoRowColumnMap(deslocamentos, unidades, row);
				
				preencheCampus( row, campus, sheet );
				row = preencheOrigensDestinos( row, unidades, sheet );
				for ( Entry<TriedaPar<Integer, Integer>, Integer> d : deslocamentosRowColumnMap.entrySet() )
				{
					writeData( d.getKey(), d.getValue(), sheet );
				}
			}

			return true;
		}

		return false;
	}
	
	private void preencheCampus(int row, Campus campus, Sheet sheet) {
		setCell(row-2,2,sheet,cellStyles[ExcelCellStyleReference.HEADER.ordinal()],campus.getNome());
	}

	private Map<TriedaPar<Integer, Integer>, Integer> criaDeslocamentoRowColumnMap(
			List<DeslocamentoUnidade> deslocamentos, List<Unidade> unidades, int row)
	{
		Map<TriedaPar<Integer, Integer>, Integer> deslocamentosRowColumnMap =
				new HashMap<TriedaPar<Integer, Integer>, Integer>();
		
		int column = 3;
		int origemRow = row;
		for (Unidade unidadeOrigem : unidades)
		{
			int destinoColumn = column;
			for (Unidade unidadeDestino : unidades)
			{
				int tempoDeslocamento = 0;
				for (DeslocamentoUnidade deslocamento : deslocamentos)
				{
					if (unidadeOrigem.getCodigo().equals(deslocamento.getOrigem().getCodigo())
							&& unidadeDestino.getCodigo().equals(deslocamento.getDestino().getCodigo()))
					{
						tempoDeslocamento = deslocamento.getTempo();
					}

				}
				deslocamentosRowColumnMap.put(TriedaPar.create(origemRow, destinoColumn), tempoDeslocamento);
				destinoColumn++;
			}
			origemRow++;
		}
		
		return deslocamentosRowColumnMap;
	}

	private int preencheOrigensDestinos(int row, List<Unidade> unidades, Sheet sheet )
	{
		int origensRow = row;
		int destinoColumn = 3;
		setCell(row-1,2,sheet,cellStyles[ExcelCellStyleReference.HEADER.ordinal()],"Tempo de Deslocamento");
		for (Unidade unidade : unidades)
		{
			setCell(origensRow,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getNome());
			setCell(row-1,destinoColumn,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getNome());
			origensRow++;
			destinoColumn++;
		}
		
		return origensRow+3;
	}

	private void writeData( TriedaPar<Integer, Integer> rowColumnPar,
			Integer tempo, Sheet sheet )
	{
		int row = rowColumnPar.getPrimeiro();
		int column = rowColumnPar.getSegundo();
		// Nome
		setCell(row,column,sheet,cellStyles[ExcelCellStyleReference.INTEGER.ordinal()],tempo);
		
	}
	
	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
	
	public AtendimentosFaixaDemandaFiltroExcel getFilter()
	{
		return filter;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter != null && filter instanceof AtendimentosFaixaDemandaFiltroExcel )
		{
			this.filter = (AtendimentosFaixaDemandaFiltroExcel) filter;
		}
		else
		{
			this.filter = null;
		}
	}
}