package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.CheckBoxListView;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.AlunoDisciplinaCursadaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class AlunoDisciplinaCursadaFormPresenter
	implements Presenter
{
	public interface Display
	extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		boolean isValid();
		SimpleModal getSimpleModal();
		CurriculoComboBox getCurriculoComboBox();
		AlunosComboBox getAlunoComboBox();
		DisciplinaAutoCompleteBox getDisciplinaPreRequisitoComboBox();
		CheckBoxListView<CurriculoDisciplinaDTO> getDisciplinasList();
		ContentPanel getDisciplinasListPanel();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private SimpleGrid< AlunoDisciplinaCursadaDTO > gridPanel;
	private Display display;
	private String errorMessage;

	public AlunoDisciplinaCursadaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this( instituicaoEnsinoDTO, cenario, display, null );
	}

	public AlunoDisciplinaCursadaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleGrid< AlunoDisciplinaCursadaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final CurriculosServiceAsync service = Services.curriculos();
					service.saveAlunoDisciplinaCursada(cenario, display.getAlunoComboBox().getValue(),
							display.getDisciplinasList().getChecked(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							if(gridPanel != null) {
								gridPanel.updateList();
							}
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", errorMessage, null);
				}
			}
		});
		
		display.getCurriculoComboBox().addSelectionChangedListener(new SelectionChangedListener< CurriculoDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< CurriculoDTO > se )
			{
				display.getDisciplinasList().mask( display.getI18nMessages().loading() );
				display.getDisciplinasListPanel().setEnabled(true);
				populaLista();
			}
		});
	}
	
	private void populaLista() {
		final FutureResult<ListLoadResult<CurriculoDisciplinaDTO>> futureDisciplinaDTOList = new FutureResult<ListLoadResult<CurriculoDisciplinaDTO>>();
		Services.curriculos().getAlunosDisciplinasNaoCursadasList(cenario, display.getAlunoComboBox().getValue(),
				display.getCurriculoComboBox().getValue(), futureDisciplinaDTOList);
		FutureSynchronizer synch = new FutureSynchronizer(futureDisciplinaDTOList);
		synch.addCallback(new AsyncCallback<Boolean>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(Boolean result) {
				ListLoadResult<CurriculoDisciplinaDTO> disciplinaDTOList = futureDisciplinaDTOList.result();
				
				ListStore<CurriculoDisciplinaDTO> storeCurso = display.getDisciplinasList().getStore();  
				storeCurso.add(disciplinaDTOList.getData());
				display.getDisciplinasList().setStore(storeCurso);
				display.getDisciplinasList().refresh();
				display.getDisciplinasList().unmask();
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid();
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
