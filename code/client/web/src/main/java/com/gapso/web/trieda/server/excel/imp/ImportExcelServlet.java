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
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.UsuariosServiceImpl;
import com.gapso.web.trieda.server.util.GTriedaI18nConstants;
import com.gapso.web.trieda.server.util.GTriedaI18nMessages;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ImportExcelServlet
	extends HttpServlet
{
	private static final long serialVersionUID = 1889121953846517684L;
	private static TriedaI18nConstants i18nConstants = null;
	private static TriedaI18nMessages i18nMessages = null;

	private Cenario cenario = null;
	{
		i18nConstants = new GTriedaI18nConstants();
		i18nMessages = new GTriedaI18nMessages();
	}

	@Override
	protected void doPost(
		HttpServletRequest request, HttpServletResponse response )
		throws ServletException, IOException
	{
		UsuariosServiceImpl usuariosService = new UsuariosServiceImpl();
		InstituicaoEnsino instituicaoEnsino
			= usuariosService.getInstituicaoEnsinoUser();

		if ( instituicaoEnsino == null )
		{
			System.out.println( "A instituicao de ensino nao foi informada. " +
				"A importacao dos dados nao podera ser realizada." );

			return;
		}

		this.cenario = Cenario.findMasterData( instituicaoEnsino );

		FileItemFactory factory = new DiskFileItemFactory();
        ServletFileUpload upload = new ServletFileUpload( factory );
        InputStream inputStream = null;

        try
        {
        	@SuppressWarnings( "unchecked" )
			List< FileItem > itens = upload.parseRequest( request );

        	String fileName = null;
        	String informationToBeImported = null;

			for ( FileItem iten : itens )
			{
				if ( iten.getFieldName().equals(
					ExcelInformationType.getInformationParameterName() ) )
				{
					informationToBeImported = iten.getString();
				}
				else if ( iten.getFieldName().equals(
					ExcelInformationType.getFileParameterName() ) )
				{
					fileName = iten.getName();
					inputStream = iten.getInputStream();
				}
			}

			if ( inputStream != null && informationToBeImported != null )
			{
				IImportExcel importer = ImportExcelFactory.createImporter( informationToBeImported,
					this.cenario, i18nConstants, i18nMessages, instituicaoEnsino );

				if ( importer != null )
				{
					if ( !importer.load( fileName, inputStream ) )
					{
						response.setContentType( "text/html" );

						for ( String msg : importer.getWarnings() )
						{
							response.getWriter().println(
								ExcelInformationType.prefixWarning() + msg );
						}
						for ( String msg : importer.getErrors() )
						{
							response.getWriter().println(
								ExcelInformationType.prefixError() + msg );
						}

						response.getWriter().flush();
					}
				}
				else
				{
					response.setContentType( "text/html" );

					response.getWriter().println(
						ExcelInformationType.prefixError() +
						i18nMessages.excelErroImportadorNulo( informationToBeImported ) );
				}
			}
		}
        catch ( FileUploadException e )
        {
			e.printStackTrace();
		}
        finally
        {
			if ( inputStream != null )
			{
				try
				{
					inputStream.close();
				}
				catch ( IOException e )
				{
					e.printStackTrace();
				}
			}
		}
	}
}
