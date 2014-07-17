package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.AlunosDemandaServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class AtendimentosMatriculaExportExcel 
	extends AbstractExportExcel 
{
	enum ExcelCellStyleReference
	{
		TEXT( 6,2 ),
		PERCENTE( 6,18 ),
		INTEGER( 6,6 ),
		DOUBLE( 6,15 );

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
	private Sheet sheet;
	
	public AtendimentosMatriculaExportExcel( Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	
	public AtendimentosMatriculaExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino,
			String fileExtension)
		{
			super( true, ExcelInformationType.ATENDIMENTOS_MATRICULA.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

			this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
			this.removeUnusedSheets = removeUnusedSheets;
			this.initialRow = 6;
		}

	@Override
	public String getFileName()
	{
		return getI18nConstants().atendimentosMatricula();
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
		return getI18nConstants().atendimentosMatricula();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		boolean result = false;

		List< ResumoMatriculaDTO > resumoMatriculaDTOList
			= new ArrayList< ResumoMatriculaDTO >();
		AlunosDemandaServiceImpl alunosDemandaServiceImpl = new AlunosDemandaServiceImpl();
		
		PagingLoadConfig todasPaginas = new BasePagingLoadConfig(0, AlunoDemanda.countMatriculas(instituicaoEnsino, getCenario(), null, null, null, null));
		
		resumoMatriculaDTOList = alunosDemandaServiceImpl.getResumoMatriculasList(
				ConvertBeans.toCenarioDTO(getCenario()), null, null, null, null, todasPaginas).getData();


		if ( !resumoMatriculaDTOList.isEmpty() )
		{
			System.out.println(this.getSheetName());
			this.sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for ( ResumoMatriculaDTO resumoMatriculaDTO : resumoMatriculaDTOList )
			{
				nextRow = writeData( resumoMatriculaDTO, nextRow);
			}
			
			this.sheet = null;

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}
	
	private int writeData( ResumoMatriculaDTO resumoMatriculaDTO, int row)
	{
		Sheet newSheet = restructuringWorkbookIfRowLimitIsViolated(row,1,sheet);
		if (newSheet != null) {
			row = this.initialRow;
			sheet = newSheet;
		}
		
		int i = 2;
		// Campus
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoMatriculaDTO.getCampusString() );
		
		// Matricula
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoMatriculaDTO.getAlunoMatricula() );

		// Demanda P1 (creditos)
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				resumoMatriculaDTO.getCredDemandaP1() );
		
		// Atendidos P1 (creditos)
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoMatriculaDTO.getCredAtendidosP1() );

		// Nao Atendidos P1 (creditos)
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				resumoMatriculaDTO.getCredNaoAtendidosP1() );

		// Atendidos P2 (creditos)
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				resumoMatriculaDTO.getCredAtendidosP2() );

		// Demanda P1 (disciplina)
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				resumoMatriculaDTO.getDisDemandaP1() );

		// Atendidos P1 (disciplina)
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoMatriculaDTO.getDisAtendidosP1() );

		// Nao Atendidos P1 (disciplina)
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				resumoMatriculaDTO.getDisNaoAtendidosP1() );

		// Atendidos P2 (disciplina)
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				resumoMatriculaDTO.getDisAtendidosP2() );

		// Excesso P2 (creditos)
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				resumoMatriculaDTO.getCredExcessoP2() );
		row++;
		return row;
	}
	
	private void fillInCellStyles(Sheet sheet)
	{
		for ( ExcelCellStyleReference cellStyleReference
			: ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
