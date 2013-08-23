package com.gapso.web.trieda.main.client.mvp.presenter;


import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class ResumoAtendimentosFaixaDemandaFormPresenter 
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		SimpleModal getSimpleModal();
		NumberField getAbaixoNumberField();
		NumberField getAcimaNumberField();
		Button getSalvarButton();
		Button getAddNovaFaixaButton();
		LayoutContainer getLeft();
		LayoutContainer getRight();
		FormPanel getFormPanel();
		boolean isValid();
	}
	
	private Display display;
	private int numFaixas;
	private Grid< AtendimentoFaixaDemandaDTO > grid;
	private CampusDTO campusDTO;
	private CenarioDTO cenarioDTO;
	
	public ResumoAtendimentosFaixaDemandaFormPresenter (
			CenarioDTO cenarioDTO, Display display,
			Grid< AtendimentoFaixaDemandaDTO > grid, CampusDTO campusDTO)
	{
		this.display = display;
		this.numFaixas = 0;
		this.grid = grid;
		this.campusDTO = campusDTO;
		this.cenarioDTO = cenarioDTO;
		
		setListeners();
	}
	
	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if ( isValid() )
				{
					grid.mask( display.getI18nMessages().loading() );
					display.getSimpleModal().hide();
					Services.alunosDemanda().getResumoFaixaDemandaList( cenarioDTO, campusDTO, getFaixas(),
					new AbstractAsyncCallbackWithDefaultOnFailure< List < AtendimentoFaixaDemandaDTO > >( display )
					{
						@Override
						public void onSuccess(
							List< AtendimentoFaixaDemandaDTO > list )
							{
								grid.getStore().removeAll();
								grid.getStore().add( list );
								grid.unmask();
							}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!", "Verifique os campos digitados", null );
				}
			}
		});
		
		display.getAddNovaFaixaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if ( numFaixas == 0 ) {
						if ( display.getFormPanel().getFields().get(numFaixas).isValid() ) {
							createNewFaixa((Double)display.getFormPanel().getFields().get(numFaixas).getValue());
							numFaixas++;
							display.getFormPanel().layout();	
						}
						else {
							MessageBox.alert("Erro", "Valor do limite superior inválido ou nulo", null);
							return;
						}
					}
					else {
						if ( display.getFormPanel().getFields().get(display.getFormPanel().getFields().size() - 2).isValid()  && 
								display.getFormPanel().getFields().get(display.getFormPanel().getFields().size() - 3).isValid()	) {
							createNewFaixa((Double)display.getFormPanel().getFields().get(display.getFormPanel().getFields().size() - 2).getValue() - 1);
							numFaixas++;
							 display.getFormPanel().layout();
						}
						else {
							MessageBox.alert("Erro", "Valor da faixa anterior inválido ou nulo", null);
							return;
						}
					}
				}
			});
	}

	private boolean isValid()
	{
		return display.isValid();
	}
	
	private List<ParDTO<Integer, Integer>> getFaixas() {
		List<ParDTO<Integer, Integer>> faixas = new ArrayList<ParDTO<Integer, Integer>>();
		double valorSup = (Double) display.getFormPanel().getFields().get(0).getValue();
		double valorInf = 0;
		faixas.add(ParDTO.create((int)valorSup, (int)valorInf));
		for (int i = 1; i <= numFaixas; i++) {
			valorSup = (Double) display.getFormPanel().getFields().get(i).getValue();
			valorInf = (Double) display.getFormPanel().getFields().get(i+numFaixas).getValue();
			faixas.add(ParDTO.create((int)valorSup, (int)valorInf));
		}
		valorSup = 0;
		valorInf = (Double) display.getFormPanel().getFields().get(display.getFormPanel().getFields().size() - 1).getValue();
		faixas.add(ParDTO.create((int)valorSup, (int)valorInf));
		
		return faixas;
	}

	private void createNewFaixa( double valorSup) {
		NumberField faixaSup = new NumberField();
		NumberField faixaInf = new NumberField();
		FormData formData = new FormData( "-20" );
		
		faixaSup.setName("entreSup" + numFaixas);
		faixaSup.setFieldLabel("Faixa (entre");
		faixaSup.setLabelStyle("width: 75");
		faixaSup.setAllowBlank(false);
		faixaSup.setAllowDecimals(false);
		faixaSup.setMaxValue(999);
		faixaInf.setMinValue(1);
		faixaSup.setValue((int)valorSup);
	    display.getLeft().add(faixaSup, formData);
		
	    faixaInf.setName("entreinf" + numFaixas);
	    faixaInf.setFieldLabel("e");
	    faixaInf.setAllowBlank(false);
	    faixaInf.setAllowDecimals(false);
	    faixaInf.setMaxValue(999);
	    faixaInf.setMinValue(1);
	    display.getRight().add(faixaInf, formData);
	}
	
	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
