package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class ProfessoresExportExcel extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		NUMBER_DOUBLE( 6, 10 ),
		NUMBER_INT( 6, 5 );

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

	private HSSFCellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;

	public ProfessoresExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this( true, cenario, i18nConstants, i18nMessages );
	}

	public ProfessoresExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		super( cenario, i18nConstants, i18nMessages );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.PROFESSORES.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().professores();
	}

	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().professores();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Professor > professores = Professor.findByCenario( getCenario() );

		if ( !professores.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.sheetName, workbook );
			}

			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );
			int nextRow = this.initialRow;

			for ( Professor professor : professores )
			{
				nextRow = writeData( professor, nextRow, sheet );
			}

			return true;
		}

		return false;
	}

	// CPF | Nome | Tipo | Carga Horária Máx. | Carga Horária Min. |
	// Titulação | Área de Titulação | Carga Horária Anterior | Crédito (R$)
	private int writeData( Professor professor, int row, HSSFSheet sheet )
	{
		// CPF
		setCell( row, 2, sheet,
				 cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				 professor.getCpf() );

		// Nome
		setCell( row, 3, sheet,
				 cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				 professor.getNome() );

		// Tipo
		setCell( row, 4, sheet,
				 cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				 professor.getTipoContrato().getNome() );

		// Carga Horária Máx
		setCell( row, 5, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ],
				 professor.getCargaHorariaMax() );

		// Carga Horária Min
		setCell( row, 6, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ],
				 professor.getCargaHorariaMin() );

		// Titulação
		setCell( row, 7, sheet,
				 cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				 professor.getTitulacao().getNome() );

		// Área de Titulação
		setCell( row, 8, sheet,
				 cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				 professor.getAreaTitulacao().getCodigo() );

		/*
		 * Código relacionado com a issue http://jira.gapso.com.br/browse/TRIEDA-1040
		 "Acrescentar coluna "Nota Desempenho" na aba "Professores" da planilha de exportação"
		 */
		// Nota de Desempenho
		setCell( row, 9, sheet,
				 cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				 TriedaUtil.getNotaDesempenhoProfessor( professor ) );

		// Carga Horária Anterior
		setCell( row, 10, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ],
				 professor.getCreditoAnterior() );

		// Crédito (R$)
		setCell( row, 11, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_DOUBLE.ordinal() ],
				 professor.getValorCredito() );

		row++;
		return row;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell( cellStyleReference.getRow(),
				cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
