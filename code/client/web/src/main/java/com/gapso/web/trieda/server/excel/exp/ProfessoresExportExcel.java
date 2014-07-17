package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
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

	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public ProfessoresExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public ProfessoresExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.PROFESSORES.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().professores();
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
		return getI18nConstants().professores();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List< Professor > professores = Professor.findByCenario( this.instituicaoEnsino, getCenario() );
		
		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
		if ( !professores.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;

			for ( Professor professor : professores )
			{
				nextRow = writeData( professor, nextRow, sheet );
			}

			return true;
		}

		return false;
	}

	private Double getNotaDesempenho( Professor p )
	{
		if ( p == null || p.getDisciplinas() == null
				|| p.getDisciplinas().size() <= 0 )
		{
			return 0.0;
		}

		Double average = 0.0;
		for ( ProfessorDisciplina pd : p.getDisciplinas() )
		{
			average += pd.getNota();
		}

		average /= p.getDisciplinas().size();
		return average;
	}
	
	// CPF | Nome | Tipo | Carga Horária Máx. | Carga Horária Min. |
	// Titulação | Área de Titulação | Carga Horária Anterior | Crédito (R$)
	private int writeData( Professor professor, int row, Sheet sheet )
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
				 cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				 professor.getTitulacao().getNome() );

		// Área de Titulação
		setCell( row, 8, sheet,
				 cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				 professor.getAreaTitulacao() == null ? "" : professor.getAreaTitulacao().getCodigo() );

		/*
		 * Código relacionado com a issue http://jira.gapso.com.br/browse/TRIEDA-1040
		 "Acrescentar coluna "Nota Desempenho" na aba "Professores" da planilha de exportação"
		 */
		// Nota de Desempenho
		setCell( row, 9, sheet,
				 cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], this.getNotaDesempenho( professor ) );

		// Carga Horária Anterior
		setCell( row, 10, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ],
				 professor.getCreditoAnterior() );

		// Crédito (R$)
		setCell( row, 11, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_DOUBLE.ordinal() ],
				 professor.getValorCredito() );
		
		// Max Dias Semana
		setCell( row, 12, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ],
				 professor.getMaxDiasSemana() );
		
		// Min Creditos Dia
		setCell( row, 13, sheet,
				 cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ],
				 professor.getMinCreditosDia() );

		row++;
		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell( cellStyleReference.getRow(),
				cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
