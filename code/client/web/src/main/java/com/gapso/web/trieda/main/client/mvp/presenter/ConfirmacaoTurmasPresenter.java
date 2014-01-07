package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.gapso.web.trieda.main.client.mvp.view.DesmarcarTurmasFormView;
import com.gapso.web.trieda.main.client.mvp.view.MarcarTurmasFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ConfirmacaoTurmasPresenter implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSaveButton();
		Button getCancelButton();
		Button getMarcarTudoButton();
		Button getMarcarMaiorXButton();
		Button getDesmarcarTudoButton();
		Button getDesmarcarMenorXButton();
		Button getMarcarFormandosButton();
		FormPanel getPainelIndicadores();
		LabelField getLeftLabelButton();
		LabelField getRightLabelButton();
		LabelField getCenterLabelButton();
		SimpleGrid< ConfirmacaoTurmaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< ConfirmacaoTurmaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display;
	private GTab gTab;

	public ConfirmacaoTurmasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO  = instituicaoEnsinoDTO;
		this.cenario = cenarioDTO;
		this.display = display;

		configureProxy();
		setListeners();
		loadIndicadores();
	}

	private void configureProxy()
	{
		final AtendimentosServiceAsync service = Services.atendimentos();

		RpcProxy< PagingLoadResult< ConfirmacaoTurmaDTO > > proxy =
			new RpcProxy< PagingLoadResult< ConfirmacaoTurmaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< ConfirmacaoTurmaDTO > > callback)
			{
				service.getConfirmacaoTurmasList(cenario, (PagingLoadConfig) loadConfig, callback);
			}
		};

		display.setProxy( proxy );
	}

	private void setListeners()
	{
		display.getSaveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<ConfirmacaoTurmaDTO> list = display.getGrid().getGrid().getStore().getModels();
				AtendimentosServiceAsync service = Services.atendimentos();
				service.saveConfirmacoes(cenario, list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().getGrid().getStore().commitChanges();
						Info.display("Salvo", "Confirmações atualizadas com sucesso");
					}
				});
			}
		});
		display.getCancelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().getGrid().getStore().rejectChanges();
				Info.display("Cancelado", "Alterações canceladas com sucesso");
			}
		});
		display.getMarcarTudoButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				AtendimentosServiceAsync service = Services.atendimentos();
				service.confirmarTodasTurmas(cenario, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Salvo", "Todas as turmas foram marcadas como confirmadas");
					}
				});
			}
		});
		display.getDesmarcarTudoButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				AtendimentosServiceAsync service = Services.atendimentos();
				service.desconfirmarTodasTurmas(cenario, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Salvo", "Todas as turmas foram desmarcadas como confirmadas");
					}
				});
			}
		});
		display.getMarcarFormandosButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				AtendimentosServiceAsync service = Services.atendimentos();
				service.confirmarTurmasComAlunosFormandos(cenario, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Salvo", "Todas as turmas foram desmarcadas como confirmadas");
					}
				});
			}
		});
		
		display.getMarcarMaiorXButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new MarcarTurmasFormPresenter( instituicaoEnsinoDTO, cenario,
					new MarcarTurmasFormView(), display.getGrid() );

				presenter.go( null );
			}
		});
		
		display.getDesmarcarMenorXButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DesmarcarTurmasFormPresenter( instituicaoEnsinoDTO, cenario,
					new DesmarcarTurmasFormView(), display.getGrid() );

				presenter.go( null );
			}
		});
	}
	
	public void loadIndicadores()
	{
		display.getPainelIndicadores().mask( display.getI18nMessages().loading(), "loading" );
		final AtendimentosServiceAsync service = Services.atendimentos();
		service.carregaIndicadores(cenario, new AsyncCallback<TrioDTO<String, String, String>>() {
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
			}
			@Override
			public void onSuccess(TrioDTO<String, String, String> result) {
				display.getLeftLabelButton().setValue(result.getPrimeiro());
				display.getCenterLabelButton().setValue(result.getSegundo());
				display.getRightLabelButton().setValue(result.getTerceiro());
				display.getPainelIndicadores().layout();
				display.getPainelIndicadores().unmask();
			}
		});
		
	}

	@Override
	public void go( Widget widget )
	{
		this.gTab = (GTab) widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}
}
