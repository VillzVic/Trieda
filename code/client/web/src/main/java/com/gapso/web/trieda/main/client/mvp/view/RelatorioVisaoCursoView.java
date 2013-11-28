package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.GXT;
import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.core.El;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.widget.form.ComboBox.TriggerAction;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoCursoPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaCursoGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.DOM;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.Event;

public class RelatorioVisaoCursoView extends RelatorioVisaoView	implements RelatorioVisaoCursoPresenter.Display{
	private CampusComboBox campusCB;
	private CursoComboBox cursoCB;
	private TurnoComboBox turnoCB;
	private CurriculoComboBox curriculoCB;
	private SimpleComboBox<Integer> periodoCB;
	private RelatorioVisaoCursoFiltro filtro;

	public RelatorioVisaoCursoView(CenarioDTO cenarioDTO){
		super(cenarioDTO, new RelatorioVisaoCursoFiltro());
	}

	@Override
	protected String getHeadingPanel(){
		return "Master Data » Grade Horária Visão Curso";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Curso", Resources.DEFAULTS.saidaCurso16());
	}
	
	@Override
	protected GradeHorariaCursoGrid createGradeHorariaVisao(){
		return new GradeHorariaCursoGrid();
	}

	@Override
	protected void createFilter(){
		Map<LabelAlign, List<Field<?>>> mapLayout = new HashMap<LabelAlign, List<Field<?>>>();
		
		List<Field<?>> leftList = new ArrayList<Field<?>>();
		
		this.cursoCB = new CursoComboBox(cenarioDTO, true);
		filtro.addCursoValueListener(this.cursoCB);
		filtro.addCursoValueListener(this.cursoCB);
		leftList.add(this.cursoCB);
		
		this.curriculoCB = new CurriculoComboBox(cenarioDTO, this.cursoCB, true);
		this.curriculoCB.setUseQueryCache(false);
		filtro.addCurriculoValueListener(this.curriculoCB);
		leftList.add(this.curriculoCB);
		
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
		
		List<Field<?>> centerList = new ArrayList<Field<?>>();
		
		this.campusCB = new CampusComboBox(cenarioDTO, this.curriculoCB, false, true);
		filtro.addCampusValueListener(this.campusCB);
		centerList.add(this.campusCB);
		
		this.periodoCB = new SimpleComboBox<Integer>()
		{
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
			    DOM.appendChild(span.dom, twinTrigger.dom);
		        
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
		};
		this.periodoCB.setFieldLabel("Período");
		this.periodoCB.setEditable(false);
		this.periodoCB.setTriggerAction(TriggerAction.ALL);
		filtro.addPeriodoValueListener(this.periodoCB);
		centerList.add(this.periodoCB);
		
		this.turnoCB = new TurnoComboBox(this.campusCB, cenarioDTO, true);
		filtro.addTurnoValueListener(this.turnoCB);
		rightList.add(this.turnoCB);

		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		mapLayout.put(LabelAlign.TOP, centerList);
		
		super.createFilter(mapLayout);
	}
	
	@Override
	public GradeHorariaCursoGrid getGrid(){
		return (GradeHorariaCursoGrid) this.grid;
	}

	@Override
	public RelatorioVisaoCursoFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoCursoFiltro) filtro;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return this.campusCB;
	}

	@Override
	public CursoComboBox getCursoComboBox() {
		return this.cursoCB;
	}

	@Override
	public CurriculoComboBox getCurriculoComboBox() {
		return this.curriculoCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox() {
		return this.turnoCB;
	}

	@Override
	public SimpleComboBox<Integer> getPeriodoComboBox() {
		return this.periodoCB;
	}

}
