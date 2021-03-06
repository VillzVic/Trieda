package com.gapso.web.trieda.server.excel.exp;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
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
public class HabilitacoesProfessoresExportExcel	
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
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

	private Sheet sheet;
	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public HabilitacoesProfessoresExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public HabilitacoesProfessoresExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.HABILITACAO_PROFESSORES.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return HtmlUtils.htmlUnescape( getI18nConstants().habilitacaoProfessores() );
	}

	@Override
	protected String getPathExcelTemplate()
	{
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return HtmlUtils.htmlUnescape( getI18nConstants().habilitacaoProfessores() );
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List< ProfessorDisciplina > professoresDisciplinas
			= ProfessorDisciplina.findByCenario(this.instituicaoEnsino, this.getCenario());

		Map< String, Boolean > registeredData
			= new HashMap< String, Boolean >();
		
		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
		if ( !professoresDisciplinas.isEmpty() )
		{
			 sheet = workbook.getSheet( this.getSheetName() );
			 fillInCellStyles(sheet);
			int nextRow = this.initialRow;

			for ( ProfessorDisciplina professorDisciplina : professoresDisciplinas )
			{
				//Set< Campus > campiProfessor = professorDisciplina.getProfessor().getCampi();

//				for ( Campus c : campiProfessor )
//				{
//					// Impedindo a inserção de dados repetidos
//					String key = getKeyData( professorDisciplina, c );
//					Boolean data = registeredData.get( key );
//
//					if ( data == null || data == false )
//					{
						nextRow = writeData( null, professorDisciplina, nextRow );
//						registeredData.put( key, true );
//					}
//				}
			}

			return true;
		}

		return false;
	}

	private String getKeyData( ProfessorDisciplina pd, Campus c )
	{
		Professor professor = pd.getProfessor();
		String codigoCampus = ( c == null ? "" : c.getCodigo() );

		return codigoCampus + "-" + professor.getCpf() +
			"-" + pd.getDisciplina().getCodigo() +
			"-" + pd.getPreferencia() + "-" + pd.getNota();
	}

	private int writeData( Campus campus, ProfessorDisciplina pd, int row)
	{
		Professor professor = pd.getProfessor();
		
		if (isXls()){
			Sheet newSheet = restructuringWorkbookIfRowLimitIsViolated(row,1,sheet);
			if (newSheet != null) {
				row = this.initialRow;
				sheet = newSheet;
			}
		}
		
		// CPF Professor
		setCell( row, 2, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], professor.getCpf() );

		// Disciplina
		setCell( row, 3, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], pd.getDisciplina().getCodigo() );

		// Preferência
		setCell( row, 4, sheet, cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ], pd.getPreferencia() );

		// Nota Desempenho
		setCell( row, 5, sheet, cellStyles[ ExcelCellStyleReference.NUMBER_INT.ordinal() ], pd.getNota() );

		row++;
		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
