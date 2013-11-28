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
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.DOM;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CurriculoComboBox
	extends ComboBox< CurriculoDTO >
{
	private CursoComboBox cursoComboBox;
	private CenarioDTO cenarioDTO;
	private boolean clearButton;

	public CurriculoComboBox( CenarioDTO cenario )
	{
		this( cenario, null, false );
	}

	public CurriculoComboBox( CenarioDTO cenario, CursoComboBox cursoCB, boolean clearButton )
	{
		this.cursoComboBox = cursoCB;
		this.cenarioDTO = cenario;
		this.clearButton = clearButton;

		RpcProxy< ListLoadResult< CurriculoDTO > > proxy =
			new RpcProxy< ListLoadResult< CurriculoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< CurriculoDTO > > callback )
			{
				if ( cursoComboBox != null )
				{
					Services.curriculos().getListByCurso(
						cursoComboBox.getValue(), callback );
				}
				else
				{
					Services.curriculos().getListAll( cenarioDTO, callback );
				}
			}
		};

		setStore( new ListStore< CurriculoDTO >(
			new BaseListLoader< BaseListLoadResult< CurriculoDTO > >( proxy ) ) );

		if ( cursoComboBox != null )
		{
			setEnabled( false );
			addListeners();
		}

		setDisplayField( CurriculoDTO.PROPERTY_DISPLAY_TEXT );
		setFieldLabel( "Matriz Curricular" );
		setEmptyText( "Selecione a matriz curricular" );
		setSimpleTemplate( "{" + CurriculoDTO.PROPERTY_DISPLAY_TEXT + "}" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}

	private void addListeners()
	{
		cursoComboBox.addSelectionChangedListener(
			new SelectionChangedListener< CursoDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< CursoDTO > se )
			{
				final CursoDTO cursoDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue( null );
				setEnabled( cursoDTO != null );

				if ( cursoDTO != null )
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
}
