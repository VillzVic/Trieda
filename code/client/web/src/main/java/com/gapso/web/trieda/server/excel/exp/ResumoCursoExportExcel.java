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
import com.gapso.web.trieda.server.CursosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

@ProgressDeclarationAnnotation
public class ResumoCursoExportExcel
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

	private ResumoCursoFiltroExcel filter;
	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public ResumoCursoExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public ResumoCursoExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}
		
	public ResumoCursoExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}

	public ResumoCursoExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino,
		String fileExtension)
	{
		super( true, ExcelInformationType.RESUMO_CURSO.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
		this.setFilter( filter );
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().resumoCurso();
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
		return getI18nConstants().resumoCurso();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		boolean result = false;

		List< CampusDTO > campusDTOList = null;
		if ( getFilter() == null
			|| this.getFilter().getCampusDTO() == null
			|| this.getFilter().getCampusDTO().getId() == null )
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
			campusDTOList.add( getFilter().getCampusDTO() );
		}

		List< ResumoCursoDTO > resumoCursoDTOList = new ArrayList< ResumoCursoDTO >();
		CursosServiceImpl cursosServiceImpl = new CursosServiceImpl();

		for ( CampusDTO campusDTO : campusDTOList )
		{
			resumoCursoDTOList.addAll(cursosServiceImpl.getResumos(campusDTO));
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
		if ( !resumoCursoDTOList.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;

			for ( ResumoCursoDTO resumoCursoDTO1 : resumoCursoDTOList )
			{
				for ( ModelData resumoCursoDTO2 : resumoCursoDTO1.getChildren() )
				{
					for ( ModelData resumoCursoDTO3 : ( (ResumoCursoDTO) resumoCursoDTO2 ).getChildren() )
					{
						nextRow = writeData( (ResumoCursoDTO) resumoCursoDTO3, nextRow, sheet );
					}
				}
			}

			result = true;
		}
		
		return result;
	}

	private int writeData( ResumoCursoDTO resumoCursoDTO, int row, Sheet sheet )
	{
		int i = 2;

		// Campus
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getCampusString() );

		// Turno
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getTurnoString() );

		// Curso
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getCursoString() );

		// Matriz Curricular
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getMatrizCurricularString() );

		// Período
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ], resumoCursoDTO.getPeriodo() );

		// Disciplina
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getDisciplinaString() );

		// Turma
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getTurma() );

		// Tipo de Crédito
		String tipoDeCredito = resumoCursoDTO.getTipoCreditoTeorico() ?
			getI18nConstants().teorico() : getI18nConstants().pratico();
		tipoDeCredito = HtmlUtils.htmlUnescape( tipoDeCredito );

		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], tipoDeCredito );

		// Créditos
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ], resumoCursoDTO.getCreditos() );

		// Qtde. Alunos
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ], resumoCursoDTO.getQuantidadeAlunos() );

		// Rateio
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.PERCENTE.ordinal() ], resumoCursoDTO.getRateio() );
		
		// Professor CPF
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getProfessorCPF() );
		
		// Professor Nome
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], resumoCursoDTO.getProfessorNome() );

		// Custo Docente
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ], resumoCursoDTO.getCustoDocente().getDoubleValue() );

		// Receita
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ], resumoCursoDTO.getReceita().getDoubleValue() );

		// Margem
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ], resumoCursoDTO.getMargem().getDoubleValue() );

		// Margem %
		setCell( row, i++, sheet, cellStyles[ ExcelCellStyleReference.PERCENTE.ordinal() ], resumoCursoDTO.getMargemPercente() );

		row++;
		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] =
				getCell( cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}

	public ResumoCursoFiltroExcel getFilter()
	{
		return filter;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter != null && filter instanceof ResumoCursoFiltroExcel )
		{
			this.filter = (ResumoCursoFiltroExcel) filter;
		}
		else
		{
			this.filter = null;
		}
	}
}
