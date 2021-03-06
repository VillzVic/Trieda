package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class CampiImportExcel extends AbstractImportExcel<CampiImportExcelBean>
{
	static public String CODIGO_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String ESTADO_COLUMN_NAME;
	static public String MUNICIPIO_COLUMN_NAME;
	static public String BAIRRO_COLUMN_NAME;
	static public String CUSTO_CREDITO_COLUMN_NAME;

	private List<String> headerColumnsNames;

	public CampiImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		this.resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(NOME_COLUMN_NAME);
		this.headerColumnsNames.add(ESTADO_COLUMN_NAME);
		this.headerColumnsNames.add(MUNICIPIO_COLUMN_NAME);
		this.headerColumnsNames.add(BAIRRO_COLUMN_NAME);
		this.headerColumnsNames.add(CUSTO_CREDITO_COLUMN_NAME);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected CampiImportExcelBean createExcelBean(Row header, Row row)
	{
		CampiImportExcelBean bean = new CampiImportExcelBean(row.getRowNum() + 1);

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
					else if (NOME_COLUMN_NAME.endsWith(columnName))
					{
						bean.setNomeStr(cellValue);
					}
					else if (ESTADO_COLUMN_NAME.equals(columnName))
					{
						bean.setEstadoStr(cellValue);
					}
					else if (MUNICIPIO_COLUMN_NAME.equals(columnName))
					{
						bean.setMunicipioStr(cellValue);
					}
					else if (BAIRRO_COLUMN_NAME.equals(columnName))
					{
						bean.setBairroStr(cellValue);
					}
					else if (CUSTO_CREDITO_COLUMN_NAME.equals(columnName))
					{
						bean.setCustoMedioCreditoStr(cellValue);
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
		return ExcelInformationType.CAMPI.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<CampiImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre

		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (CampiImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<CampiImportExcelBean> sheetContent)
	{
		// Verifica se algum campus apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<CampiImportExcelBean> sheetContent)
	{
		// Map com os códigos dos campi e as linhas
		// em que o mesmo aparece no arquivo de entrada

		// [ CódigoCampus -> Lista de Linhas do Arquivo de Entrada ]
		Map<String, List<Integer>> campusCodigoToRowsMap = new HashMap<String, List<Integer>>();

		for (CampiImportExcelBean bean : sheetContent)
		{
			List<Integer> rows = campusCodigoToRowsMap.get(bean.getCodigoStr());

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				campusCodigoToRowsMap.put(bean.getCodigoStr(), rows);
			}

			rows.add(bean.getRow());
		}

		// Verifica se algum campus apareceu
		// mais de uma vez no arquivo de entrada
		for (Entry<String, List<Integer>> entry : campusCodigoToRowsMap.entrySet())
		{
			if (entry.getValue().size() > 1)
			{
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(), entry.getValue().toString()));
			}
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando Banco de Dados")
	@Override
	protected void updateDataBase(List<CampiImportExcelBean> sheetContent)
	{
		Map<String, Campus> campiBDMap = Campus.buildCampusCodigoToCampusMap(Campus.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Campus> persistedCampi = new ArrayList<Campus>();
		for (CampiImportExcelBean campusExcel : sheetContent)
		{
			Campus campusBD = campiBDMap.get(campusExcel.getCodigoStr());

			if (campusBD != null)
			{
				// Update
				campusBD.setInstituicaoEnsino(this.instituicaoEnsino);
				campusBD.setNome(campusExcel.getNomeStr());
				campusBD.setEstado(campusExcel.getEstado());
				campusBD.setMunicipio(campusExcel.getMunicipioStr());
				campusBD.setBairro(campusExcel.getBairroStr());
				campusBD.setValorCredito(campusExcel.getCustoMedioCredito());

				campusBD.merge();
				Campus.entityManager().refresh(campusBD);
			}
			else
			{
				// Insert
				Campus newCampus = new Campus();

				newCampus.setInstituicaoEnsino(this.instituicaoEnsino);
				newCampus.setCenario(getCenario());
				newCampus.setCodigo(campusExcel.getCodigoStr());
				newCampus.setNome(campusExcel.getNomeStr());
				newCampus.setEstado(campusExcel.getEstado());
				newCampus.setMunicipio(campusExcel.getMunicipioStr());
				newCampus.setBairro(campusExcel.getBairroStr());
				newCampus.setValorCredito(campusExcel.getCustoMedioCredito());

				newCampus.persistAndPreencheHorarios();
				Campus.entityManager().refresh(newCampus);
				persistedCampi.add(newCampus);
			}
		}

		if (!persistedCampi.isEmpty())
		{
			List<SemanaLetiva> semanasLetivas = SemanaLetiva.findByCenario(instituicaoEnsino, getCenario());
			Campus.preencheHorariosDosCampi(persistedCampi, semanasLetivas);
		}
	}

	private void resolveHeaderColumnNames()
	{
		if (CODIGO_COLUMN_NAME == null)
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoCampus());
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().nome());
			ESTADO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().estado());
			MUNICIPIO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().municipio());
			BAIRRO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().bairro());
			CUSTO_CREDITO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().custoMedioCreditoExcel());
		}
	}

}
