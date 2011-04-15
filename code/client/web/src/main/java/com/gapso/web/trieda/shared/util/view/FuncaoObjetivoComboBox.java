package com.gapso.web.trieda.shared.util.view;

import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.ListModelPropertyEditor;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.util.view.FuncaoObjetivoComboBox.CargaHoraria;

public class FuncaoObjetivoComboBox extends SimpleComboBox<CargaHoraria> {

	public FuncaoObjetivoComboBox() {
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

	public void setValue(Integer valueInt) {
		if(valueInt == null) {
			setValueField(null);
		} else {
			for(SimpleComboValue<CargaHoraria> value : getStore().getModels()) {
				if(value.getValue().equals(CargaHoraria.values()[valueInt])) {
					super.setValue(value);
				}
			}
		}
	}
	
	public String getValueString() {
		return super.getValue().getValue().name();
	}

	public enum CargaHoraria {
		
	    MINIMIZAR("Minimizar ????"),
	    MAXIMIZARMARGEM("Maximizar Margem"),
	    MINIMIZARCUSTODOCENTE("Minimizar Custo Docente");
	    
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
