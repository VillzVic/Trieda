package com.gapso.web.trieda.server.excel.imp;

import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationImpl;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportWriter;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class MultiImportExcelSalas extends ProgressDeclarationImpl implements IImportExcel
{

	protected List<String> errors;
	protected List<String> warnings;
	private Cenario cenario;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;
	private InstituicaoEnsino instituicaoEnsino;

	protected MultiImportExcelSalas(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		this.instituicaoEnsino = instituicaoEnsino;
		this.cenario = cenario;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;

		this.errors = new ArrayList<String>();
		this.warnings = new ArrayList<String>();
	}

	@Override
	public boolean load(String fileName, Workbook workbook, boolean obrigatorio)
	{
		if (checkAnyValidSheet(workbook))
		{
			this.errors.add("Não foi encontrada nenhuma aba válida para a importação");
			return false;
		}

		List<IImportExcel> importers = new ArrayList<IImportExcel>();

		importers.add(new SalasImportExcel(this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino));
		importers.add(new DisponibilidadesSalasImportExcel(this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino));
		
		boolean flag = true;
		for (IImportExcel importer : importers)
		{
			ProgressReportWriter prw = this.getProgressReport();

			if(prw != null) {
				prw.setInitNewPartial("Importando " + importer.getSheetName());
			}

			flag = (importer.load(fileName, workbook, false) && flag);

			if(prw != null) {
				prw.setPartial("Etapa concluída");
			}

			for (String error : importer.getErrors())
			{
				getErrors().add(importer.getSheetName() + ": " + error);
			}

			for (String warning : importer.getWarnings())
			{
				getWarnings().add(importer.getSheetName() + ": " + warning);
			}
		}
		return flag;
	}

	@Override
	public boolean load(String fileName, InputStream inputStream)
	{
		try
		{
			Workbook workbook = WorkbookFactory.create(inputStream);
			return load(fileName, workbook, false);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			String errorMessage = TriedaUtil.extractMessage(e);
			getErrors().add(getI18nMessages().excelErroGenericoImportacao(errorMessage));
			return false;
		}
	}

	@Override
	public boolean load(String fileName, File file)
	{
		try
		{
			Workbook workbook = WorkbookFactory.create(file);
			return load(fileName, workbook, false);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			String errorMessage = TriedaUtil.extractMessage(e);
			getErrors().add(getI18nMessages().excelErroGenericoImportacao(errorMessage));
			return false;
		}
	}

	private boolean checkAnyValidSheet(Workbook workbook)
	{
		boolean nenhumaAbaValida = true;
		for (int sheetIndex = 0; sheetIndex < workbook.getNumberOfSheets(); sheetIndex++)
		{
			for (ExcelInformationType value : ExcelInformationType.values())
			{
				if (value.getSheetName().equals(workbook.getSheetName(sheetIndex)))
				{
					nenhumaAbaValida = false;
				}
			}
		}
		return nenhumaAbaValida;
	}

	@Override
	public List<String> getErrors()
	{
		return this.errors;
	}

	@Override
	public List<String> getWarnings()
	{
		return this.warnings;
	}

	protected Cenario getCenario()
	{
		return this.cenario;
	}

	protected TriedaI18nConstants getI18nConstants()
	{
		return this.i18nConstants;
	}

	protected TriedaI18nMessages getI18nMessages()
	{
		return this.i18nMessages;
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.TUDO.getSheetName();
	}

}
