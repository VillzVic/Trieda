package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class TiposCursoImportExcel extends AbstractImportExcel<TiposCursoImportExcelBean>
{
	static public String CODIGO_COLUMN_NAME;
	static public String DESCRICAO_COLUMN_NAME;
	private List<String> headerColumnsNames;

	public TiposCursoImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(DESCRICAO_COLUMN_NAME);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected TiposCursoImportExcelBean createExcelBean(Row header, Row row)
	{
		TiposCursoImportExcelBean bean = new TiposCursoImportExcelBean(row.getRowNum() + 1);

		for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++)
		{
			Cell cell = row.getCell(cellIndex);

			if (cell != null)
			{
				Cell headerCell = header.getCell(cell.getColumnIndex());

				if (headerCell != null)
				{
					String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);

					if (CODIGO_COLUMN_NAME.equals(columnName))
					{
						bean.setCodigoStr(cellValue);
					}
					else if (DESCRICAO_COLUMN_NAME.endsWith(columnName))
					{
						bean.setDescricaoStr(cellValue);
					}
				}
			}
		}

		return bean;
	}

	@Override
	protected String getHeaderToString()
	{
		return this.headerColumnsNames.toString();
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.TIPOS_CURSO.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<TiposCursoImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (TiposCursoImportExcelBean bean : sheetContent)
		{
			List<ImportExcelError> errorsBean = bean.checkSyntacticErrors();

			for (ImportExcelError error : errorsBean)
			{
				List<Integer> rowsWithErrors = syntacticErrorsMap.get(error);

				if (rowsWithErrors == null)
				{
					rowsWithErrors = new ArrayList<Integer>();
					syntacticErrorsMap.put(error, rowsWithErrors);
				}

				rowsWithErrors.add(bean.getRow());
			}
		}

		// Coleta os erros e adiciona os mesmos na lista de mensagens
		for (ImportExcelError error : syntacticErrorsMap.keySet())
		{
			List<Integer> linhasComErro = syntacticErrorsMap.get(error);

			getErrors().add(error.getMessage(linhasComErro.toString(), getI18nMessages()));
		}

		return syntacticErrorsMap.isEmpty();
	}

	@Override
	protected boolean doLogicValidation(List<TiposCursoImportExcelBean> sheetContent)
	{
		// Verifica se alguma unidade apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<TiposCursoImportExcelBean> sheetContent)
	{
		// Map com os códigos das unidades e as
		// linhas em que a mesmo aparece no arquivo de entrada
		// [ CódigoUnidade -> Lista de Linhas do Arquivo de Entrada ]
		Map<String, List<Integer>> unidadeCodigoToRowsMap = new HashMap<String, List<Integer>>();

		for (TiposCursoImportExcelBean bean : sheetContent)
		{
			List<Integer> rows = unidadeCodigoToRowsMap.get(bean.getCodigoStr());

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				unidadeCodigoToRowsMap.put(bean.getCodigoStr(), rows);
			}

			rows.add(bean.getRow());
		}

		// Verifica se alguma unidade apareceu
		// mais de uma vez no arquivo de entrada
		for (Entry<String, List<Integer>> entry : unidadeCodigoToRowsMap.entrySet())
		{
			if (entry.getValue().size() > 1)
			{
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(), entry.getValue().toString()));
			}
		}
	}

	@Transactional
	@Override
	protected void updateDataBase(List<TiposCursoImportExcelBean> sheetContent)
	{
		Map<String, TipoCurso> tiposCursoBDMap = TipoCurso.buildTipoCursoCodigoToTipoCursoMap(TipoCurso.findByCenario(this.instituicaoEnsino, getCenario()));

		for (TiposCursoImportExcelBean tipoCursoExcel : sheetContent)
		{
			TipoCurso tipoCursoBD = tiposCursoBDMap.get(tipoCursoExcel.getCodigoStr());
			if (tipoCursoBD != null)
			{
				// Update
				tipoCursoBD.setCodigo(tipoCursoExcel.getCodigoStr());
				tipoCursoBD.setDescricao(tipoCursoExcel.getDescricaoStr());

				tipoCursoBD.merge();
			}
			else
			{
				// Insert
				TipoCurso newTipoCurso = new TipoCurso();

				newTipoCurso.setCodigo(tipoCursoExcel.getCodigoStr());
				newTipoCurso.setDescricao(tipoCursoExcel.getDescricaoStr());
				newTipoCurso.setInstituicaoEnsino(instituicaoEnsino);
				newTipoCurso.setCenario(getCenario());

				newTipoCurso.persist();
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if (CODIGO_COLUMN_NAME == null)
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigo());
			DESCRICAO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().descricao());
		}
	}
}
