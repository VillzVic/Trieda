package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
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
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class UnidadesDeslocamentoImportExcel extends AbstractImportExcel<UnidadesDeslocamentoImportExcelBean>
{
	static public String TEMPO_COLUMN_NAME;
	private Campus currentCampus;
	private Map<Campus, List<String>> campusToHeaderMap = new HashMap<Campus, List<String>>();

	public UnidadesDeslocamentoImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		createHeaderColumnsNames();

	}

	protected List<String> getHeaderColumnsNames(Campus campus)
	{
		if (this.campusToHeaderMap.containsKey(campus)) {
			return this.campusToHeaderMap.get(campus);
		} else {
			return new ArrayList<String>();
		}
	}

	private void createHeaderColumnsNames()
	{
		List<Campus> campi = Campus.findByCenario(instituicaoEnsino, getCenario());
		for (Campus campus : campi)
		{
			List<Unidade> unidadesDoCampus = new ArrayList<Unidade>(campus.getUnidades());
			Collections.sort(unidadesDoCampus, new Comparator<Unidade>()
			{
				@Override
				public int compare(Unidade o1, Unidade o2)
				{
					return o1.getCodigo().compareTo(o2.getCodigo());
				}
			});
			for (Unidade unidade : unidadesDoCampus)
			{
				if (campusToHeaderMap.get(campus) == null)
				{
					List<String> newHeader = new ArrayList<String>();
					newHeader.add(TEMPO_COLUMN_NAME);
					newHeader.add(unidade.getCodigo());
					campusToHeaderMap.put(campus, newHeader);
				}
				else
				{
					campusToHeaderMap.get(campus).add(unidade.getCodigo());
				}
			}
		}
	}

	protected Campus isCampusValid(Row candidateHeader, int sheetIndex, Sheet sheet, Workbook workbook, List<Campus> campiNames)
	{
		Map<String, Campus> campusToNomeCampusMap = Campus.buildCampusNomeToCampusMap(campiNames);
		if (candidateHeader != null)
		{
			// Para cada coluna da linha a ser verificada
			for (int cellIndex = candidateHeader.getFirstCellNum(); cellIndex <= candidateHeader.getLastCellNum(); cellIndex++)
			{
				Cell cell = candidateHeader.getCell(cellIndex);
				if (cell != null && cell.getCellType() == Cell.CELL_TYPE_STRING)
				{
					String columnName = cell.getRichStringCellValue().getString();
					String campusNome = (columnName == null ? "" : columnName.trim());
					if (campusToNomeCampusMap.get(campusNome) != null)
					{
						return campusToNomeCampusMap.get(campusNome);
					}
				}
			}
		}
		return null;
	}

	@Override
	protected UnidadesDeslocamentoImportExcelBean createExcelBean(Row header, Row row)
	{
		UnidadesDeslocamentoImportExcelBean bean = new UnidadesDeslocamentoImportExcelBean(row.getRowNum() + 1);

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

					if (TEMPO_COLUMN_NAME.endsWith(columnName))
					{
						bean.setUnidadeOrigemStr(cellValue);
					}
					else
					{
						for (String columnHeader : getHeaderColumnsNames(currentCampus))
						{
							if (columnHeader.endsWith(columnName))
							{
								bean.addUnidadeDestinoStr(columnName, cellValue);
							}
						}
					}
				}
			}
		}

		return bean;
	}

	@Override
	protected String getHeaderToString()
	{
		return this.getHeaderColumnsNames(this.currentCampus).toString();
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.UNIDADES_DESLOCAMENTO.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<UnidadesDeslocamentoImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (UnidadesDeslocamentoImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<UnidadesDeslocamentoImportExcelBean> sheetContent)
	{
		checkUniqueness(sheetContent);

		checkUnidadeNaoCadastrada(sheetContent);

		checkOrigensDestinos(sheetContent);

		checkDeslocamentosMesmaUnidade(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkUnidadeNaoCadastrada(List<UnidadesDeslocamentoImportExcelBean> sheetContent)
	{
		// [ CodigoCampus -> Campus ]
		Map<String, Unidade> unidadeBDMap = Unidade.buildUnidadeNomeToUnidadeMap(Unidade.findByCenario(this.instituicaoEnsino, getCenario()));

		if (unidadeBDMap == null || unidadeBDMap.size() == 0)
		{
			return;
		}

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (UnidadesDeslocamentoImportExcelBean bean : sheetContent)
		{
			Unidade unidadeOrigem = unidadeBDMap.get(bean.getUnidadeOrigemStr());

			if (unidadeOrigem != null)
			{
				bean.setUnidadeOrigem(unidadeOrigem);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}

			for (Entry<String, String> destinos : bean.getUnidadeDestinoStrToTempoStrMap().entrySet())
			{
				Unidade unidadeDestino = unidadeBDMap.get(destinos.getKey());

				if (unidadeDestino != null)
				{
					bean.addUnidadeDestino(unidadeDestino, Integer.parseInt(destinos.getValue()));
				}
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas("Unidade", rowsWithErrors.toString()));
		}

	}

	private void checkOrigensDestinos(List<UnidadesDeslocamentoImportExcelBean> sheetContent)
	{
		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		int count = 0;
		for (UnidadesDeslocamentoImportExcelBean bean : sheetContent)
		{
			String unidadeOrigem = bean.getUnidadeOrigemStr();
			int posNaListaUnidadeOrigem = count;
			if (posNaListaUnidadeOrigem < bean.getUnidadesDestinos().size())
			{
				String unidadeDestino = bean.getUnidadesDestinos().get(posNaListaUnidadeOrigem);
				if (!unidadeOrigem.equals(unidadeDestino))
				{
					rowsWithErrors.add(bean.getRow());
				}
			}
			else
			{
				// TODO: coletar erro
			}
			count++;
		}

		/*
		 * Map<Campus, List<UnidadesDeslocamentoImportExcelBean>> campusToUnidadeNomeMap = new HashMap<Campus, List<UnidadesDeslocamentoImportExcelBean>>();
		 * 
		 * 
		 * 
		 * for (UnidadesDeslocamentoImportExcelBean bean : sheetContent ) { if (campusToUnidadeNomeMap.get(bean.getUnidadeOrigem().getCampus()) == null) {
		 * List<UnidadesDeslocamentoImportExcelBean> novoBean = new ArrayList<UnidadesDeslocamentoImportExcelBean>(); novoBean.add(bean);
		 * campusToUnidadeNomeMap.put(bean.getUnidadeOrigem().getCampus(), novoBean); } else { campusToUnidadeNomeMap.get(bean.getUnidadeOrigem().getCampus()).add(bean); } }
		 * 
		 * for (Campus campus : campusToHeaderMap.keySet()) { if ((campusToHeaderMap.get(campus).size()-1) == campusToUnidadeNomeMap.get(campus).size()) { for (int i = 1; i <
		 * campusToHeaderMap.get(campus).size(); i++) { if (!campusToHeaderMap.get(campus).get(i).equals(campusToUnidadeNomeMap.get(campus).get(i-1).getUnidadeOrigem().getNome()))
		 * { rowsWithErrors.add(campusToUnidadeNomeMap.get(campus).get(i-1).getRow()); } } } }
		 */
		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoDeslocamentoAssimetrico(rowsWithErrors.toString()));
		}
	}

	private void checkDeslocamentosMesmaUnidade(List<UnidadesDeslocamentoImportExcelBean> sheetContent)
	{

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (UnidadesDeslocamentoImportExcelBean bean : sheetContent)
		{
			for (Entry<String, String> destino : bean.getUnidadeDestinoStrToTempoStrMap().entrySet())
			{
				if (bean.getUnidadeOrigemStr().equals(destino.getKey()) && Integer.parseInt(destino.getValue()) != 0)
				{
					rowsWithErrors.add(bean.getRow());
				}
			}
		}
		if (!rowsWithErrors.isEmpty())
		{
			getWarnings().add(getI18nMessages().excelAvisoMesmaEntidadeDeslocamento(rowsWithErrors.toString()));
		}
	}

	private void checkUniqueness(List<UnidadesDeslocamentoImportExcelBean> sheetContent)
	{
		// Map com as divisoes de creditos e as
		// linhas em que a mesmo aparece no arquivo de entrada
		// [ CodigoTurno -> Lista de Linhas do Arquivo de Entrada ]
		Map<String, List<Integer>> divisaoCodigoToRowsMap = new HashMap<String, List<Integer>>();

		for (UnidadesDeslocamentoImportExcelBean bean : sheetContent)
		{
			List<Integer> rows = divisaoCodigoToRowsMap.get(bean.getUnidadeOrigemStr());

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				divisaoCodigoToRowsMap.put(bean.getUnidadeOrigemStr(), rows);
			}

			rows.add(bean.getRow());
		}

		// Verifica se alguma divisao de credito apareceu
		// mais de uma vez no arquivo de entrada
		for (Entry<String, List<Integer>> entry : divisaoCodigoToRowsMap.entrySet())
		{
			if (entry.getValue().size() > 1)
			{
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(), entry.getValue().toString()));
			}
		}
	}

	@Transactional
	@Override
	protected void updateDataBase(List<UnidadesDeslocamentoImportExcelBean> sheetContent)
	{
		List<DeslocamentoUnidade> deslocamentos = DeslocamentoUnidade.findAll(instituicaoEnsino, this.cenario);

		for (DeslocamentoUnidade deslocamento : deslocamentos)
		{
			deslocamento.remove();
		}

		for (UnidadesDeslocamentoImportExcelBean deslocamentoExcel : sheetContent)
		{
			for (Entry<Unidade, Integer> destinos : deslocamentoExcel.getUnidadeDestinoToTempoMap().entrySet())
			{
				if (destinos.getValue() != 0)
				{
					DeslocamentoUnidade novoDeslocamento = new DeslocamentoUnidade();
					novoDeslocamento.setOrigem(deslocamentoExcel.getUnidadeOrigem());
					novoDeslocamento.setDestino(destinos.getKey());
					novoDeslocamento.setTempo(destinos.getValue());

					novoDeslocamento.persist();
				}
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if (TEMPO_COLUMN_NAME == null)
		{
			TEMPO_COLUMN_NAME = HtmlUtils.htmlUnescape("Tempo de Deslocamento");
		}
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.getHeaderColumnsNames(this.currentCampus);
	}
}
