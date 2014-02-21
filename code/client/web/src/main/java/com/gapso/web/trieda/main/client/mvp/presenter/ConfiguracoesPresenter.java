package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.ExportExcelFormPresenter.Display;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ConfiguracoesModal;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ConfiguracoesPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSalvarBt();
		boolean isValid();
		FormPanel getFormPanel();
		ConfiguracoesModal getConfiguracoesModal();
		TextField<String> getDataSourceTextField();
		TextField<String> getUrlOtimizacaoTextField();
		TextField<String> getNomeOtimizacaoTextField();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	
	public ConfiguracoesPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO,
		Display display )
	{
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;

		setListeners();
	}
	
	private void setListeners()
	{
		display.getSalvarBt().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					Services.instituicoesEnsino().saveConfiguracoes(getDTO(),  new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
								MessageBox.alert( "ERRO!",
										"Deu falha na conexão", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getConfiguracoesModal().hide();
							
							MessageBox.alert( "ERRO!",
									"Configurações Salvas com Sucesso!", null );
						}
					});
				}
				else {
					MessageBox.alert("ERRO!", "Verifique os campos", null);
				}
			}
		});
	}
	
	private boolean isValid()
	{
		return display.isValid();
	}
	
	private ParametroConfiguracaoDTO getDTO()
	{
		ParametroConfiguracaoDTO dto = new ParametroConfiguracaoDTO();
		
		dto.setUrlOtimizacao(display.getUrlOtimizacaoTextField().getValue());
		dto.setNomeOtimizacao(display.getNomeOtimizacaoTextField().getValue());
		dto.setDataSource(display.getDataSourceTextField().getValue());
		
		return dto;
	}
	
	@Override
	public void go( Widget widget )
	{
		display.getConfiguracoesModal().show();
	}
}
