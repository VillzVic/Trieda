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

import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class ImportExcelServlet extends HttpServlet {

	private static final long serialVersionUID = 1889121953846517684L;

	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		
		FileItemFactory factory = new DiskFileItemFactory();
        ServletFileUpload upload = new ServletFileUpload(factory);
        try {
        	@SuppressWarnings("unchecked")
			List<FileItem> itens = upload.parseRequest(request);
        	
        	InputStream inputStream = null;
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
				IImportExcel importer = ImportExcelFactory.createImporter(informationToBeImported);
				if (!importer.load(fileName,inputStream)) {
					response.setContentType("text/plain");
					response.getWriter().print("An error has occured");// TODO: incluir mensagem de erro
				}
			}
		} catch (FileUploadException e) {
			e.printStackTrace();
		}
	}
}