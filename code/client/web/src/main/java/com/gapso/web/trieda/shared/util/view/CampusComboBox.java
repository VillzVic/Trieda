package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.GXT;
import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.core.El;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.DOM;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CampusComboBox
	extends ComboBox< CampusDTO >
{
	private CurriculoComboBox curriculoComboBox;
	private boolean campusTodos;
	private CenarioDTO cenario;
	private boolean clearButton;

	public CampusComboBox(CenarioDTO cenarioDTO)
	{
		this( cenarioDTO, null, false, false );
	}

	public CampusComboBox( CenarioDTO cenarioDTO, CurriculoComboBox curriculoCB, boolean adicionaCampusTodos, boolean clearButton )
	{
		this.curriculoComboBox = curriculoCB;
		this.campusTodos = adicionaCampusTodos;
		this.cenario = cenarioDTO;
		this.clearButton = clearButton;

		RpcProxy< ListLoadResult< CampusDTO > > proxy =
			new RpcProxy< ListLoadResult< CampusDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< CampusDTO > > callback )
			{
				if ( curriculoComboBox != null )
				{
					Services.campi().getListByCurriculo( cenario,
						curriculoComboBox.getValue(), callback );
				}
				else
				{
					if( campusTodos )
					{
						//Lista com todos os campi mais um campus com nome TODOS
						Services.campi().getListAllCampiTodos( cenario, callback );
					}
					else
					{
						Services.campi().getListAll( cenario, callback );
					}
				}
			}
		};

		
		setStore( new ListStore< CampusDTO >(
			new BaseListLoader< BaseListLoadResult< CampusDTO > >( proxy ) ) );

		if ( curriculoComboBox != null )
		{
			setEnabled( false );
			addListeners();
		}

		setDisplayField( CampusDTO.PROPERTY_CODIGO );
		setFieldLabel( "Campus" );
		setEmptyText( "Selecione o campus" );
		setSimpleTemplate( "{" + CampusDTO.PROPERTY_NOME +
			"} ({" + CampusDTO.PROPERTY_CODIGO + "})" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}

	private void addListeners()
	{
		curriculoComboBox.addSelectionChangedListener(
			new SelectionChangedListener< CurriculoDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< CurriculoDTO > se )
			{
				final CurriculoDTO curriculoDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue( null );
				setEnabled( curriculoDTO != null );

				if( curriculoDTO != null )
				{
					getStore().getLoader().load();
				}
			}
		});
	}
	
	protected El twinTrigger;
    
    private final String twinTriggerStyle = "x-form-clear-trigger";
    private El span;
    
    @Override
    protected void onResize(int width, int height) {
        super.onResize(width, height);
        int tw = span.getWidth();
        if (width != Style.DEFAULT) {
            getInputEl().setWidth(width - tw);
        }
    }
    
    @Override
    protected void afterRender() {
        super.afterRender();
        addStyleOnOver(twinTrigger.dom, "x-form-trigger-over");
    }
    
    @Override
    public void onComponentEvent(ComponentEvent ce) {
        super.onComponentEvent(ce);
        int type = ce.getEventTypeInt();
        if ((ce.getTarget() == twinTrigger.dom) && (type == Event.ONCLICK)) {
            onTwinTriggerClick(ce);
        }
    }
    
    @Override
    protected void onRender(Element target, int index) {
        input = new El(DOM.createInputText());
        El wrap = new El(DOM.createDiv());
        wrap.dom.setClassName("x-form-field-wrap");
        
        trigger = new El(DOM.createImg());
        trigger.dom.setClassName("x-form-trigger " + triggerStyle);
        trigger.dom.setPropertyString("src", GXT.BLANK_IMAGE_URL);
        
        twinTrigger = new El(DOM.createImg());
        twinTrigger.dom.setClassName("x-form-trigger " + twinTriggerStyle);
        twinTrigger.dom.setPropertyString("src", GXT.BLANK_IMAGE_URL);
        
        span = new El(DOM.createSpan());
        span.dom.setClassName("x-form-twin-triggers");
        
        DOM.appendChild(span.dom, trigger.dom);
        if (this.clearButton)
        {
	        DOM.appendChild(span.dom, twinTrigger.dom);
        }
        
        DOM.appendChild(wrap.dom, input.dom);
        DOM.appendChild(wrap.dom, span.dom);
        
        setElement(wrap.dom, target, index);
        
        addStyleOnOver(twinTrigger.dom, "x-form-trigger-over");
        addStyleOnOver(trigger.dom, "x-form-trigger-over");
        
        if (isHideTrigger()) {
          span.setVisible(false);
        }
        
        super.onRender(target, index);
        
        if (!isEditable()) {
            setEditable(true);
            setEditable(false);
        }
        
        DOM.sinkEvents(twinTrigger.dom, Event.ONCLICK | Event.MOUSEEVENTS);
        DOM.sinkEvents(wrap.dom, Event.FOCUSEVENTS);
        DOM.sinkEvents(trigger.dom, Event.ONCLICK | Event.MOUSEEVENTS);
    }
    
    protected void onTwinTriggerClick(ComponentEvent ce) {
        setValue(null);
        clearInvalid();
        fireEvent(Events.TwinTriggerClick, ce);
    }
    
    @Override
    public void onLoad(StoreEvent<CampusDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Campus cadastrados", null);
        }
    }
}
