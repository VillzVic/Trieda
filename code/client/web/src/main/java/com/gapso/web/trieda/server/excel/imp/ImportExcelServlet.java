package com.gapso.web.trieda.server.excel.imp;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.ProgressReportServiceImpl;
import com.gapso.web.trieda.server.UsuariosServiceImpl;
import com.gapso.web.trieda.server.util.GTriedaI18nConstants;
import com.gapso.web.trieda.server.util.GTriedaI18nMessages;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclaration;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportReader;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class ImportExcelServlet extends HttpServlet
{
	private static final long serialVersionUID = 1889121953846517684L;
	private static TriedaI18nConstants i18nConstants = null;
	private static TriedaI18nMessages i18nMessages = null;
	{
		i18nConstants = new GTriedaI18nConstants();
		i18nMessages = new GTriedaI18nMessages();
	}

	private Cenario cenario = null;

	private static final String TMP_DIR_PATH = "/tmp";
	private File tmpDir;

	@Override
	public void init(ServletConfig config) throws ServletException
	{
		super.init(config);

		String realPath = getServletContext().getRealPath(TMP_DIR_PATH);
		tmpDir = new File(realPath);
		if (!tmpDir.isDirectory())
			if (!tmpDir.mkdir())
				throw new ServletException("Impossível criar diretório " + TMP_DIR_PATH + "!");
	}

	@SuppressWarnings("unchecked")
	@Override
	@Transactional
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
	{
		UsuariosServiceImpl usuariosService = new UsuariosServiceImpl();
		InstituicaoEnsino instituicaoEnsino = usuariosService.getInstituicaoEnsinoUser();

		if (instituicaoEnsino == null)
		{
			System.out.println("A instituicao de ensino nao foi informada. " + "A importacao dos dados nao podera ser realizada.");
			return;
		}

		FileItemFactory factory = new DiskFileItemFactory();
		ServletFileUpload upload = new ServletFileUpload(factory);
//		InputStream inputStream = null;
		File file = null;

		try
		{
			List<FileItem> itens = upload.parseRequest(request);

			String fileName = null;
			String informationToBeImported = null;
			String chaveRegistro = null;
			Long cenarioId = null;

			for (FileItem item : itens)
				if (item.getFieldName().equals(ExcelInformationType.getInformationParameterName()))
					informationToBeImported = item.getString();
				else if (item.getFieldName().equals(ExcelInformationType.getFileParameterName()))
				{
					fileName = item.getName().trim();
					
					file = new File(tmpDir, item.getName());
					item.write(file);
					
//					fileName = item.getName();
//					inputStream = item.getInputStream();
				}
				else if (item.getFieldName().equals(ExcelInformationType.getCenarioParameterName()))
					cenarioId = Long.parseLong(item.getString());
				else if (item.getFieldName().equals("chaveRegistro"))
					chaveRegistro = item.getString();

			this.cenario = Cenario.find(cenarioId, instituicaoEnsino);

			if (file != null && informationToBeImported != null)
			{
				IImportExcel importer = ImportExcelFactory.createImporter(informationToBeImported, this.cenario, i18nConstants, i18nMessages, instituicaoEnsino);

				if (importer != null)
				{
					if (importer instanceof ProgressDeclaration)
					{
						try
						{
							List<String> feedbackList = new ArrayList<String>();
							// File f =
							// ProgressReportCreate.getFile(chaveRegistro);
							((ProgressDeclaration) importer).setProgressReport(feedbackList);

							// ProgressReportReader progressSource = new
							// ProgressReportFileReader(f);
							ProgressReportReader progressSource = new ProgressReportListReader(feedbackList);
							progressSource.start();
							ProgressReportServiceImpl.getProgressReportSession(request).put(chaveRegistro, progressSource);
						}
						catch (Exception e)
						{
							System.out.println("Nao foi possivel realizar o acompanhamento da progressao.");
						}
					}

					if (!importer.load(fileName, file))
					{
						response.setContentType("text/html");

						for (String msg : importer.getWarnings())
						{
							response.getWriter().println(ExcelInformationType.prefixWarning() + msg);
						}

						for (String msg : importer.getErrors())
						{
							response.getWriter().println(ExcelInformationType.prefixError() + msg);
						}

						response.getWriter().flush();
					}
				}
				else
				{
					response.setContentType("text/html");

					response.getWriter().println(ExcelInformationType.prefixError() + i18nMessages.excelErroImportadorNulo(informationToBeImported));
				}
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
			String errorMsg = TriedaUtil.extractMessage(e);
			response.setContentType("text/html");
			response.getWriter().println(ExcelInformationType.prefixError() + i18nMessages.excelErroGenericoImportacao(errorMsg));
		}
		catch (OutOfMemoryError e)
		{
			e.printStackTrace();
			String errorMsg = "Erro de falta de memória";
			response.setContentType("text/html");
			response.getWriter().println(ExcelInformationType.prefixError() + i18nMessages.excelErroGenericoImportacao(errorMsg));
		}
		finally
		{
			if (file != null)
			{
				try
				{
					file.delete();
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		}
	}
}
