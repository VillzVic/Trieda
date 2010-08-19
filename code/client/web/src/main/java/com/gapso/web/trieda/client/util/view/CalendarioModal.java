package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.FlexTable.FlexCellFormatter;
import com.google.gwt.user.client.ui.HasHorizontalAlignment;

public class CalendarioModal extends Window {

	public CalendarioModal(String heading) {
		setHeading(heading);
		configuration();
		init();
	}
	
	private void configuration() {
		setModal(true);
		setLayout(new FitLayout());
		setBodyBorder(false);
		addButton();
		setWidth(600);
	}
	
	private void init() {
	    final FlexTable flexTable = new FlexTable();
	    flexTable.addStyleName("cw-FlexTable");
	    flexTable.setCellSpacing(0);
	    flexTable.setCellPadding(3);
	    flexTable.setBorderWidth(1);
	    
	    
	    FlexCellFormatter cellFormatter = flexTable.getFlexCellFormatter();
	    cellFormatter.setHorizontalAlignment(0, 1, HasHorizontalAlignment.ALIGN_LEFT);
	    
	    flexTable.setHTML(0, 2, "Seg");
	    flexTable.setHTML(0, 3, "Ter");
	    flexTable.setHTML(0, 4, "Qua");
	    flexTable.setHTML(0, 5, "Qui");
	    flexTable.setHTML(0, 6, "Sex");
	    flexTable.setHTML(0, 7, "Sab");
	    flexTable.setHTML(0, 8, "Dom");
	    
	    int it = 1;
	    boolean first = true;
	    for(String turno : new String[]{"Manhã", "Tarde", "Noite"}) {
	    	flexTable.setHTML(it, 0, turno);
	    	cellFormatter.setWidth(it, 0, "100%");
	    	cellFormatter.setRowSpan(it, 0, 5);
	    	first = true;
	    	for(String horario : new String[]{"08:00/08:50", "09:00/09:50", "10:00/10:50", "11:00/11:50", "12:00/12:50"}){
	    		flexTable.setHTML(it, first?1:0, horario);
	    		for(int s = 2; s <= 8; s++) {
		    		ToggleImageButton tb = new ToggleImageButton(it%2==0, Resources.DEFAULTS.save16(), Resources.DEFAULTS.cancel16());
	    			flexTable.setWidget(it, s - (!first?1:0), tb);
	    			cellFormatter.setStyleName(it, s - (!first?1:0), "cw-FlexTable-content");
	    		}
	    		first = false;
	    		it++;
	    	}
	    }
	    
		add(flexTable);
	}
	
	private boolean save() {
		return true;
	}
	
	private void addButton() {
		Button salvarBt = new Button("Salvar");
		salvarBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		salvarBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(save()) hide();
			}
		});
		addButton(salvarBt);
	}
}
