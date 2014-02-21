package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.GXT;
import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.core.El;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.Loader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.DOM;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SalaAutoCompleteBox extends ComboBox<SalaDTO> {
	
	private CenarioDTO cenarioDTO;
	
	public SalaAutoCompleteBox(CenarioDTO cenarioDTO) {
		this(cenarioDTO, false);
	}

	public SalaAutoCompleteBox(CenarioDTO cenarioDTO, boolean readOnly) {
		this.cenarioDTO = cenarioDTO;
		configureContentOfComboBox(readOnly, 0, 10);
		configureView(readOnly);
	}

	private void configureContentOfComboBox(boolean readOnly, final int offset, final int limit) {
		if(!readOnly){
			// Configura a store
			
			RpcProxy<ListLoadResult<SalaDTO>> proxy = new RpcProxy<ListLoadResult<SalaDTO>>() {
				// realiza o filtro dos valores do comboBox
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<SalaDTO>> callback){
					Services.salas().getAutoCompleteList(cenarioDTO, (BasePagingLoadConfig)loadConfig, callback);
				}
			};
			
			ListLoader<BaseListLoadResult<SalaDTO>> listLoader = new BaseListLoader<BaseListLoadResult<SalaDTO>>(proxy);
			listLoader.addListener(Loader.BeforeLoad, new Listener<LoadEvent> () {
				public void handleEvent(LoadEvent be) {
					be.<ModelData>getConfig().set("offset",offset);
					be.<ModelData>getConfig().set("limit",limit);
				}
			});
			
			setStore(new ListStore<SalaDTO>(listLoader));
		}
		else{
			setStore(new ListStore<SalaDTO>());
		}
	}
	
	private void configureView(boolean readOnly) {
		setDisplayField(SalaDTO.PROPERTY_CODIGO);
		setFieldLabel("Ambiente");
		setEmptyText("Selecione um ambiente");
		setTemplate(getTemplateCB());
		setReadOnly(readOnly);
		setHideTrigger(true);
		setTriggerAction(TriggerAction.QUERY);
		setMinChars(1);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{codigo} ({numero})</div>',
			'</tpl>'
		].join("");
	}-*/;

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
        
        if (!isHideTrigger()) {
        	DOM.appendChild(span.dom, trigger.dom);
        }
        DOM.appendChild(span.dom, twinTrigger.dom);
        
        DOM.appendChild(wrap.dom, input.dom);
        DOM.appendChild(wrap.dom, span.dom);
        
        setElement(wrap.dom, target, index);
        
        addStyleOnOver(twinTrigger.dom, "x-form-trigger-over");
        addStyleOnOver(trigger.dom, "x-form-trigger-over");
        
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
