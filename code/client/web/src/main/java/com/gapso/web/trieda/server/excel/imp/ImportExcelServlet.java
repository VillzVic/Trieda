package com.gapso.web.trieda.server.excel.imp;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.teklabs.gwt.i18n.client.LocaleFactory;
import com.teklabs.gwt.i18n.server.LocaleProxy;

public class ImportExcelServlet extends HttpServlet {

	private static final long serialVersionUID = 1889121953846517684L;
	private static TriedaI18nConstants i18nConstants = null;
	private static TriedaI18nMessages i18nMessages = null;
	private Cenario cenario = null;

	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		LocaleProxy.initialize();
		i18nConstants = LocaleFactory.get(TriedaI18nConstants.class);
		i18nMessages = LocaleFactory.get(TriedaI18nMessages.class);
		cenario = Cenario.findMasterData();
		
		FileItemFactory factory = new DiskFileItemFactory();
        ServletFileUpload upload = new ServletFileUpload(factory);
        InputStream inputStream = null;
        try {
        	@SuppressWarnings("unchecked")
			List<FileItem> itens = upload.parseRequest(request);
        	
        	String fileName = null;
        	String informationToBeImported = null;
			for (FileItem iten : itens) {
				if (iten.getFieldName().equals(ExcelInformationType.getInformationParameterName())) {
					informationToBeImported = iten.getString();
				} else if (iten.getFieldName().equals(ExcelInformationType.getFileParameterName())) {
					fileName = iten.getName();
					inputStream = iten.getInputStream();
				}
			}
			
			if (inputStream != null && informationToBeImported != null) {
				IImportExcel importer = ImportExcelFactory.createImporter(informationToBeImported,cenario,i18nConstants,i18nMessages);
				if (importer != null) {
					if (!importer.load(fileName,inputStream)) {
						response.setContentType("text/html");
						for (String msg : importer.getWarnings()) {
							response.getWriter().println(ExcelInformationType.prefixWarning()+msg);
						}
						for (String msg : importer.getErrors()) {
							response.getWriter().println(ExcelInformationType.prefixError()+msg);
						}
						response.getWriter().flush();
					}
				} else {
					response.setContentType("text/html");
					response.getWriter().println(ExcelInformationType.prefixError()+i18nMessages.excelErroImportadorNulo(informationToBeImported));
				}
			}
		} catch (FileUploadException e) {
			e.printStackTrace();
		} finally {
			if (inputStream != null) {
				try {
					inputStream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}