package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class CurriculoFormPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway {
		Button getSalvarButton();
		TextField< String > getCodigoTextField();
		TextField< String > getDescricaoTextField();
		CursoComboBox getCursoComboBox();
		SemanaLetivaComboBox getSemanaLetivaComboBox();
		CurriculoDTO getCurriculoDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO; 
	private CenarioDTO cenario;
	private SimpleGrid< CurriculoDTO > gridPanel;
	private Display display;

	public CurriculoFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleGrid< CurriculoDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				if (isValid()) {
					final CurriculosServiceAsync service = Services.curriculos();
					String errorMsg = display.getI18nMessages().erroAoSalvar(display.getI18nConstants().matrizCurricular());
					service.save(getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(errorMsg,display) {
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							if (gridPanel != null) {
								gridPanel.updateList();
							}
							Info.display("Salvo","Item salvo com sucesso!");
						}
					});
				}
				else {
					MessageBox.alert("ERRO!","Verifique os campos digitados",null);
				}
			}
		});
	}

	private boolean isValid()
	{
		return display.isValid();
	}

	private CurriculoDTO getDTO()
	{
		CurriculoDTO curriculoDTO = display.getCurriculoDTO();

		curriculoDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		curriculoDTO.setCenarioId( cenario.getId() );
		curriculoDTO.setCodigo( display.getCodigoTextField().getValue() );
		curriculoDTO.setDescricao( display.getDescricaoTextField().getValue() );
		curriculoDTO.setCursoId( display.getCursoComboBox().getSelection().get( 0 ).getId() );
		curriculoDTO.setCursoString( display.getCursoComboBox().getSelection().get( 0 ).getCodigo() );
		curriculoDTO.setSemanaLetivaId( display.getSemanaLetivaComboBox().getSelection().get( 0 ).getId() );

		return curriculoDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
