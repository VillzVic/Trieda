package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

@ProgressDeclarationAnnotation
public class ProfessoresImportExcel extends AbstractImportExcel<ProfessoresImportExcelBean>
{
	static public String CPF_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String TIPO_COLUMN_NAME;
	static public String CARGA_HORARIA_MAX_COLUMN_NAME;
	static public String CARGA_HORARIA_MIN_COLUMN_NAME;
	static public String TITULACAO_COLUMN_NAME;
	static public String AREA_TITULACAO_COLUMN_NAME;
	static public String NOTA_DESEMPENHO_COLUMN_NAME;
	static public String CARGA_HORARIA_ANTERIOR_COLUMN_NAME;
	static public String VALOR_CREDITO_COLUMN_NAME;
	static public String MAX_DIAS_SEMANA;
	static public String MIN_CREDITOS_DIA;

	private List<String> headerColumnsNames;

	public ProfessoresImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CPF_COLUMN_NAME);
		this.headerColumnsNames.add(NOME_COLUMN_NAME);
		this.headerColumnsNames.add(TIPO_COLUMN_NAME);
		this.headerColumnsNames.add(CARGA_HORARIA_MAX_COLUMN_NAME);
		this.headerColumnsNames.add(CARGA_HORARIA_MIN_COLUMN_NAME);
		this.headerColumnsNames.add(TITULACAO_COLUMN_NAME);
		this.headerColumnsNames.add(AREA_TITULACAO_COLUMN_NAME);
		this.headerColumnsNames.add(NOTA_DESEMPENHO_COLUMN_NAME);
		this.headerColumnsNames.add(CARGA_HORARIA_ANTERIOR_COLUMN_NAME);
		this.headerColumnsNames.add(VALOR_CREDITO_COLUMN_NAME);
		this.headerColumnsNames.add(MAX_DIAS_SEMANA);
		this.headerColumnsNames.add(MIN_CREDITOS_DIA);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected ProfessoresImportExcelBean createExcelBean(Row header, Row row)
	{
		ProfessoresImportExcelBean bean = new ProfessoresImportExcelBean(row.getRowNum() + 1);

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

					if (CPF_COLUMN_NAME.equals(columnName))
					{
						cell.setCellType(Cell.CELL_TYPE_STRING);

						cellValue = TriedaUtil.formatStringCPF(cell.getRichStringCellValue().getString().trim());

						bean.setCpfStr(cellValue);
					}
					else if (NOME_COLUMN_NAME.equals(columnName))
					{
						bean.setNomeStr(cellValue);
					}
					else if (TIPO_COLUMN_NAME.equals(columnName))
					{
						bean.setTipoStr(cellValue);
					}
					else if (CARGA_HORARIA_MAX_COLUMN_NAME.equals(columnName))
					{
						bean.setCargaHorariaMaxStr(cellValue);
					}
					else if (CARGA_HORARIA_MIN_COLUMN_NAME.equals(columnName))
					{
						bean.setCargaHorariaMinStr(cellValue);
					}
					else if (TITULACAO_COLUMN_NAME.equals(columnName))
					{
						bean.setTitulacaoStr(cellValue);
					}
					else if (AREA_TITULACAO_COLUMN_NAME.equals(columnName))
					{
						bean.setAreaTitulacaoStr(cellValue);
					}
					else if (CARGA_HORARIA_ANTERIOR_COLUMN_NAME.equals(columnName))
					{
						bean.setCargaHorariaAnteriorStr(cellValue);
					}
					else if (VALOR_CREDITO_COLUMN_NAME.equals(columnName))
					{
						bean.setValorCreditoStr(cellValue);
					}
					else if (MAX_DIAS_SEMANA.equals(columnName))
					{
						bean.setMaxDiasSemanaStr(cellValue);
					}
					else if (MIN_CREDITOS_DIA.equals(columnName))
					{
						bean.setMinCreditosDiaStr(cellValue);
					}
				}
			}
		}

		return bean;
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.PROFESSORES.getSheetName();
	}

	@Override
	protected String getHeaderToString()
	{
		return this.headerColumnsNames.toString();
	}

	@Override
	protected boolean doSyntacticValidation(List<ProfessoresImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (ProfessoresImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<ProfessoresImportExcelBean> sheetContent)
	{
		// Verifica se algum professor apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);

		// Verifica se há referência a
		// algum tipo de contrato não cadastrado
		checkNonRegisteredTipoContrato(sheetContent);

		checkNonRegisteredTitulacao(sheetContent);
		checkNonRegisteredAreaTitulacao(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<ProfessoresImportExcelBean> sheetContent)
	{
		// Map com os códigos dos professores e as
		// linhas em que o mesmo aparece no arquivo de entrada
		// [ CpfProfessor -> Lista de Linhas do Arquivo de Entrada ]
		Map<String, List<Integer>> professorCpfToRowsMap = new HashMap<String, List<Integer>>();

		for (ProfessoresImportExcelBean bean : sheetContent)
		{
			List<Integer> rows = professorCpfToRowsMap.get(bean.getCpfStr());

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				professorCpfToRowsMap.put(bean.getCpfStr(), rows);
			}

			rows.add(bean.getRow());
		}

		// Verifica se algum professor apareceu
		// mais de uma vez no arquivo de entrada
		for (Entry<String, List<Integer>> entry : professorCpfToRowsMap.entrySet())
		{
			if (entry.getValue().size() > 1)
			{
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(), entry.getValue().toString()));
			}
		}
	}

	private void checkNonRegisteredTipoContrato(List<ProfessoresImportExcelBean> sheetContent)
	{
		// [ NomeTipoContrato -> TipoContrato ]
		Map<String, TipoContrato> tiposContratoBDMap = TipoContrato.buildTipoContratoNomeToTipoContratoMap(TipoContrato.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (ProfessoresImportExcelBean bean : sheetContent)
		{
			TipoContrato tipoContrato = tiposContratoBDMap.get(bean.getTipoStr());

			if (tipoContrato != null)
			{
				bean.setTipo(tipoContrato);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(TIPO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredTitulacao(List<ProfessoresImportExcelBean> sheetContent)
	{
		// [ NomeTitulacao -> Titulacao ]
		Map<String, Titulacao> titulacoesBDMap = Titulacao.buildTitulacaoNomeToTitulacaoMap(Titulacao.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (ProfessoresImportExcelBean bean : sheetContent)
		{
			Titulacao titulacao = titulacoesBDMap.get(bean.getTitulacaoStr());
			if (titulacao != null)
			{
				bean.setTitulacao(titulacao);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(TITULACAO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredAreaTitulacao(List<ProfessoresImportExcelBean> sheetContent)
	{
		// [ CodigoAreaTitulacao -> AeraTitulacao ]
		Map<String, AreaTitulacao> areasTitulacoesBDMap = AreaTitulacao.buildAreaTitulacaoCodigoToAreaTitulacaoMap(AreaTitulacao.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (ProfessoresImportExcelBean bean : sheetContent)
		{
			String areaTitulacaoStr = bean.getAreaTitulacaoStr() != null ? bean.getAreaTitulacaoStr() : "";
			if (!areaTitulacaoStr.isEmpty())
			{
				AreaTitulacao areaTitulacao = areasTitulacoesBDMap.get(bean.getAreaTitulacaoStr());

				if (areaTitulacao != null)
				{
					bean.setAreaTitulacao(areaTitulacao);
				}
				else
				{
					rowsWithErrors.add(bean.getRow());
				}
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(AREA_TITULACAO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	@Transactional
	@Override
	protected void updateDataBase(List<ProfessoresImportExcelBean> sheetContent)
	{
		Map<String, Professor> professoresBDMap = Professor.buildProfessorCpfToProfessorMap(Professor.findByCenario(this.instituicaoEnsino, getCenario()));
		Set<SemanaLetiva> semanasLetivas = new HashSet<SemanaLetiva>(SemanaLetiva.findByCenario(instituicaoEnsino, getCenario()));

		List<Professor> persistedProfessores = new ArrayList<Professor>();
		int count = 0, total = sheetContent.size();
		System.out.print(" " + total);
		for (ProfessoresImportExcelBean professorExcel : sheetContent)
		{
			Professor professorBD = professoresBDMap.get(professorExcel.getCpfStr());

			if (professorBD != null)
			{
				// Update
				professorBD.setNome(professorExcel.getNomeStr());
				professorBD.setTipoContrato(professorExcel.getTipo());
				professorBD.setCargaHorariaMax(professorExcel.getCargaHorariaMax());
				professorBD.setCargaHorariaMin(professorExcel.getCargaHorariaMin());
				professorBD.setTitulacao(professorExcel.getTitulacao());
				professorBD.setAreaTitulacao(professorExcel.getAreaTitulacao());
				professorBD.setCreditoAnterior(professorExcel.getCargaHorariaAnterior());
				professorBD.setValorCredito(professorExcel.getValorCredito());
				professorBD.setMaxDiasSemana(professorExcel.getMaxDiasSemana());
				professorBD.setMinCreditosDia(professorExcel.getMinCreditosDia());

				professorBD.mergeWithoutFlush();
			}
			else
			{
				// Insert
				Professor newProfessor = new Professor();

				newProfessor.setCenario(getCenario());
				newProfessor.setCpf(professorExcel.getCpfStr());
				newProfessor.setNome(professorExcel.getNomeStr());
				newProfessor.setTipoContrato(professorExcel.getTipo());
				newProfessor.setCargaHorariaMax(professorExcel.getCargaHorariaMax());
				newProfessor.setCargaHorariaMin(professorExcel.getCargaHorariaMin());
				newProfessor.setTitulacao(professorExcel.getTitulacao());
				newProfessor.setAreaTitulacao(professorExcel.getAreaTitulacao());
				newProfessor.setCreditoAnterior(professorExcel.getCargaHorariaAnterior());
				newProfessor.setValorCredito(professorExcel.getValorCredito());
				newProfessor.setMaxDiasSemana(professorExcel.getMaxDiasSemana());
				newProfessor.setMinCreditosDia(professorExcel.getMinCreditosDia());

				newProfessor.persistAndPreencheHorarios(semanasLetivas);
				persistedProfessores.add(newProfessor);
			}

			count++;
			total--;
			if (count == 100)
			{
				System.out.println("\t   Faltam " + total + " professores");
				count = 0;
			}
		}

		/*
		 * if (!persistedProfessores.isEmpty()) { List<SemanaLetiva> semanasLetivas = SemanaLetiva.findByCenario(instituicaoEnsino, getCenario());
		 * Professor.preencheHorariosDosProfessores(persistedProfessores,semanasLetivas); }
		 */
	}

	private void resolveHeaderColumnNames()
	{
		if (CPF_COLUMN_NAME == null)
		{
			CPF_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cpf());
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().nomeProfessor());
			TIPO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().tipoContrato());
			CARGA_HORARIA_MAX_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cargaHorariaMax());
			CARGA_HORARIA_MIN_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cargaHorariaMin());
			TITULACAO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().titulacao());
			AREA_TITULACAO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().areaTitulacao());
			NOTA_DESEMPENHO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().notaDesempenho());
			CARGA_HORARIA_ANTERIOR_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cargaHorariaAnterior());
			VALOR_CREDITO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().valorCredito());
			MAX_DIAS_SEMANA = HtmlUtils.htmlUnescape(getI18nConstants().maxDiasSemana());
			MIN_CREDITOS_DIA = HtmlUtils.htmlUnescape(getI18nConstants().minCreditosDia());
		}
	}
}
