package com.gapso.web.trieda.shared.util.view;

import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.ListModelPropertyEditor;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.util.view.CargaHorariaComboBox.CargaHoraria;

public class CargaHorariaComboBox extends SimpleComboBox<CargaHoraria> {

	public CargaHorariaComboBox() {
		List<CargaHoraria> enumList = Arrays.asList(CargaHoraria.values());
		add(enumList);
		setSimpleValue(enumList.get(0));
		setLazyRender(false);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	
		setPropertyEditor(new ListModelPropertyEditor<SimpleComboValue<CargaHoraria>>() {
			@Override
			public String getStringValue(SimpleComboValue<CargaHoraria> value) {
				return value.getValue().getText();
			}
		});
		setDisplayField("value");

	}

	public void setValue(String name) {
		if(name == null) setValueField(null); 
		for(SimpleComboValue<CargaHoraria> value : getStore().getModels()) {
			if(value.getValue().name().equals(name)) {
				super.setValue(value);
			}
		}
	}
	
	public String getValueString() {
		return super.getValue().getValue().name();
	}

	public enum CargaHoraria {
		
	    CONCENTRAR("Concentrar em poucos dias"),
	    DISTRIBUIR("Distribuir em todos os dias"),
	    INDIFERENTE("Indiferente");
	    
	    private String text;
	    
	    CargaHoraria(String text) {
	    	this.text = text;
	    }

		public String getText() {
			return text;
		}

		@Override
		public String toString() {
			return text;
		}
	    
	}
	
}
