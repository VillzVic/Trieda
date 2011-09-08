package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.GrupoSalaAssociarSalaView;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.GruposSalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.user.client.ui.Widget;

public class GrupoSalaFormPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		Button getSalvarEAssociarButton();
		TextField< String > getCodigoTextField();
		TextField< String > getNomeTextField();
		UnidadeComboBox getUnidadeComboBox();
		GrupoSalaDTO getSalaDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< GrupoSalaDTO > gridPanel;
	private Display display;

	public GrupoSalaFormPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display, SimpleGrid< GrupoSalaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(
				new SelectionListener<ButtonEvent>() {
					@Override
					public void componentSelected(ButtonEvent ce) {
						if (isValid()) {
							final GruposSalasServiceAsync service = Services
									.gruposSalas();
							service.save(
									getDTO(),
									new AbstractAsyncCallbackWithDefaultOnFailure<GrupoSalaDTO>(
											display) {
										@Override
										public void onSuccess(
												GrupoSalaDTO result) {
											display.getSimpleModal().hide();
											gridPanel.updateList();
											Info.display("Salvo",
													"Item salvo com sucesso!");
										}
									});
						} else {
							MessageBox.alert("ERRO!",
									"Verifique os campos digitados", null);
						}
					}
				});
		display.getSalvarEAssociarButton().addSelectionListener(
				new SelectionListener<ButtonEvent>() {
					@Override
					public void componentSelected(ButtonEvent ce) {
						if (isValid()) {
							final GruposSalasServiceAsync service = Services
									.gruposSalas();
							service.save(
									getDTO(),
									new AbstractAsyncCallbackWithDefaultOnFailure<GrupoSalaDTO>(
											display) {
										@Override
										public void onSuccess(
												GrupoSalaDTO result) {
											display.getSimpleModal().hide();
											gridPanel.updateList();
											Info.display("Salvo",
													"Item salvo com sucesso!");

											Presenter presenter = new GrupoSalaAssociarSalaPresenter(
													new GrupoSalaAssociarSalaView(
															result), gridPanel);
											presenter.go(null);
										}
									});
						} else {
							MessageBox.alert("ERRO!",
									"Verifique os campos digitados", null);
						}
					}
				});
	}

	private boolean isValid()
	{
		return display.isValid();
	}

	private GrupoSalaDTO getDTO()
	{
		GrupoSalaDTO grupoSalaDTO = display.getSalaDTO();

		grupoSalaDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		grupoSalaDTO.setCodigo( display.getCodigoTextField().getValue() );
		grupoSalaDTO.setNome( display.getNomeTextField().getValue() );
		grupoSalaDTO.setUnidadeId(
			display.getUnidadeComboBox().getSelection().get(0).getId() );
		grupoSalaDTO.setUnidadeString(
			display.getUnidadeComboBox().getSelection().get(0).getCodigo() );

		return grupoSalaDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
