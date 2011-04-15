package com.gapso.web.trieda.main.client.util.view;

import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.ListModelPropertyEditor;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.main.client.util.view.DificuldadeComboBox.Dificuldade;

public class DificuldadeComboBox extends SimpleComboBox<Dificuldade> {

	public DificuldadeComboBox() {
		List<Dificuldade> enumList = Arrays.asList(Dificuldade.values());
		add(enumList);
		setSimpleValue(enumList.get(0));
		setLazyRender(false);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	
		setPropertyEditor(new ListModelPropertyEditor<SimpleComboValue<Dificuldade>>() {
			@Override
			public String getStringValue(SimpleComboValue<Dificuldade> value) {
				return value.getValue().getNome();
			}
		});
		setDisplayField("value");

	}

	public void setValue(String valueString) {
		if(valueString == null) setValueField(null); 
		for(SimpleComboValue<Dificuldade> value : getStore().getModels()) {
			if(value.getValue().name().equals(valueString)) {
				super.setValue(value);
			}
		}
	}
	
	public enum Dificuldade {
		MEDIO("Médio"),
		BAIXO("Baixo"),
	    ALTO("Alto");
	    
	    private String nome;
	    
	    Dificuldade(String nome) {
	    	this.nome = nome;
	    }

	    public String getNome() {
	    	return nome;
	    }
	    
	    @Override
	    public String toString() {
	    	return getNome();
	    }
	    
	}
	
}
