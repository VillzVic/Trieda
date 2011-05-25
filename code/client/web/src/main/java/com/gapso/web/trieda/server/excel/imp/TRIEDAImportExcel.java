package com.gapso.web.trieda.server.excel.imp;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class TRIEDAImportExcel implements IImportExcel {

	protected List<String> errors;
	protected List<String> warnings;
	private Cenario cenario;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;
	
	protected TRIEDAImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		this.errors = new ArrayList<String>();
		this.warnings = new ArrayList<String>();
		
		this.cenario = cenario;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;
	}
	
	@Override
	public boolean load(String fileName, HSSFWorkbook workbook) {
		return false;
	}
	
	@Transactional
	@Override
	public boolean load(String fileName, InputStream inputStream) {
		boolean flag = true;
		try {
			POIFSFileSystem poifs = new POIFSFileSystem(inputStream);
			HSSFWorkbook workbook = new HSSFWorkbook(poifs);
		
			List<IImportExcel> importers = new ArrayList<IImportExcel>();
			
			// TODO Alterar a ordem
			importers.add(new CampiImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new UnidadesImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new SalasImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new DisciplinasImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new AreasTitulacaoImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new ProfessoresImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new CursosImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new CursoAreasTitulacaoImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new CurriculosImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new DisciplinasSalasImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new EquivalenciasImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new DemandasImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new CampiTrabalhoImportExcel(cenario,i18nConstants,i18nMessages));
			importers.add(new HabilitacoesProfessoresImportExcel(cenario,i18nConstants,i18nMessages));
			
			for(IImportExcel importer : importers) {
				flag = flag && importer.load(fileName, workbook);
				for(String error : importer.getErrors()) {
					getErrors().add(importer.getSheetName()+": " + error);
				}
				for(String warning : importer.getWarnings()) {
					getErrors().add(importer.getSheetName()+": " + warning);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return flag;
	}
	
	@Override
	public List<String> getErrors() {
		return errors;
	}

	@Override
	public List<String> getWarnings() {
		return warnings;
	}
	
	protected Cenario getCenario() {
		return cenario;
	}
	
	protected TriedaI18nConstants getI18nConstants() {
		return i18nConstants;
	}
	
	protected TriedaI18nMessages getI18nMessages() {
		return i18nMessages;
	}
	
	@Override
	public String getSheetName() {
		return ExcelInformationType.TUDO.getSheetName();
	}

}
