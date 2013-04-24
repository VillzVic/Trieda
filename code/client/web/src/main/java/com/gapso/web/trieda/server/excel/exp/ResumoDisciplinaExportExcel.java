package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.extjs.gxt.ui.client.data.ModelData;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.DisciplinasServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

@ProgressDeclarationAnnotation
public class ResumoDisciplinaExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		PERCENTE( 6, 11 ),
		INTEGER( 6, 5 ),
		DOUBLE( 6, 9 );

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
	private ResumoDisciplinaFiltroExcel filter;

	public ResumoDisciplinaExportExcel( Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public ResumoDisciplinaExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, 
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public ResumoDisciplinaExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}

	public ResumoDisciplinaExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.RESUMO_DISCIPLINA.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
		this.setFilter( filter );
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().resumoDisciplina();
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
		return getI18nConstants().resumoDisciplina();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		boolean result = false;
		CenarioDTO cenarioDTO = ConvertBeans.toCenarioDTO( getCenario() );

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

		List< ResumoDisciplinaDTO > resumoDisciplinaDTOList
			= new ArrayList< ResumoDisciplinaDTO >();
		DisciplinasServiceImpl disciplinasServiceImpl = new DisciplinasServiceImpl();
		for ( CampusDTO campusDTO : campusDTOList )
		{
			resumoDisciplinaDTOList.addAll(
				disciplinasServiceImpl.getResumos( cenarioDTO, campusDTO ) );
		}

		if ( !resumoDisciplinaDTOList.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int nextRow = this.initialRow;
			for ( ResumoDisciplinaDTO resumoDisciplinaDTO1 : resumoDisciplinaDTOList )
			{
				for ( ModelData resumoDisciplinaDTO2 : resumoDisciplinaDTO1.getChildren() )
				{
					nextRow = writeData( (ResumoDisciplinaDTO) resumoDisciplinaDTO2, nextRow, sheet );
				}
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}

	private int writeData( ResumoDisciplinaDTO resumoDisciplinaDTO, int row, Sheet sheet )
	{
		int i = 2;
		// Campus
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoDisciplinaDTO.getCampusString());
		
		// Disciplina
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoDisciplinaDTO.getDisciplinaStringDaAula() );

		// Turma
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoDisciplinaDTO.getTurma() );

		// Tipo de Crédito
		String tipoDeCredito = resumoDisciplinaDTO.getTipoCreditoTeorico() ?
			getI18nConstants().teorico() : getI18nConstants().pratico();
		tipoDeCredito = HtmlUtils.htmlUnescape( tipoDeCredito );

		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], tipoDeCredito);

		// Créditos
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoDisciplinaDTO.getCreditos() );

		// Total de Créditos
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoDisciplinaDTO.getTotalCreditos() );

		// Qtde. Alunos
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoDisciplinaDTO.getQuantidadeAlunos() );

		// Custo Docente
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoDisciplinaDTO.getCustoDocente().getDoubleValue() );

		// Receita
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoDisciplinaDTO.getReceita().getDoubleValue() );

		// Margem
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoDisciplinaDTO.getMargem().getDoubleValue() );

		// Margem %
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.PERCENTE.ordinal() ],
			resumoDisciplinaDTO.getMargemPercente() );

		row++;
		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference
			: ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}

	public ResumoDisciplinaFiltroExcel getFilter()
	{
		return filter;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter != null && filter instanceof ResumoDisciplinaFiltroExcel )
		{
			this.filter = (ResumoDisciplinaFiltroExcel) filter;
		}
		else
		{
			this.filter = null;
		}
	}
}
