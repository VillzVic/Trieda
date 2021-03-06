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
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class OfertasCursosCampiImportExcel extends AbstractImportExcel<OfertasCursosCampiImportExcelBean>
{

	static public String CAMPUS_COLUMN_NAME;
	static public String TURNO_COLUMN_NAME;
	static public String CURSO_COLUMN_NAME;
	static public String MATRIZ_CURRICULAR_COLUMN_NAME;
	static public String RECEITA_COLUMN_NAME;

	private List<String> headerColumnsNames;

	protected OfertasCursosCampiImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);

		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CAMPUS_COLUMN_NAME);
		this.headerColumnsNames.add(TURNO_COLUMN_NAME);
		this.headerColumnsNames.add(CURSO_COLUMN_NAME);
		this.headerColumnsNames.add(MATRIZ_CURRICULAR_COLUMN_NAME);
		this.headerColumnsNames.add(RECEITA_COLUMN_NAME);
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.OFERTAS_CURSOS_CAMPI.getSheetName();
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected OfertasCursosCampiImportExcelBean createExcelBean(Row header, Row row)
	{
		OfertasCursosCampiImportExcelBean bean = new OfertasCursosCampiImportExcelBean(row.getRowNum() + 1);

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

					if (CAMPUS_COLUMN_NAME.equals(columnName))
					{
						bean.setCampusStr(cellValue);
					}
					else if (TURNO_COLUMN_NAME.equals(columnName))
					{
						bean.setTurnoStr(cellValue);
					}
					else if (CURSO_COLUMN_NAME.equals(columnName))
					{
						bean.setCursoStr(cellValue);
					}
					else if (MATRIZ_CURRICULAR_COLUMN_NAME.equals(columnName))
					{
						bean.setMatrizCurricularStr(cellValue);
					}
					else if (RECEITA_COLUMN_NAME.equals(columnName))
					{
						bean.setReceitaStr(cellValue);
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
	protected boolean doSyntacticValidation(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo
		// ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (OfertasCursosCampiImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		// Verifica se uma demanda ( Campus + Turno + Curso + Matriz Curricular
		// é adicionada mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);

		// Verifica se há referência a algum tipo de contrato não cadastrado
		checkNonRegisteredCampus(sheetContent);
		checkNonRegisteredTurno(sheetContent);
		checkNonRegisteredCurso(sheetContent);
		checkNonRegisteredCurriculo(sheetContent);

		return getErrors().isEmpty();
	}

	@Transactional
	@Override
	protected void updateDataBase(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		Map<String, Oferta> ofertasBDMap = Oferta.buildCampusTurnoCurriculoToOfertaMap(Oferta.findByCenario(this.instituicaoEnsino, getCenario()));

		Map<TriedaPar<Oferta, Integer>, List<TriedaPar<Demanda, Integer>>> ofertaParPeriodoMapDemandaParQuantidade = new HashMap<TriedaPar<Oferta, Integer>, List<TriedaPar<Demanda, Integer>>>();

		for (OfertasCursosCampiImportExcelBean demandasExcel : sheetContent)
		{
			String codeOferta = getCodeOferta(demandasExcel);

			Oferta oferta = ofertasBDMap.get(codeOferta);

			if (oferta == null)
			{
				oferta = new Oferta();

				oferta.setCampus(demandasExcel.getCampus());
				oferta.setTurno(demandasExcel.getTurno());
				oferta.setCurriculo(demandasExcel.getMatrizCurricular());
				oferta.setCurso(demandasExcel.getCurso());
				oferta.setReceitaWithPrecision(demandasExcel.getReceita() == null ? 0.0 : demandasExcel.getReceita());

				oferta.persist();

				Oferta.entityManager().refresh(oferta);
				ofertasBDMap.put(codeOferta, oferta);
			}
			else
			{
				oferta.setReceitaWithPrecision(demandasExcel.getReceita() == null ? 0.0 : demandasExcel.getReceita());
				oferta.merge();
			}
		}
	}

	private void checkUniqueness(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		// Map com os códigos " Campus + Turno + Matriz Curricular"
		// e as linhas em que o mesmo aparece no arquivo de entrada
		// [ "Campus + Turno + Matriz Curricular" -> Lista de Linhas do Arquivo
		// de Entrada ]
		Map<String, List<Integer>> ofestasToRowsMap = new HashMap<String, List<Integer>>();

		for (OfertasCursosCampiImportExcelBean bean : sheetContent)
		{
			String ofertaPeriodo = getCodeOferta(bean);
			List<Integer> rows = ofestasToRowsMap.get(ofertaPeriodo);

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				ofestasToRowsMap.put(ofertaPeriodo, rows);
			}

			rows.add(bean.getRow());
		}

		for (Entry<String, List<Integer>> entry : ofestasToRowsMap.entrySet())
		{
			if (entry.getValue().size() > 1)
			{
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(), entry.getValue().toString()));
			}
		}
	}

	private void checkNonRegisteredCampus(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		// [ CódidoCampus -> Campus ]
		Map<String, Campus> campiBDMap = Campus.buildCampusCodigoToCampusMap(Campus.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (OfertasCursosCampiImportExcelBean bean : sheetContent)
		{
			Campus campus = campiBDMap.get(bean.getCampusStr());
			if (campus != null)
			{
				bean.setCampus(campus);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CAMPUS_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredTurno(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		// [ NomeTurno -> Turno ]
		Map<String, Turno> turnosBDMap = Turno.buildTurnoNomeToTurnoMap(Turno.findAll(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (OfertasCursosCampiImportExcelBean bean : sheetContent)
		{
			Turno turno = turnosBDMap.get(bean.getTurnoStr());

			if (turno != null)
			{
				bean.setTurno(turno);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(TURNO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredCurso(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		// [ CódigoCurso -> Curso ]
		Map<String, Curso> cursosBDMap = Curso.buildCursoCodigoToCursoMap(Curso.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (OfertasCursosCampiImportExcelBean bean : sheetContent)
		{
			Curso curso = cursosBDMap.get(bean.getCursoStr());

			if (curso != null)
			{
				bean.setCurso(curso);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CURSO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredCurriculo(List<OfertasCursosCampiImportExcelBean> sheetContent)
	{
		// [ CodigoCurriculo -> Curriculo ]
		Map<String, Curriculo> curriculosBDMap = Curriculo.buildCurriculoCodigoToCurriculoMap(Curriculo.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (OfertasCursosCampiImportExcelBean bean : sheetContent)
		{
			Curriculo curriculo = curriculosBDMap.get(bean.getMatrizCurricularStr());

			if (curriculo != null)
			{
				bean.setMatrizCurricular(curriculo);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(MATRIZ_CURRICULAR_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	// Campus + Turno + Matriz Curricular
	private String getCodeOferta(OfertasCursosCampiImportExcelBean bean)
	{
		return bean.getCampusStr() + "-" + bean.getTurnoStr() + "-" + bean.getMatrizCurricularStr();
	}

	private void resolveHeaderColumnNames()
	{
		if (CAMPUS_COLUMN_NAME == null)
		{
			CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoCampus());
			TURNO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().turno());
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoCurso());
			MATRIZ_CURRICULAR_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().matrizCurricular());
			RECEITA_COLUMN_NAME = HtmlUtils.htmlUnescape("Receita por Crédito (R$)");
		}
	}

}
