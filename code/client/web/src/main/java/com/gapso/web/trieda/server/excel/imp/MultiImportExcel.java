package com.gapso.web.trieda.server.excel.imp;

import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationImpl;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class MultiImportExcel extends ProgressDeclarationImpl implements IImportExcel {
	protected List<String> errors;
	protected List<String> warnings;
	private Cenario cenario;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;
	private InstituicaoEnsino instituicaoEnsino;
	private Class<? extends IImportExcel>[] arrayImporters;

	protected MultiImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino, Class<? extends IImportExcel>[] arrayImporters) {
		this.instituicaoEnsino = instituicaoEnsino;
		this.cenario = cenario;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;
		this.arrayImporters = arrayImporters;

		this.errors = new ArrayList<String>();
		this.warnings = new ArrayList<String>();
	}

	@Override
	public boolean load(String fileName, Workbook workbook) {
		return false;
	}

	@Override
	public boolean load(String fileName, InputStream inputStream) {
		boolean flag = true;
		try {
			Workbook workbook = WorkbookFactory.create(inputStream);

			List<IImportExcel> importers = new ArrayList<IImportExcel>();
			for (Class<? extends IImportExcel> c : this.arrayImporters) {
				Constructor<? extends IImportExcel> constructor = c.getConstructor(new Class[]{Cenario.class,TriedaI18nConstants.class,TriedaI18nMessages.class,InstituicaoEnsino.class});
				importers.add((IImportExcel)constructor.newInstance(this.cenario,this.i18nConstants,this.i18nMessages,this.instituicaoEnsino));
			}

			for (IImportExcel importer : importers) {
				getProgressReport().setInitNewPartial("Importando " + importer.getSheetName());
				flag = (importer.load(fileName, workbook) && flag);
				getProgressReport().setPartial("Etapa concluída");

				for (String error : importer.getErrors()) {
					getErrors().add(importer.getSheetName() + ": " + error);
				}

				for (String warning : importer.getWarnings()) {
					getErrors().add(importer.getSheetName() + ": " + warning);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			flag = false;
			String errorMessage = TriedaUtil.extractMessage(e);
			getErrors().add(getI18nMessages().excelErroGenericoImportacao(errorMessage));
		}

		return flag;
	}

	@Override
	public List<String> getErrors() {
		return this.errors;
	}

	@Override
	public List<String> getWarnings() {
		return this.warnings;
	}

	protected Cenario getCenario() {
		return this.cenario;
	}

	protected TriedaI18nConstants getI18nConstants() {
		return this.i18nConstants;
	}

	protected TriedaI18nMessages getI18nMessages() {
		return this.i18nMessages;
	}

	@Override
	public String getSheetName() {
		return ExcelInformationType.TUDO.getSheetName();
	}
}
